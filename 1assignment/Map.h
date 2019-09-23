#ifndef MAP_H
#define MAP_H

#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

struct Vertex {
    typedef bool isWaterEdge;
    typedef pair<Vertex*, isWaterEdge> Edge;

    string name;
    string owner;
    string continent;
    int numArmies;
    vector<Edge> edges;

    Vertex(string aName, string continent) : name(aName), continent(continent) {
        numArmies = 0;
        owner = "None";
    }

    void addEdge(Vertex* vertex, bool isWaterEdge) {
        // cout << "Adding edge from " << name << " to " << vertex->name << endl;
        edges.push_back(Edge(vertex, isWaterEdge));
    }
};

typedef unordered_map<string, Vertex*> Vertices;

class GameMap {
    Vertices* vertices;

public:
    GameMap();
    ~GameMap();
    void addVertex(const string& name, const string& continent);
    void addEdge(const string& startVertex, const string& endVertex, const bool isWaterEdge);
    Vertices* getVertices();
};

#endif
