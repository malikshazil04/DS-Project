#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include "Vehicle.h"
#include "TrafficSignal.h"
#include "RoadNetwork.h"
#include "RoutePlanner.h"
#include "MetricsCollector.h"
using namespace std;

class SimulationEngine
{
private:

    RoadNetwork* network;    
    RoutePlanner* planner; 
    MetricsCollector metrics;
    vector<Vehicle> vehicles;  
    vector<TrafficSignal> signals;            
    vector<vector<int>> vehiclePaths;   
    vector<int>         pathProgress;   
    vector<int>         startStep;      
    vector<double>      freeFlowTimes;  
    vector<bool>        launched; 
    int currentStep;
    int rerouteInterval;


    vector<vector<int>> buildQueueSizes() const
    {
        vector<vector<int>> allQueueSizes;
        for (const TrafficSignal& sig : signals)
        {
            vector<int> qSizes;
            for (int roadId : sig.getIncomingRoadIds())
            {
                const auto& allRoads = network->getAllRoads();
                auto it = allRoads.find(roadId);
                qSizes.push_back(it != allRoads.end() ? it->second->getQueueSize() : 0);
            }
            allQueueSizes.push_back(qSizes);
        }
        return allQueueSizes;
    }

    bool isGreenAt(int destNode, int roadId) const
    {
        for (const TrafficSignal& sig : signals)
            if (sig.getIntersectionId() == destNode)
                return sig.isGreen(roadId);
        return true; 
    }

    Road* findRoad(int fromNode, int toNode) const
    {
        const auto& allRoads = network->getAllRoads();
        for (const auto& rPair : allRoads)
        {
            Road* r = rPair.second;
            if (r->getFrom() == fromNode && r->getTo() == toNode)
                return r;
        }
        return nullptr;
    }

