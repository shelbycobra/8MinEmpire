#include "../GameEngine.h"

int main() {

    StartUpGameEngine::instance()->startGame();

    GameEngine* engine = GameEngineFactory::create();

    PhaseObserver phaseObserver(engine);
    StatsObserver statsObserver(engine);

    engine->runGame();

    return 0;
}