#ifndef PLAYER_H
#define PLAYER_H

#include <set>
#include <string>
#include <unordered_map>
#include <stdio.h>

#include "Cards.h"
#include "Map.h"
#include "Bidder.h"

enum ActionType { MOVE_OVER_LAND, ADD_ARMY, DESTROY_ARMY, MOVE_OVER_WATER, BUILD_CITY };

class Card;
class Vertex;
typedef unordered_map<string, Vertex*> Vertices;
class Bidder;

class Player {

    string* name;
    Vertices* countries;
    int* armies;
    int* cities;
    int* coins;
    vector<Card*>* hand;
    Bidder* bidder;

public:
    Player();
    Player(const string &name);
    Player(const string &name, const int& startCoins);
    Player(Player* player);
    Player& operator=(Player& player);
    ~Player();

    bool PayCoins(const int& amount);
    bool PlaceNewArmies(const int& newArmies, Vertex* country, const string& start);
    bool MoveArmies(const int& numArmies, Vertex* start, Vertex* end, const bool& moveOverWater);
    bool MoveOverLand(const int& numArmies, Vertex* start, Vertex* end);
    bool BuildCity(Vertex* country);
    bool DestroyArmy(Vertex* country, Player* opponent);
    bool Ignore();

    void addCardToHand(Card* card);
    void addCountry(Vertex* country);
    void removeCountry(Vertex* country);
    void printCountries();
    bool isAdjacent(Vertex* country, const bool& overWaterAllowed);
    bool isAdjacent(const string& countryName, const bool& overWaterAllowed);
    int getArmiesOnCountry(Vertex* country);
    int getCitiesOnCountry(Vertex* country);

    // Getters
    string getName() { return *name; }
    Vertices* getCountries() { return countries; }
    int getArmies() { return *armies; }
    int getCities() { return *cities; }
    int getCoins() { return *coins; }
    vector<Card*>* getHand() { return hand;}
    Bidder* getBidder() { return bidder; }

    //Setters
    void setCoins(const int& coins) { *this->coins = coins; }

private:
    void addArmiesToCountry(Vertex* country, const int& numArmies);
    void reMoveArmiesFromCountry(Vertex* country, const int& numArmies);
    void increaseAvailableArmies(const int& numArmies);
    void decreaseAvailableArmies(const int& numArmies);
};

typedef unordered_map<string, Player*> Players;

#endif