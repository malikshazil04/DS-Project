#pragma once
#include<vector>
#include<cmath>  // power builtin function used
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
	int capacity;  // cij

	int currentFlow; // fij(t)
	int queueSize;  // Qij(t), vehicles waiting at intersection

	double freeTravelTime;  // wij free
	double currentTravelTime;  // wij(t)

	double alpha;  // congestion sensitivity
	double beta;  // non-linearity factor

	vector<Vehicle*> movingVehicles;   // vehicles on road (fij)
	vector<Vehicle*> waitingQueue;     // vehicles at intersection (Qij)
	// vector<Vehicle*> is the list that stores addresses of vehicle objects

public:
	// Constructor
	Road(int id, int from, int to, double len, double speed, int cap)
		: roadID(id), fromIntersection(from), toIntersection(to),
		length(len), maxSpeed(speed), capacity(cap), currentFlow(0),
		queueSize(0), alpha(0.5), beta(2.0)

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

	int getCurrentFlow() const { return currentFlow; }

	int getQueueSize() const { return queueSize; }

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
	void addVehicle(Vehicle* v)  // pointer to Vehicle object
	{
		currentFlow++;  // fij++, vehicles currently on road
		movingVehicles.push_back(v);  // add in the actual storage of vehicles
	}

	// Remove vehicle from road
	void removeVehicle()
	{
		if (currentFlow > 0)
			currentFlow--;
	}
	// vehicle leaves road then the flow decreases.

	// Add to intersection queue
	void enqueueVehicle(Vehicle* v)
	{
		queueSize++;  // Qij++, cars waiting at red light
		waitingQueue.push_back(v);
	}

	//Discharge vehicles from queue
	void dischargeVehicle()
	{
		if (queueSize > 0)
			queueSize--;
	}
	// vehicle leaves waiting queue when signal allows movement.

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

