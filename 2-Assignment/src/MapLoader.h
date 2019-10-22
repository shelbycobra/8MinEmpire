#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "Map.h"

using namespace std;

class MapLoader {
    string* mapFilePath;

public:
    MapLoader();
    MapLoader(const string& fileName);
    MapLoader(MapLoader* mapLoader);
    MapLoader& operator=(MapLoader& mapLoader);
    ~MapLoader();
    GameMap* generateMap();

    string getMapFilePath() { return *mapFilePath; }
    void setMapFilePath(const string& fileName);

private:
    bool loadCountries(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap);
    bool loadEdges(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap);
    bool loadImage(GameMap* map, ifstream* mapFile);
};

#endif