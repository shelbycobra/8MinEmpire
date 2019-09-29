#include "MapUtil.h"

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
    for(Edge& edge: edges)
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
            for(Edge& edge: edges)
                nextToVisit.push(edge.first);
        }
    }

    // Check if all nodes of graph have been added to visited set
    Vertices::iterator it;
    for (it = vertices->begin(); it != vertices->end(); ++it)
        if (visited.find(it->second) == visited.end()) {
            cout << "Node " << it->second->name << " was not connected.\n" << endl;
            return false;
        }

    return true;
}

bool validateContinents(GameMap* map) {
    vector<set<string>* > continents = getMapContinents(map);
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
                cout << "ERROR: Continent size is " << currentSet->size() << ". It must contain more than one vertex.\n" << endl;
                return false;
            }

            if(currentSet->find(currentNode->name) != currentSet->end()) {
                if (continentFound) {
                    cout << "ERROR: " << currentNode-> name << " was found on TWO continents!\n" << endl;
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

vector<set<string>* > getMapContinents(GameMap* map){
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
            cout << "\nNew Continent!\n" << endl;
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

            // cout << firstNode->name << " ";

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
                cout << "ERROR: " << it->second->name << " points to itself. \n" << endl;
                return false;
            }
            // cout << "Adding " << edge.first->name << " to edgeVertices.\n" << endl;
            edgeVertices.insert(edge.first->name);
        }
    }

    return true;
}

bool playerOccupiedCountriesAreFoundOnMap(set<Vertex*>* countries, Vertices* vertices) {
    set<Vertex*>::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        if (vertices->find((*it)->name) == vertices->end()) {
            cout << (*it)->name << " does not exist on the map.\n" << endl;
            return false;
        } else {
            cout << (*it)->name << " exists on the map!\n" << endl;
        }
    }
    return true;
}

GameMap* generateValidMap() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Valid Map\n"
    <<"\nW---Z   A---B---E===O---P---Q"
    <<"\n \\  |   |\\  |  /     \\  |  /"
    <<"\n  \\ |   | \\ | /       \\ | /"
    <<"\n   \\|   |  \\|/         \\|/"
    <<"\nY---X===C   D           R\n" << endl;

    string continent1 = "continent1", continent2 = "continent2", continent3 = "continent3";

    string nodes1[] = {"A", "B", "C", "D", "E"};
    string nodes2[] = {"O", "P", "Q", "R"};
    string nodes3[] = {"W", "X", "Y", "Z"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, continent3);

    map->addEdge("A", "B", false);
    map->addEdge("A", "D", false);
    map->addEdge("A", "C", false);
    map->addEdge("B", "E", false);
    map->addEdge("B", "D", false);
    map->addEdge("E", "D", false);

    map->addEdge("E", "O", true);
    map->addEdge("O", "P", false);
    map->addEdge("O", "R", false);
    map->addEdge("P", "Q", false);
    map->addEdge("P", "R", false);
    map->addEdge("Q", "R", false);

    map->addEdge("C", "X", true);
    map->addEdge("X", "Y", false);
    map->addEdge("X", "Z", false);
    map->addEdge("X", "W", false);
    map->addEdge("Z", "W", false);

    return map;
}

GameMap* generateSmallSimpleMap() {
    GameMap* map = new GameMap();

    cout << "\nGenerate SmallSimpleMap\n"
    <<"\nW---Z\n" << endl;

    map->addVertex("W", "continent1");
    map->addVertex("Z", "continent1");

    map->addEdge("Z", "W", false);

    return map;
}

GameMap* generateValidMapContainingNodeWithTwoWaterEdges() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Valid Map Containing Node With Two Water Edges\n"
    <<"\nA---B----F===O---P---R"
    <<"\n    |   /    ="
    <<"\n    |  /     ="
    <<"\n    | /      ="
    <<"\n    D        K---L\n" << endl;

    string continent1 = "continent1", continent2 = "continent2", continent3 = "continent3";

    string nodes1[] = {"A", "B", "D", "F"};
    string nodes2[] = {"O", "P", "R"};
    string nodes3[] = {"K", "L"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, continent3);

    map->addEdge("A", "B", false);
    map->addEdge("B", "F", false);
    map->addEdge("B", "D", false);
    map->addEdge("F", "D", true);

    map->addEdge("F", "O", true);
    map->addEdge("O", "P", false);
    map->addEdge("P", "R", false);

    map->addEdge("O", "K", true);
    map->addEdge("K", "L", false);

    return map;
}

