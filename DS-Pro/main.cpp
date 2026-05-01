// ============================================================
//  Traffic Flow Optimization System — main.cpp
//  Member 3: Setup + Run + Report
//  All simulation logic is inside SimulationEngine.
// ============================================================

#include <iostream>
#include <iomanip>
#include "RoadNetwork.h"
#include "RoutePlanner.h"
#include "SimulationEngine.h"
#include "InputHandler.h"

using namespace std;

// ============================================================
//  Preset city network
//  6 intersections, 8 roads, 8 vehicles (multi-source/dest)
//
//   0 ---[1]--> 1 ---[2]--> 2
//   |           |           |
//  [3]         [4]         [5]
//   v           v           v
//   3 ---[6]--> 4 ---[7]--> 5
//   ^
//  [8] (back road 3->0)
// ============================================================
void loadPreset(RoadNetwork& network, SimulationEngine& engine)
{
    // --- Roads: addRoad(id, from, to, length, maxSpeed, capacity, dischargeRate) ---
    network.addRoad(1, 0, 1, 10.0, 2.0, 8, 2);
    network.addRoad(2, 1, 2, 10.0, 2.0, 8, 2);
    network.addRoad(3, 0, 3, 10.0, 2.0, 6, 2);
    network.addRoad(4, 1, 4, 10.0, 1.5, 5, 1);  // bottleneck
    network.addRoad(5, 2, 5, 10.0, 2.0, 6, 2);
    network.addRoad(6, 3, 4, 10.0, 2.0, 8, 2);
    network.addRoad(7, 4, 5, 10.0, 2.0, 8, 2);
    network.addRoad(8, 3, 0, 5.0, 2.0, 4, 1);  // limited back road

    // --- Traffic Signals (one per destination intersection) ---
    TrafficSignal s1(1, 4, 2); s1.addIncomingRoad(1);
    TrafficSignal s2(2, 4, 2); s2.addIncomingRoad(2);
    TrafficSignal s3(3, 3, 1); s3.addIncomingRoad(3);
    TrafficSignal s4(4, 3, 2); s4.addIncomingRoad(4); s4.addIncomingRoad(6);
    TrafficSignal s5(5, 4, 2); s5.addIncomingRoad(5); s5.addIncomingRoad(7);

    engine.addTrafficSignal(s1);
    engine.addTrafficSignal(s2);
    engine.addTrafficSignal(s3);
    engine.addTrafficSignal(s4);
    engine.addTrafficSignal(s5);

    // --- Vehicles: Vehicle(id, source, destination) ---
    engine.addVehicle(Vehicle(1, 0, 5));
    engine.addVehicle(Vehicle(2, 0, 5));
    engine.addVehicle(Vehicle(3, 0, 2));
    engine.addVehicle(Vehicle(4, 0, 4));
    engine.addVehicle(Vehicle(5, 3, 5));
    engine.addVehicle(Vehicle(6, 3, 2));
    engine.addVehicle(Vehicle(7, 0, 5));
    engine.addVehicle(Vehicle(8, 1, 5));
}

// ============================================================
//  MAIN
// ============================================================
int main()
{
    cout << "============================================================\n";
    cout << "      Traffic Flow Optimization System (Graph Theory)       \n";
    cout << "============================================================\n";

    // --- Build subsystems (Member 1) ---
    RoadNetwork  network;
    RoutePlanner planner(&network);

    // --- Create engine (Member 3 integration hub) ---
    SimulationEngine engine(&network, &planner, /*rerouteEvery=*/5);

    // --- Input ---
    bool usePreset = InputHandler::askUsePreset();

    if (usePreset)
    {
        loadPreset(network, engine);
        cout << "\n[+] Preset loaded: 6 nodes, 8 roads, 8 vehicles.\n";
    }
    else
    {
        // Manual input: build network first, then add signals & vehicles to engine
        InputHandler::buildNetworkFromUser(network);

        vector<TrafficSignal> signals;
        InputHandler::buildSignalsFromUser(signals, network);
        for (const TrafficSignal& s : signals) engine.addTrafficSignal(s);

        vector<Vehicle> vehicles = InputHandler::buildVehiclesFromUser();
        for (const Vehicle& v : vehicles) engine.addVehicle(v);
    }

    // --- Compute initial shortest paths for all vehicles ---
    engine.initializeRoutes();

    // --- Run ---
    int steps = InputHandler::askSimulationSteps();
    engine.run(steps);

    // --- Final Reports ---
    engine.getMetrics().printReport(engine.getTotalVehicles());

    cout << fixed << setprecision(4);
    cout << "\n  Objective Function Value  : "
        << engine.getMetrics().computeObjectiveFunction(network) << "\n";
    cout << "  (min ΣΣ[α·Q_ij + β·(f_ij/c_ij)²]  — lower is better)\n";

    engine.getMetrics().printCongestionHistory();
    engine.displayRoadState();

    return 0;
}