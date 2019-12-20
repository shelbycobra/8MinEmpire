#include "../GameMainLoop.h"


int main() {
    MainGameEngine gameEngine;

    gameEngine.startGame();

    string cont;

    while(true) {

        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();

        gameEngine.addNewCardToBackOfHand();

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    Players* players = gameEngine.getPlayers();

    for (Players::iterator it = players->begin(); it != players->end(); ++it) {
        cout << "\n[ PLAYER CARDS ] " << it->first << endl;

        for (Card* c : *it->second->getHand()) {
            cout << "{ CARD: " << c->getGood() << " : " << c->getAction() << " }" << endl;
        }
    }

    players = nullptr;

    return 0;
}