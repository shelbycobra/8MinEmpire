#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "Map.h"
#include "Player.h"

#include <sstream>

using namespace std;

typedef pair<Vertex*, bool> Edge;

enum actionType { MOVE_OVER_LAND, ADD_ARMY, DESTROY_ARMY, MOVE_OVER_WATER, BUILD_CITY };

bool isConnectedMap(GameMap* map);
bool validateContinents(GameMap* map);
vector<set<string>* > getMapContinents(GameMap* map);
bool validateEdges(GameMap* map);
bool playerOccupiedCountriesAreFoundOnMap(Vertices* countries, Vertices* vertices);
vector<string>* split(string& str, char delimiter);
bool performCardAction(Player* player, string action, GameMap* map, Players* players);
Vertex* chooseStartVertex(Player* player);
Vertex* chooseEndVertex(Player* player, actionType type, GameMap* map);

GameMap* generateValidMap();
GameMap* generateSmallSimpleMap();
GameMap* generateValidMapContainingNodeWithTwoWaterEdges();
GameMap* generateDisconnectedMap();
GameMap* generateInvalidContinentMap();
GameMap* generateInvalidContinentMap2();
GameMap* generateInvalidContinentMap3();
GameMap* generateMapWithInternalWaterEdge();
GameMap* generateCompletelyDisconnectedMap();
GameMap* generateDuplicateEdgesMap();
GameMap* generateSelfLoopMap();

#endif