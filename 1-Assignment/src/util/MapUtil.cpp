#include "MapUtil.h"

typedef pair<Vertex*, bool> Edge;

vector<string>* split(string& str, char delimiter) {
    vector<string>* arr = new vector<string>();
    string next;

    for(string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == delimiter) {
            if (!next.empty()) {
                arr->push_back(next);
                next.clear();
            }
        } else {
            next += *it;
        }
    }

    if (!next.empty())
        arr->push_back(next);

    return arr;
}


void performCardAction(Player* player, string action, GameMap* map, Players* players) {
    //if action contains OR -> ask player which to use
    int orPos = action.find("OR");
    if ( orPos != -1 )
        action = chooseORAction(action);

    vector<string>* actionArr = split(action, ' ');

    for(vector<string>::iterator it = actionArr->begin(); it != actionArr->end(); ++it) {
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

Vertex* chooseStartVertex(Player* player){

    string startName;
    Vertex* startVertex;

    while(true){
        cout << "[ GAME ] You must choose a country to take armies from." << endl;
        player->printCountries();
        cout << "[ GAME ] > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (player->getCountries()->find(startName) == player->getCountries()->end()) {
            cout << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            startVertex = player->getCountries()->find(startName)->second;
            break;
        }
    }
    return startVertex;
}

Vertex* chooseEndVertex(Player* player, actionType type, GameMap* map){
    string endName;
    Vertex* endVertex;

    while(true){
        cout << "[ GAME ] Please choose a country:" << endl;

        if (type == actionType::ADD_ARMY || type == actionType::BUILD_CITY)
            player->printCountries();
        else if (type == actionType::MOVE_OVER_LAND || type == actionType::MOVE_OVER_WATER || type == actionType::DESTROY_ARMY)
            map->printMap();
        else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return NULL;
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

    endVertex = map->getVertices()->find(endName)->second;
    return endVertex;
}

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

        if (remainderArmies - armies >= 0) {
            cout << endl;
            break;
        }

        cout << "[ GAME ] You can only choose a maximum of " << remainderArmies << " " << armyStr << "." << endl;
    }

    return armies;
}

string chooseORAction(string action) {
    string answer;
    string firstChoice;
    string secondChoice;

    int orPos = action.find("OR");

    while(true) {
        cout << "\n[ GAME ] Which action do you want? 1 or 2 ?" << endl;

        firstChoice = action.substr(0, orPos - 1);
        secondChoice = action.substr(orPos + 2);

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

void executeMoveArmies(Player* player,string action, GameMap* map) {
    Vertex* startVertex;
    Vertex* endVertex;
    int maxArmies;
    int remainderArmies;

    stringstream toInt(action.substr(5, 6));
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    size_t negOne = -1;
    bool overWater = action.find("water") != negOne;
    actionType type = overWater ? actionType::MOVE_OVER_WATER : actionType::MOVE_OVER_LAND;
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

        endVertex = chooseEndVertex(player, actionType::ADD_ARMY, map);
        armies = chooseArmies(maxArmies, remainderArmies);

        if (!player->placeNewArmies(armies, endVertex, *map->getStartVertex())) {
            continue;
        }

        remainderArmies -= armies;
    }

    player->printCountries();
}

void executeDestroyArmy(Player* player, string action, GameMap* map, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertex* endVertex;
    string opponentName;
    Player* opponent;

    opponent = chooseOpponent(players, player);

    while(true) {
        endVertex = chooseEndVertex(player, actionType::DESTROY_ARMY, map);
        if (!player->destroyArmy(endVertex, opponent)) {
            continue;
        }
        break;
    }

    opponent->printCountries();
}

void executeBuildCity(Player* player, string action, GameMap* map) {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertex* endVertex;

    while (true) {
        endVertex = chooseEndVertex(player, actionType::BUILD_CITY, map);
        if (!player->buildCity(endVertex)) {
            continue;
        }
        break;
    }
    player->printCountries();
}
