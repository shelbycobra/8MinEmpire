#include "../GameMainLoop.h"


int main() {
    GameMainEngine gameEngine;

    gameEngine.getStartUpPhase()->startGame();

    string cont;

    while(true) {

        Player* currentPlayer = gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand(currentPlayer);

        gameEngine.addNewCardToBackOfHand();

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    Players* players = gameEngine.getStartUpPhase()->getInitPhase()->getPlayers();

    for (Players::iterator it = players->begin(); it != players->end(); ++it) {
        cout << "\n[ PLAYER CARDS ] " << it->first << endl;

        for (Card* c : *it->second->getHand()) {
            cout << "{ CARD: " << c->getGood() << " : " << c->getAction() << " }" << endl;
        }
    }

    players = nullptr;

    return 0;
}