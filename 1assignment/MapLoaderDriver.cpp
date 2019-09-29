#include "MapLoader.h"
#include "MapUtil.h"

void test_ReadValidMapFiles();
void test_CreateValidGraphFromMap();
void test_ReadInvalidMapFiles();
void test_CreateInvalidGraphFromMap();

int main() {
    GameMap* map = loadMap("../gotmap/got.map");
    cout << map->getVertices()->size() << endl;

    cout << "ConnectMap: " << isConnectedMap(map)<< endl;
    cout << "Valid Continents: " << validateContinents(map) << endl;
    cout << "Valid edges: " << validateEdges(map) << endl;
    // Vertices* vertices = map->getVertices();
    // Vertices::iterator it;
    // for(it = vertices->begin(); it != vertices->end(); ++it) {
    //     vector<Edge> edges = it->second->edges;
    //     cout << "VERTEX: " << it->second->name << endl;
    //     for (Edge& edge: edges) {
    //         cout << "EDGE: " << edge.first->name << endl;
    //     }
    //     cout << endl;
    // }
}