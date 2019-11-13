#ifndef GAME_START_UP_H
#define GAME_START_UP_H

#include "GameInit.h"

class StartUpGameEngine {

    InitGameEngine* initPhase;
    queue<Player*>* nextTurn;
    int* coinSupply;

public:
    StartUpGameEngine();
    StartUpGameEngine(StartUpGameEngine* otherStartUpEngine);
    StartUpGameEngine& operator=(StartUpGameEngine& startUpEngine);
    ~StartUpGameEngine();

    void startGame();

    //Getters
    InitGameEngine* getInitPhase() { return initPhase; }
    queue<Player*>* getNextTurnQueue() { return nextTurn; }

    int getCoinSupply() { return *coinSupply; }
    void addCoinsToSupply(int amount);
    void removeCoinsFromSupply(int amount);

private:
    void chooseAnonVertex(string &player, Player* anonPlayer);
    void selectStartVertex();
    void distributeCoins();
    void placeStartingArmies();
    void setPlayerOrderInQueue(Player* firstPlayer);
    void placeAnonArmies();
};

#endif