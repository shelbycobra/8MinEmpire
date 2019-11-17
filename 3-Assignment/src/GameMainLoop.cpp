#include "GameMainLoop.h"
#include "GameObservers.h"
#include "PlayerStrategies.h"

/**
 * Default Constructor
 */
MainGameEngine::MainGameEngine():
    startUpPhase(new StartUpGameEngine()),
    currentPlayer(new Player()),
    currentCard(new Card()) {}

/**
 * Copy Constructor
 */
MainGameEngine::MainGameEngine(MainGameEngine* otherMainGameEngine) {
    startUpPhase = otherMainGameEngine->getStartUpPhase(),
    currentPlayer = otherMainGameEngine->getCurrentPlayer();
    currentCard = otherMainGameEngine->getCurrentCard();
}

/**
 * Assignment Operator
 */
MainGameEngine& MainGameEngine::operator=(MainGameEngine& otherMainGameEngine) {
    startUpPhase = otherMainGameEngine.getStartUpPhase();
    currentPlayer = otherMainGameEngine.getCurrentPlayer();
    currentCard = otherMainGameEngine.getCurrentCard();
    return *this;
}

/**
 * Destructor
 */
MainGameEngine::~MainGameEngine() {
    delete startUpPhase;

    startUpPhase = nullptr;
    currentPlayer = nullptr;
    currentCard = nullptr;
}

/**
 * Gets the next player in the nextTurn queue.
 */
void MainGameEngine::getNextPlayer() {
    queue<Player*>* nextTurn = startUpPhase->getNextTurnQueue();

    currentPlayer = nextTurn->front();
    nextTurn->pop();
    nextTurn->push(currentPlayer);

    cout << "\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;
}

/**
 * Prompts the current player to select a card from the game hand, which displays 6 card.
 * The price of each card depends on the cards position. Starting from left, the value of
 * each card is 0, 1, 1, 2, 2, 3. The player pays for the selected card immediately after.
 */
void MainGameEngine::chooseCardFromHand() {

    // Get the number of coins from the current player before and after they pay for a card.
    // Add the difference to the game coin supply.

    int startCoins = currentPlayer->getCoins();
    currentCard = startUpPhase->getHand()->exchange(currentPlayer);
    int endCoins = currentPlayer->getCoins();

    startUpPhase->addCoinsToSupply(startCoins - endCoins);
}

/**
 * Executes the action of the current card on the current player.
 *
 */
void MainGameEngine::performCardAction() {
    Players* players = startUpPhase->getPlayers();

    const string action = currentCard->getAction();

    string answer;
    cout << "[ GAME ] Ignore card action < " << action << " > (y/n)?" << endl;
    cout << "[ GAME ] > ";
    getline(cin, answer);

    if (answer == "y" || answer == "Y") {
        currentPlayer->Ignore();
    } else {
        if(action.find("OR") != size_t(-1) || action.find("AND") != size_t(-1))
            currentPlayer->AndOrAction(action, players);
        else if (action.find("Add") != size_t(-1))
            currentPlayer->PlaceNewArmies(action, players);
        else if (action.find("Destroy") != size_t(-1))
            currentPlayer->DestroyArmy(players);
        else if (action.find("Build") != size_t(-1))
            currentPlayer->BuildCity();
        else if (action.find("Move") != size_t(-1)) {
            if (action.find("water") != size_t(-1))
                currentPlayer->MoveOverWater(action, players);
            else
                currentPlayer->MoveOverLand(action, players);
        }
        else
            cout << "[ ERROR! ] Invalid action." << endl;
    }

    // Notify observers of changes
    Notify();
}

/**
 * Draws a card from the deck and adds it to the 6th position of the game hand,
 * effectively shifting each card left one slot up until the empty slot is filled.
 */
void MainGameEngine::addNewCardToBackOfHand() {
    startUpPhase->getHand()->drawCardFromDeck();
}

/**
 * Checks whether the game should continue based on the number of cards
 * in each player's hand.
 *
 * @return A boolean indicating whether the game should continue. Returns
 * false only when all players have reached the maxNumCards.
 */
