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

struct Vertex {
    typedef pair<Vertex*, bool> Edge;

    string name;
    set<string> owner;
    string continent;
    unordered_map<string, int> armies;
    unordered_map<string, int> cities;
    vector<Edge> edges;

    Vertex(string aName, string continent) : name(aName), continent(continent) {}

    void addEdge(Vertex* vertex, bool isWaterEdge) {
        edges.push_back(Edge(vertex, isWaterEdge));
    }

    void print() {
        cout << "[ COUNTRY ]\tName: " << name << endl;
        unordered_map<string, int>::iterator it;

        int numArmies, numCities;
        string player;
        for(it = armies.begin(); it != armies.end(); ++it) {
            numCities = 0;
            numArmies = it->second;
            player = it->first;
            if (cities.find(player) != cities.end()) 
                numCities = cities.find(player)->second;

            printf("\t\tOwner: %-15sArmies: %-5dCities: %d\n", player.c_str(), numArmies, numCities);
        }
    }
};

typedef unordered_map<string, Vertex*> Vertices;

class GameMap {
    Vertices* vertices;
    string* start;
    string* image;

public:
    GameMap();
    ~GameMap();
    void addVertex(const string& name, const string& continent);
    void addEdge(const string& startVertex, const string& endVertex, const bool isWaterEdge);
    Vertices* getVertices();
    string* getStartVertex();
    bool setStartVertex(string& startVertexName);
    void printMap();
    void setImage(string& newImage);
};

#endif
