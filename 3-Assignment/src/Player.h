#ifndef PLAYER_H
#define PLAYER_H

#include <set>
#include <string>
#include <map>
#include <unordered_map>
#include <stdio.h>

#include "Cards.h"
#include "Map.h"
#include "Bidder.h"
#include "util/ScoreTest.h"
#include "PlayerStrategies.h"

class Card;
class Vertex;
class Bidder;
class Player;
class GameMap;
class Strategy;

typedef map<string, Vertex*> Vertices;
typedef unordered_map<string, Player*> Players;
typedef pair<string, string> PlayerEntry;

const string ANON = "Anon";

class Player {
    string* name;
    Vertices* regions;
    int* armies;
    int* cities;
    int* coins;
    vector<Card*>* hand;
    Bidder* bidder;
    string* colour;
    PlayerEntry* playerEntry;
    int* controlledRegions;
    Strategy* strategy;

    friend class ScoreTest;

public:
    Player();
    Player(const string &name, const int& startCoins);
    Player(const string &name, const string& colour);
    Player(const string &name, const string& colour, Strategy* strategy);
    Player(Player* player);
    Player& operator=(Player& player);
    ~Player();

    bool PayCoins(const int& amount);
    void PlaceNewArmies(const string action, Players* players);
    void MoveArmies(const string action, Players* players);
    void MoveOverLand(const string action, Players* players);
    void MoveOverWater(const string action, Players* players);
    void BuildCity();
    void DestroyArmy(Players* players);
    void AndOrAction(const string action, Players* players);
    void Ignore();
    int ComputeScore();

    int getVPFromRegions();
    int computeContinentScore();
    int computeGoodsScore();
    vector<string>* getRegions();
    vector<string>* getOwnedContinents();
    unordered_map<string, int>* getGoodsCount();
    int getVPFromGoods(unordered_map<string, int>* goodsCount);

    void fillPurseFromSupply(const int& coins);
    void addCardToHand(Card* card);
    void addRegion(Vertex* region);
    void removeRegion(Vertex* region);
    void printRegions();
    bool isAdjacent(Vertex* region, const bool& overWaterAllowed);
    bool isAdjacent(const string& regionName, const bool& overWaterAllowed);
    int getArmiesOnRegion(Vertex* region);
    int getCitiesOnRegion(Vertex* region);

    bool executeMoveArmies(const int& numArmies, Vertex* start, Vertex* end, const bool& moveOverWater);
    bool executeAddArmies(const int& newArmies, Vertex* region);
    bool executeDestroyArmy(Vertex* region, Player* opponent);
    bool executeBuildCity(Vertex* region);

    void addArmiesToRegion(Vertex* region, const int& numArmies);
    void removeArmiesFromRegion(Vertex* region, const int& numArmies);

    //GETTERS
    string getName() { return *name; }
    Vertices* getOccupiedRegions() { return regions; }
    int getArmies() { return *armies; }
    int getCities() { return *cities; }
    int getCoins() { return *coins; }
    vector<Card*>* getHand() { return hand;}
    Bidder* getBidder() { return bidder; }
    string getColour() { return *colour; }
    PlayerEntry* getPlayerEntry() { return playerEntry; }
    int getControlledRegions() { return *controlledRegions; }
    Strategy* getStrategy() { return strategy; }

    void setStrategy(Strategy* newStrategy);

private:
    void increaseAvailableArmies(const int& numArmies);
    void decreaseAvailableArmies(const int& numArmies);
    void performCardAction(string& action, Players* players);
    void findAndDistributeWildCards(unordered_map<string, int>* goodsCount);
};

#endif