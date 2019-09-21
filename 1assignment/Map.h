#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

struct Edge;
struct Vertex;


typedef vector<Edge* > Edges;
typedef vector<Vertex* > Vertices;


struct Vertex {
    string name;
    string owner;
    int numArmies;
    Edges edges;

    void addEdge(Edge* edgePtr){

    }

};

struct Edge {
    bool isWaterEdge;
    Vertex v1;
    Vertex v2;
};


class Map {
    Vertices* vertices;
    Edges* edges;
    // Continents * continents;

public:
    Map();
    void addVertex(Vertex*);
    void addEdge(Edge*);
    Vertices* getVertices();
    Edges* getEdges();
};

#endif
