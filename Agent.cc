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
	AddToFrontier(12);
	AddToFrontier(21);
	agentHasGold = false;
	actionList.clear();
	currentLocation = Location(1, 1);
	agentOrientation = RIGHT;
	breezeLocations.insert(-11);
}

void Agent::GetAPath()
{
	int goalkey;
	list<Action> actionList2;
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
			goalLocation = GetBestFrontierLocation(frontierProbabilities);
		}
	} while (!IsLocationValid(goalLocation));
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
	UpdateVisitedLocations();
	for (auto i : frontierLocations)
	{
		cout << i << endl;
	}
	if (percept.Breeze)
	{
		AddBreeze();
		frontierProbabilities = GetPitProbabilities();
	}
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
			if (frontierLocations.find(GenerateLocationKey(currentLocation)) != frontierLocations.end())
			{
				frontierLocations.erase(GenerateLocationKey(currentLocation));
			}
		}
	}
	CalculateNewFrontier();
}

void Agent::AddBreeze()
{
	breezeLocations.erase(-11);
	if (breezeLocations.find(GenerateLocationKey(currentLocation)) == breezeLocations.end())
	{
		breezeLocations.insert(GenerateLocationKey(currentLocation));
	}
}

void Agent::CalculateNewFrontier()
{
	int newkey = GenerateLocationKey(currentLocation);
	AddToFrontier(newkey + 10);
	AddToFrontier(newkey - 10);
	AddToFrontier(newkey + 1);
	AddToFrontier(newkey - 1);

}

void Agent::AddToFrontier(int location)
{
	if (frontierLocations.find(location) == frontierLocations.end() &&
		visitedLocations.find(location) == visitedLocations.end())
	{
		if (IsValidKey(location))
		{
			frontierLocations.insert(location);
		}
	}
}

bool Agent::IsValidKey(int key)
{
	bool valid = true;
	if (key == 11 || key <= 10 || key % 10 == 0 || key % 10 > 4)
	{
		valid = false;
	}
	return valid;
}

set<int> Agent::BreezesGivenPits(vector<int> pits)
{
	set<int> theobreezes;
	for (auto pit : pits)
	{
		if (frontierLocations.find(pit + 10) != frontierLocations.end())
		{ 
			theobreezes.insert(pit + 10);
		}
		if (frontierLocations.find(pit - 10) != frontierLocations.end())
		{
			theobreezes.insert(pit - 10);
		}
		if (frontierLocations.find(pit + 1) != frontierLocations.end())
		{
			theobreezes.insert(pit + 1);
		}
		if (frontierLocations.find(pit - 1) != frontierLocations.end())
		{
			theobreezes.insert(pit - 1);
		}
	}
	return theobreezes;
}

vector<vector<int>> Agent::GetAllPossiblePits(vector<int> frontierprime)
{
	vector<vector<int>> result(0, vector<int>(0));
	vector<int> temp;
	int i, j;
	for (i = 0; i < frontierprime.size(); i++)
	{
		for (j = 0; j < i; j++)
		{
			temp.clear();
			temp = result[j];
			temp.push_back(frontierprime[i]);
			result.push_back(temp);
		}
		temp.clear();
		temp.push_back(frontierprime[i]);
		result.push_back(temp);
	}

	return result;
}

Location Agent::KeyToLocation(int key)
{
	int x = key / 10;
	int y = key % 10;
	return Location(x, y);
}


map<int, double> Agent::GetPitProbabilities()
{
	map<int, double> frontierProbs;
	vector<int> frontierprime, frontierlocs;
	vector<vector<int>> enumeratedpits;
	double Ptrue, Pfalse, Pfrontier;
	int temp;
	for (auto i : frontierLocations)
	{
		cout << "debugi " << i << endl;
		frontierlocs.push_back(i);
	}
	for(int i=0; i < frontierlocs.size(); i++)
	{
		frontierprime = frontierlocs;
		temp = frontierprime[i];
		Ptrue = 0.0;
		Pfalse = 0.0;
		cout << "debug1: " << frontierlocs.size() << endl;
		frontierprime.erase(frontierprime.begin() + i);
		enumeratedpits = GetAllPossiblePits(frontierprime);

		for (int j = 0; i < enumeratedpits.size(); i++)
		{
			cout << "pit enums = " << enumeratedpits.size();
			int num = enumeratedpits[j].size();
			Pfrontier = 0.2 * num * 0.8 *(frontierLocations.size() - num);
			cout << temp << " pfrontier : " << Pfrontier << endl;
			enumeratedpits[j].push_back(temp);
			if (ConsistentWithBreeze(enumeratedpits[j]))
			{
				Ptrue += Pfrontier;
			}
			else
			{
				Pfalse += Pfrontier;
			}
		}
		cout << temp << " pit true: " << Ptrue << endl;
		Ptrue = Ptrue * 0.2;
		Pfalse = Pfalse * 0.8;
		Ptrue = Ptrue / (Ptrue + Pfalse);
		frontierProbs.insert(pair<int, double>(temp, Ptrue));
	}
	cout << "debug" << endl;
	return frontierProbs;
}

bool Agent::ConsistentWithBreeze(vector<int> a)
{
	bool consistent = true;
	set<int> temp = BreezesGivenPits(a);
	cout << "size a: " << a.size() << endl;
	if (temp.size() != 0)
	{
		for (auto i : temp)
		{
			cout << " temp : " << i << endl;
			if (breezeLocations.find(i) == breezeLocations.end())
			{
				consistent = false;
			}
		}
		for (auto i : breezeLocations)
		{
			cout << " breeze : " << i << endl;
			if (temp.find(i) == temp.end())
			{
				consistent = false;
			}
		}
	}
	else
	{
		consistent = false;
	}
	cout << "consitent = " << consistent << endl;
	return consistent;
}

Location Agent::GetBestFrontierLocation(map<int, double> frontierprobs)
{
	double prob = 0.0;
	int loc = 0;
	set<int>::iterator it = frontierLocations.begin();
	Location returnLocation;
	for (auto i : frontierprobs)
	{ 
		if (i.second < prob)
		{
			prob = i.second;
			loc = i.first;
		}
	}
	if (loc)
	{
		returnLocation = KeyToLocation(loc);
	}
	else
	{
		
		returnLocation = KeyToLocation(*it);
	}
	return returnLocation;
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


