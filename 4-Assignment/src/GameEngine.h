#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

#include "GameObservers.h"
#include "Player.h"
#include "GameStartUp.h"

class GameEngine: public Subject {

public:
    GameEngine();
    ~GameEngine();

    void startGame() { StartUpGameEngine::instance()->startGame(); }
    virtual void runGame() = 0;
    virtual Player* getCurrentPlayer() = 0;
    virtual Card* getCurrentCard() = 0;
};

class MainGameEngine : public GameEngine {
    Player* currentPlayer;
    Card* currentCard;

public:
    MainGameEngine();
    ~MainGameEngine();

    void runGame();

    void getNextPlayer();
    void chooseCardFromHand();
    void performCardAction();
    void addNewCardToBackOfHand();
    bool continueGame();
    void declareWinner();
    int getMaxNumberOfCards();
    void askToChangePlayerStrategy();

    Player* getCurrentPlayer() { return currentPlayer; }
    Card* getCurrentCard() { return currentCard; }

    //Setters
    void setCurrentCard(Card* card) { currentCard = card; }
};

class TournamentGameEngine: public GameEngine {
    Player* currentPlayer;
    Card* currentCard;

public:
    TournamentGameEngine();
    ~TournamentGameEngine();

    void runGame();

    Player* getCurrentPlayer() { return currentPlayer; }
    Card* getCurrentCard() { return currentCard; }

    //Setters
    void setCurrentCard(Card* card) { currentCard = card; }
private:
    void getNextPlayer();
    void chooseCardFromHand();
    void performCardAction();
    void addNewCardToBackOfHand();
    bool continueGame();
    void declareWinner();

};

class GameEngineFactory {
public:
    static GameEngine* create();
};

#endif