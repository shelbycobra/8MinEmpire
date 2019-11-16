#include "../Map.h"
#include "../GameMainLoop.h"

int main() {

    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_GameMapInstanceIsInitiallyEmpty" << endl;
    cout << "=====================================================================" << endl;

    GameMap *map = GameMap::instance();

    cout << "GameMap pointer address: " << map << endl;
    cout << "Map image is " << map->getImage() << endl;
    cout << "Occupied Regions on the map: " << endl;
    map->printOccupiedRegions();
    cout << "Number of vertices: " << map->getVertices()->size() << endl;
    cout << "Start vertex is " << map->getStartVertex() << endl;

    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_GameEngineInitializesMap" << endl;
    cout << "=====================================================================\n" << endl;

    MainGameEngine mainEngine;

    mainEngine.startGame();

    cout << "\nGameMap pointer address: " << map << endl;
    cout << "Map image is " << map->getImage() << endl;
    cout << "Occupied Regions on the map: " << endl;
    map->printOccupiedRegions();
    cout << "Number of vertices: " << map->getVertices()->size() << endl;
    cout << "Start vertex is " << map->getStartVertex() << endl;

    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_PlayerActionsAlterMapInstance" << endl;
    cout << "=====================================================================" << endl;

    mainEngine.getNextPlayer();

    mainEngine.getCurrentPlayer()->PlaceNewArmies(string("Add 2 armies"));

    cout << "\nGameMap pointer address: " << map << endl;
    cout << "Map image is " << map->getImage() << endl;
    cout << "Occupied Regions on the map: " << endl;
    map->printOccupiedRegions();
    cout << "Number of vertices: " << map->getVertices()->size() << endl;
    cout << "Start vertex is " << map->getStartVertex() << endl;

    return 0;
}