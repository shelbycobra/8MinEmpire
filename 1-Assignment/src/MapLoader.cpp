#include "MapLoader.h"
#include "util/MapUtil.h"
#include <fstream>
#include <sstream>
#include <iterator>

#ifdef _WIN32
    string pathPrefix = "maps/";
#else
    string pathPrefix = "../maps/";
#endif

using namespace std;

MapLoader::MapLoader() : mapFilePath(new string(".")){}

MapLoader::MapLoader(const string& filePath): mapFilePath(new string(pathPrefix + filePath)){}

MapLoader::MapLoader(MapLoader* mapLoader){
    mapFilePath = new string(mapLoader->getMapFilePath());
}

MapLoader& MapLoader::operator=(MapLoader& mapLoader){
    mapFilePath = new string(mapLoader.getMapFilePath());
    return *this;
}

MapLoader::~MapLoader(){
    delete mapFilePath;
    mapFilePath = 0;
}

GameMap* MapLoader::generateMap(){
    ifstream mapFile(*mapFilePath);
    GameMap* map = new GameMap();
    unordered_map<string, string> nameMap;
    string line;

    if (!loadCountries(map, &mapFile, &nameMap))
        return nullptr;

    if (!loadEdges(map, &mapFile, &nameMap))
        return nullptr;

    if(!loadImage(map, &mapFile))
        return nullptr;


    return map;
}

string MapLoader::getMapFilePath(){return *mapFilePath;}

void MapLoader::setMapFilePath(const string& filePath) {
    delete mapFilePath;
    mapFilePath = new string(pathPrefix + filePath);
}

bool MapLoader::loadCountries(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap) {

    string continent;
    string line;

    getline(*mapFile, line);

    if (line == "" || line.at(0) != '$') {
        cout << line << endl;
        cout << "[ ERROR! ] Invalid file. First line must be a continent in the form of \"$CONTINENT\"." << endl;
        return false;
    }

    cout << "[ MAP LOADER ] Getting names of countries." << endl;

    while (line != "&") {

        if (line.at(0) != '$') {
            cout << line << endl;
            size_t pos = line.find(':');

            if (pos == size_t(-1)) {
                cout << "[ ERROR! ] Invalid country syntax \"" << line << "\". Must be in the form of KEY:COUNTRY_NAME." << endl;
                return false;
            }

            string key = line.substr(0, pos);
            string name = line.substr(pos+1);

            nameMap->insert(pair<string, string> (key, name));
            map->addVertex(key, name, continent);

        } else {
            //Continent found.
            continent=line.substr(1);
        }

        getline(*mapFile, line);

        if(mapFile->eof()) {
            cout << "[ ERROR! ] Reached end of file." << endl;
            return false;
        }
    }

    return true;
}

bool MapLoader::loadEdges(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap) {

    string line;

    cout << "[ MAP LOADER ] Getting edges." << endl;

    //Move file pointer past the break line "&" from the previous step.
    getline(*mapFile, line);

    while(line != "%") {
        cout << line << endl;
        //Split line into a vector of strings
        string text = line;
        istringstream iss(text);
        vector<string> edgeKeys((istream_iterator<string>(iss)), istream_iterator<string>());

        //Start vertex key is always at the first index.
        string startVertexKey = edgeKeys.at(0);

        if (nameMap->find(startVertexKey) == nameMap->end()) {
            cout << "[ ERROR! ] \"" << startVertexKey << "\" is an invalid key." << endl;
            return false;
        }

        //Iterate through edge keys and create edges that all start from the startVertex
        for(size_t index = 1; index < edgeKeys.size(); index++) {

            bool isWaterEdge = edgeKeys.at(index) == "w";
            if (isWaterEdge)
                index++; //Skip over 'w' to next edge key

            string endVertexKey = edgeKeys.at(index);

            if (nameMap->find(endVertexKey) == nameMap->end()) {
                cout << "[ ERROR! ] \"" << edgeKeys.at(index) << "\" is an invalid key." << endl;
                return false;
            }

            map->addEdge(startVertexKey, endVertexKey, isWaterEdge);
        }

        getline(*mapFile, line);

        if(mapFile->eof()) {
            cout << "[ ERROR! ] Reached end of file. Expecting image." << endl;
            return false;
        }
    }

    return true;
}

bool MapLoader::loadImage(GameMap* map, ifstream* mapFile) {

    string image = "";
    string line;
    if(mapFile->eof()) {
        cout << "[ ERROR! ] Reached end of file. Expecting image." << endl;
        return false;
    }

    cout << "[ MAP LOADER ] Getting map image." << endl;

    while(getline(*mapFile, line)) {
        image.append(line);
        image.append("\n");
    }

    // Load in map image at end of file.
    mapFile->close();

    map->setImage(image);
    map->printMap();

    return true;

}