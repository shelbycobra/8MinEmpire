/*
Part 6: Main game loop: Compute the game score
Provide a group of C++ classes that compute the game score. Each player needs to implement
ComputeScore() method, that counts the victory points for each player that includes (i) the
regions he/she won, (ii) continents, (iv) and goods. The game ends when each player owns a
certain number of cards depending on the number of players.
2 Players 13 Cards
3 Players 10 Cards
4 Players 8 Cards
5 Players 7 Cards
Requirements part 6: You must deliver a driver that demonstrates:
 The result of the ComputeScore() method that counts the victory points for each
player.
 Displays the winner
*/

#include "../GameMainLoop.h"
#include "../MapLoader.h"
#include "../Cards.h"
#include "../util/TestUtil.h"


void test_onePlayerWinsGame();
void test_twoPlayersTieButHaveDifferentNumCoins();
void test_twoPlayersTieAndHaveSameNumCoins();
void test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies();
void test_playerHasWildCard();

int main() {
    // GameMainEngine mainEngine;
    // mainEngine.getStartUpPhase()->startGame();
    // int maxNumCards = mainEngine.getMaxNumberOfCards();
    // while(mainEngine.continueGame(maxNumCards)) {
    //     mainEngine.playTurn();
    // }
    // Player* winner = mainEngine.declareWinner();
    // cout << "[ GAME ] " << winner->getName() << " is the winner!" << endl;

    test_onePlayerWinsGame();
    return 0;
}


void test_onePlayerWinsGame() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_onePlayerWinsGame" << endl;
    cout << "=======================================================" << endl;

    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    Players* players = mainEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    //Create cards
    Card cardArr[] = {
        Card(0, WOOD, "Add 3 armies"),
        Card(0, WOOD, "Add 3 armies"),
        Card(0, WOOD, "Add 3 armies"),
        Card(0, WOOD, "Add 3 armies"),
        Card(0, WOOD, "Add 3 armies"),
        Card(0, STONE, "Add 3 armies"),
        Card(0, STONE, "Add 3 armies"),
        Card(0, STONE, "Add 3 armies"),
        Card(0, STONE, "Add 3 armies"),
        Card(0, STONE, "Add 3 armies"),
        Card(0, CARROT, "Add 3 armies"),
        Card(0, CARROT, "Add 3 armies"),
        Card(0, CARROT, "Add 3 armies"),
        Card(0, CARROT, "Add 3 armies"),
        Card(0, CARROT, "Add 3 armies")
    };

    int i = 0;

    // Add cards to player hand
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->addCardToHand(&cardArr[i++]);
        it->second->addCardToHand(&cardArr[i++]);
        it->second->addCardToHand(&cardArr[i++]);
        it->second->addCardToHand(&cardArr[i++]);
        it->second->addCardToHand(&cardArr[i++]);
    }

    Player* winner = mainEngine.declareWinner();

    cout << "[ GAME ] " << winner->getName() << " is the winner!" << endl;
}

void test_twoPlayersTieButHaveDifferentNumCoins() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_twoPlayersTieButHaveDifferentNumCoins" << endl;
    cout << "=======================================================" << endl;
}
void test_twoPlayersTieAndHaveSameNumCoins() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_twoPlayersTieAndHaveSameNumCoins" << endl;
    cout << "=======================================================" << endl;
}
void test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies" << endl;
    cout << "=======================================================" << endl;
}

void test_playerHasWildCard() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_playerHasWildCard" << endl;
    cout << "=======================================================" << endl;
}