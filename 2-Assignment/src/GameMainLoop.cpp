#include "GameMainLoop.h"

GameMainEngine::GameMainEngine(): startUpPhase(new GameStartUpEngine()) {}

GameMainEngine::GameMainEngine(GameMainEngine* otherGameMainEngine) {
    startUpPhase = new GameStartUpEngine(otherGameMainEngine->getStartUpPhase());
}

GameMainEngine& GameMainEngine::operator=(GameMainEngine& otherGameMainEngine) {
    startUpPhase = new GameStartUpEngine(otherGameMainEngine.getStartUpPhase());
    return *this;
}

GameMainEngine::~GameMainEngine() {
    delete startUpPhase;

    startUpPhase = nullptr;
}

void GameMainEngine::playTurn() {
    Players* players = startUpPhase->getInitPhase()->getPlayers();
    queue<Player*>* nextTurn = startUpPhase->getNextTurnQueue();
    GameMap* map = startUpPhase->getInitPhase()->getMap();
    Hand* gameHand = startUpPhase->getInitPhase()->getHand();

    Player* currentPlayer = nextTurn->front();
    nextTurn->pop();
    nextTurn->push(currentPlayer);

    cout << "\n\n\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;

    // Get the number of coins from the current player before and after they pay for a card.
    // Add the difference to the game coin supply.
    int startCoins = currentPlayer->getCoins();
    Card* currentCard = gameHand->exchange(currentPlayer);
    int endCoins = currentPlayer->getCoins();

    startUpPhase->addCoinsToSupply(startCoins - endCoins);

    performCardAction(currentPlayer, currentCard->getAction(), map, players);

    gameHand->drawCardFromDeck();
    gameHand->printHand();

    players = nullptr;
    nextTurn = nullptr;
    map = nullptr;
    gameHand = nullptr;
}

/**
 * Executes the action of a card.
 *
 * @param player A pointer to the Player object who is using the card.
 * @param action The action to be performed.
 * @param map A pointer to the GameMap object of the game.
 * @param players A pointer to the list of all players in the game.
 */
void GameMainEngine::performCardAction(Player* player, const string action, GameMap* map, Players* players) {
    string answer;
    cout << "[ GAME ] Ignore card action < " << action << " > (y/n)?" << endl;
    cout << "[ GAME ] > ";
    getline(cin, answer);

    if (answer == "y" || answer == "Y") {
        player->Ignore();
    } else {
        if(action.find("OR") != size_t(-1) || action.find("AND") != size_t(-1))
            player->AndOrAction(action, map, players);
        else if (action.find("Move") != size_t(-1)) {
            if (action.find("water") != size_t(-1))
                player->MoveOverLand(action, map);
            else
                player->MoveOverWater(action, map);
        }
        else if (action.find("Add") != size_t(-1))
            player->PlaceNewArmies(action, map);
        else if (action.find("Destroy") != size_t(-1))
            player->DestroyArmy(action, map, players);
        else if (action.find("Build") != size_t(-1))
            player->BuildCity(action, map);
        else
            cout << "[ ERROR! ] Invalid action." << endl;
    }
}

bool GameMainEngine::continueGame(int maxNumCards) {

    Players::iterator it;
    Players* players = startUpPhase->getInitPhase()->getPlayers();

    for(it = players->begin(); it != players->end(); ++it) {
        if (it->second->getHand()->size() < size_t(maxNumCards))
            return true;
    }

    return false;
}

Player* GameMainEngine::declareWinner(GameMap* map) {

    Player* winner;
    int highestScore = 0;

    Players::iterator it;
    Players* players = startUpPhase->getInitPhase()->getPlayers();

    for(it = players->begin(); it != players->end(); ++it) {
        int playerScore = it->second->ComputeScore(map);
        if(playerScore > highestScore) {
            highestScore = playerScore;
            winner = it->second;
        }
    }

    return winner;
}