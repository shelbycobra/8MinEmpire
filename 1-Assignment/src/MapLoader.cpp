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
    getline(mapFile, line);

    if (line == "" || line.at(0) != '$') {
        cout << line << endl;
        cout << "[ ERROR! ] First line must be a continent in the form of \"$CONTINENT\"." << endl;
        return nullptr;
    }

    cout << "[ MAP LOADER ] Getting names of countries." << endl;
    string continent;

    while (line != "&") {
        // cout << line << endl;
        if (line == "" || line.at(0) == '$') {
            continent=line.substr(1);
        } else {
            size_t pos = line.find(':');

            if (pos == -1) {
                cout << "[ ERROR! ] Invalid country syntax \"" << line << "\". Must be in the form of KEY:COUNTRY_NAME." << endl;
                return nullptr;
            }

            string key = line.substr(0, pos);
            string name = line.substr(pos+1);

            nameMap.insert(pair<string, string> (key, name));
            map->addVertex(key, name, continent);
        }

        getline(mapFile, line);

        if(mapFile.eof()) {
            cout << "[ ERROR! ] Reached end of file. Missing edges section" << endl;
            return nullptr;
        }
    }

    cout << "[ MAP LOADER ] Getting edges." << endl;
    getline(mapFile, line);

    while(line != "%") {

        //Split line into a vector of strings
        string text = line;
        istringstream iss(text);
        vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());

        string key = "";

        if (nameMap.find(results.at(0)) != nameMap.end())
            key = results.at(0);
        else {
            cout << "[ ERROR! ] \"" << results.at(0) << "\" is an invalid key." << endl;
            return nullptr;
        }

        string startVertex = nameMap.find(key)->second;

        auto i = results.size();
        for(i = 1; i < results.size(); i++) {

            bool isWaterEdge = false;
            if (results.at(i) == "w") {
                isWaterEdge = true;
                i++;
            }

            string endVertex = "";

            if (nameMap.find(results.at(i)) != nameMap.end())
                endVertex = nameMap.find(results.at(i))->first;
            else {
                cout << "[ ERROR! ] \"" << results.at(i) << "\" is an invalid key." << endl;
                return nullptr;
            }

            map->addEdge(key, endVertex, isWaterEdge);
        }

        getline(mapFile, line);

        if(mapFile.eof()) {
            cout << "[ ERROR! ] Reached end of file. Missing Image." << endl;
            return nullptr;
        }
    }

    string image = "";

    if (line == "" || line.at(0) != '%') {
        cout << "[ ERROR! ] Invalid input file." << endl;
        return nullptr;
    }

    cout << "[ MAP LOADER ] Getting map image." << endl;

    while(getline(mapFile, line)) {
        image.append(line);
        image.append("\n");
    }

    // Load in map image at end of file.
    mapFile.close();

    map->setImage(image);
    map->printMap();

    return map;
}

string MapLoader::getMapFilePath(){return *mapFilePath;}

void MapLoader::setMapFilePath(const string& filePath) {
    delete mapFilePath;
    mapFilePath = new string(pathPrefix + filePath);
}