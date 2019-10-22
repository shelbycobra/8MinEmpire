#ifndef GameInitEngine_H
#define GameInitEngine_H

#include "Map.h"
#include "Player.h"
#include "Cards.h"

class GameInitEngine {
    GameMap* map;
    Players* players;
    Hand* hand;
    int* numPlayers;

public:
    GameInitEngine();
    ~GameInitEngine();

    void initGame();
    void initializeMap();
    void selectNumPlayers();
    void createPlayers();

    GameMap* getMap() { return map;}
    Players* getPlayers() { return players;}
    Hand* getHand() { return hand;}
    int getNumPlayers() { return *numPlayers; }

private:
    vector<string>* getMapFiles();
    string selectMap(vector<string>* maps);
    void createPlayer(const int& coins);
};

#endif