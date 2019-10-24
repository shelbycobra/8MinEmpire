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

bool isConnectedMap(GameMap* map);
bool validateContinents(GameMap* map);
vector<set<string>* > getMapContinents(GameMap* map);
bool validateEdges(GameMap* map);
bool isContinentConnected(set<string>* continent, GameMap* map);
void performCardAction(Player* player, const string action, GameMap* map, Players* players);

#endif