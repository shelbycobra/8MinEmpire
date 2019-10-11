#include "../MapLoader.h"
#include "../util/TestUtil.h"

void test_ReadValidMapFilesAndGenerateValidMaps();
void test_ReadInvalidMapFiles();
void test_CreateInvalidGraphFromMap();

int main() {
    test_ReadValidMapFilesAndGenerateValidMaps();
    test_ReadInvalidMapFiles();
    test_CreateInvalidGraphFromMap();

    return 0;
}

void test_ReadValidMapFilesAndGenerateValidMaps() {

    cout << "\n\n\n========================================================" << endl;
    cout << "TEST: test_ReadValidMapFilesAndGenerateValidMaps" << endl;
    cout << "========================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - large map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    MapLoader loader("../maps/largeValid.map");
    GameMap* map = loader.generateMap();

    assert(1 == isConnectedMap(map));
    cout << "Large map is connected!" << endl;

    assert(1 == validateContinents(map));
    cout << "Large map has valid continents!" << endl;

    assert(1 == validateEdges(map));
    cout << "Large map has valid edges!" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - med map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    delete map;
    loader.setMapFilePath("../maps/medValid.map");
    map = loader.generateMap();

    assert(1 == isConnectedMap(map));
    cout << "Medium map is connected!" << endl;

    assert(1 == validateContinents(map));
    cout << "Medium map has valid continents!" << endl;

    assert(1 == validateEdges(map));
    cout << "Medium map has valid edges!" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - small map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    delete map;
    loader.setMapFilePath("../maps/smallValid.map");
    map = loader.generateMap();

    assert(1 == isConnectedMap(map));
    cout << "Small map is connected!" << endl;

    assert(1 == validateContinents(map));
    cout << "Small map has valid continents!" << endl;

    assert(1 == validateEdges(map));
    cout << "Small map has valid edges!" << endl;

    delete map;
    map = nullptr;
}

void test_ReadInvalidMapFiles(){
    cout << "\n\n\n========================================================" << endl;
    cout << "TEST: test_ReadInvalidMapFiles" << endl;
    cout << "========================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a garbage file." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    MapLoader loader("../maps/garbage.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing an image." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    loader.setMapFilePath("../maps/missingImage.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing edges section." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    loader.setMapFilePath("../maps/missingEdges.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file with invalid edge syntax." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    loader.setMapFilePath("../maps/invalidEdgeSyntax.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing top level continent name." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    loader.setMapFilePath("../maps/missingTopContinent.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file with invalid country name syntax." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    loader.setMapFilePath("../maps/invalidCountrySyntax.map");
    loader.generateMap();
}

void test_CreateInvalidGraphFromMap(){
    cout << "\n\n\n========================================================" << endl;
    cout << "TEST: test_CreateInvalidGraphFromMap" << endl;
    cout << "========================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a disconnected map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    MapLoader loader("../maps/disconnected.map");
    GameMap* map = loader.generateMap();

    assert(0 == isConnectedMap(map));
    cout << "Map is disconnected!" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with an internal water edge." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    delete map;
    loader.setMapFilePath("../maps/internalWaterEdge.map");
    map = loader.generateMap();

    assert(1 == isConnectedMap(map));
    cout << "Map is connected!" << endl;

    assert(0 == validateContinents(map));
    cout << "Map has invalid continents!" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with invalid edges." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    delete map;
    loader.setMapFilePath("../maps/invalidEdges.map");
    map = loader.generateMap();

    assert(0 == validateEdges(map));
    cout << "Map has invalid edges!" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with invalid continents." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    delete map;
    loader.setMapFilePath("../maps/invalidContinent.map");
    map = loader.generateMap();

    assert(1 == isConnectedMap(map));
    cout << "Map is connected!" << endl;

    assert(0 == validateContinents(map));
    cout << "Map has invalid continents!" << endl;

    delete map;
    map = nullptr;
}