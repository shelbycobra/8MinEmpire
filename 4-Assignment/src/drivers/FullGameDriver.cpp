#include "../GameEngine.h"
#include "../GameObservers.h"

int main() {
    StartUpGameEngine::instance()->startGame();

    GameEngine* engine = GameEngineFactory::create();

    PhaseObserver phaseObserver(engine);
    StatsObserver statsObserver(engine);

    engine->runGame();

    return 0;
}