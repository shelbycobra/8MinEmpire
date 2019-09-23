#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "Map.h"

bool isConnectedMap(GameMap*);
bool validateContinents(GameMap*);
vector<set<string>* > getContinents(GameMap*);
bool validateEdges(GameMap*);

typedef pair<Vertex*, bool> Edge;

bool isConnectedMap(GameMap* map){
    typedef pair<Vertex*, bool> Edge;

    set<Vertex*> visited;
    queue<Vertex*> nextToVisit;

    Vertices* vertices = map->getVertices();
    Vertex* firstNode = vertices->begin()->second;
    visited.insert(firstNode);

    //Add all edges from first node to queue
    vector<Edge> edges = firstNode->edges;
    for(auto const& edge: edges)
        nextToVisit.push(edge.first);

    //Breadth-First-Search
    while(!nextToVisit.empty()) {
        Vertex* node = nextToVisit.front();
        nextToVisit.pop();
        // If current node has not been visited yet
        if (visited.find(node) == visited.end()) {
            visited.insert(node);
            //Add all of current node's endpoint vertices to nextToVist
            vector<Edge> edges = node->edges;
            for(auto const& edge: edges)
                nextToVisit.push(edge.first);
        }
    }

    // Check if all nodes of graph have been added to visited set
    Vertices::iterator it;
    for (it = vertices->begin(); it != vertices->end(); ++it)
        if (visited.find(it->second) == visited.end()) {
            cout << "Node " << it->second->name << " was not connected." << endl;
            return false;
        }

    return true;
}

bool validateContinents(GameMap* map) {
    vector<set<string>* > continents = getContinents(map);
    cout << "\nNumber of continents: " << continents.size() << endl;
    Vertices* vertices = map->getVertices();
    Vertices::iterator it;

    for (it = vertices->begin(); it != vertices->end(); ++it) {
        Vertex* currentNode = it->second;

        vector<set<string>* >::iterator p;
        bool continentFound = false;

        for (p = continents.begin(); p != continents.end(); ++p) {
            set<string>* currentSet = *p;

            if (currentSet->size() < 2) {
                cout << "ERROR: Continent size is " << currentSet->size() << ". It must contain more than one vertex." << endl;
                return false;
            }

            if(currentSet->find(currentNode->name) != currentSet->end()) {
                if (continentFound) {
                    cout << "ERROR: " << currentNode-> name << " was found on TWO continents!" << endl;
                    return false;
                } else {
                    continentFound = true;

                    //for each edge, if water edge and if edge vertex found in continent, return false
                    vector<Edge> edges = currentNode->edges;
                    for(Edge& edge: edges) {
                        if (edge.second && (currentSet->find(edge.first->name) != currentSet->end())) {
                            cout << "ERROR: " << currentNode->continent
                                 << " contains an internal water edge between "
                                 << currentNode->name << " and " << edge.first->name << endl;
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

vector<set<string>* > getContinents(GameMap* map){
    vector<set<string>* > continents;
    Vertices* vertices = map->getVertices();
    unordered_set<string> notVisited;
    queue<string> nextToVisit;

    //Create copy of all vertices in set
    Vertices::iterator it;
    for(it = vertices->begin(); it != vertices->end(); ++it)
        notVisited.insert(it->second->name);

    int continentIndex = -1;
    string continentName;

    while(true) {
        if(notVisited.empty())
            break;

        //nextToVisit will be empty when a new continent starts
        if (nextToVisit.empty()) {
            cout << "\nNew Continent!" << endl;
            set<string>* newContinent = new set<string>;

            //Get front node from notVisited and remove it.
            string name = *notVisited.begin();
            Vertex* firstNode = vertices->find(name)->second;
            notVisited.erase(name);

            //Create New continent and add first node.
            continentName = firstNode->continent;
            newContinent->insert(name);
            continents.push_back(newContinent);
            continentIndex++;

            cout << firstNode->name << " ";

            //add edgevertices to nextToVist
            vector<Edge> edges = firstNode->edges;
            for(Edge& edge: edges)
                //Only add edge vertex if not a water edge and currently in notVisited
                if (!edge.second && (notVisited.find(edge.first->name)!=notVisited.end()))
                    nextToVisit.push(edge.first->name);
        } else { //Still in a continent
            string currentNodeName = nextToVisit.front();
            nextToVisit.pop();
            Vertex* currentNode = vertices->find(currentNodeName)->second;

            //If exist in notVisited, remove it from notVisited, add it to continent, add non-water edges to nextToVisit
            if(notVisited.find(currentNodeName) != notVisited.end()) {
                cout << currentNodeName << " ";

                // allows for nodes to be put in multiple continents
                if (currentNode->continent == continentName) {
                    notVisited.erase(currentNodeName);
                    continents.at(continentIndex)->insert(currentNodeName);

                    //add edgevertices to nextToVist
                    vector<Edge> edges = currentNode->edges;

                    for(Edge& edge: edges)
                        //Only add edge vertex if not a water edge and currently in notVisited
                        if (!edge.second && (notVisited.find(edge.first->name)!=notVisited.end()))
                            nextToVisit.push(edge.first->name);
                } else {
                    continents.at(continentIndex)->insert(currentNodeName);
                    cout << "\nWARNING: " << currentNodeName << " had a different continent name: " << currentNode->continent;
                }
            }
        }
    }

    return continents;
}

bool validateEdges(GameMap* map){
    Vertices* vertices = map->getVertices();

    //for each vertex, create new set, add edgevertex names
    Vertices::iterator it;
    for(it = vertices->begin(); it != vertices->end(); ++it) {
        // cout << "Vertex: " << it->second->name << endl;
        set<string> edgeVertices;
        vector<Edge> edges = it->second->edges;
        for (Edge& edge: edges) {
            // cout << "Current Edge = " << edge.first->name << endl;
            if (edgeVertices.find(edge.first->name)!= edgeVertices.end()) {
                cout << "ERROR: " << it->second->name << " points twice to " << edge.first->name << endl;
                return false;
            }
            if (edge.first->name == it->second->name) {
                cout << "ERROR: " << it->second->name << " points to itself. " << endl;
                return false;
            }
            // cout << "Adding " << edge.first->name << " to edgeVertices." << endl;
            edgeVertices.insert(edge.first->name);
        }
    }

    return true;
}

#endif