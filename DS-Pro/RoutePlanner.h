#pragma once
#include <vector>   // stores final path (list of nodes)
#include <unordered_map>  // fast lookup for graph data (distances, parents, intersections)
#include <queue>  // (priority_queue) -> min-heap for Dijkstra
#include <limits> // provides infinity, a value bigger than any possible real travel time
#include <algorithm>  // used for reverse()
using namespace std;

#include "RoadNetwork.h"
#include "Road.h"

// RoutePlanner = Shortest Path Engine
// Uses Dijkstra Algorithm with dynamic edge weights (travel time)

class RoutePlanner
{
private:
    RoadNetwork* network;   // pointer to graph

    // store shortest distance (travel time)
    unordered_map<int, double> dist; // Stores: node -> shortest distance(travel time)

    // store previous node for path reconstruction
    unordered_map<int, int> parent;  // store path history

public:

    // Constructor
    RoutePlanner(RoadNetwork* net)
    {
        network = net;
    }

    // Dijkstra Algorithm 
    // Edge cost = currentTravelTime

    // function returns a vector of node IDs representing shortest path
    vector<int> findShortestPath(int source, int destination)
    {
        // min-heap: (distance, node)
        // always selects node with minimum travel time first
        priority_queue<
            pair<double, int>,
            vector<pair<double, int>>,
            greater<pair<double, int>>
        > pq;

        // initialize distances
        dist.clear();
        parent.clear();

        // set all distances to infinity
        for (auto& node : network->getAllIntersections())  // loop through all nodes in a graph
        {
            dist[node.first] = numeric_limits<double>::infinity(); // initialize all nodes with infinite distance
        }
        // network -> getAllIntersections() -> returns all nodes in graph
        // node.first -> intersection ID
        // infinity -> unknown distance initially

        // source initialization
        dist[source] = 0;  // Distance to source = 0
        pq.push({ 0, source });  // Push into queue,insert source into min heap, priority = 0 (smallest)
        parent[source] = -1;  // Source has no parent, Used for path reconstruction

        // Dijkstra loop
        while (!pq.empty())  // Runs until all reachable nodes are processed.
        {
            // Get node with smallest distance
            int currentNode = pq.top().second; // second = node ID
            double currentDist = pq.top().first;  // first = distance value
            pq.pop();

            // skip outdated entries
            if (currentDist > dist[currentNode])
                continue;

            // get all outgoing roads (neighbors) — safe, returns empty if no roads
            const auto& adjList = network->getAllRoads();
            static const vector<Road*> emptyVec;
            const vector<Road*>* roadsPtr = &emptyVec;
            try {
                roadsPtr = &network->getOutgoingRoad(currentNode);
            }
            catch (...) { /* node has no outgoing roads (e.g. destination) */ }

            for (Road* road : *roadsPtr)  // loop through all connected roads
            {
                int neighbor = road->getTo(); // each road connects from -> to

                // update congestion-aware cost
                road->updateTravelTime(); // Update weight dynamically
                // congestion changes travel time so edge weight is NOT fixed

                double cost = road->getCurrentTravelTime(); // Get updated travel time (edge cost)

                if (dist[currentNode] + cost < dist[neighbor]) // If new path is shorter → update
                {
                    dist[neighbor] = dist[currentNode] + cost; // Store new shortest distance
                    parent[neighbor] = currentNode; // Store path information

                    pq.push({ dist[neighbor], neighbor });  // Push updated node into heap
                }
            }
        }

        // reconstruct path
        vector<int> path;  // Start backtracking from destination
        int current = destination;

        while (current != -1)
        {
            // Move backwards using parent array
            path.push_back(current);  // destination -> source (reverse order)
            current = parent[current];
        }

        reverse(path.begin(), path.end());
        // Convert to correct order: source -> destination

        return path; // Return final shortest path
    }

    // Get shortest distance to a node
    double getDistance(int node)
    {
        return dist[node];
    }
};