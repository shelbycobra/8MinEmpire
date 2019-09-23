#ifndef PLAYER_H
#define PLAYER_H

#include "Cards.h"
#include "Map.h"
#include <set>
#include <string>
#include <unordered_map>

typedef pair<string, Vertex*> Country;
typedef vector<Card*> Hand;

class Player {

    string* name;
    unordered_map<string, Vertex*> countries;
    int* cubes;
    int* disks;
    int* coins;
    Hand* hand;

    public:
        Player(const string& name, int cubes, int disks, int coins);
        ~Player();

        void PayCoin();
        void PlaceNewArmies();
        void MoveArmies();
        void MoveOverLand();
        void BuildCity();
        void DestroyArmy();

        string getName();
        set<Vertex>* getCountries();
        int getCubes();
        int getDisks();
        int getCoins();
        Hand* getHand();

        void addCountry(Vertex* country);
};

#endif