#pragma once
//The traffic signal controls the roads coming into an intersection, gives green to
// the road with the most waiting vehicles, allows only one road to move at a time,
// and changes the signal step by step during the simulation.
#include <iostream>
#include <vector>
using namespace std;

class TrafficSignal
{
private:
	int intersectionId;  // ID of the intersection
	vector<int>incomingRoadIds;   //IDs of the incoming road or edge entering the intersection
	int currentGreenRoadId;    // road having current green signal ID
	int greenDuration;        // the duration for which the road remains green (Tg)
	int timer;                // counts how long the timer stays green
	int queueThreshold;      //the minimum queue size , below this size the green signal switchesto another road
public:
	TrafficSignal()    // Constructor
	{
		intersectionId = -1;
		currentGreenRoadId = -1;
		greenDuration = 3;
		timer = 0;
		queueThreshold = 5;
	}
	TrafficSignal(int id, int duration, int threshold)
	{
		intersectionId = id;
		currentGreenRoadId = -1;
		greenDuration = duration;
		timer = 0;
		queueThreshold = threshold;
	}
	// Setters for attributes

    void setIntersectionId(int id)
    {
        intersectionId = id;
    }

    void setGreenDuration(int duration)
    {
        if (duration > 0)
        {
            greenDuration = duration;
        }
        else
            cout << "invalid duration";
    }

    void setQueueThreshold(int threshold)
    {
        if (threshold >= 0)
        {
            queueThreshold = threshold;
        }
        else
            cout << "invalid QueueThreshold";
    }

    // Getters
    int getIntersectionId() const
    {
        return intersectionId;
    }

    int getCurrentGreenRoadId() const
    {
        return currentGreenRoadId;
    }

    int getGreenDuration() const
    {
        return greenDuration;
    }

    int getTimer() const
    {
        return timer;
    }

    int getQueueThreshold() const
    {
        return queueThreshold;
    }
    vector<int> getIncomingRoadIds() const
    {
        return incomingRoadIds;
    }

    // function for adding incoming road to an intersection
    void addIncomingRoad(int roadId)
    {
        for (int i = 0; i < incomingRoadIds.size(); i++)
        {
            if (incomingRoadIds[i] == roadId)
            {
                return; // road already exists, so do not add again
            }
        }

        incomingRoadIds.push_back(roadId);
    }
    //function for selecting the incoming road having the longest queue and giving it green signal
    void selectGreenRoad(const vector<int>&queueSizes)
    {
        if (incomingRoadIds.empty() || incomingRoadIds.size() != queueSizes.size())
            return;
        int maxIndex = 0;
        for (int i = 0; i < queueSizes.size(); i++)
        {
            if (queueSizes[i] > queueSizes[maxIndex])
                maxIndex = i;
        }
        currentGreenRoadId = incomingRoadIds[maxIndex];
        timer = 0;
    }
    // function for updatng signal at each stimulation step
    void updateSignal(const vector<int>&queueSizes)  //passed by referce so no cpy is made
    {
        if (incomingRoadIds.empty() || incomingRoadIds.size() != queueSizes.size())
            return;
        if(currentGreenRoadId==-1)  // if no road is selected
        {
            selectGreenRoad(queueSizes);
            return;
        }
        int currentIndex = -1;
        for(int i=0; i<incomingRoadIds.size();i++) //finding the index of roaad having green signal
        {
            if (incomingRoadIds[i] == currentGreenRoadId)
            {
                currentIndex = i;
                break;
            }
        }
      
        // Safety check 
        if (currentIndex == -1)
        {
            selectGreenRoad(queueSizes);
            return; 
        }
        timer++;
        //switch:
        //The current road has already stayed green for its allowed fixed time.
        //The queue on the current green road has become small enough
        if(timer>=greenDuration||queueSizes[currentIndex]<queueThreshold)
        {
            selectGreenRoad(queueSizes);
        }


    }
    // reset timer if needed manually
    void reset()
    {
        timer = 0;
    }
    //displaying traficsignal information
    void displaySignal() const
    {
        cout << "Traffic Signal Details: \n";
        cout << "Intersection ID: " << intersectionId << "\n";
        cout << "Current Green Road ID: " << currentGreenRoadId << "\n";
        cout << "Green Duration: " << greenDuration << "\n";
        cout << "Timer: " << timer << "\n";
        cout << "Queue Threshold: " << queueThreshold << "\n";
        cout << "Incoming Roads: ";

        for (int i = 0; i < incomingRoadIds.size(); i++)
        {
            cout << incomingRoadIds[i] << " ";
        }

        cout << "\n";
    }
};
