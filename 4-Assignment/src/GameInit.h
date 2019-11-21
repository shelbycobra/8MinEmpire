#ifndef GAME_INIT_ENGINE_H
#define GAME_INIT_ENGINE_H

#include "Player.h"
#include <list>

class InitGameEngine {
    Players* players;
    Hand* hand;
    int* numPlayers;
    list<string>* colours;
    vector<string>* playerOrder;

public:
    InitGameEngine();
    InitGameEngine(InitGameEngine* otherInitEngine);
    InitGameEngine& operator=(InitGameEngine& otherInitEngine);
    ~InitGameEngine();

    void initGame();

    Players* getPlayers() { return players;}
    Hand* getHand() { return hand;}
    int getNumPlayers() { return *numPlayers; }
    list<string>* getColours() { return colours; }
    vector<string>* getPlayerOrder() { return playerOrder; }

private:
    void initializeMap();
    void selectNumPlayers();
    void createPlayers();
    vector<string>* getMapFiles();
    string selectMap(vector<string>* maps);
    Player* createPlayer();
    void printColours();
    string chooseColour();
    string chooseName();
    Strategy* chooseStrategy();
};

#endif