bool MainGameEngine::continueGame() {
    int maxNumCards = getMaxNumberOfCards();

    Players::iterator it;
    Players* players = startUpPhase->getPlayers();

    for(it = players->begin(); it != players->end(); ++it) {
        if (it->first != ANON && it->second->getHand()->size() < size_t(maxNumCards))
            return true;
    }

    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "                      E N D  O F  T H E  G A M E " << endl;
    cout << "---------------------------------------------------------------------\n" << endl;

    return false;
}

/**
 * Declares the winner of the game.
 *
 * Calls on each player to calculate their total score and uses that information to determine
 * the winner.
 *
 * From the Game Rules:
 *  "The player who has the most victory points from regions, continents,
 * and goods has the most powerful empire and is the winner! If
 * players are tied, the player with the most coins wins. If still tied, the player
 * with the most armies on the board wins. If still tied, the player with the
 * most controlled regions wins.""
 */
void MainGameEngine::declareWinner() {

    cout << "\n[ GAME ] Finding the winner.\n" << endl;

    Player* winner = new Player();
    int highestScore = 0;

    Players* players = startUpPhase->getPlayers();

    map<Player*, int> scores;

    for(Players::iterator it = players->begin(); it != players->end(); it++) {
        int playerScore = it->second->ComputeScore();

        scores.insert(pair<Player*, int>(it->second, playerScore));

        if(playerScore > highestScore) {
            highestScore = playerScore;
            winner = it->second;
        }
    }

    if (highestScore > 0) {
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
                    int winnerArmies = 14 - winner->getArmies();
                    int otherArmies = 14 - it->first->getArmies();

                    cout << "[ GAME ] " << it->first->getName() << " has the same number of coins as " << winner->getName() << "." << endl;
                    cout << "\n[ GAME ] Comparing number of armies instead ..." << endl;
                    cout << "[ GAME ] " << it->first->getName() << " has " << otherArmies
                        << " armies and " << winner->getName() << " has " << winnerArmies
                        << " armies." << endl;

                    if (otherArmies > winnerArmies) {
                        winner = it->first;
                        break;
                    } else if (otherArmies == winnerArmies) {
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
    }

    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "                     W I N N E R  I S  " << winner->getName() << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------\n" << endl;
}

/**
 * Gets the maxumim number of cards the players can have. This is the number of cards
 * each player must have in order to end the game and calculate the winner.
 *
 * The max number of cards depends on the number of players.
 * 2 Players -> 13 Cards
 * 3 Players -> 10 Cards
 * 4 Players -> 8 Cards
 * 5 Players -> 7 Cards
 *
 * @return The number of cards in each player's hand required to end the game.
 */
int MainGameEngine::getMaxNumberOfCards() {
    int numPlayers = startUpPhase->getNumPlayers();

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

void MainGameEngine::askToChangePlayerStrategy() {
    string answer;
    cout << "[ GAME ] Would you like to change the playing strategy of { " << currentPlayer->getName() << " }?" << endl;
    cout << "[ GAME ] (y/n) > ";
    getline(cin, answer);

    Strategy* newStrategy;

    if (answer == "y" || answer == "Y") {
        while (true) {
            string strategyName;

            cout << "\n[ GAME ] Which playing strategy would you like to change to?" << endl;
            cout << "[ GAME ] Current playing strategy is " << currentPlayer->getStrategy()->getType() << "." << endl;
            cout << "[ GAME ] Options:" << endl;
            cout << "\n1. HUMAN\n2. GREEDY\n3. MODERATE\n" << endl;
            cout << "[ GAME ] Please enter a number between 1 and 3." << endl;
            cout << "[ GAME ] > ";

            getline(cin, strategyName);

            try{
                int choice = stoi(strategyName);

                if (choice == 1) {
                    newStrategy = new HumanStrategy();
                    break;
                } else if (choice == 2) {
                    newStrategy = new GreedyStrategy();
                    break;
                } else if (choice == 3) {
                    newStrategy = new ModerateStrategy();
                    break;
                } else {
                    cout << "[ ERROR! ] Invalid choice." << endl;
                }
            } catch (invalid_argument &e) {
                cout << "[ ERROR! ] Please enter a number." << endl;
            }
        }

        cout << "[ GAME ] Setting strategy to " << newStrategy->getType() << "." << endl;
        currentPlayer->setStrategy(newStrategy);
    }
}