#pragma once
/* a single intersection connects to multiple roads thats why we need a structure 
that stores multiple items. Vector is perfect for dynamic size and fast access 
and also a DS for adjancency list. Adjacency list is a graph representation
where each node stores a list of connected edges (neighbors) */
#include<vector>
using namespace std;

class Road;
class Intersection {
private:
	int id;
	vector <Road*> outGoingRoads; // adjacency list
public:
	Intersection(int id) : id(id) {}

	int getID() const {
		return id;
	}

	// Connect this intersection to another intersection through a road.
	void addGoingOutRoad(Road* road)  // takes a road pointer
	{
		outGoingRoads.push_back(road);  // adds it to adjacency list
	} // e.g: like when we say from A, you can go to B via this road.
	// push_back adds an element at the end of a vector

	const vector<Road*>& getOutGoingRoads()const // this is a list that stores all roads going out from this intersection
	{ // const will prevent modification in vector
		// & _ makes sure that returns original vector
		return outGoingRoads;
	}
};
