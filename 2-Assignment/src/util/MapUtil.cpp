/**
 * MapUtil contains methods that may become a part of classes in future assignemnts.
 */

#include "MapUtil.h"

typedef pair<Vertex*, bool> Edge;

/**
 * Splits a string into a vector of strings by a delimiter.
 * 
 * @param str String to split.
 * @param delimiter The delimiter by which to split the string. 
 * @return A vector of strings.
 */
vector<string> split(string& str, char delimiter) {
    vector<string> arr;
    string next;

    for(string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == delimiter) {
            if (!next.empty()) {
                arr.push_back(next);
                next.clear();
            }
        } else {
            next += *it;
        }
    }

    if (!next.empty())
        arr.push_back(next);

    return arr;
}

/**
 * Executes the action of a card.
 * 
 * @param player A pointer to the Player object who is using the card.
 * @param action The action to be performed.
 * @param map A pointer to the GameMap object of the game.
 * @param players A pointer to the list of all players in the game.
 */
void performCardAction(Player* player, string action, GameMap* map, Players* players) {
    //if action contains OR -> ask player which to use
    int orPos = action.find("OR");
    if ( orPos != -1 )
        action = chooseORAction(action);

    vector<string> actionArr = split(action, ' ');

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it) == "Move")
            executeMoveArmies(player, action, map);
        if ((*it) ==  "Add")
            executeAddArmies(player, action, map);
        if ((*it) ==  "Destroy")
            executeDestroyArmy(player, action, map, players);
        if ((*it) == "Build")
            executeBuildCity(player, action, map);
    }
}

/**
 * Prompts a player to choose a start vertex among the player's occupied countries.
 * 
 * 
 * @param player A pointer to the Player being prompted.
 * @return A Vertex pointer to the start country.
 */
