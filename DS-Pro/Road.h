#pragma once
#include<vector>
#include<cmath>  // power builtin function used
#include <algorithm>
using namespace std;

class Vehicle;
class Road
{
private:
	int roadID;
	int fromIntersection;
	int toIntersection;

	double length; // lij
	double maxSpeed; // vij max
	int capacity;       // cij
	int dischargeRate;  // mu_ij: max vehicles leaving queue per step

	int currentFlow; // fij(t)
	int queueSize;   // Qij(t), vehicles waiting at intersection

	double freeTravelTime;  // wij free
	double currentTravelTime;  // wij(t)

	double alpha;  // congestion sensitivity
	double beta;  // non-linearity factor

	vector<Vehicle*> movingVehicles;   // vehicles on road (fij)
	vector<Vehicle*> waitingQueue;     // vehicles at intersection (Qij)
	// vector<Vehicle*> is the list that stores addresses of vehicle objects

public:
	// Constructor
	Road(int id, int from, int to, double len, double speed, int cap, int discharge = 2)
		: roadID(id), fromIntersection(from), toIntersection(to),
		length(len), maxSpeed(speed), capacity(cap), dischargeRate(discharge),
		currentFlow(0), queueSize(0), alpha(0.15), beta(4.0)

	{
		// initial values that must be set when the road is created
		// baseline travel time, if no traffic exists then cars move at max speed, only distance matters
		freeTravelTime = length / maxSpeed;
		// wij(free) = lij / vij(max)
		currentTravelTime = freeTravelTime;
		// condition for travel time is:
		// if no congestion then freeTravelTime
		// if congestion then travel Time increases dynamically
	}

	// Getters
	int getRoadID() const { return roadID; }

	int getFrom() const { return fromIntersection; }

	int getTo() const { return toIntersection; }

	double getCurrentTravelTime() const { return currentTravelTime; }

	int getCapacity() const { return capacity; }

	int getDischargeRate() const { return dischargeRate; }

	int getCurrentFlow() const { return currentFlow; }

	int getQueueSize() const { return queueSize; }

	double getFreeTravelTime() const { return freeTravelTime; }

	// Congestion Model
	double getCongestion() const
	{
		if (capacity == 0)
			return 0;
		else
			return (double)currentFlow / capacity;
	}
	// currentFlow = no of vehicles currently on road(fij)
	// capacity = max vehicles road can handle (cij)
	// pij = fij / cij
	// if road is empty then congestion = 0
	// if full then congestion = 1

	// Travel Time Model (non-linear)
	void updateTravelTime()
	{
		double rho = getCongestion();

		currentTravelTime = freeTravelTime * (1 + alpha * pow(rho, beta));
	}
	// formula of travel time model
	// low traffic then travel time = free time
	// high traffic then travel time increases sharply

	// Add vehicle to road (flow update)
		// Add vehicle to road (flow update)
	void addVehicle(Vehicle* v)  // pointer to Vehicle object
	{
		for (int i = 0; i < movingVehicles.size(); i++)
		{
			if (movingVehicles[i] == v)
				return;  // vehicle already exists, so do not add again
		}

		currentFlow++;  // fij++, vehicles currently on road
		movingVehicles.push_back(v);  // add in the actual storage of vehicles
	}

	// Remove vehicle from road
	void removeVehicle(Vehicle* v)
	{
		for (int i = 0; i < movingVehicles.size(); i++)
		{
			if (movingVehicles[i] == v)
			{
				movingVehicles.erase(movingVehicles.begin() + i);
				currentFlow--;  // vehicle leaves road then the flow decreases
				return;
			}
		}
	}

	// Check whether vehicle already exists in waiting queue
	bool isVehicleInQueue(Vehicle* v) const
	{
		for (int i = 0; i < waitingQueue.size(); i++)
		{
			if (waitingQueue[i] == v)
				return true;
		}

		return false;
	}

	// Add to intersection queue
	void enqueueVehicle(Vehicle* v)
	{
		if (!isVehicleInQueue(v))
		{
			queueSize++;  // Qij++, cars waiting at red light
			waitingQueue.push_back(v);
		}
	}

	// Discharge vehicles from queue
	void dischargeVehicle(Vehicle* v)
	{
		for (int i = 0; i < waitingQueue.size(); i++)
		{
			if (waitingQueue[i] == v)
			{
				waitingQueue.erase(waitingQueue.begin() + i);
				queueSize--;  // vehicle leaves waiting queue when signal allows movement
				return;
			}
		}
	}
	// vehicle leaves waiting queue when signal allows movement.

	// CEP Section 4.2: d_ij(t) = g_ij(t) * min(Q_ij(t), mu_ij, c_jk - f_jk(t))
	// greenSignal: g_ij(t) in {0,1}
	// nextRoadFlow: f_jk(t)  nextRoadCapacity: c_jk
	int calculateAllowedDischarge(bool greenSignal, int nextRoadFlow, int nextRoadCapacity) const
	{
		if (!greenSignal) return 0; // g_ij = 0 → blocked
		int availableCapacity = nextRoadCapacity - nextRoadFlow;
		if (availableCapacity < 0) availableCapacity = 0;
		// min(Q_ij, mu_ij, c_jk - f_jk)
		int d = queueSize;
		if (dischargeRate < d)       d = dischargeRate;
		if (availableCapacity < d)   d = availableCapacity;
		return d;
	}

	// Get Vehicle queue
	const vector<Vehicle*>& getMovingVehicles() const
	{
		return movingVehicles;
	}

	const vector<Vehicle*>& getWaitingQueue() const
	{
		return waitingQueue;
	}
	// vector<Vehicle*> = vector storing pointers of Vehicle
	// with & it returns the original vector not a copy.
};

