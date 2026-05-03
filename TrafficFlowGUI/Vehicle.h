#pragma once
// Vehicle class models a single vehicle in the traffic flow optimization system and manages its
// movement through the network by tracking road entry, travel progress, intersection arrival,
// and status changes during the simulation.
#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
private:
    int vehicleId;       // unique ID per CEP Section 4.6
    int source;          // starting node s_v
    int destination;     // ending node d_v
    int currentEdge;     // current road/edge id
    int remainingTravelTime; // r_v(t)
    string status;       // MOVING, WAITING, ARRIVED

public:
    Vehicle() // default constructor
    {
        vehicleId = -1;
        source = -1;
        destination = -1;
        currentEdge = -1;
        remainingTravelTime = 0;
        status = "WAITING";
    }
    Vehicle(int src, int dest)
    {
        vehicleId = -1;
        source = src;
        destination = dest;
        currentEdge = -1;
        remainingTravelTime = 0;
        status = "WAITING";
    }
    Vehicle(int id, int src, int dest)
    {
        vehicleId = id;
        source = src;
        destination = dest;
        currentEdge = -1;
        remainingTravelTime = 0;
        status = "WAITING";
    }

    void setVehicleId(int id) { vehicleId = id; }
    void setSource(int src)
    {
        source = src;
    }
    void setDestination(int des)
    {
        destination = des;
    }
    void setCurrentEdge(int edge)
    {
        currentEdge = edge;
    }
    void setRemainingTravelTime(int time)
    {
        remainingTravelTime = time;
    }
    void setStatus(string s)
    {
        status = s;
    }
    // Getters
    int getVehicleId() const { return vehicleId; }

    int getSource() const
    {
        return source;
    }

    int getDestination() const
    {
        return destination;
    }

    int getCurrentEdge() const
    {
        return currentEdge;
    }

    int getRemainingTravelTime() const
    {
        return remainingTravelTime;
    }

    string getStatus()
        const {
        return status;
    }
    //putting the vehicle onto a road and start its movement on that road
    void enterEdge(int edge, int travelTime)
    {
        currentEdge = edge;
        remainingTravelTime = travelTime;
        status = "MOVING";

    }
    // update remaing time
    void updateRemainingTime()
    {
        if (remainingTravelTime > 0)
        {
            remainingTravelTime--;
        }

    }

    // checking if the vehicle has reached Intersection
    bool hasReachedIntersection()
        const
    {
        return remainingTravelTime == 0;
    }
    //vehicle IS MADE TO wait at intersection 
    void waitAtIntersection()
    {
        status = "WAITING";
    }
    //vehicle arrives at intersection
    void arrive()
    {
        status = "ARRIVED";
        currentEdge = -1;
        remainingTravelTime = 0;
    }
    // check if moving
    bool isMoving()
        const {
        return status == "MOVING";
    }
    // check if waiting
    bool isWaiting() const
    {
        return status == "WAITING";
    }
    // check if arrivedd
    bool isArrived() const
    {
        return status == "ARRIVED";
    }


    void displayVehicle() const
    {
        cout << "Vehicle Information\n";
        cout << "Source: " << source << "\n";
        cout << "Destination: " << destination << "\n";
        cout << "Current Edge: " << currentEdge << "\n";
        cout << "Remaining Travel Time: " << remainingTravelTime << "\n";
        cout << "Status: " << status << "\n";
    }







};