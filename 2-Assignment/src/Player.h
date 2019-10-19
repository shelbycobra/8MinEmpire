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
    Player(string &name,int startCoins);
    Player(Player* player);
    ~Player();

    bool PayCoins(int amount);
    bool PlaceNewArmies(int newArmies, Vertex* country, string start);
    bool MoveArmies(int numArmies, Vertex* start, Vertex* end, bool moveOverWater);
    bool MoveOverLand(int numArmies, Vertex* start, Vertex* end);
    bool BuildCity(Vertex* country);
    bool DestroyArmy(Vertex* country, Player* opponent);
    bool Ignore();

    void addCardToHand(Card* card);
    void addCountry(Vertex* country);
    void removeCountry(Vertex* country);
    void printCountries();
    bool isAdjacent(Vertex* country, bool overWaterAllowed);
    bool isAdjacent(string countryName, bool overWaterAllowed);

    // Getters
    int getArmiesOnCountry(Vertex* country);
    int getCitiesOnCountry(Vertex* country);
    string getName();
    Vertices* getCountries();
    int getArmies();
    int getCities();
    int getCoins();
    vector<Card*>* getHand();
    Bidder* getBidder();

private:
    void addArmiesToCountry(Vertex* country, int numArmies);
    void reMoveArmiesFromCountry(Vertex* country, int numArmies);
    void increaseAvailableArmies(int numArmies);
    void decreaseAvailableArmies(int numArmies);
};

typedef unordered_map<string, Player*> Players;

#endif