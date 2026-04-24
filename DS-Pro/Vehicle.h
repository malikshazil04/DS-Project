#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Road; // Forward declaration

enum VehicleStatus { MOVING, WAITING, ARRIVED };

class Vehicle {
private:
    int id;
    int sourceId;          // s_v
    int destinationId;     // d_v

    Road* currentEdge;     // e_ij
    double remainingTime;  // r_v(t)
    VehicleStatus status;

    vector<int> path;      // Shortest path P_v (Sequence of intersection IDs)
    int pathIndex;         // Current progress in the path

public:
    Vehicle(int id, int src, int dest) {
        this->id = id;
        this->sourceId = src;
        this->destinationId = dest;
        this->currentEdge = nullptr;
        this->remainingTime = 0.0;
        this->status = WAITING;
        this->pathIndex = 0;
    }
    ~Vehicle() {}

    // Getters
    int getId() const { return id; }
    int getSourceId() const { return sourceId; }
    int getDestinationId() const { return destinationId; }
    Road* getCurrentEdge() const { return currentEdge; }
    double getRemainingTime() const { return remainingTime; }
    VehicleStatus getStatus() const { return status; }
    vector<int> getPath() const { return path; }
    int getPathIndex() const { return pathIndex; }

    // Setters
    void setCurrentEdge(Road* edge) { currentEdge = edge; }
    void setRemainingTime(double time) { remainingTime = time; }
    void setStatus(VehicleStatus s) { status = s; }
    void setPath(const vector<int>& newPath) { path = newPath; }
    void setPathIndex(int index) { pathIndex = index; }

    // CEP Core Functions
    // Decrement remaining travel time: r_v(t + 1) = r_v(t) - 1
    void updateRemainingTime(double timeStep = 1.0) {
        if (status == MOVING) {
            remainingTime -= timeStep;
            if (remainingTime <= 0) {
                remainingTime = 0;
                status = WAITING;
            }
        }
    }

    bool isWaitingAtIntersection() const {
        return status == WAITING && remainingTime <= 0;
    }

    string getStatusString() const {
        switch (status) {
        case MOVING: return "MOVING";
        case WAITING: return "WAITING";
        case ARRIVED: return "ARRIVED";
        default: return "UNKNOWN";
        }
    }

    // Display
    void display() const {
        cout << "[Vehicle " << id << "] "
            << "Path: " << sourceId << "->" << destinationId
            << " | Status: " << getStatusString()
            << " | Remaining Time: " << remainingTime << "s"
            << endl;
    }
};
