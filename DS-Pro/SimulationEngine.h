#pragma once

// ============================================================
// SimulationEngine.h  —  Member 3: Integration Layer
// Owns and orchestrates ALL subsystems:
//   Member 1: RoadNetwork + RoutePlanner
//   Member 2: Vehicle + TrafficSignal
//   Member 3: MetricsCollector (self)
//
// Executes the full CEP discrete-time loop each step:
//   1. Update Traffic Signals     (Section 4.8)
//   2. Update Vehicle Travel Times (Section 4.6)
//   3. Process Intersection Arrivals + Discharge (Section 4.2)
//   4. Recalculate Congestion & Travel Times (Section 4.3 / 4.4)
//   5. Dynamic Re-routing every N steps (Section 4.7)
//   6. Record Metrics (Section 4.10)
// ============================================================

#include <iostream>
#include <vector>
#include <iomanip>
#include "Vehicle.h"
#include "TrafficSignal.h"
#include "RoadNetwork.h"
#include "RoutePlanner.h"
#include "MetricsCollector.h"

using namespace std;

class SimulationEngine
{
private:
    // --- Subsystem references (Member 1 + Member 2 components) ---
    RoadNetwork* network;    // Member 1: Graph of intersections & roads
    RoutePlanner* planner;    // Member 1: Dijkstra shortest path engine

    vector<Vehicle>       vehicles;  // Member 2: All vehicle entities
    vector<TrafficSignal> signals;   // Member 2: All intersection signals

    MetricsCollector metrics;        // Member 3: Performance recorder

    // --- Internal simulation state ---
    int currentStep;
    int rerouteInterval;  // how often dynamic re-routing fires (default: 5)

    // Per-vehicle routing state
    vector<vector<int>> vehiclePaths;   // P_v: path as sequence of node IDs
    vector<int>         pathProgress;   // index of next node to reach
    vector<int>         startStep;      // step when vehicle first moved
    vector<double>      freeFlowTimes;  // T_sd^free: ideal time without congestion

    // ============================================================
    //  Private helpers
    // ============================================================

    // Build queue-size vectors for all signals from current road state
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

    // Check if a destination node has a green signal for a given road
    bool isGreenAt(int destNode, int roadId) const
    {
        for (const TrafficSignal& sig : signals)
            if (sig.getIntersectionId() == destNode)
                return sig.isGreen(roadId);
        return true; // no signal at node → always green
    }

    // Find the Road* connecting fromNode -> toNode
    Road* findRoad(int fromNode, int toNode) const
    {
        for (const auto& rPair : network->getAllRoads())
        {
            Road* r = rPair.second;
            if (r->getFrom() == fromNode && r->getTo() == toNode)
                return r;
        }
        return nullptr;
    }

    // Compute free-flow travel time along a path (sum of w_ij^free)
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

    // --- CEP Step 1: Adaptive Traffic Signals (Section 4.8) ---
    // Only ONE incoming road per intersection gets green (Σ g_ij = 1)
    void stepUpdateSignals()
    {
        vector<vector<int>> queueSizes = buildQueueSizes();
        for (int i = 0; i < (int)signals.size(); i++)
            signals[i].updateSignal(queueSizes[i]);
    }

    // --- CEP Step 2: Decrement vehicle travel times (Section 4.6) ---
    // r_v(t+1) = r_v(t) - 1
    void stepUpdateVehicleTimes()
    {
        for (Vehicle& v : vehicles)
            if (v.isMoving())
                v.updateRemainingTime();
    }

