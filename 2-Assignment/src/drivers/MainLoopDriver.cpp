#include "../GameMainLoop.h"


int main() {
    GameMainEngine gameEngine;

    gameEngine.getStartUpPhase()->startGame();

    string cont;

    while(true) {

        Player* currentPlayer = gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand(currentPlayer);

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    return 0;
}