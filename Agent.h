// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include <cstdlib>
#include <chrono>
#include <thread>
#include <list>
#include <set>
#include <string>
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

	int GenerateLocationKey(Location location);
	void UpdateVisitedLocations();

	Agent ();
	~Agent ();
	Orientation GetRandomOrientation();
	void Initialize ();
	void GetRandomGoal();
	void GetAPath();
	bool IsLocationValid(Location location);
	Action Process (Percept& percept);
	void UpdateOrientation(Action turnDirection);
	Location CalculateNewLocation(Orientation Direction);
	void GameOver (int score);

};

#endif // AGENT_H
