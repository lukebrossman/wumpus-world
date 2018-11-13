// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include <cstdlib>
#include <chrono>
#include <thread>
#include <list>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <math.h>
#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include "Search.h"

class Agent
{
public:
	bool agentHasGold;
	list<Action> actionList;
	SearchEngine searchEngine;
	Location goldLocation;
	Location currentLocation;
	Location goalLocation;
	Orientation agentOrientation;
	bool isWumpusDead;
	set<int> visitedLocations;
	set<int> frontierLocations;
	set<int> breezeLocations;
	map<int, double> frontierProbabilities;

	int GenerateLocationKey(Location location);
	void UpdateVisitedLocations();
	void AddBreeze();
	void CalculateNewFrontier();
	void AddToFrontier(int location);
	bool IsValidKey(int key);
	map<int,double> GetPitProbabilities();
	set<int> BreezesGivenPits(vector<int> pits);
	vector<vector<int>> GetAllPossiblePits(vector<int> frontierprime);
	Location GetBestFrontierLocation(map<int, double> frontierProbs);
	Location KeyToLocation(int key);


	bool ConsistentWithBreeze(vector<int> a);
	Agent ();
	~Agent ();
	Orientation GetRandomOrientation();
	void Initialize ();
	void GetAPath();
	bool IsLocationValid(Location location);
	Action Process (Percept& percept);
	void UpdateOrientation(Action turnDirection);
	Location CalculateNewLocation(Orientation Direction);
	void GameOver (int score);

};

#endif // AGENT_H
