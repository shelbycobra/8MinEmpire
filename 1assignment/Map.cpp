#include "Map.h"

void GameMap::addVertex(const string& name, const string& continent) {
    Vertices::iterator it = vertices->find(name);

    if (it == vertices->end()) {
        Vertex * vertex = new Vertex (name, continent);
        typedef pair<string, Vertex*> node;
        vertices->insert(node (name, vertex));
        return;
    }

    Vertex* v = vertices->find(name)->second;
    cout << "Vertex " << v->name << " on continent " << v->continent << " already exists on the map.\n" << endl;
}

void GameMap::addEdge(const string& startVertex, const string& endVertex, bool isWaterEdge) {
    Vertex * v1 = vertices->find(startVertex)->second;
    Vertex * v2 = vertices->find(endVertex)->second;
    v1->addEdge(v2, isWaterEdge);
    v2->addEdge(v1, isWaterEdge);
}

bool GameMap::setStartVertex(const string& startVertex){
    //check if startVertex exists in vertices, if yes, set startVertex to its name
    if (vertices->find(startVertex) != vertices->end()) {
        cout << "Setting " << startVertex << " as the START." << endl;
        *start = startVertex;
        return true;
    }
    
    return false;
}

Vertices* GameMap::getVertices(){return vertices;}

GameMap::GameMap() {
    vertices = new Vertices();
}

GameMap::~GameMap() {
    delete vertices;
    vertices = NULL;
}