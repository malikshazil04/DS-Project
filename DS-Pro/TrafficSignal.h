#pragma once
//The traffic signal controls the roads coming into an intersection, gives green to
// the road with the most waiting vehicles, allows only one road to move at a time,
// and changes the signal step by step during the simulation.
#include <iostream>
#include <vector>
using namespace std;

// TrafficSignal controls ONE intersection.
// CEP rule: at one intersection, only ONE controlled road can be GREEN at a time.
class TrafficSignal
{
private:
    int intersectionId;              // Intersection/node ID controlled by this signal
    vector<int> incomingRoadIds;     // In this GUI: roads controlled by this intersection
    int currentGreenRoadId;          // Only this road is GREEN; all others are RED
    int greenDuration;               // Maximum time a road may stay green
    int timer;                       // Counts how many steps current green has stayed active
    int queueThreshold;              // If queue becomes smaller than this, signal may switch

public:
    TrafficSignal()
    {
        intersectionId = -1;
        currentGreenRoadId = -1;
        greenDuration = 3;
        timer = 0;
        queueThreshold = 0;
    }

    TrafficSignal(int id, int duration, int threshold)
    {
        intersectionId = id;
        currentGreenRoadId = -1;
        greenDuration = duration > 0 ? duration : 3;
        timer = 0;
        queueThreshold = threshold >= 0 ? threshold : 0;
    }

    void setIntersectionId(int id) { intersectionId = id; }
    void setGreenDuration(int duration) { if (duration > 0) greenDuration = duration; }
    void setQueueThreshold(int threshold) { if (threshold >= 0) queueThreshold = threshold; }

    int getIntersectionId() const { return intersectionId; }
    int getCurrentGreenRoadId() const { return currentGreenRoadId; }
    int getGreenDuration() const { return greenDuration; }
    int getTimer() const { return timer; }
    int getQueueThreshold() const { return queueThreshold; }
    vector<int> getIncomingRoadIds() const { return incomingRoadIds; }

    void addIncomingRoad(int roadId)
    {
        for (int i = 0; i < incomingRoadIds.size(); i++)
        {
            if (incomingRoadIds[i] == roadId)
                return;
        }
        incomingRoadIds.push_back(roadId);
        // First added road becomes the default green road
        // Still, only one road is green because currentGreenRoadId stores one ID only
        if (currentGreenRoadId == -1)
            currentGreenRoadId = roadId;
    }
    bool isGreen(int roadId) const
    {
        // This single comparison enforces CEP signal constraint:
        // one road ID is green; all other roads are red.
        return currentGreenRoadId == roadId;
    }

    int findRoadIndex(int roadId) const
    {
        for (int i = 0; i < incomingRoadIds.size(); i++)
        {
            if (incomingRoadIds[i] == roadId)
                return i;
        }
        return -1;
    }

    void selectGreenRoad(const vector<int>& queueSizes)
    {
        if (incomingRoadIds.empty())
        {
            currentGreenRoadId = -1;
            timer = 0;
            return;
        }

        if (incomingRoadIds.size() != queueSizes.size())
            return;

        int maxIndex = 0; // if the queue sizes are equal it keeps the first road as the road to get green signal

        // Select the road with the longest queue.
        // If queues are equal, the first road remains selected to keep behaviour stable.
        for (int i = 1; i < queueSizes.size(); i++)
        {
            if (queueSizes[i] > queueSizes[maxIndex])
                maxIndex = i;   
        }

        currentGreenRoadId = incomingRoadIds[maxIndex];
        timer = 0;
    }

    void updateSignal(const vector<int>& queueSizes)
    {
        if (incomingRoadIds.empty() || incomingRoadIds.size() != queueSizes.size())
            return;

        if (currentGreenRoadId == -1)
        {
            selectGreenRoad(queueSizes);
            return;
        }

        int currentIndex = findRoadIndex(currentGreenRoadId);
        if (currentIndex == -1)
        {
            selectGreenRoad(queueSizes);
            return;
        }

        timer++;

        // CEP behaviour:
        // Signal changes after fixed green duration OR when current queue becomes low.
        if (timer >= greenDuration || queueSizes[currentIndex] <= queueThreshold)
            selectGreenRoad(queueSizes);
    }
    //If no incoming road exists:  currentGreenRoadId = -1 Otherwise: currentGreenRoadId = first road in incomingRoadIds
    void reset() 
    {
        timer = 0;
        currentGreenRoadId = incomingRoadIds.empty() ? -1 : incomingRoadIds[0];
    }

    void displaySignal() const
    {
        cout << "Traffic Signal Details:\n";
        cout << "Intersection ID: " << intersectionId << "\n";
        cout << "Current Green Road ID: " << currentGreenRoadId << "\n";
        cout << "Green Duration: " << greenDuration << "\n";
        cout << "Timer: " << timer << "\n";
        cout << "Queue Threshold: " << queueThreshold << "\n";
        cout << "Controlled Roads: ";
        for (int i = 0; i < (int)incomingRoadIds.size(); i++)
            cout << incomingRoadIds[i] << " ";
        cout << "\n";
    }
};
