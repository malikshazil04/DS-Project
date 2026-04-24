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

    // Main Simulation Step (Stub for Day 3)
    // Executes: 
    // 1. Update Traffic Flow
    // 2. Recalculate Congestion and Travel Times
    // 3. Compute/Update routes
    // 4. Adjust Traffic Signals
    // 5. Record Metrics
    void simulateStep() {
        metrics.incrementSimulationTime();
        // Logic to be implemented in Day 3
    }

    void runSimulation(int totalSteps) {
        for (int i = 0; i < totalSteps; i++) {
            simulateStep();
        }
    }
};
