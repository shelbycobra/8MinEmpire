#ifndef GAME_START_UP_H
#define GAME_START_UP_H

#include "GameInit.h"

class GameStartUpEngine {

    GameInitEngine* initPhase;
    queue<Player*>* nextTurn;

public:
    GameStartUpEngine();
    GameStartUpEngine(GameInitEngine* initPhase);
    GameStartUpEngine(GameStartUpEngine* otherStartUpEngine);
    GameStartUpEngine& operator=(GameStartUpEngine& startUpEngine);
    ~GameStartUpEngine();

    void startGame();

    //Getters
    GameInitEngine* getGameInitEngine() { return initPhase; }
    queue<Player*>* getNextTurnQueue() { return nextTurn; }

private:
    void selectStartVertex();
    void distributeCoins();
    void placeArmiesOnStartVertex();
    void setPlayerOrderInQueue(Player* firstPlayer);
};

#endif