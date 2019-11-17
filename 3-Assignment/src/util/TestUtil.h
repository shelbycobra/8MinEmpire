#ifndef TESTUTIL_H
#define TESTUTIL_H

#include "../Map.h"
#include "../Player.h"
#include "../Cards.h"

using namespace std;

bool playerOccupiedRegionsAreFoundOnMap(Player* player, GameMap* map);
Players createDummyPlayers(int numPlayers);

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