    // --- CEP Step 3: Intersection arrivals + discharge (Section 4.2) ---
    // d_ij(t) = g_ij(t) * min(Q_ij(t), mu_ij, c_jk - f_jk(t))
    void stepProcessIntersections(int& arrivedThisStep)
    {
        for (int i = 0; i < (int)vehicles.size(); i++)
        {
            if (!vehicles[i].hasReachedIntersection()) continue;
            if (vehicles[i].isArrived())               continue;

            // Record start step on first movement
            if (startStep[i] == -1) startStep[i] = currentStep;

            int progress = pathProgress[i];
            int pathLen = (int)vehiclePaths[i].size();

            // No valid path
            if (pathLen < 2) { vehicles[i].arrive(); continue; }

            // Reached destination node?
            if (progress >= pathLen - 1)
            {
                vehicles[i].arrive();
                int actualTime = currentStep - startStep[i];
                metrics.recordArrival(actualTime, freeFlowTimes[i]);
                arrivedThisStep++;
                continue;
            }

            int fromNode = vehiclePaths[i][progress];
            int toNode = vehiclePaths[i][progress + 1];
            Road* road = findRoad(fromNode, toNode);

            if (!road) { vehicles[i].arrive(); continue; }

            // Get the road AFTER this one for c_jk, f_jk
            int nextFlow = 0, nextCap = 9999;
            if (progress + 2 < pathLen)
            {
                Road* nextRoad = findRoad(toNode, vehiclePaths[i][progress + 2]);
                if (nextRoad)
                {
                    nextFlow = nextRoad->getCurrentFlow();
                    nextCap = nextRoad->getCapacity();
                }
            }

            bool green = isGreenAt(toNode, road->getRoadID());

            // CEP Formula: d_ij(t) = g_ij * min(Q_ij, mu_ij, c_jk - f_jk)
            int allowed = road->calculateAllowedDischarge(green, nextFlow, nextCap);

            // Vehicle not yet in queue — treat as single unit trying to enter
            if (road->getQueueSize() == 0 && green &&
                road->getCurrentFlow() < road->getCapacity())
                allowed = 1;

            if (allowed <= 0)
            {
                road->enqueueVehicle(&vehicles[i]);
                vehicles[i].waitAtIntersection();
            }
            else
            {
                int travelTime = (int)road->getCurrentTravelTime();
                if (travelTime < 1) travelTime = 1;
                road->addVehicle(&vehicles[i]);
                vehicles[i].enterEdge(road->getRoadID(), travelTime);
                pathProgress[i]++;
            }
        }
    }

    // --- CEP Step 4: Update congestion + travel times (Section 4.3 / 4.4) ---
    // rho_ij = f_ij / c_ij
    // w_ij(t) = w_ij^free * (1 + alpha * rho^beta)
    void stepUpdateTravelTimes()
    {
        for (const auto& rPair : network->getAllRoads())
            rPair.second->updateTravelTime();
    }

    // --- CEP Step 5: Dynamic re-routing (Section 4.7) ---
    // Re-run Dijkstra from each waiting vehicle's current node
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
    // ============================================================
    //  Constructor: takes ownership context of Member 1 systems
    // ============================================================
    SimulationEngine(RoadNetwork* net, RoutePlanner* rp, int rerouteEvery = 5)
        : network(net), planner(rp), currentStep(0), rerouteInterval(rerouteEvery)
    {}

    // ============================================================
    //  Setup: add vehicles and signals
    // ============================================================
    void addVehicle(const Vehicle& v)
    {
        vehicles.push_back(v);
        vehiclePaths.push_back({});
        pathProgress.push_back(0);
        startStep.push_back(-1);
        freeFlowTimes.push_back(0.0);
    }

    void addTrafficSignal(const TrafficSignal& s)
    {
        signals.push_back(s);
    }

    // Call after all vehicles and roads are added.
    // Computes initial Dijkstra paths for all vehicles (Section 4.7).
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

    // ============================================================
    //  Core: Execute ONE full simulation step (all 6 CEP phases)
    // ============================================================
    void runStep()
    {
        currentStep++;
        int arrivedThisStep = 0;

        stepUpdateSignals();          // Phase 1: Adaptive signals
        stepUpdateVehicleTimes();     // Phase 2: Decrement r_v(t)
        stepProcessIntersections(arrivedThisStep); // Phase 3: Discharge + move
        stepUpdateTravelTimes();      // Phase 4: Congestion model
        if (currentStep % rerouteInterval == 0)
            stepReroute();            // Phase 5: Dynamic re-routing

        metrics.recordStepMetrics(*network, arrivedThisStep); // Phase 6: Metrics
    }

    // ============================================================
    //  Run full simulation for N steps
    // ============================================================
    void run(int totalSteps, bool verbose = true)
    {
        cout << "\n--- Simulation Start (" << totalSteps << " steps) ---\n\n";

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

        cout << "\n--- Simulation Complete ---\n";
    }

    // ============================================================
    //  Accessors (for main.cpp to print reports)
    // ============================================================
    int  getCurrentStep()  const { return currentStep; }
    int  getTotalVehicles() const { return (int)vehicles.size(); }
    MetricsCollector& getMetrics() { return metrics; }

    int countArrivedVehicles() const
    {
        int count = 0;
        for (const Vehicle& v : vehicles) if (v.isArrived()) count++;
        return count;
    }

    // ============================================================
    //  Display helpers
    // ============================================================
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