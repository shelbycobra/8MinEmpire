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

    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "                      E N D  O F  T H E  G A M E " << endl;
    cout << "---------------------------------------------------------------------\n" << endl;

    return false;
}

void GameMainEngine::declareWinner() {

    cout << "\n[ GAME ] Finding the winner.\n" << endl;

    Player* winner;
    int highestScore = 0;

    Players* players = startUpPhase->getInitPhase()->getPlayers();

    map<Player*, int> scores;

    for(Players::iterator it = players->begin(); it != players->end(); it++) {
        int playerScore = it->second->ComputeScore(startUpPhase->getInitPhase()->getMap());

        scores.insert(pair<Player*, int>(it->second, playerScore));

        if(playerScore > highestScore) {
            highestScore = playerScore;
            winner = it->second;
        }
    }

    for(map<Player*, int>::iterator it = scores.begin(); it != scores.end(); it++) {
        if (it->second == highestScore && it->first != winner) {
            cout << "[ GAME ] " << it->first->getName() << " has the same score as " << winner->getName() << "." << endl;
            cout << "\n[ GAME ] Comparing number of coins instead ... " << endl;
            cout << "[ GAME ] " << it->first->getName() << " has " << it->first->getCoins()
                 << " coins and " << winner->getName() << " has " << winner->getCoins()
                 << " coins." << endl;
            if (it->first->getCoins() > winner->getCoins()) {
                winner = it->first;
                break;
            } else if (it->first->getCoins() == winner->getCoins()) {
                cout << "[ GAME ] " << it->first->getName() << " has the same number of coins as " << winner->getName() << "." << endl;
                cout << "\n[ GAME ] Comparing number of armies instead ..." << endl;
                cout << "[ GAME ] " << it->first->getName() << " has " << it->first->getArmies()
                     << " armies and " << winner->getName() << " has " << winner->getArmies()
                     << " armies." << endl;
                if (it->first->getArmies() > winner->getArmies()) {
                    winner = it->first;
                    break;
                } else if (it->first->getArmies() == winner->getArmies()) {
                    cout << "[ GAME ] " << it->first->getName() << " has the same number of armies as " << winner->getName() << "." << endl;
                    cout << "\n[ GAME ] Comparing number of controlled regions instead ..." << endl;
                    cout << "[ GAME ] " << it->first->getName() << " has " << it->first->getControlledRegions()
                         << " controlled regions and " << winner->getName() << " has " << winner->getControlledRegions()
                         << " controlled regions." << endl;
                    if (it->first->getControlledRegions() > winner->getControlledRegions()) {
                        winner = it->first;
                        break;
                    }
                }
            }
        }
    }

    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "                     W I N N E R  I S  " << winner->getName() << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------\n" << endl;
}

 int GameMainEngine::getMaxNumberOfCards() {
    int numPlayers = startUpPhase->getInitPhase()->getNumPlayers();

    if (numPlayers == 2)
        return 13;
    else if (numPlayers == 3)
        return 10;
    else if (numPlayers == 4)
        return 8;
    else if (numPlayers == 5)
        return 7;
    else {
        cout << "[ ERROR! ] Invalid number of players." << endl;
        return 0;
    }
 }