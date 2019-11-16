#ifndef PLAYER_H
#define PLAYER_H

#include <set>
#include <string>
#include <unordered_map>
#include <stdio.h>

#include "Cards.h"
#include "Map.h"
#include "Bidder.h"
#include "util/ScoreTest.h"
#include "PlayerStrategies.h"

enum ActionType { MOVE_OVER_LAND, ADD_ARMY, DESTROY_ARMY, MOVE_OVER_WATER, BUILD_CITY, NONE };

class Card;
class Vertex;
class Bidder;
class Player;
class GameMap;
class Strategy;

typedef unordered_map<string, Vertex*> Vertices;
typedef unordered_map<string, Player*> Players;
typedef pair<string, string> PlayerEntry;

const string ANON = "Anon";

class Player {
    string* name;
    Vertices* countries;
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
    Player(const string &name, const string& colour);
    Player(const string &name, const int& startCoins);
    Player(Player* player);
    Player& operator=(Player& player);
    ~Player();

    bool PayCoins(const int& amount);
    void PlaceNewArmies(const string action);
    void MoveArmies(const string action);
    void MoveOverLand(const string action);
    void MoveOverWater(const string action);
    void BuildCity();
    void DestroyArmy(Players* players);
    void AndOrAction(const string action, Players* players);
    void Ignore();
    int ComputeScore();

    int getVPFromRegions();
    int computeContinentScore();
    int computeGoodsScore();
    vector<string>* getOwnedRegions();
    vector<string>* getOwnedContinents();
    unordered_map<string, int>* getGoodsCount();
    int getVPFromGoods(unordered_map<string, int>* goodsCount);

    void fillPurseFromSupply(const int& coins);
    void addCardToHand(Card* card);
    void addCountry(Vertex* country);
    void removeCountry(Vertex* country);
    void printCountries();
    bool isAdjacent(Vertex* country, const bool& overWaterAllowed);
    bool isAdjacent(const string& countryName, const bool& overWaterAllowed);
    int getArmiesOnCountry(Vertex* country);
    int getCitiesOnCountry(Vertex* country);

    bool executeMoveArmies(const int& numArmies, Vertex* start, Vertex* end, const bool& moveOverWater);
    bool executeAddArmies(const int& newArmies, Vertex* country, const string& start);
    bool executeDestroyArmy(Vertex* country, Player* opponent);
    bool executeBuildCity(Vertex* country);

    void addArmiesToCountry(Vertex* country, const int& numArmies);
    void removeArmiesFromCountry(Vertex* country, const int& numArmies);

    void executeStrategy(Card* card, Players* players);

    //GETTERS
    string getName() { return *name; }
    Vertices* getCountries() { return countries; }
    int getArmies() { return *armies; }
    int getCities() { return *cities; }
    int getCoins() { return *coins; }
    vector<Card*>* getHand() { return hand;}
    Bidder* getBidder() { return bidder; }
    string getColour() { return *colour; }
    PlayerEntry* getPlayerEntry() { return playerEntry; }
    int getControlledRegions() { return *controlledRegions; }
    Strategy* getStrategy() { return strategy; }

private:
    void increaseAvailableArmies(const int& numArmies);
    void decreaseAvailableArmies(const int& numArmies);
    void performCardAction(string& action, Players* players);
    Vertex* chooseStartVertex();
    Vertex* chooseEndVertex(const ActionType& type);
    int chooseArmies(const int& maxArmies, const int& remainderArmies, int endVertexArmies, const string& startVertexName);
    string chooseORAction(const string action);
    Player* chooseOpponent(Players* players);
    void findAndDistributeWildCards(unordered_map<string, int>* goodsCount);
};

#endif