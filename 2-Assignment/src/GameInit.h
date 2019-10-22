#ifndef GameInitEngine_H
#define GameInitEngine_H

#include <dirent.h>
#include <sys/types.h>

#include "Map.h"
#include "Player.h"
#include "Cards.h"

class GameInitEngine {
    GameMap* map;
    Players* players;
    Hand* hand;
    string* mapName;
    int* numPlayers;

public:
    GameInitEngine();
    ~GameInitEngine();

    void selectMap();
    void selectNumPlayers();
    void initializeMap();
    void createPlayers();
    void createPlayer(const int& coins);
    void initGame();

    GameMap* getMap() { return map;}
    Players* getPlayers() { return players;}
    Hand* getHand() { return hand;}
    string getMapName() { return *mapName; }
    int getNumPlayers() { return *numPlayers; }

// private:
    vector<string>* getMapFiles();
};

#endif