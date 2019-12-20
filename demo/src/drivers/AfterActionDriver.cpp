#include "../GameMainLoop.h"
#include "../MapLoader.h"
#include "../Cards.h"


int main() {

    MainGameEngine gameEngine;

    gameEngine.startGame();

    string cont;

    while(true) {

        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();

        // gameEngine.performCardAction(currentPlayer, currentCard->getAction());
        cout << "\n\n---------------------------------------------------------------------" << endl;
        cout << "[ MOCK ] " << gameEngine.getCurrentPlayer()->getName() << " performs card action: " << gameEngine.getCurrentCard()->getAction() << endl;
        cout << "---------------------------------------------------------------------\n\n" << endl;

        gameEngine.addNewCardToBackOfHand();

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    return 0;
}