// Agent.cc

#include <iostream>
#include <list>
#include "Agent.h"

using namespace std;

Agent::Agent ()
{
	for (int x = 1; x <= 4; x++)
	{
		for (int y = 1; y <= 4; y++)
		{
			searchEngine.AddSafeLocation(x, y);
		}
	}
	goldLocation = Location(-1, -1);
}

Agent::~Agent ()
{

}

void Agent::Initialize ()
{
	agentHasGold = false;
	actionList.clear();
	currentLocation = Location(1, 1);
	agentOrientation = RIGHT;
}

void Agent::GetRandomGoal()
{
	goalLocation = Location((rand() % 4) + 1, (rand() % 4) + 1);
}

void Agent::GetAPath()
{
	list<Action> actionList2;
	int tries = 25;
	do {
		if (!agentHasGold && goldLocation.X != -1)
		{
			cout << "GOIN FOR GOLD" << endl;
			goalLocation = goldLocation;
			agentHasGold = true;
		}
		else if (agentHasGold)
		{
			if (currentLocation == Location(1, 1))
			{
				break;
			}
			else
			{
				goalLocation = Location(1,1);
			}
		}
		else 
		{
			if (IsLocationValid(Location(3, 1)))
			{
				goalLocation = Location(3, 1);
			}
			else if (IsLocationValid(Location(4, 4)))
			{
				goalLocation = Location(4, 4);
			} 
			else if (IsLocationValid(Location(1, 4)))
			{
				goalLocation = Location(1, 4);
			}
			else
			{
				GetRandomGoal();
			}
		}
		tries--;
	} while (!IsLocationValid(goalLocation) && tries > 0);
	actionList2 = searchEngine.FindPath(currentLocation, agentOrientation, goalLocation, LEFT);
	actionList.splice(actionList.end(), actionList2);
}

bool Agent::IsLocationValid(Location location)
{
	return !(currentLocation == location) && 
		(location.X > 0) && (0 < location.Y) &&
		(visitedLocations.find(GenerateLocationKey(location)) == visitedLocations.end()) &&
		searchEngine.SafeLocation(location.X, location.Y);
}

Action Agent::Process (Percept& percept)
{
	Action action;
	for(auto i : searchEngine.safeLocations)
	{
		cout << "Safe Location (" << i.X << "," << i.Y << ")\n";
	}
	UpdateVisitedLocations();
	if (percept.Glitter)
	{
		goldLocation = currentLocation;
		actionList.clear();
		actionList.push_front(GRAB);
		agentHasGold = true;
	} 
	else if (actionList.empty()) 
	{
		GetAPath();
		if (actionList.empty())
		{
			actionList.push_front(CLIMB);
		}
	}
 	action = actionList.front();
	actionList.pop_front();
	if (action == GOFORWARD)
	{
		currentLocation = CalculateNewLocation(agentOrientation);
		cout << "new Location (" << currentLocation.X << "," << currentLocation.Y << ")\n";
	} 
	else if (action < 3)
	{
		UpdateOrientation(action);
	}
	return action;
}

void Agent::UpdateOrientation(Action turnDirection)
{
	if (turnDirection == TURNRIGHT)
	{
		agentOrientation = Orientation((agentOrientation + 3) % 4);
	}
	else
	{
		agentOrientation = Orientation((agentOrientation + 1) % 4);
	}
	cout << "Agent Orientation: ";
	PrintOrientation(agentOrientation);
	cout << endl;
}

Location Agent::CalculateNewLocation(Orientation Direction)
{
	Location newLocation = currentLocation;
	if (Direction % 2)
	{
		newLocation.Y += (-Direction + 2);
	}
	else
	{
		newLocation.X += (-Direction + 1);
	}
	if (newLocation.X < 1 || newLocation.Y < 1)
	{
		return currentLocation;
	}
	else
	{
		return newLocation;
	}
}

int Agent::GenerateLocationKey(Location location)
{
	string newinteger;
	newinteger += location.X + 48;
	newinteger += location.Y + 48;
	return stoi(newinteger);
}

void Agent::UpdateVisitedLocations()
{
	if (visitedLocations.find(GenerateLocationKey(currentLocation)) == visitedLocations.end())
	{
		if (!(currentLocation == Location(1, 1)))
		{
			visitedLocations.insert(GenerateLocationKey(currentLocation));
		}
	}
}

Orientation Agent::GetRandomOrientation()
{
	return Orientation(rand() % 4);
}

void Agent::GameOver (int score)
{
	if (score <= -1000)
	{
		searchEngine.RemoveSafeLocation(currentLocation.X, currentLocation.Y);
	}

}


