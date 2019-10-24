#include<algorithm>
#include "GameStartUp.h"

/**
 * Default Constructor
 */
GameStartUpEngine::GameStartUpEngine(): 
    initPhase(new GameInitEngine()), nextTurn(new queue<Player*>()) {}

/**
 * Takes an GameInitEngine object.
 */
GameStartUpEngine::GameStartUpEngine(GameInitEngine* gameInit): 
    initPhase(gameInit), nextTurn(new queue<Player*>()) {}

/**
 * Copy Constructor
 */
GameStartUpEngine::GameStartUpEngine(GameStartUpEngine* otherStartUpEngine){
    initPhase = new GameInitEngine(otherStartUpEngine->getGameInitEngine());
    nextTurn = new queue<Player*>(*otherStartUpEngine->getNextTurnQueue());
}

/**
 * Assigment operator
 */
GameStartUpEngine& GameStartUpEngine::operator=(GameStartUpEngine& otherStartUpEngine) {
    initPhase = new GameInitEngine(otherStartUpEngine.getGameInitEngine());
    nextTurn = new queue<Player*>(*otherStartUpEngine.getNextTurnQueue());
    return *this;
}

/**
 * Deconstructor
 */
GameStartUpEngine::~GameStartUpEngine(){
    delete nextTurn;

    initPhase = nullptr;
    nextTurn = nullptr;
}

/**
 * Assumes initPhase has already been initialized
 */
void GameStartUpEngine::startGame() {

    distributeCoins();
    selectStartVertex();
    placeArmiesOnStartVertex();
    Player* firstPlayer = Bidder::startBid(initPhase->getPlayers());
    setPlayerOrderInQueue(firstPlayer);
}

void GameStartUpEngine::distributeCoins() {
    int coins = 18 - initPhase->getNumPlayers() * 2;
    if (initPhase->getNumPlayers() == 3 || initPhase->getNumPlayers() == 4)
        coins--;

    cout << "\n[ START ] Distributing coins to players. Each player gets " << coins << " coins." << endl;
    Players::iterator it;
    for(it = initPhase->getPlayers()->begin(); it != initPhase->getPlayers()->end(); ++it) {
        it->second->setCoins(coins);
    }
}

void GameStartUpEngine::selectStartVertex() {
    Vertices* vertices = initPhase->getMap()->getVertices();
    
    while(true) {
        string answer;
        
        initPhase->getMap()->printMap();

        cout << "\n[ START ] Please choose the start vertex on the map:" << endl;
        cout << "[ START ] > ";

        getline(cin, answer);
        transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

        if (vertices->find(answer) != vertices->end()) {
            initPhase->getMap()->setStartVertex(answer);
            break;
        }

        cout << "[ ERROR! ] That vertex doesn't exist on the map." << endl;
    }

}

void GameStartUpEngine::placeArmiesOnStartVertex() {
    string startName = initPhase->getMap()->getStartVertex();

    cout << "\n[ START ] Placing 3 armies on the start vertex < " << startName << " >.\n" << endl;
    Players* players = initPhase->getPlayers();

    Vertex* startVertex = initPhase->getMap()->getVertices()->find(startName)->second;
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->executeAddArmies(3, startVertex, startName);
    }

    if (players->size() == 2) {
        //Place 3 armies of a nonplayer on start vertex.
        cout << "{ anon } Added 3 new armies to < " << startVertex->getName() << " >." << endl;
        startVertex->getArmies()->insert(pair<string, int>("Anon", 3));
    }
}

void GameStartUpEngine::setPlayerOrderInQueue(Player* firstPlayer) {
    nextTurn->push(firstPlayer);

    Players* players = initPhase->getPlayers();
    Players::iterator it;
    for(it = players->begin(); it != players->end(); ++it)
        if(it->second != firstPlayer)
            nextTurn->push(it->second);
}