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
    int i = 1;
    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        cout << "\n" << i << ": ";
        it->second->executeAddArmies(3, startVertex, startName);
        i++;
    }

    if (players->size() == 2) {
        placeAnonArmies();
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

void GameStartUpEngine::placeAnonArmies() {
    Players* players = initPhase->getPlayers();
    queue<string> nextTurn;

    PlayerEntry anonPlayerEntry (ANON, initPhase->getColours()->front());

    cout << "\n[ START ] Because there are only 2 players, pleast take turns placing 4 armies belonging to Anon on the map.\n" << endl;

    Players::iterator it;
    for (it = players->begin(); it != players->end(); ++it)
        nextTurn.push(it->first);

    int placedArmies = 0;

    while(placedArmies < 4) {
        string player = nextTurn.front();
        nextTurn.pop();
        nextTurn.push(player);

        chooseVertex(player, &anonPlayerEntry);

        placedArmies++;
    }

    players->insert(pair<string, Player*>(ANON, new Player(ANON, anonPlayerEntry.second)));
}

void GameStartUpEngine::chooseVertex(string &player, PlayerEntry* anonPlayerEntry) {

    string startName;

    while(true){
        cout << "3: { " << player << " } Choose a region to place one of " << anonPlayerEntry->first << "'s armies." << endl;
        initPhase->getMap()->printMap();
        cout << "3: { " << player << " } > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (initPhase->getMap()->getVertices()->find(startName) == initPhase->getMap()->getVertices()->end()) {
            cout << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            Vertex* chosenVertex = initPhase->getMap()->getVertices()->find(startName)->second;

            if (chosenVertex->getArmies()->find(anonPlayerEntry) == chosenVertex->getArmies()->end()) {
                chosenVertex->getArmies()->insert(pair<PlayerEntry*, int>(anonPlayerEntry, 1));
            } else {
                int numCurrentArmies = chosenVertex->getArmies()->find(anonPlayerEntry)->second;
                chosenVertex->getArmies()->erase(anonPlayerEntry);
                chosenVertex->getArmies()->insert(pair<PlayerEntry*, int>(anonPlayerEntry, numCurrentArmies + 1));
            }

            break;
        }
    }
}