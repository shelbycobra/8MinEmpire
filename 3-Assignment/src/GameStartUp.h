#ifndef GAME_START_UP_H
#define GAME_START_UP_H

#include "GameInit.h"

class GameStartUpEngine {

    GameInitEngine* initPhase;
    queue<Player*>* nextTurn;
    int* coinSupply;

public:
    GameStartUpEngine();
    GameStartUpEngine(GameStartUpEngine* otherStartUpEngine);
    GameStartUpEngine& operator=(GameStartUpEngine& startUpEngine);
    ~GameStartUpEngine();

    void startGame();

    //Getters
    GameInitEngine* getInitPhase() { return initPhase; }
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