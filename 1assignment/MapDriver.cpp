#include "Map.h"
#include <cassert>
#include "MapUtil.h"

void test_ConnectedMap();
void test_ConnectedMap_fail();
void test_EachContinentBelongsToOneContinent();
void test_EachContinentBelongsToOneContinent_fail();
void test_ValidEdges();
void test_ValidEdges_fail();

GameMap* generateValidMap();
GameMap* generateSmallSimpleMap();
GameMap* generateValidMapContainingNodeWithTwoWaterEdges();
GameMap* generateDisconnectedMap();
GameMap* generateInvalidContinentMap();
GameMap* generateInvalidContinentMap2();
GameMap* generateInvalidContinentMap3();
GameMap* generateMapWithInternalWaterEdge();
GameMap* generateCompletelyDisconnectedMap();
GameMap* generateDuplicateEdgesMap();
GameMap* generateSelfLoopMap();

bool validateEdges(GameMap*);
bool isConnectedMap(GameMap*);
vector<set<string>* > getContinents(GameMap*);
bool isConnectedContinent(GameMap**, string);
bool validateContinents(GameMap*);

int main() {
    test_ConnectedMap();
    test_ConnectedMap_fail();
    test_EachContinentBelongsToOneContinent();
    test_EachContinentBelongsToOneContinent_fail();
    test_ValidEdges();
    test_ValidEdges_fail();

    return 0;
}

void test_ConnectedMap(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_ConnectedMap" << endl;
    cout << "---------------------------------------" << endl;

    bool result1 = isConnectedMap(generateValidMap());
    assert(result1 == true);
    cout << "Map 1 is connected." << "\n" << endl;

    bool result2 = isConnectedMap(generateSmallSimpleMap());
    assert(result2 == true);
    cout << "Map 2 is connected." << "\n" << endl;

    bool result3 = isConnectedMap(generateValidMapContainingNodeWithTwoWaterEdges());
    assert(result3 == true);
    cout << "Map 3 is connected." << "\n" << endl;
}

void test_ConnectedMap_fail(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_ConnectedMap_fail" << endl;
    cout << "---------------------------------------" << endl;

    bool result1 = isConnectedMap(generateDisconnectedMap());
    assert(result1 == false);
    cout << "Map 1 is not connected." << "\n" << endl;

    bool result2 = isConnectedMap(generateCompletelyDisconnectedMap());
    assert(result2 == false);
    cout << "Map 2 is not connected." << "\n" << endl;
}

void test_EachContinentBelongsToOneContinent(){
    cout << "\n---------------------------------------" << endl;
    cout << "test_EachcontinentBelongsToOneContinent" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    bool result1 = validateContinents(map);
    assert(result1 == 1);
    cout << "Map 1 has valid continents." << "\n" << endl;

}

void test_EachContinentBelongsToOneContinent_fail(){
    cout << "\n---------------------------------------" << endl;
    cout << "test_EachcontinentBelongsToOneContinent_fail" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map1 = generateMapWithInternalWaterEdge();
    bool result1 = validateContinents(map1);
    assert(result1 == 0);
    cout << "Map 1 has invalid continents." << "\n" << endl;

    GameMap* map2 = generateInvalidContinentMap();
    bool result2= validateContinents(map2);
    assert(result2 == 0);
    cout << "Map 2 has invalid continents." << "\n" << endl;

    GameMap* map3 = generateInvalidContinentMap2();
    bool result3 = validateContinents(map3);
    assert(result3 == 0);
    cout << "Map 3 has invalid continents." << "\n" << endl;

    GameMap* map4 = generateInvalidContinentMap3();
    bool result4 = validateContinents(map4);
    assert(result4 == 0);
    cout << "Map 4 has invalid continents." << "\n" << endl;
}

void test_ValidEdges(){
    cout << "\n---------------------------------------" << endl;
    cout << "test_ValidEdges" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map1 = generateValidMap();
    bool result1 = validateEdges(map1);
    assert(result1 == 1);
    cout << "Map 1 has valid edges." << "\n" << endl;
}

