/**
 * MapUtil contains methods that may become a part of classes in future assignemnts.
 */

#include "MapUtil.h"

/**
 * Conducts a Breadth-First-Search through the GameMap object to detect if
 * it is a fully connected map.

 * @param map A GameMap pointer to the map.
 * @return A boolean representing if the map is connnected.
 */
bool isConnectedMap(GameMap* map){
    typedef pair<Vertex*, bool> Edge;

    set<string> visited;
    queue<string> nextToVisit;

    Vertices* vertices = map->getVertices();

    //Add first vertex to visited.
    visited.insert(vertices->begin()->second->getKey());

    //Add all edges from first node to queue
    vector<Edge>* edges = vertices->begin()->second->getEdges();
    for(Edge& edge: *edges)
        nextToVisit.push(edge.first->getKey());

    //Breadth-First-Search
    while(!nextToVisit.empty()) {
        string currentKey = nextToVisit.front();
        nextToVisit.pop();

        // If current node has not been visited yet
        if (visited.find(currentKey) == visited.end()) {
            visited.insert(currentKey);

            //Add all of current node's endpoint vertices to nextToVist
            vector<Edge>* edges = vertices->find(currentKey)->second->getEdges();
            for(Edge& edge: *edges)
                nextToVisit.push(edge.first->getKey());
        }
    }

    // Check if all nodes of graph have been added to visited set
    Vertices::iterator it;
    for (it = vertices->begin(); it != vertices->end(); ++it)
        if (visited.find(it->first) == visited.end()) {
            cout << "[ERROR!] Map is not connected.\n" << endl;
            return false;
        }

    return true;
}

/**
 * Analyzes each continent in the map to determine whether it is a valid continent.
 * 
 * A valid continents means that each continent contains more than one country and
 * contains no internal water edges.

 * @param map A GameMap pointer to the map.
 * @return A boolean representing if the map contains valid continents.
 */
bool validateContinents(GameMap* map) {
    vector<set<string>* > continents = getMapContinents(map);
    Vertices* vertices = map->getVertices();

    unordered_set<string> verticesList;
    for(Vertices::iterator it = vertices->begin(); it != vertices->end(); ++it)
        verticesList.insert(it->first);

    //Iterate through each continent and verify that each vertex belongs to only one continent
    vector<set<string>* >::iterator it;
    for (it = continents.begin(); it != continents.end(); ++it) {

        //Verify each continent is a connected subgraph
        if (!isContinentConnected(*it, map))
            return false;

        if((*it)->size() > 1) {
            for (const string& vertex : *(*it)) {
                if (verticesList.find(vertex) != verticesList.end()) {
                    Vertex* currentNode = vertices->find(vertex)->second;

                    //for each edge, if water edge and endpoint vertex are found in same continent, return false
                    vector<Edge> *edges = currentNode->getEdges();
                    for(Edge& edge: *edges) {
                        if (edge.second && ((*it)->find(edge.first->getKey()) != (*it)->end())) {
                            cout << "[ ERROR! ] " << currentNode->getContinent()
                                << " contains an internal water edge between "
                                << currentNode->getName() << " and " << edge.first->getName() << endl;
                            return false;
                        }
                    }

                    verticesList.erase(vertex);

                } else {
                    cout << "[ ERROR! ] " << vertices->find(vertex)->second->getName() << " was found on TWO continents!\n" << endl;
                    return false;
                }
            }
        } else {
            cout << "[ ERROR! ] A continent can't have less than two countries!\n" << endl;
            return false;
        }
    }

    return true;
}

/**
 * Conducts a Breadth-First-Search on a continent in a map to determine whether it is a
 * connected subgraph.
 * 
 * @param continent A pointer to a set of strings of vertex keys belonging to a continent.
 * @param map A GameMap pointer to the map object.
 * @return A boolean that respresents if the continent is a connected subgraph.
 */
bool isContinentConnected(set<string>* continent, GameMap* map){
    //Iterate through set of strings and do a BFS 
    typedef pair<Vertex*, bool> Edge;

    set<string> visited;
    queue<string> nextToVisit;

    Vertices* vertices = map->getVertices();

    //Add first vertex to visited.
    visited.insert(*continent->begin());

    //Add all non-water edges from first node to queue
    vector<Edge>* edges = vertices->find(*continent->begin())->second->getEdges();
    for(Edge& edge: *edges)
        if (!edge.second)
            nextToVisit.push(edge.first->getKey());
        
    //Breadth-First-Search
    while(!nextToVisit.empty()) {
        string currentKey = nextToVisit.front();
        nextToVisit.pop();

        if (continent->find(currentKey) == continent->end()) {
            cout << "[ ERROR! ] " << currentKey << " doesn't belong to " << vertices->find(*continent->begin())->second->getContinent() << "." << endl;
            return false;
        }

        // If current node has not been visited yet
        if (visited.find(currentKey) == visited.end()) {
            visited.insert(currentKey);

            //Add all of current node's non-water endpoint vertices to nextToVist
            vector<Edge>* edges = vertices->find(currentKey)->second->getEdges();
            for(Edge& edge: *edges)
                if (!edge.second)
                    nextToVisit.push(edge.first->getKey());
        }
    }

    // Check if all nodes of continent have been added to visited set
    set<string>::iterator it;
    for (it = continent->begin(); it != continent->end(); ++it)
        if (visited.find(*it) == visited.end()) {
            cout << "[ERROR!] Continent is not connected.\n" << endl;
            return false;
        }
    return true;
}

