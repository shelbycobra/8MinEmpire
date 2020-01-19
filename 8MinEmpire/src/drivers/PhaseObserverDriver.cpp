#include "../GameEngine.h"
#include "../GameObservers.h"

int main() {
    MainGameEngine gameEngine;

    PhaseObserver PhaseObserver(&gameEngine);

    gameEngine.startGame();

    string cont;

    while(true) {

        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();
        gameEngine.performCardAction();
        gameEngine.addNewCardToBackOfHand();

        if (!gameEngine.continueGame())
            break;

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    gameEngine.declareWinner();

    return 0;
}