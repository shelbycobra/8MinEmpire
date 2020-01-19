#include "../GameEngine.h"


int main() {
    MainGameEngine gameEngine;

    gameEngine.startGame();

    string cont;

    while(true) {
        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();
        gameEngine.performCardAction();
        gameEngine.addNewCardToBackOfHand();
        gameEngine.askToChangePlayerStrategy();

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    gameEngine.declareWinner();

    return 0;
}