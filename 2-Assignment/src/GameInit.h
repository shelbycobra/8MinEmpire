#ifndef GAME_INIT_ENGINE_H
#define GAME_INIT_ENGINE_H

#include "Player.h"
#include <list>

class GameInitEngine {
    GameMap* map;
    Players* players;
    Hand* hand;
    int* numPlayers;
    list<string>* colours;

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
    list<string>* getColours() { return colours; }

private:
    void initializeMap();
    void selectNumPlayers();
    void createPlayers();

    vector<string>* getMapFiles();
    string selectMap(vector<string>* maps);
    Player* createPlayer(vector<Player*>* playerList);
    void printColours();
};

#endif