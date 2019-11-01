/*
cards, and assign an empty hand of cards to each player.
Requirements part 1: You must deliver a driver that demonstrates:
* The different valid maps can be loaded and their validity is verified (i.e. it is a connected
graph, etc.), and invalid maps are rejected without the program crashing.
* The right number of players is created.
* An assigned deck with the right number of cards is created.
* An assigned biding facility to each player.
* An assigned empty hand of cards to each player.
*/

#include "../GameStartUp.h"

int main() {
    GameStartUpEngine* startPhase = new GameStartUpEngine();
    startPhase->startGame();

    delete startPhase;
    startPhase = nullptr;

    return 0;
}