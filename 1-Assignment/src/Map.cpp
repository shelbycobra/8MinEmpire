#include "Map.h"

Vertex::Vertex():
    name(new string("None")), 
    vertexKey(new string("None")),
    owners(new set<Player*>()),
    continent(new string("None")),
    armies(new unordered_map<string, int>()),
    cities(new unordered_map<string, int>()),
    edges(new vector<Edge>()) {}

Vertex::Vertex(string aName, string key, string continent):
    name(new string(aName)), 
    vertexKey(new string(key)), 
    owners(new set<Player*>()),
    continent(new string(continent)),
    armies(new unordered_map<string, int>()),
    cities(new unordered_map<string, int>()),
    edges(new vector<Edge>()) {}

Vertex::Vertex(Vertex* vertex){
    name = new string(vertex->getName());
    vertexKey = new string(vertex->getKey());
    continent = new string(vertex->getContinent());
    owners = new set<Player*>(*vertex->getOwners());
    armies = new unordered_map<string, int>(*vertex->getArmies());
    cities = new unordered_map<string, int>(*vertex->getCities());
    edges = new vector<Edge>(*vertex->getEdges());
}

Vertex& Vertex::operator=(Vertex& vertex) {
    name = new string(vertex.getName());
    vertexKey = new string(vertex.getKey());
    continent = new string(vertex.getContinent());
    owners = new set<Player*>(*vertex.getOwners());
    armies = new unordered_map<string, int>(*vertex.getArmies());
    cities = new unordered_map<string, int>(*vertex.getCities());
    edges = new vector<Edge>(*vertex.getEdges());
    return *this;
}

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

void Vertex::addEdge(Vertex* vertex, bool isWaterEdge) {
    edges->push_back(Edge(vertex, isWaterEdge));
}

void Vertex::print() {
    cout << "\t" << *vertexKey << " : " << *name << endl;
    unordered_map<string, int>::iterator it;

    int numArmies, numCities;
    string player;
    for(it = armies->begin(); it != armies->end(); ++it) {
        numCities = 0;
        numArmies = it->second;
        player = it->first;
        if (cities->find(player) != cities->end())
            numCities = cities->find(player)->second;

        printf("\t\t%-15sArmies: %-5dCities: %d\n", player.c_str(), numArmies, numCities);
    }
}

string Vertex::getName(){return *name;}
string Vertex::getKey(){return *vertexKey;}
string Vertex::getContinent(){return *continent;}
set<Player*>* Vertex::getOwners(){return owners;}
unordered_map<string, int>* Vertex::getArmies(){return armies;}
unordered_map<string, int>* Vertex::getCities(){return cities;}
vector<Edge>* Vertex::getEdges(){return edges;}

GameMap::GameMap(): 
    vertices(new Vertices()),
    start(new string("none")), 
    image(new string("")) {}

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

bool GameMap::setStartVertex(string& startVertexKey){
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

string GameMap::getStartVertex(){return *start;}
string GameMap::getImage(){return *image;}
Vertices* GameMap::getVertices(){return vertices;}

void GameMap::addVertex(const string& key, const string& name, const string& continent) {
    Vertices::iterator it = vertices->find(key);

    if (it == vertices->end()) {
        Vertex * vertex = new Vertex (name, key, continent);
        typedef pair<string, Vertex*> node;
        vertices->insert(node (key, vertex));
        return;
    }

    Vertex* v = vertices->find(key)->second;
    cout << "[ MAP ] Vertex " << v->getName() << " on continent " << v->getContinent() << " already exists on the map.\n" << endl;
}

void GameMap::addEdge(const string& startVertexKey, const string& endVertexKey, bool isWaterEdge) {
    Vertex * v1 = vertices->find(startVertexKey)->second;
    Vertex * v2 = vertices->find(endVertexKey)->second;
    v1->addEdge(v2, isWaterEdge);
    v2->addEdge(v1, isWaterEdge);
}

void GameMap::printMap(){
    cout << *image << endl;
    Vertices::iterator it;
    cout << "--------------------------------------------------------" << endl;
    for(it = vertices->begin(); it != vertices->end(); ++it) {
        if (it->second->getArmies()->size() > 0 || it->second->getCities()->size() > 0)
            it->second->print();
    }
    cout << "--------------------------------------------------------" << endl;
}

void GameMap::setImage(string& newImage) {
    *image = newImage;
}