#pragma once
#include "TrafficNetwork.h"
#include "MetricsCollector.h"

// ==========================================
// 3. Simulation Engine
// ==========================================
class TrafficSimulator {
private:
    TrafficNetwork* network;
    MetricsCollector metrics;

public:
    TrafficSimulator(TrafficNetwork* net) : network(net) {}
    ~TrafficSimulator() {}

    MetricsCollector& getMetrics() { return metrics; }
    TrafficNetwork* getNetwork() { return network; }

    // Main Simulation Step (Implemented for Day 3)
    void simulateStep() {
        int t = metrics.getSimulationTime();
        metrics.incrementSimulationTime();

        // 1. Adjust Traffic Signals (Do this first to decide who gets green)
        for (auto& pair : network->getIntersections()) {
            pair.second->updateSignals();
        }

        // 2. Update Traffic Flow - Vehicles in transit
        for (Vehicle* v : network->getVehicles()) {
            if (v->getStatus() == MOVING) {
                v->updateRemainingTime(1.0);
                if (v->isWaitingAtIntersection()) {
                    Road* currentEdge = v->getCurrentEdge();
                    if (currentEdge) currentEdge->enqueueVehicleToIntersection(v);
                }
            }
            else if (v->getStatus() == WAITING && v->getCurrentEdge() == nullptr) {
                // Vehicle has not entered the network yet
                if (v->getStartTime() == -1) v->setStartTime(t);

                vector<int> path = v->getPath();
                if (v->getPathIndex() < (int)path.size() - 1) {
                    Intersection* src = network->getIntersections().at(path[v->getPathIndex()]);
                    int nextNode = path[v->getPathIndex() + 1];
                    auto it = src->getOutgoingRoads().find(nextNode);
                    if (it != src->getOutgoingRoads().end()) {
                        Road* firstRoad = it->second;
                        if (firstRoad->getCurrentFlow() < firstRoad->getCapacity()) {
                            firstRoad->enterRoad();
                            v->setCurrentEdge(firstRoad);
                            v->setRemainingTime(firstRoad->getCurrentTravelTime());
                            v->setStatus(MOVING);
                        }
                    }
                }
            }
        }

        // 3. Update Traffic Flow - Vehicles in queues at intersections
        for (Road* r : network->getRoads()) {
            if (!r->hasGreenSignal()) continue;

            int count = 0;
            // Discharge vehicles up to the discharge rate
            while (count < r->getDischargeRate() && r->getQueueLength() > 0) {
                Vehicle* v = r->peekFrontVehicle();
                vector<int> path = v->getPath();
                int nextNodeIndex = v->getPathIndex() + 1;

                if (nextNodeIndex >= (int)path.size() - 1) {
                    // Vehicle arrived at destination
                    r->dequeueVehicleFromIntersection();
                    v->setStatus(ARRIVED);
                    v->setPathIndex(nextNodeIndex);

                    double actualTime = t - v->getStartTime();
                    double freeTime = v->getFreeFlowTravelTime();
                    metrics.recordVehicleArrival(actualTime, freeTime);

                    count++;
                }
                else {
                    // Enter next road
                    Intersection* currInt = network->getIntersections().at(path[nextNodeIndex]);
                    int nextNextNode = path[nextNodeIndex + 1];
                    auto it = currInt->getOutgoingRoads().find(nextNextNode);
                    if (it != currInt->getOutgoingRoads().end()) {
                        Road* nextRoad = it->second;
                        if (nextRoad->getCurrentFlow() < nextRoad->getCapacity()) {
                            r->dequeueVehicleFromIntersection();
                            nextRoad->enterRoad();
                            v->setCurrentEdge(nextRoad);
                            v->setRemainingTime(nextRoad->getCurrentTravelTime());
                            v->setStatus(MOVING);
                            v->setPathIndex(nextNodeIndex);
                            count++;
                        }
                        else {
                            break; // Blocked by capacity
                        }
                    }
                    else {
                        // Dead end or invalid path
                        r->dequeueVehicleFromIntersection();
                        v->setStatus(ARRIVED);
                        break;
                    }
                }
            }
        }

        // 4. Recalculate Congestion and Travel Times
        for (Road* r : network->getRoads()) {
            r->updateTravelTime();
        }

        // 5. Dynamic Routing Update (Recalculate routes periodically, e.g., every 5 steps)
        if (t > 0 && t % 5 == 0) {
            for (Vehicle* v : network->getVehicles()) {
                if (v->getStatus() == WAITING && v->getCurrentEdge() != nullptr) {
                    int currentInter = v->getPath()[v->getPathIndex() + 1];
                    if (currentInter != v->getDestinationId()) {
                        vector<int> newPath = network->calculateShortestPath(currentInter, v->getDestinationId());
                        if (!newPath.empty()) {
                            vector<int> fullPath(v->getPath().begin(), v->getPath().begin() + v->getPathIndex() + 1);
                            fullPath.insert(fullPath.end(), newPath.begin() + 1, newPath.end());
                            v->setPath(fullPath);
                        }
                    }
                }
            }
        }
    }

    void runSimulation(int totalSteps) {
        for (int i = 0; i < totalSteps; i++) {
            simulateStep();
        }
    }
};
