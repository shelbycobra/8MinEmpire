#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "../Map.h"
#include "../Player.h"
#include "../Cards.h"

#include <sstream>
#include <algorithm>

using namespace std;

typedef unordered_map<string, Player*> Players;
typedef pair<Vertex*, bool> Edge;

bool isConnectedMap();
bool validateContinents();
vector<set<string>* > getMapContinents();
bool validateEdges();
bool isContinentConnected(set<string>* continent);
void performCardAction(Player* player, const string action, GameMap* map, Players* players);

#endif