#include "../GameMainLoop.h"
#include "../GameObservers.h"

int main() {
    MainGameEngine gameEngine;

    PhaseObserver PhaseObserver(&gameEngine);
    StatsObserver statsObserver(&gameEngine);

    gameEngine.startGame();

    int maxNumCards = gameEngine.getMaxNumberOfCards();

    string cont;

    while(true) {

        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();
        gameEngine.performCardAction();
        gameEngine.addNewCardToBackOfHand();

        if (!gameEngine.continueGame(maxNumCards))
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