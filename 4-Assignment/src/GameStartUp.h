#ifndef GAME_START_UP_H
#define GAME_START_UP_H

#include "GameInit.h"

class Player;

class StartUpGameEngine {
    static StartUpGameEngine* startUpInstance;
    queue<Player*>* nextTurn;
    int* coinSupply;

public:
    ~StartUpGameEngine();

    static StartUpGameEngine* instance();
    void startGame();

    //Getters
    queue<Player*>* getNextTurnQueue() { return nextTurn; }
    int getCoinSupply() { return *coinSupply; }

    Players* getPlayers() { return InitGameEngine::instance()->getPlayers(); }
    Hand* getHand() { return InitGameEngine::instance()->getHand();}
    int getNumPlayers() { return InitGameEngine::instance()->getNumPlayers(); }
    list<string>* getColours() { return InitGameEngine::instance()->getColours(); }
    vector<string>* getPlayerOrder() { return InitGameEngine::instance()->getPlayerOrder(); }

    void addCoinsToSupply(int amount);
    void removeCoinsFromSupply(int amount);

private:
    StartUpGameEngine();
    void chooseAnonVertex(string &player, Player* anonPlayer);
    void selectStartVertex();
    void distributeCoins();
    void placeStartingArmies();
    void setPlayerOrderInQueue(Player* firstPlayer);
    void placeAnonArmies();
};

#endif