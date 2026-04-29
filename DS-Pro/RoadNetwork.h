#pragma once
#include<vector>  // stores list of neighbors, needed for adjacency list traversal, dynamic size
#include<unordered_map>  // ideal for graph storage, find node quickly, fast lookup by ID
using namespace std;

#include "Intersection.h"
#include "Road.h"

// Road Network = Graph (G = V,E)
// Nodes = intersections
// Edges = roads
// Connections = adjacency list
class RoadNetwork
{
private:
	// Store all intersections (nodes V)
	// value is in pointer as it will lead to no copying and also it is used in graphs
	unordered_map<int, Intersection*> intersections;

	// Store all roads (edges E)
	unordered_map<int, Road*> roads;

	// Adjacency list: node -> list of outgoing roads
	// stores full grapgh connectivity
	// used for algorithm (Dijkstra)
	unordered_map<int, vector<Road*>> adjacencyList;
	// each node store multiple out going edges thats why vector used in value

public:
	// Constructor
	RoadNetwork()  // empty graph
	{ }

	// Destructor
	~RoadNetwork()
	{
		// delete all roads
		// r.first = key(roadID)
		// r.second = pointer to Road (Road*) value
		for (auto& r : roads)
			delete r.second;  // delete the Road object (memory cleanup)
		// we have used dynamic memory(new) so we must free it

		// delete all intersections
		for (auto& i : intersections)
			delete i.second;
	}

	// Add Intersection (Node)
	void addIntersection(int id)
	{
		// avoid duplicates
		if (intersections.find(id) == intersections.end()) // if id not found then return end
		{
			intersections[id] = new Intersection(id);  // if not exist then create
			//key = id, value = pointer to object
		}
	}

	// Add Road(Edge)
	void addRoad(int roadID, int from, int to, double length,
		double maxSpeed, int capacity)
	{
		// ensure nodes exist
		addIntersection(from);
		addIntersection(to);

		// create new road, dynamically allocated object
		Road* newRoad = new Road(roadID, from, to, length, maxSpeed, capacity);

		// store in road list, key= id, roadID = pointer to Road
		roads[roadID] = newRoad;

		// update adjacency list
		adjacencyList[from].push_back(newRoad);
		// e.g: Intersection A -> Road A->B added

		// also update Intersection class (node level data)
		intersections[from]->addGoingOutRoad(newRoad);
	}

	// Get neighbors (imp for Dijkstra)
	// returns all roads from a node
	const vector<Road*>& getOutgoingRoad(int intersectionID) const
	{
		return adjacencyList.at(intersectionID); 
		// .at() = safe access, thorws error if not found
	}
	// vector<Road*> = list of roads
	// & -> returns reference (no copy)

	// GETTERS - to access stored data
	// Get specific road by ID
	Road* getRoad(int roadID)
	{
		return roads[roadID];
	}

	// Get intersection by ID
	Intersection* getIntersection(int id)
	{
		return intersections[id];
	}

	// Get all intersections
	const unordered_map<int, Intersection*> getAllIntersections()const
	{
		return intersections;
	}

	// Get all roads
	const unordered_map<int, Road*>& getAllRoads()  const
	{
		return roads;
	}
};