#include "ScoreTest.h"
#include "../GameMainLoop.h"

void ScoreTest::test_onePlayerWinsGame() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_onePlayerWinsGame" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card* cardArr[] = {
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
    }

    mainEngine.declareWinner();
}

void ScoreTest::test_twoPlayersTieButHaveDifferentNumCoins() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_twoPlayersTieButHaveDifferentNumCoins" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card* cardArr[] = {
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
    }

    mainEngine.declareWinner();
}

void ScoreTest::test_twoPlayersTieAndHaveSameNumCoins() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_twoPlayersTieAndHaveSameNumCoins" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card* cardArr[] = {
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
    }

    Vertices* vertices = mainEngine.getStartUpPhase()->getInitPhase()->getMap()->getVertices();

    Vertex* PEVertex = vertices->find("PE")->second;

    players->begin()->second->addArmiesToCountry(PEVertex, 1);
    *players->begin()->second->armies -= 2;

    mainEngine.declareWinner();
}

void ScoreTest::test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card* cardArr[] = {
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
    }

    Vertices* vertices = mainEngine.getStartUpPhase()->getInitPhase()->getMap()->getVertices();

    Vertex* BAVertex = vertices->find("BA")->second;

    players->begin()->second->addArmiesToCountry(BAVertex, 1);

    mainEngine.declareWinner();
}

void ScoreTest::test_playerHasWildCard() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_playerHasWildCard" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card* cardArr[] = {
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WILD, "Add 3 armies"),
        new Card(0, WILD, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, WILD, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, STONE, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, WOOD, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies"),
        new Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
        it->second->addCardToHand(cardArr[i++]);
    }

    mainEngine.declareWinner();
}


void ScoreTest::test_gameEndsWhenPlayersHaveMaxNumCards() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_gameEndsWhenPlayersHaveMaxNumCards" << endl;
    cout << "=====================================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    int maxNumCards = mainEngine.getMaxNumberOfCards();

    while(true) {

        Player* currentPlayer = mainEngine.getNextPlayer();

        currentPlayer->addCardToHand(new Card(1, STONE, ""));

        if (!mainEngine.continueGame(maxNumCards))
            break;
    }

    mainEngine.declareWinner();
}