#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "Map.h"

using namespace std;

class MapLoader {
    string* mapFilePath;

public:
    MapLoader();
    MapLoader(const string& filePath);
    MapLoader(MapLoader* mapLoader);
    MapLoader& operator=(MapLoader& mapLoader);
    ~MapLoader();
    GameMap* generateMap();

    string getMapFilePath();
    void setMapFilePath(const string& filePath);
};

#endif