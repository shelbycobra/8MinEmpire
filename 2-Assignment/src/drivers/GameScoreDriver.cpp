#include "../util/ScoreTest.h"
#include <string>
#include <iostream>
using namespace std;

int main() {
    ScoreTest test;

    while (true) {
        string num;

        cout << "[ GAME SCORE TEST ] Please choose which test to run: " << endl;
        cout << "1. test_onePlayerWinsGame()" << endl;
        cout << "2. test_twoPlayersTieButHaveDifferentNumCoins()" << endl;
        cout << "3. test_twoPlayersTieAndHaveSameNumCoins()" << endl;
        cout << "4. test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies()" << endl;
        cout << "5. test_playerHasWildCard()" << endl;
        cout << "6. test_gameEndsWhenPlayersHaveMaxNumCards()" << endl;
        cout << "[ GAME SCORE TEST ] > ";

        getline(cin, num);

        try {
            int testNum = stoi(num);

            if (testNum > 0 && testNum < 7) {
                switch(testNum) {
                    case 1:
                        test.test_onePlayerWinsGame();
                        break;
                    case 2:
                        test.test_twoPlayersTieButHaveDifferentNumCoins();
                        break;
                    case 3:
                        test.test_twoPlayersTieAndHaveSameNumCoins();
                        break;
                    case 4:
                        test.test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies();
                        break;
                    case 5:
                        test.test_playerHasWildCard();
                        break;
                    case 6:
                        test.test_gameEndsWhenPlayersHaveMaxNumCards();
                        break;
                }
                break;
            } else {
                cout << "\n[ ERROR! ] Please put in a number from 1-6.\n" << endl;
            }
        } catch (invalid_argument &e) {
            cout << "\n[ ERROR! ] Please put in a number.\n" << endl;
        }
    }

    return 0;
}