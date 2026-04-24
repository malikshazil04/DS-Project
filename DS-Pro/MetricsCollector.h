#pragma once
#include <vector>
#include "Road.h"

using namespace std;

// ==========================================
// 4.10 Performance Metrics Collector
// ==========================================
class MetricsCollector {
private:
    int totalVehiclesReached;
    double totalTravelTime;
    double totalDelay;
    int simulationTime;

public:
    MetricsCollector() : totalVehiclesReached(0), totalTravelTime(0.0), totalDelay(0.0), simulationTime(0) {}
    ~MetricsCollector() {}

    // Getters
    int getTotalVehiclesReached() const { return totalVehiclesReached; }
    double getTotalTravelTime() const { return totalTravelTime; }
    double getTotalDelay() const { return totalDelay; }
    int getSimulationTime() const { return simulationTime; }

    // Setters / Updaters
    void incrementSimulationTime() { simulationTime++; }

    void recordVehicleArrival(double actualTime, double freeFlowTime) {
        totalVehiclesReached++;
        totalTravelTime += actualTime;
        totalDelay += (actualTime - freeFlowTime);
    }

    // 4.10 Calculated Metrics
    double getAverageTravelTime() const {
        return totalVehiclesReached == 0 ? 0.0 : totalTravelTime / totalVehiclesReached;
    }

    double getThroughput() const {
        return simulationTime == 0 ? 0.0 : (double)totalVehiclesReached / simulationTime;
    }

    double getAverageCongestion(const vector<Road*>& roads) const {
        if (roads.empty()) return 0.0;
        double totalCongestion = 0.0;
        for (Road* r : roads) {
            totalCongestion += r->getCongestionLevel();
        }
        return totalCongestion / roads.size();
    }
};
