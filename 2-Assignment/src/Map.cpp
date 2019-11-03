#include "Map.h"
#include <algorithm>

/**
 * Default Constructor
 */
Vertex::Vertex():
    name(new string("None")), 
    vertexKey(new string("None")),
    owners(new set<Player*>()),
    continent(new string("None")),
    armies(new unordered_map<PlayerEntry*, int>()),
    cities(new unordered_map<PlayerEntry*, int>()),
    edges(new vector<Edge>()) {}

/**
 * Initializes a Vertex object that represents a country on a GameMap object.
 * 
 * @param aName The name of the country.
 * @param key The key corresponding to the name of the country.
 * @param continent The name of the continent where the country sits.
 */
Vertex::Vertex(string aName, string key, string continent):
    name(new string(aName)), 
    vertexKey(new string(key)), 
    owners(new set<Player*>()),
    continent(new string(continent)),
    armies(new unordered_map<PlayerEntry*, int>()),
    cities(new unordered_map<PlayerEntry*, int>()),
    edges(new vector<Edge>()) {}

/**
 * Copy Constructor
 */
Vertex::Vertex(Vertex* vertex){
    name = new string(vertex->getName());
    vertexKey = new string(vertex->getKey());
    continent = new string(vertex->getContinent());
    owners = new set<Player*>(*vertex->getOwners());
    armies = new unordered_map<PlayerEntry*, int>(*vertex->getArmies());
    cities = new unordered_map<PlayerEntry*, int>(*vertex->getCities());
    edges = new vector<Edge>(*vertex->getEdges());
}

/**
 * Assignment operator
 */
Vertex& Vertex::operator=(Vertex& vertex) {
    name = new string(vertex.getName());
    vertexKey = new string(vertex.getKey());
    continent = new string(vertex.getContinent());
    owners = new set<Player*>(*vertex.getOwners());
    armies = new unordered_map<PlayerEntry*, int>(*vertex.getArmies());
    cities = new unordered_map<PlayerEntry*, int>(*vertex.getCities());
    edges = new vector<Edge>(*vertex.getEdges());
    return *this;
}

/**
 * Destructor
 */
Vertex::~Vertex(){
    delete name;
    delete vertexKey;
    delete owners;
    delete continent;
    delete armies;
    delete cities;
    delete edges;

    name = nullptr;
    vertexKey = nullptr;
    owners = nullptr;
    continent = nullptr;
    armies = nullptr;
    cities = nullptr;
    edges = nullptr;
}

/**
 * Adds an edge to endVertex.
 *
 * @param endVertex A Vertex pointer to the end point vertex.
 * @param isWaterEdge A boolean representing if the edge is a water edge.
 */
void Vertex::addEdge(Vertex* endVertex, bool isWaterEdge) {
    edges->push_back(Edge(endVertex, isWaterEdge));
}

/**
 * Prints a list to the console of the armies and cities currently on the vertex.
 */ 
void Vertex::print() {
    cout << "      " << *vertexKey << " : " << *name << endl;
    unordered_map<PlayerEntry*, int>::iterator it;

    int numArmies, numCities;
    PlayerEntry* player;
    for(it = armies->begin(); it != armies->end(); ++it) {
        numCities = 0;
        numArmies = it->second;
        player = it->first;
        if (cities->find(player) != cities->end())
            numCities = cities->find(player)->second;

        printf("\t\t%-10s %-10s Armies: %-5d Cities: %d\n", player->first.c_str(), ("[ " + player->second + " ]").c_str(), numArmies, numCities);
    }
}

/**
 * Gets the owner of the region, if any.
 * 
 * @return The owner of the region. Returns an empty string if no one owns it.
 */
string Vertex::getRegionOwner() {
    unordered_map<PlayerEntry*, int>::iterator it;
    int highestCount = 0;
    string owner = "";
    
    for(it = armies->begin(); it != armies->end(); ++it) {
        int numArmies = it->second;
        int numCities = 0;

        if (cities->find(it->first) != cities->end())
            numCities = cities->find(it->first)->second;

        int combinedCount = numArmies + numCities;

        if (combinedCount > highestCount) {
            highestCount = combinedCount;
            owner = it->first->first;
        } else if (combinedCount == highestCount) { 
            owner = "";
        }
    }
    return owner;
}

/**
 * Default Constructor
 */
GameMap::GameMap(): 
    vertices(new Vertices()),
    start(new string("none")), 
    image(new string("")) {}