GameMap* generateDisconnectedMap() {
    GameMap* map = new GameMap();

    cout <<"\n\nGenerate Disconnected Map\n"
        <<"\nW---Z   A---B----E   O---P---Q"
        <<"\n \\  |   |\\  |   /     \\  |  /"
        <<"\n  \\ |   | \\ |  /       \\ | /"
        <<"\n   \\|   |  \\|/          \\|/"
        <<"\nY---X   C   D            R\n" << endl;

    string continent1 = "continent1", continent2 = "continent2", continent3 = "continent3";

    string nodes1[] = {"A", "B", "C", "D", "E", "F"};
    string nodes2[] = {"O", "P", "Q", "R", "S"};
    string nodes3[] = {"W", "X", "Y", "Z"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, continent3);

    map->addEdge("A", "B", false);
    map->addEdge("A", "D", false);
    map->addEdge("A", "C", false);
    map->addEdge("B", "E", false);
    map->addEdge("B", "D", false);
    map->addEdge("E", "D", false);

    map->addEdge("O", "P", false);
    map->addEdge("O", "R", false);
    map->addEdge("P", "Q", false);
    map->addEdge("P", "R", false);
    map->addEdge("Q", "R", false);

    map->addEdge("X", "Y", false);
    map->addEdge("X", "Z", false);
    map->addEdge("X", "W", false);
    map->addEdge("Z", "W", false);

    return map;
}

GameMap* generateInvalidContinentMap() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Invalid Continent Map\n"
    <<"\nZ===A---B----E===O---P---Q"
    <<"\n    |\\  |   /     \\  |  /"
    <<"\n    | \\ |  /       \\ | /"
    <<"\n    |  \\| /         \\|/"
    <<"\n    C   D            R\n" << endl;

    string continent1 = "continent1", continent2 = "continent2", continent3 = "continent3";

    string nodes1[] = {"A", "B", "C", "D", "E"};
    string nodes2[] = {"O", "P", "Q", "R"};
    string nodes3[] = {"Z"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, continent3);

    map->addEdge("A", "B", false);
    map->addEdge("A", "D", false);
    map->addEdge("A", "C", false);
    map->addEdge("B", "E", false);
    map->addEdge("B", "D", false);
    map->addEdge("E", "D", false);

    map->addEdge("E", "O", true);
    map->addEdge("O", "P", false);
    map->addEdge("O", "R", false);
    map->addEdge("P", "Q", false);
    map->addEdge("P", "R", false);
    map->addEdge("Q", "R", false);

    map->addEdge("Z", "A", true);

    return map;
}

GameMap* generateInvalidContinentMap2() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Invalid Continent Map2\n"
    <<"\nZ===A";


    string continent1 = "continent1", continent2 = "continent2";

    string nodes1[] = {"A"};
    string nodes2[] = {"Z"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    map->addEdge("Z", "A", true);

    return map;
}

GameMap* generateInvalidContinentMap3() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Invalid Continent Map3\n"
    <<"\nA---B----F===O---P"
    <<"\n        /     \\  |"
    <<"\n       /       \\ |"
    <<"\n      /         \\|"
    <<"\n     D-----------R\n" << endl;


    string continent1 = "continent1", continent2 = "continent2";

    string nodes1[] = {"A", "B", "D", "F"};
    string nodes2[] = {"O", "P", "R"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    map->addEdge("A", "B", false);
    map->addEdge("B", "F", false);
    map->addEdge("F", "D", false);

    map->addEdge("F", "O", true);
    map->addEdge("O", "P", false);
    map->addEdge("O", "R", false);
    map->addEdge("P", "R", false);
    map->addEdge("D", "R", false);

    return map;
}


GameMap* generateMapWithInternalWaterEdge() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Map With Internal Water Edge\n"
    <<"\nA---B----F===O---P"
    <<"\n    =   /     \\  |"
    <<"\n    =  /       \\ |"
    <<"\n    = /         \\|"
    <<"\n    D            R\n" << endl;

    string continent1 = "continent1", continent2 = "continent2";

    string nodes1[] = {"A", "B", "D", "F"};
    string nodes2[] = {"O", "P", "R"};

    for (const string& node : nodes1)
        map->addVertex(node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, continent2);

    map->addEdge("A", "B", false);
    map->addEdge("B", "F", false);
    map->addEdge("B", "D", true);
    map->addEdge("F", "D", false);

    map->addEdge("F", "O", true);
    map->addEdge("O", "P", false);
    map->addEdge("O", "R", false);
    map->addEdge("P", "R", false);

    return map;
}

GameMap* generateCompletelyDisconnectedMap() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Completely Disconnected Map\n"
    <<"\nA B C\n" << endl;

    map->addVertex("A", "continent1");
    map->addVertex("B", "continent2");
    map->addVertex("C", "continent2");

    return map;
}

GameMap* generateDuplicateEdgesMap() {
    GameMap* map = new GameMap();

    cout << "Generate Duplicate Edges Map\n"
    <<"\nA---B---C"
    <<"\n\\__/\n" << endl;

    map->addVertex("A", "continent2");
    map->addVertex("B", "continent2");
    map->addVertex("C", "continent2");

    map->addEdge("A", "B", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}

GameMap* generateSelfLoopMap() {
    GameMap* map = new GameMap();

    cout << "\nGenerate Self Loop Map\n"
    <<"\n A---B---C"
    <<"\n/_\\\n" << endl;

    map->addVertex("A", "continent2");
    map->addVertex("B", "continent2");
    map->addVertex("C", "continent2");

    map->addEdge("A", "A", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}