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

    Players* getPlayers() { return initPhase->getPlayers(); }
    Hand* getHand() { return initPhase->getHand();}
    int getNumPlayers() { return initPhase->getNumPlayers(); }
    list<string>* getColours() { return initPhase->getColours(); }
    vector<string>* getPlayerOrder() { return initPhase->getPlayerOrder(); }

private:
    void chooseAnonVertex(string &player, Player* anonPlayer);
    void selectStartVertex();
    void distributeCoins();
    void placeStartingArmies();
    void setPlayerOrderInQueue(Player* firstPlayer);
    void placeAnonArmies();
};

#endif