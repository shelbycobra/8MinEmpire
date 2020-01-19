#include "../GameStartUp.h"

int main() {
    StartUpGameEngine::instance()->startGame();

    delete StartUpGameEngine::instance();

    return 0;
}