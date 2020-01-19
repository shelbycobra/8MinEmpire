#include<algorithm>
#include "GameStartUp.h"

StartUpGameEngine* StartUpGameEngine::startUpInstance = 0;

/**
 * Default Constructor
 */
StartUpGameEngine::StartUpGameEngine():
    nextTurn(new queue<Player*>()),
    coinSupply(new int(44)) {}

/**
 * Deconstructor
 */
StartUpGameEngine::~StartUpGameEngine(){
    delete InitGameEngine::instance();
    delete nextTurn;

    startUpInstance = nullptr;
    nextTurn = nullptr;
}

/**
 * Returns a singleton instance of a StartUpGameEngine object.
 */
StartUpGameEngine* StartUpGameEngine::instance() {
    if (!startUpInstance) {
        startUpInstance = new StartUpGameEngine();
    }
    return startUpInstance;
}

/**
 * Starts the game.
 *
 * Uses the InitGameEngine object to initialize map and players, then
 * sets up the "game board" by distributing the starter coins to each player, asking for the start
 * region, and placing 3 armies on the start region for each player.
 *
 * If there are only two players, a third player "Anon" is created and the other
 * two players each take turns placing armies anywhere on the map until there are
 * 4 armies belonging to Anon on the map.
 *
 */
void StartUpGameEngine::startGame() {
    // Create Map and Players
    InitGameEngine::instance()->initGame();

    // Setup Game Board
    distributeCoins();
    selectStartVertex();
    placeStartingArmies();

    // Ask the players their bids to determine the first player.
    Player* winner = Bidder::startBid(InitGameEngine::instance()->getPlayers());

    int winningBid = winner->getBidder()->getBidAmount();
    removeCoinsFromSupply(winningBid);

    Player* firstPlayer = Bidder::getFirstPlayer(winner, InitGameEngine::instance()->getPlayers());
    setPlayerOrderInQueue(firstPlayer);
}

/**
 * Distrbutes coins to each player from the coin supply. The amount depends on how
 * many players there are:
 *
 * five players  ->  8 coins
 * four players  ->  9 coins
 * three players ->  11 coins
 * two players   ->  14 coins.
 */
void StartUpGameEngine::distributeCoins() {
    int coins = 18 - InitGameEngine::instance()->getNumPlayers() * 2;
    if (InitGameEngine::instance()->getNumPlayers() == 3 || InitGameEngine::instance()->getNumPlayers() == 4)
        coins--;

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "[ START ] Distributing coins to players. Each player gets " << coins << " coins." << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

    Players::iterator it;
    for(it = InitGameEngine::instance()->getPlayers()->begin(); it != InitGameEngine::instance()->getPlayers()->end(); ++it) {
        it->second->fillPurseFromSupply(coins);
        removeCoinsFromSupply(coins);
    }
}

/**
 * Prompts a user to choose a start region on the map.
 */
void StartUpGameEngine::selectStartVertex() {
    Vertices* vertices = GameMap::instance()->getVertices();

    while(true) {
        string answer;

        GameMap::instance()->printMap();
        GameMap::instance()->printOccupiedRegions();

        cout << "\n[ START ] Please choose the start vertex on the map:" << endl;
        cout << "[ START ] > ";

        getline(cin, answer);
        transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

        if (vertices->find(answer) != vertices->end()) {
            GameMap::instance()->setStartVertex(answer);
            break;
        }

        cout << "[ ERROR! ] That vertex doesn't exist on the map." << endl;
    }

}

/**
 * Places 3 armies on the start region for each player.
 *
 * If there are only two players, a third player "Anon" is created and the other
 * two players each take turns placing armies anywhere on the map until there are
 * 4 armies belonging to Anon on the map.
 */
void StartUpGameEngine::placeStartingArmies() {
    Vertex* startVertex = GameMap::instance()->getStartVertex();

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "[ START ] Placing 3 armies on the start vertex < " << startVertex->getName() << " >." << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;
    Players* players = InitGameEngine::instance()->getPlayers();

    for(Players::iterator it = players->begin(); it != players->end(); ++it) {
        it->second->executeAddArmies(3, startVertex);
    }

    if (InitGameEngine::instance()->getPlayers()->size() == 2 && !InitGameEngine::instance()->isTournament())
        placeAnonArmies();
}

