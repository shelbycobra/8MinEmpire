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

    bool generateMap();
    string getMapFilePath() { return *mapFilePath; }
    void setMapFilePath(const string& fileName);

private:
    bool loadRegions(ifstream* mapFile, unordered_map<string, string>* nameMap);
    bool loadEdges(ifstream* mapFile, unordered_map<string, string>* nameMap);
    bool loadImage(ifstream* mapFile);
};

#endif