void test_ValidEdges_fail(){
    cout << "\n---------------------------------------" << endl;
    cout << "test_ValidEdges_fail" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map1 = generateDuplicateEdgesMap();
    bool result1 = validateEdges(map1);
    assert(result1 == 0);
    cout << "Map 1 has invalid edges." << "\n" << endl;

    GameMap* map2 = generateSelfLoopMap();
    bool result2= validateEdges(map2);
    assert(result2 == 0);
    cout << "Map 2 has invalid edges." << "\n" << endl;
}

GameMap* generateValidMap() {
    GameMap* map = new GameMap();

    cout << "\ngenerateValidMap"
    <<"\nW---Z   A---B---E===O---P---Q"
    <<"\n \\  |   |\\  |  /     \\  |  /"
    <<"\n  \\ |   | \\ | /       \\ | /"
    <<"\n   \\|   |  \\|/         \\|/"
    <<"\nY---X===C   D           R" << endl;

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
    GameMap* map = new GameMap();

    cout << "\ngenerateSmallSimpleMap"
    <<"\nW---Z" << endl; 

    map->addVertex(node, "W", "continent1");
    map->addVertex(node, "Z", "continent1");

    map->addEdge("Z", "W", false);

    return map;
}

GameMap* generateValidMapContainingNodeWithTwoWaterEdges() {
    GameMap* map = new GameMap();

    cout << "\ngenerateValidMapContainingNodeWithTwoWaterEdges"
    <<"\nA---B----F===O---P---R"
    <<"\n    |   /    ="
    <<"\n    |  /     ="
    <<"\n    | /      ="
    <<"\n    D        K---L" << endl;

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
    GameMap* map = new GameMap();

    cout <<"\n\ngenerateDisconnectedMap:"
        <<"\nW---Z   A---B----E   O---P---Q"
        <<"\n \\  |   |\\  |   /     \\  |  /"
        <<"\n  \\ |   | \\ |  /       \\ | /"
        <<"\n   \\|   |  \\|/          \\|/"
        <<"\nY---X   C   D            R" << endl;

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
    GameMap* map = new GameMap();

    cout << "\ngenerateInvalidContinentMap:"
    <<"\nZ===A---B----E===O---P---Q"
    <<"\n    |\\  |   /     \\  |  /"
    <<"\n    | \\ |  /       \\ | /"
    <<"\n    |  \\| /         \\|/"
    <<"\n    C   D            R" << endl;

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
    GameMap* map = new GameMap();

    cout << "\ngenerateInvalidContinentMap2"
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
    GameMap* map = new GameMap();

    cout << "\ngenerateInvalidContinentMap3"
    <<"\nA---B----F===O---P"
    <<"\n        /     \\  |"
    <<"\n       /       \\ |"
    <<"\n      /         \\|"
    <<"\n     D-----------R" << endl;


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
    GameMap* map = new GameMap();

    cout << "\ngenerateMapWithInternalWaterEdge"
    <<"\nA---B----F===O---P"
    <<"\n    =   /     \\  |"
    <<"\n    =  /       \\ |"
    <<"\n    = /         \\|"
    <<"\n    D            R" << endl;

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
    GameMap* map = new GameMap();

    cout << "\ngenerateCompletelyDisconnectedMap"
    <<"\nA B C" << endl;

    map->addVertex(node, "A", "continent1");
    map->addVertex(node, "B", "continent2");
    map->addVertex(node, "C", "continent2");

    return map;
}

GameMap* generateDuplicateEdgesMap() {
    GameMap* map = new GameMap();

    cout << "generateDuplicateEdgesMap"
    <<"\nA---B---C"
    <<"\n\\__/" << endl;

    map->addVertex(node, "A", "continent2");
    map->addVertex(node, "B", "continent2");
    map->addVertex(node, "C", "continent2");

    map->addEdge("A", "B", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}

GameMap* generateSelfLoopMap() {
    GameMap* map = new GameMap();

    cout << "\ngenerateSelfLoopMap"
    <<"\n A---B---C"
    <<"\n/_\\" << endl;

    map->addVertex(node, "A", "continent2");
    map->addVertex(node, "B", "continent2");
    map->addVertex(node, "C", "continent2");

    map->addEdge("A", "A", false);
    map->addEdge("A", "B", false);
    map->addEdge("B", "C", false);

    return map;
}