#include "../GameEngine.h"
#include "../GameObservers.h"

int main() {
    MainGameEngine gameEngine;

    PhaseObserver PhaseObserver(&gameEngine);
    StatsObserver statsObserver(&gameEngine);

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
        if(cont != "y" && cont != "Y") {
            //Add cards to each player until they reach their max card capacity to end the game.
            while(true) {
                gameEngine.getNextPlayer();
                gameEngine.getCurrentPlayer()->addCardToHand(new Card(1, STONE, ""));

                if (!gameEngine.continueGame()) {
                    gameEngine.Notify();
                    break;
                }
            }
        }
    }

    return 0;
}