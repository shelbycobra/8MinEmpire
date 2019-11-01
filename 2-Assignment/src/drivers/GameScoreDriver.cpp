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


int main() {
    GameMainEngine mainEngine;

    mainEngine.getStartUpPhase()->startGame();

    int maxNumCards;
    int numPlayers = mainEngine.getStartUpPhase()->getInitPhase()->getNumPlayers();
    if (numPlayers == 2)
        maxNumCards = 13;
    else if (numPlayers == 3)
        maxNumCards = 10;
    else if (numPlayers == 4)
        maxNumCards = 8;
    else if (numPlayers == 5)
        maxNumCards = 7;
    else {
        cout << "[ ERROR! ] Invalid number of players." << endl;
    }

    while(mainEngine.continueGame(maxNumCards)) {

        mainEngine.playTurn();

        // string cont;
        // cout << "CONTINUE DEMO?" << endl;
        // cout << "'y' or 'Y' to continue > ";
        // getline(cin, cont);
        // if(cont != "y" && cont != "Y")
        //     break;
    }

    Player* winner = mainEngine.declareWinner(initPhase.getPlayers(), initPhase.getMap());

    cout << "[ GAME ] " << winner->getName() << " is the winner!" << endl;

    return 0;
}