Vertex* chooseStartVertex(Player* player){

    string startName;

    while(true){
        cout << "[ GAME ] You must choose a country to take armies from." << endl;
        player->printCountries();
        cout << "[ GAME ] > ";
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
 * @param player A pointer to the Player being prompted.
 * @param type An ActionType enum representing the type of action being called.
 * @param map A GameMap pointer to the map.
 * @return A Vertex pointer to the end country.
 */
Vertex* chooseEndVertex(Player* player, ActionType type, GameMap* map){
    string endName;

    while(true){
        cout << "[ GAME ] Please choose a country:" << endl;

        if (type == ActionType::ADD_ARMY || type == ActionType::BUILD_CITY)
            player->printCountries();
        else if (type == ActionType::MOVE_OVER_LAND || type == ActionType::MOVE_OVER_WATER || type == ActionType::DESTROY_ARMY)
            map->printMap();
        else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return 0;
        }

        cout << "[ GAME ] > ";
        getline(cin, endName);
        transform(endName.begin(), endName.end(), endName.begin(), ::toupper);
        cout << "[ GAME ] You chose < " << endName << " >.\n" << endl;

        if (map->getVertices()->find(endName) == map->getVertices()->end()) {
            cerr << "[ ERROR! ] That country doesn't exist on the map." << endl;
            continue;
        }

        break;
    }

    return map->getVertices()->find(endName)->second;
}

/**
 * Prompts a player to choose how many armies to place on a country.
 * 
 * @param maxArmies The maximum number of armies the player can place. Determined by the card chosen.
 * @param remainderArmies The amount of armies left that the player can choose from.
 * @return The number of armies chosen to move.
 */
int chooseArmies(int maxArmies, int remainderArmies) {
    int armies;
    string armiesStr;

    while (true) {
        string armyStr = remainderArmies == 1 ? "army" : "armies";

        cout << "[ GAME ] How many armies? (MAX " << remainderArmies << " " << armyStr << ")" << endl;
        cout << "[ GAME ] > ";
        getline(cin, armiesStr);

        stringstream toInt(armiesStr);
        toInt >> armies;

        if (remainderArmies - armies >= 0 && remainderArmies - armies <= maxArmies) {
            cout << endl;
            break;
        }

        cout << "[ GAME ] You can only choose a maximum of " << remainderArmies << " " << armyStr << "." << endl;
    }

    return armies;
}

/**
 * Prompts a player to choose one of the action in an OR'd card.
 * 
 * @param action The action containing an OR'd action.
 * @return The action chosen by the player.
 */
string chooseORAction(string action) {
    string answer;
    string firstChoice;
    string secondChoice;

    int orPos = action.find("OR");

    while(true) {
        cout << "\n[ GAME ] Which action do you want? 1 or 2 ?" << endl;

        firstChoice = action.substr(0, orPos - 1);
        secondChoice = action.substr(orPos + 3);

        cout << "[ OPTION 1 ] " << firstChoice << endl;
        cout << "[ OPTION 2 ] " << secondChoice << endl;

        cout << "[ GAME ] > ";
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
 * @param players A pointer to the list of Players in the game.
 * @param currentPlayer A pointer to the Player being prompted.
 * @return A Player pointer to the chosen opponent.
 */
Player* chooseOpponent(Players* players, Player* currentPlayer) {
    string opponentName;

    while (true) {
        cout << "[ GAME ] Choose an opponent." << endl;
        cout << "[ GAME ] > ";
        getline(cin, opponentName);

        if (opponentName == currentPlayer->getName()) {
            cerr << "[ ERROR! ] You can't destroy your own army." << endl;
            continue;
        }

        if (players->find(opponentName) != players->end())
            return players->find(opponentName)->second;

        cerr << "[ ERROR! ] " << opponentName << " doesn't exist. Try again." << endl;
    }
}

/**
 * Executes the action "Move # armies" or "Move # armies over water".
 * 
 * The player can move armies around the map to adjacent countries as many times as the card dictates.
 * 
 * @param player A pointer to the Player being prompted.
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void executeMoveArmies(Player* player,string action, GameMap* map) {
    Vertex* startVertex;
    Vertex* endVertex;
    int maxArmies;
    int remainderArmies;

    stringstream toInt(action.substr(5, 6));
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    bool overWater = action.find("water") != size_t(-1);
    ActionType type = overWater ? ActionType::MOVE_OVER_WATER : ActionType::MOVE_OVER_LAND;
    string actionSuffix = overWater ? " armies over water.\n" : " armies over land.\n";


    cout << "\n\n[[ ACTION ]] Move " << maxArmies << actionSuffix << "\n\n" << endl;
    cout << "[ GAME ] You can move " << maxArmies << " armies around the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        startVertex = chooseStartVertex(player);
        endVertex = chooseEndVertex(player, type, map);
        armies = chooseArmies(maxArmies, remainderArmies);

        if (!player->moveArmies(armies, startVertex, endVertex, overWater)) {
            continue;
        }

        remainderArmies -= armies;
    }
    player->printCountries();
}

/**
 * Executes the action "Add # armies".
 * 
 * The player can add armies to any country the player currently has a city or to the start country.
 * 
 * @param player A pointer to the Player being prompted.
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void executeAddArmies(Player* player, string action, GameMap* map) {
    Vertex* endVertex;

    stringstream toInt(action.substr(4, 5));
    int maxArmies;
    int remainderArmies;
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    cout << "\n\n[[ ACTION ]] Add " << maxArmies << " armies.\n\n" << endl;
    cout << "[ GAME ] You have the choice of adding " << maxArmies << " armies on the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        endVertex = chooseEndVertex(player, ActionType::ADD_ARMY, map);
        armies = chooseArmies(maxArmies, remainderArmies);

        if (!player->placeNewArmies(armies, endVertex, map->getStartVertex())) {
            continue;
        }

        remainderArmies -= armies;
    }

    player->printCountries();
}

/**
 * Executes the action "Destroy army".
 * 
 * The player chooses an opponent's army to destroy on a country where the oponnent has an army.
 * 
 * @param player A pointer to the Player being prompted.
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 * @param players A list of players in the game.
 */
void executeDestroyArmy(Player* player, string action, GameMap* map, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertex* endVertex;
    string opponentName;
    Player* opponent;

    opponent = chooseOpponent(players, player);

    while(true) {
        endVertex = chooseEndVertex(player, ActionType::DESTROY_ARMY, map);
        if (!player->destroyArmy(endVertex, opponent)) {
            continue;
        }
        break;
    }

    opponent->printCountries();
}

/**
 * Executes the action "Build city".
 * 
 * The player can build a city on a country where they currently have armies.
 * 
 * @param player A pointer to the Player being prompted.
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void executeBuildCity(Player* player, string action, GameMap* map) {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertex* endVertex;

    while (true) {
        endVertex = chooseEndVertex(player, ActionType::BUILD_CITY, map);
        if (!player->buildCity(endVertex)) {
            continue;
        }
        break;
    }
    player->printCountries();
}
