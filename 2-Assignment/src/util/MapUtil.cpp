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
void performCardAction(Player* player, const string action, GameMap* map, Players* players) {
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