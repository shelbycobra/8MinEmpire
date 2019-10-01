#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "../Map.h"
#include "../Player.h"
#include "../Cards.h"

#include <sstream>

using namespace std;

typedef unordered_map<string, Player*> Players;
typedef pair<Vertex*, bool> Edge;

vector<string>* split(string& str, char delimiter);
void performCardAction(Player* player, string action, GameMap* map, Players* players);
Vertex* chooseStartVertex(Player* player);
Vertex* chooseEndVertex(Player* player, actionType type, GameMap* map);
int chooseArmies(int maxArmies, int remainderArmies);
string chooseORAction(string action);
Player* chooseOpponent(Players* players, Player* currentPlayer);
int selectPositionOfCardFromGameHand(Hand* gameHand);
void executeMoveArmies(Player* player,string action, GameMap* map);
void executeAddArmies(Player* player, string action, GameMap* map);
void executeDestroyArmy(Player* player, string action, GameMap* map, Players* players);
void executeBuildCity(Player* player, string action, GameMap* map);

#endif