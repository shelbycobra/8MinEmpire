#ifndef GAME_INIT_ENGINE_H
#define GAME_INIT_ENGINE_H

#include "Player.h"

class GameInitEngine {
    GameMap* map;
    Players* players;
    Hand* hand;
    int* numPlayers;

public:
    GameInitEngine();
    GameInitEngine(GameInitEngine* otherInitEngine);
    GameInitEngine& operator=(GameInitEngine& otherInitEngine);
    ~GameInitEngine();

    void initGame();

    GameMap* getMap() { return map;}
    Players* getPlayers() { return players;}
    Hand* getHand() { return hand;}
    int getNumPlayers() { return *numPlayers; }

private:
    void initializeMap();
    void selectNumPlayers();
    void createPlayers();

    vector<string>* getMapFiles();
    string selectMap(vector<string>* maps);
    void createPlayer();
};

#endif