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
    Player* currentPlayer;
    Card* currentCard;

public:
    MainGameEngine();
    MainGameEngine(MainGameEngine* otherMainGameEngine);
    MainGameEngine& operator=(MainGameEngine& otherMainGameEngine);
    ~MainGameEngine();

    StartUpGameEngine* getStartUpPhase() { return startUpPhase; }

    void getNextPlayer();
    void chooseCardFromHand();
    void performCardAction();
    void addNewCardToBackOfHand();
    bool continueGame(int maxNumCards);
    void declareWinner();
    int getMaxNumberOfCards();

    void startGame() { startUpPhase->startGame(); }

    Player* getCurrentPlayer() { return currentPlayer; }
    Card* getCurrentCard() { return currentCard; }
    GameMap* getMap() { return startUpPhase->getMap(); }
    Players* getPlayers() { return startUpPhase->getPlayers(); }
    Hand* getHand() { return startUpPhase->getHand();}
    int getNumPlayers() { return startUpPhase->getNumPlayers(); }
    list<string>* getColours() { return startUpPhase->getColours(); }
    vector<string>* getPlayerOrder() { return startUpPhase->getPlayerOrder(); }
};

#endif