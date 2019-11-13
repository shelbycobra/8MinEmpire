#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

#include "GameObservers.h"
#include "Player.h"
#include "GameStartUp.h"

// class Subject;
// class Subject {
// public:
//     virtual void Attach(Observer* o);
//     virtual void Detach(Observer* o);
//     virtual void Notify();
//     Subject();
//     ~Subject();
// private:
//     list<Observer*> *observers;
// };

class MainGameEngine : public Subject {

    StartUpGameEngine* startUpPhase;

public:
    MainGameEngine();
    MainGameEngine(MainGameEngine* otherMainGameEngine);
    MainGameEngine& operator=(MainGameEngine& otherMainGameEngine);
    ~MainGameEngine();

    StartUpGameEngine* getStartUpPhase() { return startUpPhase; }
    
    Player* getNextPlayer();
    Card* chooseCardFromHand(Player* player);
    void performCardAction(Player* player, const string action);
    void addNewCardToBackOfHand();
    bool continueGame(int maxNumCards);
    void declareWinner();
    int getMaxNumberOfCards();
};

#endif