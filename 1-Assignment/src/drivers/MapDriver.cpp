#include <cassert>
#include "../util/TestUtil.h"

void test_ConnectedMap();
void test_EachCountryBelongsToOneContinent();
void test_ValidEdges();
void test_SettingAndGettingStartVertex();

bool validateEdges(GameMap*);
bool isConnectedMap(GameMap*);
vector<set<string>* > getMapContinents(GameMap*);
bool isConnectedContinent(GameMap**, string);
bool validateContinents(GameMap*);

int main() {
    test_ConnectedMap();
    test_EachCountryBelongsToOneContinent();
    test_ValidEdges();
    test_SettingAndGettingStartVertex();

    return 0;
}

void test_ConnectedMap(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_ConnectedMap" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: .\n" << endl;

    bool result1 = isConnectedMap(generateValidMap());
    assert(result1 == 1);
    cout << "Map 1 is connected." << "\n" << endl;

    bool result2 = isConnectedMap(generateSmallSimpleMap());
    assert(result2 == 1);
    cout << "Map 2 is connected." << "\n" << endl;

    bool result3 = isConnectedMap(generateValidMapContainingNodeWithTwoWaterEdges());
    assert(result3 == 1);
    cout << "Map 3 is connected." << "\n" << endl;

    bool result4 = isConnectedMap(generateDisconnectedMap());
    assert(result4 == 0);
    cout << "Map 4 is not connected." << "\n" << endl;

    bool result5 = isConnectedMap(generateCompletelyDisconnectedMap());
    assert(result5 == 0);
    cout << "Map 5 is not connected." << "\n" << endl;
}

void test_EachCountryBelongsToOneContinent(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_EachCountryBelongsToOneContinent" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    bool result = validateContinents(map);
    assert(result == 1);
    cout << "Map 1 has valid continents." << "\n" << endl;

    delete map;
    map = nullptr;

    GameMap* map1 = generateMapWithInternalWaterEdge();
    bool result1 = validateContinents(map1);
    assert(result1 == 0);
    cout << "Map 1 has invalid continents." << "\n" << endl;

    delete map1;
    map1 = nullptr;

    GameMap* map2 = generateInvalidContinentMap();
    bool result2= validateContinents(map2);
    assert(result2 == 0);
    cout << "Map 2 has invalid continents." << "\n" << endl;

    delete map2;
    map2 = nullptr;

    GameMap* map3 = generateInvalidContinentMap2();
    bool result3 = validateContinents(map3);
    assert(result3 == 0);
    cout << "Map 3 has invalid continents." << "\n" << endl;

    delete map3;
    map3 = nullptr;

    GameMap* map4 = generateInvalidContinentMap3();
    bool result4 = validateContinents(map4);
    assert(result4 == 0);
    cout << "Map 4 has invalid continents." << "\n" << endl;

    delete map4;
    map4 = nullptr;
}

void test_ValidEdges(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_ValidEdges" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map1 = generateValidMap();
    bool result1 = validateEdges(map1);
    assert(result1 == 1);
    cout << "Map 1 has valid edges." << "\n" << endl;

    delete map1;
    map1 = nullptr;

    GameMap* map2 = generateDuplicateEdgesMap();
    bool result2 = validateEdges(map2);
    assert(result2 == 0);
    cout << "Map 2 has invalid edges." << "\n" << endl;

    delete map2;
    map2 = nullptr;

    GameMap* map3 = generateSelfLoopMap();
    bool result3= validateEdges(map3);
    assert(result3 == 0);
    cout << "Map 3 has invalid edges." << "\n" << endl;

    delete map3;
    map3 = nullptr;
}

void test_SettingAndGettingStartVertex(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_SettingAndGettingStartVertex" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateSmallSimpleMap();

    cout << "TEST: Getting start vertex before being set returns \"none\".\n" << endl;
    string start;
    start = map->getStartVertex();
    assert(start == "none");
    cout << "Success! Start == " << start << endl;

    cout << "\nTEST: Set start vertex to \"W\" successfully.\n" << endl;

    bool result;
    string startName = "W";
    result = map->setStartVertex(startName);
    start = map->getStartVertex();
    assert(start == startName);
    cout << "Success! Start == " << start << endl;

    cout << "\nTEST: Set start vertex to \"Z\" unsuccessfully.\n" << endl;

    startName = "Z";
    result = map->setStartVertex(startName);
    assert(start != startName);
    cout << "Success! Start == " << start << " and not the excepted \"" << startName << "\"" << endl;

    delete map;
    map = nullptr;
}