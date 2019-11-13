#include "../GameMainLoop.h"

int main() {
    MainGameEngine gameEngine;

    gameEngine.getStartUpPhase()->startGame();

    int maxNumCards = gameEngine.getMaxNumberOfCards();

    while(true) {

        Player* currentPlayer = gameEngine.getNextPlayer();
        Card* currentCard = gameEngine.chooseCardFromHand(currentPlayer);

        gameEngine.performCardAction(currentPlayer, currentCard->getAction());
        gameEngine.addNewCardToBackOfHand();

        if (!gameEngine.continueGame(maxNumCards)) 
            break;
    }

    gameEngine.declareWinner();

    return 0;
}