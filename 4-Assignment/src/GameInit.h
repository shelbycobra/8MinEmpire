#ifndef GAME_INIT_ENGINE_H
#define GAME_INIT_ENGINE_H

#include "Player.h"
#include <list>

class InitGameEngine {
    static InitGameEngine* initInstance;
    Players* players;
    Hand* hand;
    int* numPlayers;
    list<string>* colours;
    vector<string>* playerOrder;
    bool* isGameTournament;

public:
    ~InitGameEngine();

    static InitGameEngine* instance();

    void initGame();
    void setIsTournament(bool isTournamentBool);
    bool isTournament() { return *isGameTournament; }

    Players* getPlayers() { return players;}
    Hand* getHand() { return hand;}
    int getNumPlayers() { return *numPlayers; }
    list<string>* getColours() { return colours; }
    vector<string>* getPlayerOrder() { return playerOrder; }

private:
    InitGameEngine();
    void askGameMode();
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