/**
 * Populates the nextTurn queue in "clock-wise order" (aka, the order of creation) starting with
 * the first player.
 *
 * @param firstPlayer A Player pointer to the first player chosen by the winner during bidding.
 */
void StartUpGameEngine::setPlayerOrderInQueue(Player* firstPlayer) {
    nextTurn->push(firstPlayer);

    Players* players = InitGameEngine::instance()->getPlayers();
    vector<string>* playerOrder = InitGameEngine::instance()->getPlayerOrder();

    vector<string>::iterator it;

    bool afterFirstPlayer = false;

    // Add all players created after firstPlayer to nextTurn queue
    // so that it matches the "clock wise" order of players.
    for(it = playerOrder->begin(); it != playerOrder->end(); ++it) {
        if ((*it) == firstPlayer->getName()) {
            afterFirstPlayer = true;
        } else if(afterFirstPlayer) {
            nextTurn->push(players->find(*it)->second);
        }
    }

    // Then, add all players created before the first player in a "clock-wise" order
    for(it = playerOrder->begin(); it != playerOrder->end(); ++it) {
        if ((*it) == firstPlayer->getName())
            break;

        if((*it) != firstPlayer->getName() && (*it) != ANON) {
            nextTurn->push(players->find(*it)->second);
        }
    }
}

/**
 * Prompts players to place Anon's armies anywhere on the map.
 * This method is called when there are only 2 players. Each players
 * chooses 2 regions to add an army, up to 4 total Anon armies.
 */
void StartUpGameEngine::placeAnonArmies() {
    Players* players = InitGameEngine::instance()->getPlayers();
    queue<string> nextTurn;

    PlayerEntry anonPlayerEntry (ANON, InitGameEngine::instance()->getColours()->front());

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "[ START ] Because there are only 2 players, pleast take turns" << endl;
    cout << "\t  placing 4 armies belonging to a third Anon player on the map." << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

    Players::iterator it;
    for (it = players->begin(); it != players->end(); ++it)
        nextTurn.push(it->first);

    int placedArmies = 0;
    Player* anonPlayer = new Player(ANON, anonPlayerEntry.second);

    while(placedArmies < 4) {
        string player = nextTurn.front();
        nextTurn.pop();
        nextTurn.push(player);

        chooseAnonVertex(player, anonPlayer);

        placedArmies++;
    }

    GameMap::instance()->printMap();
    GameMap::instance()->printOccupiedRegions();

    players->insert(pair<string, Player*>(ANON, anonPlayer));
}


/**
 * Prompts the player to choose a vertex for the Anon player.
 */
void StartUpGameEngine::chooseAnonVertex(string &player, Player* anonPlayer) {

    string startName;

    while(true){
        cout << "\n---------------------------------------------------------------------------" << endl;
        cout << "{ " << player << " } Choose a region to place one of " << anonPlayer->getName() << "'s armies." << endl;
        cout << "---------------------------------------------------------------------------\n" << endl;

        GameMap::instance()->printMap();
        GameMap::instance()->printOccupiedRegions();

        cout << "{ " << player << " } > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (GameMap::instance()->getVertices()->find(startName) == GameMap::instance()->getVertices()->end()) {
            cout << "[ ERROR! ] You chose an invalid region name. Please try again." << endl;
        } else {
            Vertex* chosenVertex = GameMap::instance()->getVertices()->find(startName)->second;
            anonPlayer->addArmiesToRegion(chosenVertex, 1);
            break;
        }
    }
}

/**
 * Adds coins to the coin supply.
 */
void StartUpGameEngine::addCoinsToSupply(int amount) {
    *coinSupply += amount;
    cout << "[ GAME COINS ] Coin supply now contains " << getCoinSupply() << " coins. " << endl;
}

/**
 * Removes coins from the coin supply.
 */
void StartUpGameEngine::removeCoinsFromSupply(int amount) {
    *coinSupply -= amount;
    cout << "[ GAME COINS ] Coin supply now contains " << getCoinSupply() << " coins. " << endl;
}