    double computeFreeFlowTime(const vector<int>& path) const
    {
        double fft = 0.0;
        for (int j = 0; j + 1 < (int)path.size(); j++)
        {
            Road* r = findRoad(path[j], path[j + 1]);
            if (r) fft += r->getFreeTravelTime();
        }
        return fft;
    }
	// Update traffic signals based on current queue sizes at each intersection
    void stepUpdateSignals()
    {
        vector<vector<int>> queueSizes = buildQueueSizes();
        for (int i = 0; i < (int)signals.size(); i++)
            signals[i].updateSignal(queueSizes[i]);
    }
	// Update remaining travel times for all moving vehicles
    void stepUpdateVehicleTimes()
    {
        for (Vehicle& v : vehicles)
            if (v.isMoving())
                v.updateRemainingTime();
    }
	// Process vehicle movements at intersections, handle arrivals, and update metrics
    void stepProcessIntersections(int& arrivedThisStep)
    {
        map<int, int> dischargedThisStep;
        for (int i = 0; i < (int)vehicles.size(); i++)
        {
            if (vehicles[i].isArrived()) continue;

            int progress = pathProgress[i];
            int pathLen = (int)vehiclePaths[i].size();
            if (pathLen < 2) { vehicles[i].arrive(); continue; }

			//vehiclae is not launched yet, try to put it on the first road if possible
            if (!launched[i])
            {
                Road* firstRoad = findRoad(vehiclePaths[i][0], vehiclePaths[i][1]);
                if (!firstRoad) { vehicles[i].arrive(); continue; }

                if (firstRoad->getCurrentFlow() < firstRoad->getCapacity())
                {
                    startStep[i] = currentStep;
                    launched[i] = true;
                    firstRoad->addVehicle(&vehicles[i]);
                    int tt = max(1, (int)firstRoad->getCurrentTravelTime());
                    vehicles[i].enterEdge(firstRoad->getRoadID(), tt);
                    pathProgress[i] = 1; 
                }
                continue;
            }

			// Vehicle is moving, check if it has reached the end of current edge

            if (vehicles[i].isMoving())
            {
                if (vehicles[i].getRemainingTravelTime() > 0) continue;


                vehicles[i].waitAtIntersection(); 

                if (progress > 0) {
                    Road* currRoad = findRoad(vehiclePaths[i][progress - 1], vehiclePaths[i][progress]);
                    if (currRoad) currRoad->enqueueVehicle(&vehicles[i]);
                }
            }

			// Vehicle is waiting at intersection, try to move to next edge if possible

            if (progress >= pathLen - 1)
            {
                int fromNode = vehiclePaths[i][progress - 1];
                int toNode = vehiclePaths[i][progress];
                Road* currRoad = findRoad(fromNode, toNode);
                if (currRoad) {
                    currRoad->dischargeVehicle(&vehicles[i]); 
                    currRoad->removeVehicle(&vehicles[i]);    
                }
				// Vehicle has reached destination
                vehicles[i].arrive();
                int actualTime = currentStep - startStep[i];
                metrics.recordArrival(actualTime, freeFlowTimes[i]);
                arrivedThisStep++;
                continue;
            }
			// Vehicle is waiting at intersection, try to move to next edge if possible
            int fromNode = vehiclePaths[i][progress - 1];
            int toNode = vehiclePaths[i][progress];
            Road* currRoad = findRoad(fromNode, toNode);

            int nextFrom = vehiclePaths[i][progress];
            int nextTo = vehiclePaths[i][progress + 1];
            Road* nextRoad = findRoad(nextFrom, nextTo);
			// If next road doesn't exist, treat as arrival
            if (!nextRoad)
            {
                if (currRoad) {
                    currRoad->dischargeVehicle(&vehicles[i]);
                    currRoad->removeVehicle(&vehicles[i]);
                }
                vehicles[i].arrive(); 
                continue;
            }
			// Check traffic signal and capacity
            bool green = true;
            if (currRoad) {
                green = isGreenAt(toNode, currRoad->getRoadID());
            }

            int allowed = 0;
            if (green && nextRoad->getCurrentFlow() < nextRoad->getCapacity())
            {
                int mu = currRoad ? currRoad->getDischargeRate() : 999;
                int available = nextRoad->getCapacity() - nextRoad->getCurrentFlow();
                allowed = min(1, min(mu, available)); 
            }
			// If allowed to move, discharge from current road and add to next road
            if (allowed > 0)
            {
                if (currRoad) {
                    int roadId = currRoad->getRoadID();
                    if (dischargedThisStep[roadId] >= currRoad->getDischargeRate()) {
                        continue; 
                    }
                    dischargedThisStep[roadId]++;
                    currRoad->dischargeVehicle(&vehicles[i]);
                    currRoad->removeVehicle(&vehicles[i]);
                }

                nextRoad->addVehicle(&vehicles[i]);
                int tt = max(1, (int)nextRoad->getCurrentTravelTime());
                vehicles[i].enterEdge(nextRoad->getRoadID(), tt);
                pathProgress[i]++;
            }
        }
    }
	// Update travel times on all roads based on current congestion
    void stepUpdateTravelTimes()
    {
        for (const auto& rPair : network->getAllRoads())
            rPair.second->updateTravelTime();
    }
	// Reroute vehicles based on current traffic conditions every rerouteInterval steps
    void stepReroute()
    {
        for (int i = 0; i < (int)vehicles.size(); i++)
        {
            if (vehicles[i].isArrived()) continue;
            if (pathProgress[i] >= (int)vehiclePaths[i].size() - 1) continue;

            int currentNode = vehiclePaths[i][pathProgress[i]];
            int destNode = vehicles[i].getDestination();

            vector<int> newTail = planner->findShortestPath(currentNode, destNode);
            if (newTail.size() >= 2)
            {
                vehiclePaths[i].resize(pathProgress[i] + 1);
                for (int j = 1; j < (int)newTail.size(); j++)
                    vehiclePaths[i].push_back(newTail[j]);
            }
        }
    }

public:
    SimulationEngine(RoadNetwork* net, RoutePlanner* rp, int rerouteEvery = 5)
        : network(net), planner(rp), currentStep(0), rerouteInterval(rerouteEvery)
    {}

    void addVehicle(const Vehicle& v)
    {
        vehicles.push_back(v);
        vehiclePaths.push_back({});
        pathProgress.push_back(0);
        startStep.push_back(-1);
        freeFlowTimes.push_back(0.0);
        launched.push_back(false);
    }

