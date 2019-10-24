#include "../GameMainLoop.h"
#include "../MapLoader.h"
#include "../Cards.h"


int main() {
    GameInitEngine initPhase;
    initPhase.initGame();

    GameStartUpEngine startPhase(&initPhase);
    startPhase.startGame();

    GameMainEngine mainLoop;
    string cont;

    while(true) {

        mainLoop.playTurn(initPhase.getPlayers(), startPhase.getNextTurnQueue(), initPhase.getMap(), initPhase.getHand());

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    return 0;
}