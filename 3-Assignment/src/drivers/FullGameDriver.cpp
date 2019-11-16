#include "../GameMainLoop.h"
#include "../GameObservers.h"

int main() {
    MainGameEngine gameEngine;

    PhaseObserver phaseObserver(&gameEngine);
    StatsObserver statsObserver(&gameEngine);

    gameEngine.startGame();

    while(true) {
        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();
        gameEngine.performCardAction();
        gameEngine.addNewCardToBackOfHand();
        gameEngine.askToChangePlayerStrategy();
    }

    return 0;
}