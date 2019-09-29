#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "Map.h"
#include "Player.h"

typedef pair<Vertex*, bool> Edge;

bool isConnectedMap(GameMap* map);
bool validateContinents(GameMap* map);
vector<set<string>* > getMapContinents(GameMap* map);
bool validateEdges(GameMap* map);
bool playerOccupiedCountriesAreFoundOnMap(set<Vertex*>* countries, Vertices* vertices);
vector<string>* split(string& str, char delimiter);
bool performCardAction(Player* player, string action);
Vertex* chooseStartVertex(Player* player);
Vertex* chooseEndVertex(Player* player);

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