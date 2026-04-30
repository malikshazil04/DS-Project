#pragma once
// Member 3 - MetricsCollector
// Responsibility: Record and report all CEP performance metrics (Section 4.10)
// Metrics: Average Travel Time, Total Delay, Throughput, Average Congestion

#include <iostream>
#include <vector>
#include <iomanip>
#include "RoadNetwork.h"
using namespace std;

class MetricsCollector
{
private:
    // Vehicles that reached destination
    int totalArrivedVehicles;

    // Sum of all actual travel times (for avg calculation)
    double totalTravelTime;

    // Sum of all delays = actual time - free flow time
    double totalDelay;

    // History of congestion per step (for reporting trends)
    vector<double> congestionHistory;

    // History of arrived count per step (for throughput)
    vector<int> arrivedHistory;

    // Total simulation steps run
    int totalSteps;

public:
    MetricsCollector()
    {
        totalArrivedVehicles = 0;
        totalTravelTime = 0.0;
        totalDelay = 0.0;
        totalSteps = 0;
    }

    // Called when a vehicle arrives at destination
    // actualTime = steps it took, freeFlowTime = ideal time without congestion
    void recordArrival(int actualTime, double freeFlowTime)
    {
        totalArrivedVehicles++;
        totalTravelTime += actualTime;
        double delay = actualTime - freeFlowTime;
        if (delay < 0) delay = 0;
        totalDelay += delay;
    }

    // Called each step to snapshot congestion level across all roads
    void recordStepMetrics(const RoadNetwork& network, int arrivedThisStep)
    {
        totalSteps++;
        arrivedHistory.push_back(arrivedThisStep);

        // Average congestion across all roads this step
        const auto& allRoads = network.getAllRoads();
        if (allRoads.empty())
        {
            congestionHistory.push_back(0.0);
            return;
        }
        double totalCongestion = 0.0;
        for (const auto& r : allRoads)
        {
            totalCongestion += r.second->getCongestion();
        }
        congestionHistory.push_back(totalCongestion / allRoads.size());
    }

    // === Metric Computations (Section 4.10) ===

    // Average Travel Time = (1/N) * sum(T_sd)
    double getAverageTravelTime() const
    {
        if (totalArrivedVehicles == 0) return 0.0;
        return totalTravelTime / totalArrivedVehicles;
    }

    // Total Delay = sum(T_sd - T_sd_free)
    double getTotalDelay() const
    {
        return totalDelay;
    }

    // Throughput = vehicles arrived / total time steps
    double getThroughput() const
    {
        if (totalSteps == 0) return 0.0;
        return (double)totalArrivedVehicles / totalSteps;
    }

    // Average Congestion = (1/|E|) * sum(f_ij / c_ij)
    double getAverageCongestion() const
    {
        if (congestionHistory.empty()) return 0.0;
        double sum = 0.0;
        for (double c : congestionHistory) sum += c;
        return sum / congestionHistory.size();
    }

    // CEP Section 4.9 - Objective Function
    // Evaluates: min ΣΣ [ α*Q_ij(t) + β*(f_ij(t)/c_ij)² ]
    // Lower value = better traffic state (less queue + less congestion)
    double computeObjectiveFunction(const RoadNetwork& network,
        double alpha = 1.0, double beta = 1.0) const
    {
        double objective = 0.0;
        for (const auto& rPair : network.getAllRoads())
        {
            Road* r = rPair.second;
            double queueTerm = alpha * r->getQueueSize();
            double rho = r->getCongestion(); // f_ij / c_ij
            double congTerm = beta * rho * rho;
            objective += queueTerm + congTerm;
        }
        return objective;
    }

    int getTotalArrived() const { return totalArrivedVehicles; }
    int getTotalSteps() const { return totalSteps; }

    // Print the final performance report
    void printReport(int totalVehicles) const
    {
        cout << "\n";
        cout << "============================================================\n";
        cout << "         TRAFFIC FLOW OPTIMIZATION - FINAL REPORT           \n";
        cout << "============================================================\n";
        cout << fixed << setprecision(2);
        cout << "  Total Simulation Steps    : " << totalSteps << "\n";
        cout << "  Total Vehicles            : " << totalVehicles << "\n";
        cout << "  Vehicles Arrived          : " << totalArrivedVehicles << "\n";
        cout << "  Vehicles Still En Route   : " << totalVehicles - totalArrivedVehicles << "\n";
        cout << "------------------------------------------------------------\n";
        cout << "  Avg Travel Time           : " << getAverageTravelTime() << " steps\n";
        cout << "  Total Delay               : " << getTotalDelay() << " steps\n";
        cout << "  System Throughput         : " << getThroughput() << " vehicles/step\n";
        cout << "  Avg Network Congestion    : " << getAverageCongestion() * 100 << "%\n";
        cout << "============================================================\n";
    }

    // Print step-by-step congestion trend (for viva demo)
    void printCongestionHistory() const
    {
        cout << "\n[Congestion History per Step]\n";
        for (int i = 0; i < (int)congestionHistory.size(); i++)
        {
            cout << "  Step " << setw(3) << i + 1 << ": ";
            int bars = (int)(congestionHistory[i] * 20);
            for (int b = 0; b < bars; b++) cout << "#";
            cout << " " << congestionHistory[i] * 100 << "%\n";
        }
    }
};
