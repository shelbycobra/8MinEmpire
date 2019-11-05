#include "../GameMainLoop.h"
#include "../MapLoader.h"
#include "../Cards.h"


int main() {

    GameMainEngine gameEngine;

    gameEngine.getStartUpPhase()->startGame();

    string cont;

    while(true) {

        Player* currentPlayer = gameEngine.getNextPlayer();
        Card* currentCard = gameEngine.chooseCardFromHand(currentPlayer);

        // gameEngine.performCardAction(currentPlayer, currentCard->getAction());
        cout << "\n\n---------------------------------------------------------------------" << endl;
        cout << "[ MOCK ] " << currentPlayer->getName() << " performs card action: " << currentCard->getAction() << endl; 
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