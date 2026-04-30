#include <iostream>
#include "TrafficNetwork.h"
#include "TrafficSimulator.h"
#include "MetricsCollector.h"


using namespace std;

int main() {
    cout << "==========================================" << endl;
    cout << "      Traffic Flow Optimization System    " << endl;
    cout << "==========================================" << endl;

    TrafficNetwork network;

    // 1. Create a city map (Intersections)
    // 0: Suburb A
    // 1: Highway Entrance
    // 2: Downtown Junction
    // 3: Industrial Park
    for (int i = 0; i < 4; i++){
        network.addIntersection(i);
    }

    // 2. Add Roads
    // addRoad(src, dest, length, maxSpeed, capacity, dischargeRate)
    network.addRoad(0, 8, 10.0, 1.0, 10, 2); // 0 -> 1 (Travel time: 10s)
    network.addRoad(0, 6, 15.0, 1.0, 8, 2);  // 0 -> 6 (Travel time: 15s)
    network.addRoad(1, 2, 5.0, 1.0, 5, 1);  // 1 -> 2 (Travel time: 5s)
    network.addRoad(1, 6, 20.0, 1.0, 15, 3); // 1 -> 3 (Travel time: 20s)
    network.addRoad(2, 3, 10.0, 1.0, 10, 2); // 2 -> 3 (Travel time: 10s)

    // 3. Add Vehicles
    // Generating vehicles from Suburb A (0) to Industrial Park (3)
    for (int i = 1; i <= 5; i++) {
        network.addVehicle(i, 0, 3);
    }
    // Vehicles from 0 to 2
    for (int i = 6; i <= 8; i++) {
        network.addVehicle(i, 0, 2);
    }

    // 4. Initialize Simulator
    TrafficSimulator simulator(&network);

    cout << "\n[Initial State] Vehicles Shortest Paths (Computed via Dijkstra):" << endl;
    for (Vehicle* v : network.getVehicles()) {
        cout << "Vehicle " << v->getId() << " Path: ";
        for (int node : v->getPath()) {
            cout << node << " ";
        }
        cout << endl;
    }

    // 5. Run Simulation
    int simulationSteps = 45;
    cout << "\n--- Starting Simulation (" << simulationSteps << " steps) ---" << endl;
    for (int step = 1; step <= simulationSteps; step++) {
        simulator.simulateStep();

        // Print detailed info every 15 steps
        if (step % 15 == 0 || step == simulationSteps) {
            cout << "\n[Step " << step << "]" << endl;
            for (Road* r : network.getRoads()) {
                r->display();
            }
        }
    }

    // 6. Display Metrics
    cout << "\n==========================================" << endl;
    cout << "       Final Performance Metrics          " << endl;
    cout << "==========================================" << endl;
    MetricsCollector& metrics = simulator.getMetrics();
    cout << "Total Vehicles Arrived: " << metrics.getTotalVehiclesReached() << " out of " << network.getVehicles().size() << endl;
    cout << "Average Travel Time:    " << metrics.getAverageTravelTime() << " time steps" << endl;
    cout << "Total Delay:            " << metrics.getTotalDelay() << " time steps" << endl;
    cout << "System Throughput:      " << metrics.getThroughput() << " vehicles/step" << endl;
    cout << "Avg Network Congestion: " << metrics.getAverageCongestion(network.getRoads()) * 100 << "%" << endl;
    cout << "==========================================" << endl;

    return 0;
}
