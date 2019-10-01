#ifndef TESTUTIL_H
#define TESTUTIL_H

#include "Map.h"
#include "Player.h"
#include "Cards.h"

using namespace std;

typedef pair<Vertex*, bool> Edge;

bool isConnectedMap(GameMap* map);
bool validateContinents(GameMap* map);
vector<set<string>* > getMapContinents(GameMap* map);
bool validateEdges(GameMap* map);
bool playerOccupiedCountriesAreFoundOnMap(Vertices* countries, Vertices* vertices);

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