    void addTrafficSignal(const TrafficSignal& s)
    {
        signals.push_back(s);
    }
    void initializeRoutes()
    {
        cout << "\n[Routing] Computing initial shortest paths (Dijkstra)...\n";
        for (int i = 0; i < (int)vehicles.size(); i++)
        {
            int src = vehicles[i].getSource();
            int dest = vehicles[i].getDestination();
            vehiclePaths[i] = planner->findShortestPath(src, dest);
            freeFlowTimes[i] = computeFreeFlowTime(vehiclePaths[i]);

            cout << "  V" << vehicles[i].getVehicleId()
                << " [" << src << "->" << dest << "] Path: ";
            for (int node : vehiclePaths[i]) cout << node << " ";
            cout << "(free-flow: " << fixed << setprecision(1)
                << freeFlowTimes[i] << "s)\n";
        }
    }

    void runStep()
    {
        currentStep++;
        int arrivedThisStep = 0;

        stepUpdateSignals();          
        stepUpdateVehicleTimes();     
        stepProcessIntersections(arrivedThisStep); 
        stepUpdateTravelTimes();   
        if (currentStep % rerouteInterval == 0)
            stepReroute();         

        metrics.recordStepMetrics(*network, arrivedThisStep); 
    }

    void run(int totalSteps, bool verbose = true)
    {
        cout << "\n Simulation Start (" << totalSteps << " steps) \n\n";

        for (int s = 1; s <= totalSteps; s++)
        {
            runStep();

            if (verbose && (s % 5 == 0 || s == totalSteps))
            {
                cout << "[Step " << setw(3) << s << "] "
                    << "Arrived: " << setw(2) << countArrivedVehicles()
                    << "/" << vehicles.size()
                    << "  |  Avg Congestion: "
                    << fixed << setprecision(1)
                    << metrics.getAverageCongestion() * 100 << "%"
                    << "  |  Objective: "
                    << setprecision(3) << metrics.computeObjectiveFunction(*network)
                    << "\n";
            }

            if (countArrivedVehicles() == (int)vehicles.size()) break;
        }

        cout << "\n   Simulation Complete \n";
    }

    int  getCurrentStep()  const { return currentStep; }
    int  getTotalVehicles() const { return (int)vehicles.size(); }
    MetricsCollector& getMetrics() { return metrics; }
    const vector<TrafficSignal>& getSignals() const { return signals; }

    int countArrivedVehicles() const
    {
        int count = 0;
        for (const Vehicle& v : vehicles) if (v.isArrived()) count++;
        return count;
    }

    void displayVehicleState() const
    {
        cout << "\n[Vehicle State at Step " << currentStep << "]\n";
        for (int i = 0; i < (int)vehicles.size(); i++)
        {
            cout << "  V" << setw(2) << vehicles[i].getVehicleId()
                << " [" << vehicles[i].getSource()
                << "->" << vehicles[i].getDestination() << "]"
                << "  Status: " << setw(8) << vehicles[i].getStatus()
                << "  Edge: " << vehicles[i].getCurrentEdge()
                << "  TimeLeft: " << vehicles[i].getRemainingTravelTime()
                << "  Node: "
                << (pathProgress[i] < (int)vehiclePaths[i].size()
                    ? to_string(vehiclePaths[i][pathProgress[i]]) : "DONE")
                << "\n";
        }
    }

    void displayRoadState() const
    {
        cout << "\n[Road State at Step " << currentStep << "]\n";
        cout << left
            << setw(8) << "RoadID"
            << setw(10) << "From->To"
            << setw(10) << "Flow/Cap"
            << setw(8) << "Queue"
            << setw(13) << "Congestion%"
            << "TravelTime\n";
        cout << string(60, '-') << "\n";
        for (const auto& rPair : network->getAllRoads())
        {
            Road* r = rPair.second;
            cout << left
                << setw(8) << r->getRoadID()
                << setw(3) << r->getFrom() << "->" << setw(5) << r->getTo()
                << setw(4) << r->getCurrentFlow() << "/" << setw(5) << r->getCapacity()
                << setw(8) << r->getQueueSize()
                << setw(13) << fixed << setprecision(1) << r->getCongestion() * 100 << "%"
                << r->getCurrentTravelTime() << "s\n";
        }
    }
};