/**
 * Conducts a Breadth-First-Search through the GameMap object to find all the continents
 * in the map.
 * 
 * @param map A GameMap pointer to the map.
 * @return a list of country names grouped in sets representing the different continents on the map.
 */
vector<set<string>* > getMapContinents(GameMap* map){
    vector<set<string>* > continents;
    Vertices* vertices = map->getVertices();
    unordered_set<string> notVisited;
    queue<string> nextToVisit;

    //Create copy of all vertices in set
    Vertices::iterator it;
    for(it = vertices->begin(); it != vertices->end(); ++it)
        notVisited.insert(it->first);

    int continentIndex = -1;
    string continentName;

    while(true) {
        if(notVisited.empty())
            break;

        //nextToVisit will be empty when a new continent starts
        if (nextToVisit.empty()) {
            set<string>* newContinent = new set<string>;

            //Get front node from notVisited and remove it.
            string key = *notVisited.begin();
            Vertex* firstNode = vertices->find(key)->second;
            notVisited.erase(key);

            //Create New continent and add first node.
            continentName = firstNode->getContinent();
            newContinent->insert(key);
            continents.push_back(newContinent);
            continentIndex++;

            //add edgevertices to nextToVist
            vector<Edge>* edges = firstNode->getEdges();
            for(Edge& edge: *edges)
                //Only add edge vertex if not a water edge and currently in notVisited
                if (!edge.second && (notVisited.find(edge.first->getKey())!=notVisited.end()))
                    nextToVisit.push(edge.first->getKey());
        } else { //Still in a continent
            string currentNodeKey = nextToVisit.front();
            nextToVisit.pop();
            Vertex* currentNode = vertices->find(currentNodeKey)->second;

            //If exist in notVisited, remove it from notVisited, add it to continent, add non-water edges to nextToVisit
            if(notVisited.find(currentNodeKey) != notVisited.end()) {

                // allows for nodes to be put in multiple continents
                if (currentNode->getContinent() == continentName) {
                    notVisited.erase(currentNodeKey);
                    continents.at(continentIndex)->insert(currentNodeKey);

                    //add edgevertices to nextToVist
                    vector<Edge> *edges = currentNode->getEdges();

                    for(Edge& edge: *edges)
                        //Only add edge vertex if not a water edge and currently in notVisited
                        if (!edge.second && (notVisited.find(edge.first->getKey())!=notVisited.end()))
                            nextToVisit.push(edge.first->getKey());
                } else {
                    continents.at(continentIndex)->insert(currentNodeKey);
                    cout << "\n[WARNING] " << currentNodeKey << " had a different continent name: " << currentNode->getContinent();
                }
            }
        }
    }

    return continents;
}

/**
 * Iterates through all the vertices in a GameMap object to determine if the edges of
 * the map are valid.
 * 
 * An invalid edge is when the start vertex points to itself or contains two edges that 
 * point to the same vertex.

 * @param map A GameMap pointer to the map.
 * @return A boolean representing if the map contains valid edges.
 */
bool validateEdges(GameMap* map){
    Vertices* vertices = map->getVertices();

    //for each vertex, create new set, add edgevertex names
    Vertices::iterator it;
    for(it = vertices->begin(); it != vertices->end(); ++it) {

        set<string> edgeVertices;
        vector<Edge>* edges = it->second->getEdges();
        for (Edge& edge: *edges) {

            // If one of the end vertices has been added to the list already, there exists more than one edge to the same vertex
            if (edgeVertices.find(edge.first->getName()) != edgeVertices.end()) {
                cout << "[ ERROR! ] " << it->second->getName() << " points twice to " << edge.first->getName() << endl;
                return false;
            }

            // If the end vertex is the same as the current vertex, the edge is a self loop.
            if (edge.first == it->second) {
                cout << "[ ERROR! ] " << it->second->getName() << " points to itself. \n" << endl;
                return false;
            }

            edgeVertices.insert(edge.first->getName());
        }
    }

    return true;
}

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

        if (!player->MoveArmies(armies, startVertex, endVertex, overWater)) {
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

        if (!player->PlaceNewArmies(armies, endVertex, map->getStartVertex())) {
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
        if (!player->DestroyArmy(endVertex, opponent)) {
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
        if (!player->BuildCity(endVertex)) {
            continue;
        }
        break;
    }
    player->printCountries();
}
