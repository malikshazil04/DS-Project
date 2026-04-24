#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Road.h"

using namespace std;

class Intersection {
private:
    int id;
    // Map of incoming road source ID -> Road pointer
    // Used to control traffic signals for roads entering this intersection
    unordered_map<int, Road*> incomingRoads;

    // Map of outgoing roads: dest ID -> Road pointer
    unordered_map<int, Road*> outgoingRoads;

public:
    Intersection(int id) : id(id) {}
    ~Intersection() {}

    // Getters
    int getId() const { return id; }
    const unordered_map<int, Road*>& getIncomingRoads() const { return incomingRoads; }
    const unordered_map<int, Road*>& getOutgoingRoads() const { return outgoingRoads; }

    // Modifiers for roads
    void addIncomingRoad(int srcId, Road* road) { incomingRoads[srcId] = road; }
    void addOutgoingRoad(int destId, Road* road) { outgoingRoads[destId] = road; }

    // 4.8 Traffic Signal Model (Adaptive)
    void updateSignals() {
        if (incomingRoads.empty()) return;

        int maxQueueLength = -1;
        Road* roadToGetGreen = nullptr;

        // Find the incoming road with the longest queue
        for (auto& pair : incomingRoads) {
            pair.second->setGreenSignal(false); // Turn all to Red (0) initially

            int qSize = pair.second->getQueueLength();
            if (qSize > maxQueueLength) {
                maxQueueLength = qSize;
                roadToGetGreen = pair.second;
            }
        }

        // Give Green (1) to the road with the maximum queue
        if (roadToGetGreen != nullptr) {
            roadToGetGreen->setGreenSignal(true);
        }
    }
};
