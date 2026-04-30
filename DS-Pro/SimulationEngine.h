#pragma once

// SimulationEngine class handles by managing vehicles,
// updating their travel time step by step, updating traffic signals, and applying
// simple movement/waiting/arrival decisions during the simulation.

#include <iostream>
#include <vector>
#include "Vehicle.h"
#include "TrafficSignal.h"
using namespace std;

class SimulationEngine
{
private:
    vector<Vehicle> vehicles;          // all vehicles in the simulation
    vector<TrafficSignal> signals;     // all traffic signals
    int currentStep;                   // current simulation step

public:
    // Default constructor
    SimulationEngine()
    {
        currentStep = 0;
    }

    // Add a vehicle to the simulation
    void addVehicle(const Vehicle& v)
    {
        vehicles.push_back(v);
    }

    // Add a traffic signal to the simulation
    void addTrafficSignal(const TrafficSignal& s)
    {
        signals.push_back(s);
    }

    // Get all vehicles
    vector<Vehicle>& getVehicles()
    {
        return vehicles;
    }

    // Get all traffic signals
    vector<TrafficSignal>& getSignals()
    {
        return signals;
    }

    // Get current simulation step
    int getCurrentStep() const
    {
        return currentStep;
    }

    // Update remaining travel time of all moving vehicles
    void updateVehicleTimes()
    {
        for (int i = 0; i < vehicles.size(); i++)
        {
            if (vehicles[i].isMoving())
            {
                vehicles[i].updateRemainingTime();
            }
        }
    }

    // Update all traffic signals using queue sizes
    // Each signal gets its own queue-size vector in matching order of incomingRoadIds
    void updateAllSignals(const vector<vector<int>>& allQueueSizes)
    {
        if (signals.size() != allQueueSizes.size())
        {
            return;
        }

        for (int i = 0; i < signals.size(); i++)
        {
            signals[i].updateSignal(allQueueSizes[i]);
        }
    }

    // Check whether a vehicle's current road has green signal at a specific signal
    bool canVehicleMove(int signalIndex, int roadId) const
    {
        if (signalIndex < 0 || signalIndex >= signals.size())
        {
            return false;
        }

        return signals[signalIndex].isGreen(roadId);
    }

    // Put vehicle on a road
    void moveVehicleToRoad(int vehicleIndex, int roadId, int travelTime)
    {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size())
        {
            return;
        }

        vehicles[vehicleIndex].enterEdge(roadId, travelTime);
    }

    // Make vehicle wait at intersection
    void makeVehicleWait(int vehicleIndex)
    {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size())
        {
            return;
        }

        vehicles[vehicleIndex].waitAtIntersection();
    }

    // Mark vehicle as arrived
    void markVehicleArrived(int vehicleIndex)
    {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size())
        {
            return;
        }

        vehicles[vehicleIndex].arrive();
    }

    // Process one vehicle after it reaches an intersection
    // reachedDestination = true if this was the final destination
    // greenSignal = true if next road is allowed to move
    void processIntersectionArrival(int vehicleIndex, bool greenSignal, int nextRoadId, int nextTravelTime, bool reachedDestination)
    {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size())
        {
            return;
        }

        if (!vehicles[vehicleIndex].hasReachedIntersection())
        {
            return;
        }

        if (reachedDestination)
        {
            markVehicleArrived(vehicleIndex);
        }
        else if (greenSignal)
        {
            moveVehicleToRoad(vehicleIndex, nextRoadId, nextTravelTime);
        }
        else
        {
            makeVehicleWait(vehicleIndex);
        }
    }

    // Count arrived vehicles
    int countArrivedVehicles() const
    {
        int count = 0;

        for (int i = 0; i < vehicles.size(); i++)
        {
            if (vehicles[i].isArrived())
            {
                count++;
            }
        }

        return count;
    }

    // Run one simulation step for Member 2 part
    void runStep(const vector<vector<int>>& allQueueSizes)
    {
        updateAllSignals(allQueueSizes);
        updateVehicleTimes();
        currentStep++;
    }

    // Display full simulation state
    void displaySimulationState() const
    {
        cout << "Simulation Step: " << currentStep << "\n\n";

        cout << "Vehicles:\n";
        for (int i = 0; i < vehicles.size(); i++)
        {
            vehicles[i].displayVehicle();
            cout << "\n";
        }

        cout << "Traffic Signals:\n";
        for (int i = 0; i < signals.size(); i++)
        {
            signals[i].displaySignal();
            cout << "\n";
        }
    }
};