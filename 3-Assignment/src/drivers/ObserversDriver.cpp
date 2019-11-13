#include "../GameMainLoop.h"
#include "../GameObservers.h"

int main() {
    MainGameEngine gameEngine;

    GameObserver gameObserver;
    StatsObserver statsObserver;

    gameEngine.Attach(&gameObserver);
    gameEngine.Attach(&statsObserver);

    gameEngine.getStartUpPhase()->startGame();

    int maxNumCards = gameEngine.getMaxNumberOfCards();

    string cont;

    while(true) {

        Player* currentPlayer = gameEngine.getNextPlayer();
        Card* currentCard = gameEngine.chooseCardFromHand(currentPlayer);

        gameEngine.performCardAction(currentPlayer, currentCard->getAction());
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