#ifndef PLAYER_H
#define PLAYER_H

#include "Cards.h"
#include "Map.h"
#include <set>
#include <string>
#include <unordered_map>

typedef vector<Card*> Hand;

class Player {

    string* name;
    set<string>* countries;
    int* armies;
    int* cities;
    int* coins;
    Hand* hand;

    public:
        Player(const string& name,int startCoins);
        ~Player();

        bool PayCoin(int amount);
        bool PlaceNewArmies(int newArmies, Vertex* country, const string& start);
        bool MoveArmies(int numArmies, Vertex* start, Vertex* end);
        bool MoveOverLand(int numArmies, Vertex* start, Vertex* end);
        bool BuildCity(Vertex* country);
        bool DestroyArmy(Vertex* country);

        string getName();
        set<string>* getCountries();
        int getArmies();
        int getCities();
        int getCoins();
        Hand* getHand();

    private:
        void addCountry(Vertex* country);
        void removeCountry(Vertex* country);
        void addArmiesToCountry(Vertex* country, int numArmies);
        void removeArmiesFromCountry(Vertex* country, int numArmies);
        bool isAdjacent(Vertex* country, bool overWaterAllowed);
};

#endif