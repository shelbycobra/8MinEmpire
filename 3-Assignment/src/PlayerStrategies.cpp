#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include <algorithm>

/**
 * Default Constructor
 */
Strategy::Strategy(): type(new string("none")) {}

/**
 * Constructor
 *
 * @param type The type of strategy.Either HUMAN, MODERATE or GREEDY.
 */
Strategy::Strategy(const string& type): type(new string(type)) {}

/**
 * Destructor
 */
Strategy::~Strategy() {
    delete type;
}

/**
 * Constructor
 */
GreedyStrategy::GreedyStrategy(): Strategy(GREEDY) {}

/**
 * Destructor
 */
GreedyStrategy::~GreedyStrategy() {
}

Vertex* GreedyStrategy::chooseStartVertex(Player* player) {
    return nullptr;

}

Vertex* GreedyStrategy::chooseEndVertex(Player* player, const ActionType& type) {
    return nullptr;
}

int GreedyStrategy::chooseArmies(Player* player, const int& maxArmies, const int& remainderArmies, int startVertexArmies, const string& startVertex) {
    return 0;
}

string GreedyStrategy::chooseORAction(Player* player, const string action) {
    return "";
}

Player* GreedyStrategy::chooseOpponent(Player* player, Players* players) {
    return nullptr;
}

/**
 * Constructor
 */
ModerateStrategy::ModerateStrategy(): Strategy(MODERATE) {}

/**
 * Destructor
 */
ModerateStrategy::~ModerateStrategy() {
}

Vertex* ModerateStrategy::chooseStartVertex(Player* player) {
    return nullptr;
}

Vertex* ModerateStrategy::chooseEndVertex(Player* player, const ActionType& type) {
    return nullptr;
}

int ModerateStrategy::chooseArmies(Player* player, const int& maxArmies, const int& remainderArmies, int startVertexArmies, const string& startVertex) {
    return 0;
}

string ModerateStrategy::chooseORAction(Player* player, const string action) {
    return "";
}

Player* ModerateStrategy::chooseOpponent(Player* player, Players* players) {
    return nullptr;
}

/**
 * Constructor
 */
HumanStrategy::HumanStrategy(): Strategy(HUMAN) {}

/**
 * Destructor
 */
HumanStrategy::~HumanStrategy() {
}

/**
 * Prompts a player to choose how many armies to place on a country.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param maxArmies The maximum number of armies the player can place. Determined by the card chosen.
 * @param remainderArmies The amount of armies left that the player can choose from.
 * @param startVertexArmies The number of armies on the vertex where the armies are coming from.
 * @param startVertex The name of the vertex that the armies are coming from.
 * @return The number of armies chosen to move.
 */
int HumanStrategy::chooseArmies(Player* player, const int& maxArmies, const int& remainderArmies, int startVertexArmies, const string& startVertex) {
    int armies;
    string armiesStr;

    if (startVertex == "none")
        startVertexArmies = remainderArmies;

    while (true) {
        string remainderArmiesStr = remainderArmies == 1 ? "army" : "armies";
        string startVertexArmiesStr = startVertexArmies == 1 ? "army" : "armies";

        cout << "{ " << player->getName() << " } How many armies? ( Max armies " << remainderArmies << ")" << endl;
        cout << "{ " << player->getName() << " } > ";
        getline(cin, armiesStr);

        try {
            armies = stoi(armiesStr);

            if (startVertexArmies - armies >= 0 && remainderArmies - armies >= 0 && remainderArmies - armies <= maxArmies) {
                cout << endl;
                break;
            }

            if (remainderArmies - armies)
                cout << "[ ERROR! ] You can only choose a maximum of " << remainderArmies << " " << remainderArmiesStr << "." << endl;
            else
                cout << "[ ERROR! ] < " << startVertex << " > only has " << startVertexArmies << " " << startVertexArmiesStr << " available." << endl;

        } catch(invalid_argument& e) {
            cout << "[ ERROR! ] Please enter a number." << endl;
        }
    }

    return armies;
}

/**
 * Prompts a player to choose a start vertex among the player's occupied countries.
 *
 * @param player A pointer to the player who is using this strategy.
 * @return A Vertex pointer to the start country.
 */
Vertex* HumanStrategy::chooseStartVertex(Player* player){

    string startName;

    while(true){
        cout << "{ " << player->getName() << " } You must choose a country to take armies from." << endl;
        player->printCountries();
        cout << "{ " << player->getName() << " } > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (player->getCountries()->find(startName) == player->getCountries()->end()) {
            cout << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            return player->getCountries()->find(startName)->second;
        }
    }
}

/**
 * Prompts a player to choose an end vertex on the map.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param type An ActionType enum representing the type of action being called.
 * @return A Vertex pointer to the end country.
 */
Vertex* HumanStrategy::chooseEndVertex(Player* player, const ActionType& type){
    string endName;

    while(true){
        if (type == ActionType::ADD_ARMY || type == ActionType::BUILD_CITY)
            player->printCountries();
        else if (type == ActionType::MOVE_OVER_LAND || type == ActionType::MOVE_OVER_WATER || type == ActionType::DESTROY_ARMY) {
            GameMap::instance()->printMap();
            GameMap::instance()->printOccupiedRegions();
        } else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return 0;
        }

        cout << "{ " << player->getName() << " } > ";
        getline(cin, endName);
        transform(endName.begin(), endName.end(), endName.begin(), ::toupper);
        cout << "{ " << player->getName() << " } You chose < " << endName << " >.\n" << endl;

        if (GameMap::instance()->getVertices()->find(endName) == GameMap::instance()->getVertices()->end()) {
            cerr << "[ ERROR! ] That country doesn't exist on the map." << endl;
            continue;
        }

        return GameMap::instance()->getVertices()->find(endName)->second;
    }
}


/**
 * Prompts a player to choose one of the action in an OR'd card.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param action The action containing an OR'd action.
 * @return The action chosen by the player.
 */
string HumanStrategy::chooseORAction(Player* player, const string action) {
    string answer;
    string firstChoice;
    string secondChoice;

    int orPos = action.find("OR");

    while(true) {
        cout << "\n{ " << player->getName() << " } Which action do you want? 1 or 2 ?" << endl;

        firstChoice = action.substr(0, orPos - 1);
        secondChoice = action.substr(orPos + 3);

        cout << "[ OPTION 1 ] " << firstChoice << endl;
        cout << "[ OPTION 2 ] " << secondChoice << endl;

        cout << "{ " << player->getName() << " } > ";
        getline(cin, answer);

        if (answer == "1")
            return firstChoice;
        if (answer == "2")
            return secondChoice;

        cout << "[ ERROR! ] Invalid choice. Please enter either '1' or '2'." << endl;
    }
}


/**
 * Prompts a player to choose an opponent.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param players A pointer to the list of Players in the game.
 * @return A Player pointer to the chosen opponent.
 */
Player* HumanStrategy::chooseOpponent(Player* player, Players* players) {
    string opponentName;

    while (true) {
        cout << "{ " << player->getName() << " } Choose an opponent." << endl;

        for (Players::iterator it = players->begin(); it != players->end(); ++it)
            if (it->second != player)
                cout << "[ OPPONENT ] " << it->first << endl;

        cout << "{ " << player->getName() << " } > ";
        getline(cin, opponentName);

        if (opponentName == player->getName()) {
            cerr << "[ ERROR! ] You can't destroy your own army." << endl;
            continue;
        }

        if (players->find(opponentName) != players->end())
            return players->find(opponentName)->second;

        cerr << "[ ERROR! ] " << opponentName << " doesn't exist. Try again." << endl;
    }
}