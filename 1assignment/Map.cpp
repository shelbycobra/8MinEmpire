#include "Map.h"

GameMap::GameMap(): start(new string("none")), image(new string("")) {
    vertices = new Vertices();
}

GameMap::~GameMap() {
    delete vertices;
    vertices = NULL;
}

bool GameMap::setStartVertex(string& startVertexName){
    Vertices::iterator it;

    if (*start != "none"){
        cout << "[ MAP ] Start vertex was already set to < " << *start << " >. Start vertex can only be set once." << endl;
        return false;
    }

    cout << "[ MAP ] Setting start vertex. Searching for \"" << startVertexName << "\"." << endl;

    for(it = vertices->begin(); it != vertices->end(); ++it) {
        if (startVertexName == it->second->name) {
            *start = it->second->name;
            cout << "[ MAP ] Start vertex is now < " << it->second->name << " >." << endl;
            return true;
        }
    }
    cout << "[ MAP ] The vertex < " << startVertexName << " > doesn't exist on the map." << endl;
    return false;
}

string* GameMap::getStartVertex(){return start;}
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
    cout << "[ MAP ] Vertex " << v->name << " on continent " << v->continent << " already exists on the map.\n" << endl;
}

void GameMap::addEdge(const string& startVertex, const string& endVertex, bool isWaterEdge) {
    Vertex * v1 = vertices->find(startVertex)->second;
    Vertex * v2 = vertices->find(endVertex)->second;
    v1->addEdge(v2, isWaterEdge);
    v2->addEdge(v1, isWaterEdge);
}

void GameMap::printMap(){
    cout << *image << endl;
    Vertices::iterator it;
    for(it = vertices->begin(); it != vertices->end(); ++it) {
        if (it->second->armies.size() > 0 || it->second->cities.size() > 0)
            it->second->print();
    }
}

void GameMap::setImage(string& newImage) {
    *image = newImage;
}