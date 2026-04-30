#pragma once
// Member 3 - InputHandler
// Responsibility: Safe user input + city network configuration
// Validates all user inputs before they enter the simulation

#include <iostream>
#include <string>
#include <limits>
#include "RoadNetwork.h"
#include "TrafficSignal.h"
#include "Vehicle.h"
using namespace std;

class InputHandler
{
public:
    // === Safe Input Helpers ===

    // Read a positive integer from user with prompt
    static int readPositiveInt(const string& prompt)
    {
        int val;
        while (true)
        {
            cout << prompt;
            if (cin >> val && val > 0)
                return val;
            cout << "  [!] Invalid input. Please enter a positive integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Read a non-negative integer
    static int readNonNegInt(const string& prompt)
    {
        int val;
        while (true)
        {
            cout << prompt;
            if (cin >> val && val >= 0)
                return val;
            cout << "  [!] Invalid input. Please enter 0 or a positive integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Read a positive double
    static double readPositiveDouble(const string& prompt)
    {
        double val;
        while (true)
        {
            cout << prompt;
            if (cin >> val && val > 0.0)
                return val;
            cout << "  [!] Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Read integer in a given range [lo, hi]
    static int readIntInRange(const string& prompt, int lo, int hi)
    {
        int val;
        while (true)
        {
            cout << prompt;
            if (cin >> val && val >= lo && val <= hi)
                return val;
            cout << "  [!] Invalid. Please enter a value between " << lo << " and " << hi << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // === Network Setup ===

    // Interactively build the RoadNetwork from user
    static void buildNetworkFromUser(RoadNetwork& network)
    {
        cout << "\n--- Road Network Setup ---\n";
        int numRoads = readPositiveInt("Enter number of roads to add: ");

        for (int i = 0; i < numRoads; i++)
        {
            cout << "\n  [Road " << i + 1 << "]\n";
            int id = readPositiveInt("    Road ID       : ");
            int from = readNonNegInt("    From Node ID  : ");
            int to = readNonNegInt("    To Node ID    : ");

            if (from == to)
            {
                cout << "  [!] A road cannot start and end at the same node. Skipping.\n";
                i--;
                continue;
            }

            double len = readPositiveDouble("    Length (km)   : ");
            double speed = readPositiveDouble("    Max Speed     : ");
            int    cap = readPositiveInt("    Capacity      : ");

            network.addRoad(id, from, to, len, speed, cap);
            cout << "  [+] Road " << id << " added: " << from << " -> " << to << "\n";
        }
    }

    // Setup traffic signals for each intersection's incoming roads
    static void buildSignalsFromUser(vector<TrafficSignal>& signals, const RoadNetwork& network)
    {
        cout << "\n--- Traffic Signal Setup ---\n";
        const auto& allIntersections = network.getAllIntersections();

        for (const auto& pair : allIntersections)
        {
            int nodeId = pair.first;
            cout << "\n  Intersection " << nodeId << ":\n";
            int duration = readPositiveInt("    Green duration (steps): ");
            int threshold = readNonNegInt("    Queue threshold       : ");

            TrafficSignal sig(nodeId, duration, threshold);

            // Gather incoming road IDs from user
            int numIncoming = readNonNegInt("    Number of incoming roads: ");
            for (int j = 0; j < numIncoming; j++)
            {
                int roadId = readPositiveInt("      Incoming Road ID: ");
                sig.addIncomingRoad(roadId);
            }

            signals.push_back(sig);
        }
    }

    // Create vehicles interactively
    static vector<Vehicle> buildVehiclesFromUser()
    {
        vector<Vehicle> vehicles;
        cout << "\n--- Vehicle Setup ---\n";
        int numVehicles = readPositiveInt("Enter number of vehicles: ");

        for (int i = 0; i < numVehicles; i++)
        {
            cout << "  [Vehicle " << i + 1 << "]\n";
            int src = readNonNegInt("    Source Node      : ");
            int dest = readNonNegInt("    Destination Node : ");

            if (src == dest)
            {
                cout << "  [!] Source and destination must differ. Skipping.\n";
                i--;
                continue;
            }

            vehicles.push_back(Vehicle(src, dest));
        }
        return vehicles;
    }

    // Ask user whether to use preset or manual input
    static bool askUsePreset()
    {
        char choice;
        cout << "\nUse built-in preset city network? (y/n): ";
        while (true)
        {
            cin >> choice;
            if (choice == 'y' || choice == 'Y') return true;
            if (choice == 'n' || choice == 'N') return false;
            cout << "  [!] Enter 'y' or 'n': ";
        }
    }

    // Ask how many simulation steps to run
    static int askSimulationSteps()
    {
        return readPositiveInt("\nEnter number of simulation steps to run: ");
    }
};
