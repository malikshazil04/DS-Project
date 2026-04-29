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
    int source; // starting node
    int destination;// ending node
    int currentEdge; //  basially the CURRENT road number or edge id
    int remainingTravelTime;  // the remaing travel time
    string status;  // moving, waiting, arrived

public:
    Vehicle() // constructiors
    {
        source = -1;
        destination = -1;
        currentEdge = -1;
        remainingTravelTime = 0;
        status = "WAITING";
    }
    Vehicle(int src, int dest)
    {
        source = src;
        destination = dest;
        currentEdge = -1;
        remainingTravelTime = 0;
        status = "WAITING";
    }

    void setSource(int src) // making setters for all the attributes
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
     // making getters for all the attributes
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
   const{
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