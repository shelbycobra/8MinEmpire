#include "../GameStartUp.h"

int main() {
    GameStartUpEngine* startPhase = new GameStartUpEngine();
    startPhase->startGame();

    delete startPhase;
    startPhase = nullptr;

    return 0;
}