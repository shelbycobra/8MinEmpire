#include "Map.h"
#include <algorithm>

GameMap *GameMap::mapInstance = 0;

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
 * Initializes a Vertex object that represents a region on a GameMap object.
 *
 * @param aName The name of the region.
 * @param key The key corresponding to the name of the region.
 * @param continent The name of the continent where the region sits.
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
    if (&vertex != this) {
        delete name;
        delete vertexKey;
        delete owners;
        delete continent;
        delete armies;
        delete cities;
        delete edges;

        name = new string(vertex.getName());
        vertexKey = new string(vertex.getKey());
        continent = new string(vertex.getContinent());
        owners = new set<Player*>(*vertex.getOwners());
        armies = new unordered_map<PlayerEntry*, int>(*vertex.getArmies());
        cities = new unordered_map<PlayerEntry*, int>(*vertex.getCities());
        edges = new vector<Edge>(*vertex.getEdges());
    }
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
    string owner = getRegionOwner();

    printf("%10s %s : %s\n", ("{ " + owner + " }").c_str(), vertexKey->c_str(), name->c_str());

    int numArmies, numCities;
    PlayerEntry* player;
    unordered_map<PlayerEntry*, int>::iterator it;

    for(it = armies->begin(); it != armies->end(); ++it) {
        numCities = 0;
        numArmies = it->second;
        player = it->first;
        if (cities->find(player) != cities->end())
            numCities = cities->find(player)->second;
        if (numArmies > 0 || numCities > 0)
            printf("\t\t%-10s %10s Armies: %-5d Cities: %d\n", player->first.c_str(), ("[ " + player->second + " ]").c_str(), numArmies, numCities);
    }

    for (it = cities->begin(); it != cities->end(); ++it) {
        player = it->first;

        // Vertex has 0 armies but at least one city belonging to Player
        if (armies->find(player) == armies->end()) {
            numArmies = 0;
            numCities = it->second;
            printf("\t\t%-10s %10s Armies: %-5d Cities: %d\n", player->first.c_str(), ("[ " + player->second + " ]").c_str(), numArmies, numCities);
        }
    }

    cout << endl;
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
    start = new string(map->getStartVertexName());
    image = new string(map->getImage());
}

/**
 * Deconstructor.
 */
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
    mapInstance = nullptr;
}

/**
 * Returns a singleton instance of a GameMap object.
 */
GameMap* GameMap::instance() {
    if (!mapInstance) {
        mapInstance = new GameMap();
    }
    return mapInstance;
}

/**
 * Sets the start vertex member variable of a GameMap object.
 * The variable is initialized to "none" and can only be set once.
 * Once the variable is set, it can't be changed again.
 *
 * @param startVertexKey The vertex key representing the start region
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
        startVertex = vertices->find(startVertexKey)->second;
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
 * @param key The vertex key that corresponds to the region.
 * @param name The name of the region.
 * @param continent The name of the continent the region is on.
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
 * @param startVertexKey The key corresponding to the start region.
 * @param endVertexKey The key corresponding to the end region.
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
 * regions.
 */
void GameMap::printMap(){
    cout << *image << endl;
}

void GameMap::printOccupiedRegions() {
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
 * @return a list of region names grouped in sets representing the different continents on the map.
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

/**
 * Gets the owner of the continent.
 *
 * @param continent A pointer to a set of strings representing a continent and its regions.
 * @return The name of the owner. Returns "" if there's no owner.
 */
string GameMap::getContinentOwner(set<string>* continent) {
    // Create an empty map that tracks the number of owned regions per player.
    unordered_map<string, int> ownedRegionsPerPlayer;

    // Iterate through the vertices on the continent and count the number of regions owned per player.
    for(set<string>::iterator it = continent->begin(); it != continent->end(); ++it) {
        Vertex* region = vertices->find(*it)->second;
        string owner = region->getRegionOwner(); // Returns "" if owned by no one.

        // Increment the number of owned regions per player.
        if(ownedRegionsPerPlayer.find(owner) != ownedRegionsPerPlayer.end()) {
            int numRegions = ownedRegionsPerPlayer.find(owner)->second;
            ownedRegionsPerPlayer.erase(owner);
            ownedRegionsPerPlayer.insert(pair<string,int>(owner, numRegions+1));
        } else {
            ownedRegionsPerPlayer.insert(pair<string,int>(owner, 1));
        }
    }

    return findOwnerOfContinent(&ownedRegionsPerPlayer);
}

//PRIVATE
/**
 * Looks through a map of the number of owned regions per player in a continent to find the owner of the continent.
 *
 * @param ownedRegionsPerPlayer A pointer to the an unordered_map of the number of regions owned per player.
 * @return The owner of the continent. Returns "" if no owner.
 */
string GameMap::findOwnerOfContinent(unordered_map<string, int> *ownedRegionsPerPlayer) {
    int highestRegionCount = 0;
    string owner = "";

    // Iterate through the ownedRegionsPerPlayer map and find the number of regions owned by the player to determine
    // the owner of the current continent.
    for(unordered_map<string, int>::iterator it = ownedRegionsPerPlayer->begin(); it != ownedRegionsPerPlayer->end(); ++it) {

        if (it->first != "") {
            if(it->second > highestRegionCount) {
                highestRegionCount = it->second;
                owner = it->first;
            }
            else if (it->second == highestRegionCount) {
                owner = "";
            }
        }
    }

    return owner;
}