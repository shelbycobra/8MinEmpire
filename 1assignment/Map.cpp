#include "Map.h"
#include <iostream>


void Map::addVertex(Vertex* vertex) { vertices->push_back(vertex);}
void Map::addEdge(Edge* edge) {edges->push_back(edge);}
Vertices* Map::getVertices(){return vertices;}
Edges* Map::getEdges() {return edges;}

int main() {
    Edges edges;
    Vertex* node = new Vertex {"Node", "ME", 0, edges};
    Edges edges2;
    Vertex* node2 = new Vertex {"Node2", "opponent", 1, edges2};

    
    cout << node->name << endl;
    cout << node->owner << endl;
    cout << node->numArmies << endl;
    cout << node->edges.size() << endl;
    cout << node2->name << endl;
    cout << node2->owner << endl;
    cout << node2->numArmies << endl;
    cout << node2->edges.size() << endl;

    return 0;
}
