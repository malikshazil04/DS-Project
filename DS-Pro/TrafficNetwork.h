#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include "Intersectionsh.h"
#include "Road.h"
#include "Vehicle.h"

using namespace std;

// ==========================================
// Core System / Graph Controller
// ==========================================
class TrafficNetwork {
private:
    unordered_map<int, Intersection*> intersections; // V
    vector<Road*> roads;                             // E
    vector<Vehicle*> vehicles;                       // Active Vehicles

public:
    TrafficNetwork() {}

    ~TrafficNetwork() {
        for (auto& pair : intersections) delete pair.second;
        for (Road* r : roads) delete r;
        for (Vehicle* v : vehicles) delete v;
    }

    // Getters
    const unordered_map<int, Intersection*>& getIntersections() const { return intersections; }
    const vector<Road*>& getRoads() const { return roads; }
    const vector<Vehicle*>& getVehicles() const { return vehicles; }

    void addIntersection(int id) {
        if (intersections.find(id) == intersections.end()) {
            intersections[id] = new Intersection(id);
        }
    }

    void addRoad(int src, int dest, double length, double maxSpeed, int capacity, int dischargeRate = 2) {
        // Ensure intersections exist
        addIntersection(src);
        addIntersection(dest);

        Road* newRoad = new Road(src, dest, length, maxSpeed, capacity, dischargeRate);
        roads.push_back(newRoad);

        // Link edge to nodes
        intersections[dest]->addIncomingRoad(src, newRoad);
        intersections[src]->addOutgoingRoad(dest, newRoad);
    }

    void addVehicle(int id, int src, int dest) {
        Vehicle* v = new Vehicle(id, src, dest);
        vector<int> path = calculateShortestPath(src, dest);
        v->setPath(path);

        double freeTime = 0.0;
        if (path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; ++i) {
                Intersection* curr = intersections[path[i]];
                auto it = curr->getOutgoingRoads().find(path[i + 1]);
                if (it != curr->getOutgoingRoads().end()) {
                    freeTime += it->second->getFreeTravelTime();
                }
            }
        }
        v->setFreeFlowTravelTime(freeTime);

        vehicles.push_back(v);
    }

    // 4.7 Routing Model (Implemented for Day 2)
    // Uses Dijkstra's algorithm based on current w_ij(t)
    vector<int> calculateShortestPath(int source, int destination) {
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        unordered_map<int, double> minCost;
        unordered_map<int, int> previousNode;

        for (const auto& pair : intersections) {
            minCost[pair.first] = numeric_limits<double>::infinity();
        }

        minCost[source] = 0.0;
        pq.push({ 0.0, source });

        while (!pq.empty()) {
            auto current = pq.top();
            double currentCost = current.first;
            int u = current.second;
            pq.pop();

            if (u == destination) break;
            if (currentCost > minCost[u]) continue;

            Intersection* currIntersection = intersections[u];
            for (const auto& pair : currIntersection->getOutgoingRoads()) {
                int v = pair.first;
                Road* r = pair.second;
                double weight = r->getCurrentTravelTime(); // w_ij(t)

                if (minCost[u] + weight < minCost[v]) {
                    minCost[v] = minCost[u] + weight;
                    previousNode[v] = u;
                    pq.push({ minCost[v], v });
                }
            }
        }

        vector<int> path;
        int curr = destination;

        if (minCost[curr] == numeric_limits<double>::infinity()) {
            return path; // No path found
        }

        while (curr != source) {
            path.push_back(curr);
            curr = previousNode[curr];
        }
        path.push_back(source);

        reverse(path.begin(), path.end());
        return path;
    }
};