/**
 * Copy Constructor
 */
GameMap::GameMap(GameMap* map) {
    vertices = new Vertices(*map->getVertices());
    start = new string(map->getStartVertex());
    image = new string(map->getImage());
}

GameMap& GameMap::operator=(GameMap& map) {
    vertices = new Vertices(*map.getVertices());
    start = new string(map.getStartVertex());
    image = new string(map.getImage());

    return *this;
}

GameMap::~GameMap() {
    // Delete all vertices on the map.
    for(Vertices::iterator it = vertices->begin(); it != vertices->end(); ++it)
        delete it->second;

    delete vertices;
    delete start;
    delete image;

    vertices = nullptr;
    start = nullptr;
    image = nullptr;
}

/**
 * Sets the start vertex member variable of a GameMap object.
 * The variable is initialized to "none" and can only be set once.
 * Once the variable is set, it can't be changed again.
 *
 * @param startVertexKey The vertex key representing the start country
 * @return a boolean that shows the action was successful.
 */
bool GameMap::setStartVertex(string& startVertexKey){
    transform(startVertexKey.begin(), startVertexKey.end(), startVertexKey.begin(), ::toupper);
    Vertices::iterator it;

    if (*start != "none"){
        cout << "[ ERROR! ] Start vertex was already set to < " << *start << " >. Start vertex can only be set once.\n" << endl;
        return false;
    }

    cout << "[ MAP ] Setting start vertex. Searching for \"" << startVertexKey << "\"." << endl;
    if (vertices->find(startVertexKey) != vertices->end()) {
        *start = startVertexKey;
        cout << "[ MAP ] Start vertex is now < " << vertices->find(startVertexKey)->second->getName() << " >.\n" << endl;
        return true;
    }

    cout << "[ ERROR! ] The vertex < " << startVertexKey << " > doesn't exist on the map.\n" << endl;
    return false;
}

/**
 * Sets the image string of the GameMap object. This string produces an 
 * ASCII based image of the map that is printed throughout the game.
 * 
 * The image comes from the text file used to create the map (see MapLoader.cpp).
 *
 * @param newImage The string representing an image of the map.
 */
void GameMap::setImage(const string& newImage) {
    *image = newImage;
}

/**
 * Adds a vertex object to the list of vertices belonging to the GameMap.
 *
 * @param key The vertex key that corresponds to the country.
 * @param name The name of the country.
 * @param continent The name of the continent the country is on.
 */
void GameMap::addVertex(const string& key, const string& name, const string& continent) {

    if (vertices->find(key) == vertices->end()) {
        Vertex * vertex = new Vertex (name, key, continent);
        typedef pair<string, Vertex*> node;
        vertices->insert(node (key, vertex));
        return;
    }

    Vertex* v = vertices->find(key)->second;
    cout << "[ MAP ] Vertex " << v->getName() << " on continent " << v->getContinent() << " already exists on the map.\n" << endl;
}

/**
 * Creates an edge on the map between two vertices.
 *
 * @param startVertexKey The key corresponding to the start country.
 * @param endVertexKey The key corresponding to the end country.
 * @param isWaterEdge A bool representing wher the edge is over water.
 */
void GameMap::addEdge(const string& startVertexKey, const string& endVertexKey, bool isWaterEdge) {
    Vertex * v1 = vertices->find(startVertexKey)->second;
    Vertex * v2 = vertices->find(endVertexKey)->second;
    v1->addEdge(v2, isWaterEdge);
    v2->addEdge(v1, isWaterEdge);
}

/**
 * Prints the image of the GameMap along with a list of all the currently occupied
 * countries. 
 */
void GameMap::printMap(){
    cout << *image << endl;
    Vertices::iterator it;
    cout << "---------------------------------------------------------------------" << endl;
    for(it = vertices->begin(); it != vertices->end(); ++it) {
        if (it->second->getArmies()->size() > 0 || it->second->getCities()->size() > 0)
            it->second->print();
    }
    cout << "---------------------------------------------------------------------" << endl;
}

/**
 * Conducts a Breadth-First-Search through the GameMap object to find all the continents
 * in the map.
 * 
 * @return a list of country names grouped in sets representing the different continents on the map.
 */
vector<set<string>* > GameMap::getContinents(){
    vector<set<string>* > continents;
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

                // allows for nodes to erroneously be put in multiple continents indicating a faulty map file.
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
