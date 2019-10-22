#ifndef MAP_H
#define MAP_H

#include "Player.h"

#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Player;
class Vertex;
typedef pair<Vertex*, bool> Edge;

class Vertex {
    string *name;
    string *vertexKey;
    set<Player*> *owners;
    string *continent;
    unordered_map<string, int> *armies;
    unordered_map<string, int> *cities;
    vector<Edge> *edges;

public:
    Vertex();
    Vertex(string aName, string key, string continent);
    Vertex(Vertex* vertex);
    Vertex& operator =(Vertex& vertex);
    ~Vertex();

    void addEdge(Vertex* vertex, bool isWaterEdge);
    void print();
    string getName();
    string getKey();
    string getContinent();
    set<Player*>* getOwners();
    unordered_map<string, int>* getArmies();
    unordered_map<string, int>* getCities();
    vector<Edge>* getEdges();
};

typedef unordered_map<string, Vertex*> Vertices;

class GameMap {
    Vertices* vertices;
    string* start;
    string* image;

public:
    GameMap();
    GameMap(GameMap* map);
    GameMap& operator=(GameMap& map);
    ~GameMap();

    void addVertex(const string& key, const string& name, const string& continent);
    void addEdge(const string& startVertex, const string& endVertex, const bool isWaterEdge);
    void printMap();

    Vertices* getVertices() { return vertices; }
    string getStartVertex() { return *start; }
    string getImage() { return *image; }

    void setImage(string& newImage);
    bool setStartVertex(string& startVertexName);
};

#endif
