#include "../MapLoader.h"
#include "../util/TestUtil.h"

void test_ReadValidMapFiles();
void test_CreateValidGraphFromMap();
void test_ReadInvalidMapFiles();
void test_CreateInvalidGraphFromMap();

int main() {
    MapLoader loader("maps/gotmap/got.map");
    GameMap* map = loader.generateMap();
    cout << map->getVertices()->size() << endl;

    cout << "ConnectMap: " << isConnectedMap(map)<< endl;
    cout << "Valid Continents: " << validateContinents(map) << endl;
    cout << "Valid edges: " << validateEdges(map) << endl;
    // Vertices* vertices = map->getVertices();
    // Vertices::iterator it;
    // for(it = vertices->begin(); it != vertices->end(); ++it) {
    //     vector<Edge> edges = it->second->getEdges();
    //     cout << "VERTEX: " << it->second->getName() << endl;
    //     for (Edge& edge: edges) {
    //         cout << "EDGE: " << edge.first->getName() << endl;
    //     }
    //     cout << endl;
    // }

    delete map;
    map = nullptr;
}