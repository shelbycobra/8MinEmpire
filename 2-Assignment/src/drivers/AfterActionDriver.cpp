#include "../GameMainLoop.h"
#include "../MapLoader.h"
#include "../Cards.h"


int main() {

    GameMainEngine* gameEngine = new GameMainEngine();

    gameEngine->getStartUpPhase()->startGame();

    string cont;

    while(true) {

        gameEngine->playTurn();

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
        if(cont != "y" && cont != "Y")
            break;
    }

    return 0;
}