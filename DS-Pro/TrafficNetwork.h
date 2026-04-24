#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
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
        vehicles.push_back(new Vehicle(id, src, dest));
    }

    // 4.7 Routing Model (Stub for Day 2)
    // Uses Dijkstra's algorithm based on current w_ij(t)
    vector<int> calculateShortestPath(int source, int destination) {
        // To be implemented in Day 2
        return vector<int>();
    }
};
