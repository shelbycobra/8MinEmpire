#include "../GameMainLoop.h"
#include "../MapLoader.h"


int main() {
    MainGameEngine mainLoop;

    mainLoop.startGame();

    Card move4Card(1, GEM, "Move 4 armies");
    Card orCard(2, CARROT, "Add 2 armies OR Build city");
    Card andCard(3, CARROT, "Destroy an army AND Add 1 army");
    Card buildCard(4, GEM, "Build city");

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Move armies over land." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    mainLoop.getNextPlayer();
    mainLoop.setCurrentCard(&move4Card);
    mainLoop.performCardAction();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Or card (Add 2 armies)." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    mainLoop.getNextPlayer();
    mainLoop.setCurrentCard(&orCard);
    mainLoop.performCardAction();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: And card (Destroy an army AND Add 1 army)." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    mainLoop.getNextPlayer();
    mainLoop.setCurrentCard(&andCard);
    mainLoop.performCardAction();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Build a city." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    mainLoop.getNextPlayer();
    mainLoop.setCurrentCard(&buildCard);
    mainLoop.performCardAction();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Ignore action." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    mainLoop.getNextPlayer();
    mainLoop.setCurrentCard(&move4Card);
    mainLoop.performCardAction();

    return 0;
}