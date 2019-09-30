#ifndef PLAYER_H
#define PLAYER_H

#include "Cards.h"
#include "Map.h"
#include <set>
#include <string>
#include <unordered_map>
#include <stdio.h>

typedef unordered_map<string, Player*> Players;

struct Card;
struct Vertex;

class Player {

    string* name;
    Vertices* countries;
    int* armies;
    int* cities;
    int* coins;
    vector<Card*>* hand;

    public:

        //Constructors & Destructors
        Player(string &name,int startCoins);
        ~Player();

        //Copy Constructor & Assignment Overload

        bool payCoins(int amount);
        bool placeNewArmies(int newArmies, Vertex* country, string& start);
        bool moveArmies(int numArmies, Vertex* start, Vertex* end, bool moveOverWater);
        bool moveOverLand(int numArmies, Vertex* start, Vertex* end);
        bool buildCity(Vertex* country);
        bool destroyArmy(Vertex* country, Player* opponent);

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
        int getAvailableArmies();
        int getAvailableCities();
        int getCoins();
        vector<Card*>* getHand();

    private:
        void addArmiesToCountry(Vertex* country, int numArmies);
        void removeArmiesFromCountry(Vertex* country, int numArmies);
        void increaseAvailableArmies(int amount);
        void decreaseAvailableArmies(int amount);
};

#endif