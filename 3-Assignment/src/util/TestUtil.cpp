/**
 * TestUtil provides methods that are used for testing purposes, such as:
 *  - generating valid and invalid maps
 *  - connectivity
 *  - continent validity
 *  - edge validity
 * 
 */

#include "TestUtil.h"

/**
 * Iterates through a Player's regions and checks if they are indeed found on the map.
 * 
 * @param player A pointer to a Player object.
 * @param map A GameMap pointer to the map.
 * @return A boolean representing that the Player's regions exist on the map.
 */
bool playerOccupiedRegionsAreFoundOnMap(Player* player, GameMap* map) {
    Vertices::iterator it;

    for(it = player->getOccupiedRegions()->begin(); it != player->getOccupiedRegions()->end(); ++it) {
        if (map->getVertices()->find(it->second->getKey()) == map->getVertices()->end()) {
            cout << it->second->getName() << " does not exist on the map.\n" << endl;
            return false;
        } else {
            cout << it->second->getName() << " exists on the map!\n" << endl;
        }
    }
    return true;
}

/**
 * Creates a number of dummy players for testing purposes.
 *
 * @param numPlayers The number of players to create. Must be between 2 and 5 inclusive.
 * @return A list of Player pointers.
 */
Players createDummyPlayers(int numPlayers){
    cout << "\n[ TEST UTIL ] Creating " << numPlayers << " dummy playeres.\n" << endl;
    Players players;

    if (numPlayers < 2 || numPlayers > 5) {
        cout << "[ ERROR! ] Invalid number of players selected. Must be 2, 3, 4, or 5 players." << endl;
        return players;
    }

    //Follows the rules of the game for distributing coins per player.
    int coins = 18 - numPlayers * 2;
    if (numPlayers == 3 || numPlayers == 4)
        coins--;

    for(int i = 1; i <= numPlayers; i++) {
        string name = "Player " + to_string(i);
        Player* player = new Player(name, coins);
        players.insert(pair<string, Player*>(name, player));
    }

    return players;
}

GameMap* generateValidMap() {
    GameMap* map = GameMap::instance();

    cout << "\nGenerate Valid Map\n";

    string image = "\nW---Z   A---B---E===O---P---Q\n \\  |   |\\  |  /     \\  |  /\n  \\ |   | \\ | /       \\ | /\n   \\|   |  \\|/         \\|/\nY---X===C   D           R\n";
    map->setImage(image);

    map->printMap();

    string continent1 = "continent1", continent2 = "continent2", continent3 = "continent3";

    string nodes1[] = {"A", "B", "C", "D", "E"};
    string nodes2[] = {"O", "P", "Q", "R"};
    string nodes3[] = {"W", "X", "Y", "Z"};

    for (const string& node : nodes1)
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, node, continent3);

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
    GameMap* map = GameMap::instance();

    cout << "\nGenerate SmallSimpleMap\n"
    <<"\nW---Z\n" << endl;

    map->addVertex("W", "W", "continent1");
    map->addVertex("Z", "Z", "continent1");

    map->addEdge("Z", "W", false);

    return map;
}

GameMap* generateValidMapContainingNodeWithTwoWaterEdges() {
    GameMap* map = GameMap::instance();

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
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, node, continent3);

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
    GameMap* map = GameMap::instance();

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
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, node, continent3);

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
    GameMap* map = GameMap::instance();

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
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

    for (const string& node : nodes3)
        map->addVertex(node, node, continent3);

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
    GameMap* map = GameMap::instance();

    cout << "\nGenerate Invalid Continent Map2\n"
    <<"\nZ===A";


    string continent1 = "continent1", continent2 = "continent2";

    string nodes1[] = {"A"};
    string nodes2[] = {"Z"};

    for (const string& node : nodes1)
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

    map->addEdge("Z", "A", true);

    return map;
}

GameMap* generateInvalidContinentMap3() {
    GameMap* map = GameMap::instance();

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
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

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
    GameMap* map = GameMap::instance();

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
        map->addVertex(node, node, continent1);

    for (const string& node : nodes2)
        map->addVertex(node, node, continent2);

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
    GameMap* map = GameMap::instance();

    cout << "\nGenerate Completely Disconnected Map\n"
    <<"\nA B C\n" << endl;

    map->addVertex("A", "A", "continent1");
    map->addVertex("B", "B", "continent2");
    map->addVertex("C", "C", "continent2");

    return map;
}

GameMap* generateDuplicateEdgesMap() {
    GameMap* map = GameMap::instance();

    cout << "Generate Duplicate Edges Map\n"
    <<"\nA---B---C"
    <<"\n\\__/\n" << endl;

    map->addVertex("A", "A", "continent2");
    map->addVertex("B", "B", "continent2");
    map->addVertex("C", "C", "continent2");

    map->addEdge("A", "B", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}

GameMap* generateSelfLoopMap() {
    GameMap* map = GameMap::instance();

    cout << "\nGenerate Self Loop Map\n"
    <<"\n A---B---C"
    <<"\n/_\\\n" << endl;

    map->addVertex("A", "A", "continent2");
    map->addVertex("B", "B", "continent2");
    map->addVertex("C", "C", "continent2");

    map->addEdge("A", "A", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}