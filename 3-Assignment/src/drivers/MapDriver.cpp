#include <cassert>
#include "../util/TestUtil.h"

void test_ConnectedMap();
void test_EachRegionBelongsToOneContinent();
void test_ValidEdges();
void test_SettingAndGettingStartVertex();

bool validateEdges();
bool isConnectedMap();
vector<set<string>* > getMapContinents();
bool validateContinents();

int main() {
    test_ConnectedMap();
    test_EachRegionBelongsToOneContinent();
    test_ValidEdges();
    test_SettingAndGettingStartVertex();

    return 0;
}

void test_ConnectedMap(){
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_ConnectedMap" << endl;
    cout << "=====================================================================" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a connected map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map = generateValidMap();
    bool result1 = isConnectedMap();
    assert(result1 == 1);
    cout << "Map 1 is connected." << "\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a small connected map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete map;
    map = generateSmallSimpleMap();

    bool result2 = isConnectedMap();
    assert(result2 == 1);
    cout << "Map 2 is connected." << "\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a connected map with a node with two water edges." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete map;
    map = generateValidMapContainingNodeWithTwoWaterEdges();

    bool result3 = isConnectedMap();
    assert(result3 == 1);
    cout << "Map 3 is connected." << "\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a disconnected map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete map;
    map = generateDisconnectedMap();

    bool result4 = isConnectedMap();
    assert(result4 == 0);
    cout << "Map 4 is not connected." << "\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a completely disconnected map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete map;
    map = generateCompletelyDisconnectedMap();

    bool result5 = isConnectedMap();
    assert(result5 == 0);
    cout << "Map 5 is not connected." << "\n" << endl;

    delete map;
    map = nullptr;
}

void test_EachRegionBelongsToOneContinent(){
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_EachRegionBelongsToOneContinent" << endl;
    cout << "=====================================================================" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a valid map where each region belongs to one and only one continent." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map = generateValidMap();
    bool result = validateContinents();
    assert(result == 1);
    cout << "Map 1 has valid continents." << "\n" << endl;

    delete map;
    map = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with an internal water edge." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map1 = generateMapWithInternalWaterEdge();
    bool result1 = validateContinents();
    assert(result1 == 0);
    cout << "Map 1 has invalid continents." << "\n" << endl;

    delete map1;
    map1 = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with an invalid continent version 1." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map2 = generateInvalidContinentMap();
    bool result2 = validateContinents();
    assert(result2 == 0);
    cout << "Map 2 has invalid continents." << "\n" << endl;

    delete map2;
    map2 = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with an invalid continent version 2." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map3 = generateInvalidContinentMap2();
    bool result3 = validateContinents();
    assert(result3 == 0);
    cout << "Map 3 has invalid continents." << "\n" << endl;

    delete map3;
    map3 = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with an invalid continent version 3." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map4 = generateInvalidContinentMap3();
    bool result4 = validateContinents();
    assert(result4 == 0);
    cout << "Map 4 has invalid continents." << "\n" << endl;

    delete map4;
    map4 = nullptr;
}

void test_ValidEdges(){
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_ValidEdges" << endl;
    cout << "=====================================================================" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with valid edges." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map1 = generateValidMap();
    bool result1 = validateEdges();
    assert(result1 == 1);
    cout << "Map 1 has valid edges." << "\n" << endl;

    delete map1;
    map1 = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with duplicate edges." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map2 = generateDuplicateEdgesMap();
    bool result2 = validateEdges();
    assert(result2 == 0);
    cout << "Map 2 has invalid edges." << "\n" << endl;

    delete map2;
    map2 = nullptr;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Generate a map with a self loop." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    GameMap* map3 = generateSelfLoopMap();
    bool result3= validateEdges();
    assert(result3 == 0);
    cout << "Map 3 has invalid edges." << "\n" << endl;

    delete map3;
    map3 = nullptr;
}

void test_SettingAndGettingStartVertex(){
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_SettingAndGettingStartVertex" << endl;
    cout << "=====================================================================" << endl;

    GameMap* map = generateSmallSimpleMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Getting start vertex before being set returns \"none\"." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    string start;
    start = map->getStartVertexName();
    assert(start == "none");
    cout << "Success! Start == " << start << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "\nTEST: Set start vertex to \"W\" successfully." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    bool result;
    string startName = "W";
    result = map->setStartVertex(startName);
    start = map->getStartVertexName();
    assert(start == startName && result == 1);
    cout << "Success! Start == " << start << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "\nTEST: Set start vertex to \"Z\" unsuccessfully." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    startName = "Z";
    result = map->setStartVertex(startName);
    assert(start != startName && result == 0);
    cout << "Success! Start == " << start << " and not the excepted \"" << startName << "\"" << endl;

    delete map;
    map = nullptr;
}