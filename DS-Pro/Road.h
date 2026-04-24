#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <queue>
#include "Vehicle.h"

using namespace std;

class Road {
private:
    int sourceId;               // Node i
    int destId;                 // Node j

    // Physical Parameters
    double length;              // l_ij
    double maxSpeed;            // v_ij^max
    int capacity;               // c_ij
    int dischargeRate;          // mu_ij: Maximum discharge rate

    // Dynamic Traffic Parameters
    int currentFlow;            // f_ij(t)
    queue<Vehicle*> waitingVehicles; // Q_ij(t): The physical queue of vehicles at the intersection
    double currentTravelTime;   // w_ij(t)
    double freeTravelTime;      // w_ij^free

    // Traffic Signal Control
    bool greenSignal;           // g_ij(t)

    // Congestion/Travel Time parameters
    double alpha;
    double beta;

public:
    Road(int src, int dest, double len, double speed, int cap, int discharge = 2, double a = 0.15, double b = 4.0) {
        this->sourceId = src;
        this->destId = dest;
        this->length = len;
        this->maxSpeed = speed;
        this->capacity = cap;
        this->dischargeRate = discharge;

        this->currentFlow = 0;
        this->freeTravelTime = len / speed; // w_ij^free
        this->currentTravelTime = this->freeTravelTime;

        this->greenSignal = false;
        this->alpha = a;
        this->beta = b;
    }
    ~Road() {}

    // Getters
    int getSourceId() const { return sourceId; }
    int getDestId() const { return destId; }
    double getLength() const { return length; }
    double getMaxSpeed() const { return maxSpeed; }
    int getCapacity() const { return capacity; }
    int getDischargeRate() const { return dischargeRate; }
    int getCurrentFlow() const { return currentFlow; }
    int getQueueLength() const { return waitingVehicles.size(); }
    double getCurrentTravelTime() const { return currentTravelTime; }
    double getFreeTravelTime() const { return freeTravelTime; }
    bool hasGreenSignal() const { return greenSignal; }

    // Setters
    void setCurrentFlow(int flow) { currentFlow = flow; }
    void setGreenSignal(bool status) { greenSignal = status; }

    // CEP Core Functions
    // Add vehicle to road flow: a_ij(t)
    void enterRoad() {
        currentFlow++;
    }

    // Move vehicle from road flow into the queue at the end of the road
    void enqueueVehicleToIntersection(Vehicle* vehicle) {
        waitingVehicles.push(vehicle);
        currentFlow--; // Leaves the moving road, enters the waiting queue
        if (currentFlow < 0) currentFlow = 0;
    }

    // Removes and returns the next vehicle ready to leave the intersection (d_ij)
    Vehicle* dequeueVehicleFromIntersection() {
        if (!waitingVehicles.empty()) {
            Vehicle* v = waitingVehicles.front();
            waitingVehicles.pop();
            return v;
        }
        return nullptr;
    }

    Vehicle* peekFrontVehicle() const {
        if (waitingVehicles.empty()) return nullptr;
        return waitingVehicles.front();
    }

    // Calculates allowed discharge formula: d_ij(t) = g_ij(t) * min(Q_ij(t), mu_ij, c_jk - f_jk(t))
    int calculateAllowedDischarge(int nextRoadAvailableCapacity) const {
        if (!greenSignal) return 0; // Red signal blocks movement (g_ij(t) = 0)

        int currentQ = getQueueLength();
        int minDischarge = min(currentQ, min(dischargeRate, nextRoadAvailableCapacity));
        return minDischarge;
    }

    // Get Congestion level: rho_ij = f_ij / c_ij
    double getCongestionLevel() const {
        if (capacity <= 0) return 1.0;
        return static_cast<double>(currentFlow) / capacity;
    }

    // Update Travel Time: w_ij(t) = w_ij^free * (1 + alpha * (f_ij(t) / c_ij)^beta)
    void updateTravelTime() {
        double congestionRatio = getCongestionLevel();
        currentTravelTime = freeTravelTime * (1.0 + alpha * pow(congestionRatio, beta));
    }

    // Display
    void display() const {
        cout << "[Road " << sourceId << "->" << destId << "] "
            << "Flow: " << currentFlow << "/" << capacity
            << " | Queue: " << getQueueLength()
            << " | Discharge Rate: " << dischargeRate
            << " | Congestion: " << fixed << setprecision(2) << getCongestionLevel() * 100 << "%"
            << " | Travel Time: " << currentTravelTime << "s"
            << " | Signal: " << (greenSignal ? "GREEN" : "RED")
            << endl;
    }
};
