#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include "RoadNetwork.h"
using namespace std;

class MetricsCollector
{
private:
    int totalArrivedVehicles;
    double totalTravelTime;
    double totalDelay;
    int totalSteps;

    // History of congestion per step (for trends)
    vector<double> congestionHistory;
    // History of arrived count per step (for throughput)
    vector<int> arrivedHistory;

public:
    MetricsCollector()
    {
        totalArrivedVehicles = 0;
        totalTravelTime = 0.0;
        totalDelay = 0.0;
        totalSteps = 0;
    }

    // Called when a vehicle arrives at destination
    void recordArrival(int actualTime, double freeFlowTime)
    {
        totalArrivedVehicles++;
        totalTravelTime += actualTime;
        double delay = actualTime - freeFlowTime;
        if (delay < 0) delay = 0;
        totalDelay += delay;
    }

    //  Record congestion level across all roads
    void recordStepMetrics(const RoadNetwork& network, int arrivedThisStep)
    {
        totalSteps++;
        arrivedHistory.push_back(arrivedThisStep);

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


    double getAverageTravelTime() const
    {
        if (totalArrivedVehicles == 0) return 0.0;
        return totalTravelTime / totalArrivedVehicles;
    }

    double getTotalDelay() const
    {
        return totalDelay;
    }

    double getThroughput() const
    {
        if (totalSteps == 0) return 0.0;
        return (double)totalArrivedVehicles / totalSteps;
    }

    double getAverageCongestion() const
    {
        if (congestionHistory.empty()) return 0.0;
        double sum = 0.0;
        for (double c : congestionHistory) sum += c;
        return sum / congestionHistory.size();
    }

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
    void printReport(int totalVehicles) const
    {
        cout << getReportString(totalVehicles);
    }
    string getReportString(int totalVehicles) const
    {
        ostringstream oss;
        oss << "\n";
        oss << "============================================================\n";
        oss << "         TRAFFIC FLOW OPTIMIZATION - FINAL REPORT           \n";
        oss << "============================================================\n";
        oss << fixed << setprecision(2);
        oss << "  Total Simulation Steps    : " << totalSteps << "\n";
        oss << "  Total Vehicles            : " << totalVehicles << "\n";
        oss << "  Vehicles Arrived          : " << totalArrivedVehicles << "\n";
        oss << "  Vehicles Still En Route   : " << totalVehicles - totalArrivedVehicles << "\n";
        oss << "------------------------------------------------------------\n";
        oss << "  Avg Travel Time           : " << getAverageTravelTime() << " steps\n";
        oss << "  Total Delay               : " << getTotalDelay() << " steps\n";
        oss << "  System Throughput         : " << getThroughput() << " vehicles/step\n";
        oss << "  Avg Network Congestion    : " << getAverageCongestion() * 100 << "%\n";
        oss << "============================================================\n";
        return oss.str();
    }

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
