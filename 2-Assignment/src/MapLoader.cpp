#include "MapLoader.h"
#include "util/MapUtil.h"
#include <fstream>
#include <sstream>
#include <iterator>

#define PATH_PREFIX "maps/"

using namespace std;

/**
 * Default Constructor
 */
MapLoader::MapLoader() : mapFilePath(new string(".")){}

/**
 * Initializes a MapLoader object.
 * 
 * @param fileName The name of the file. This can be a path start from any directory inside the maps/ directory.
 */
MapLoader::MapLoader(const string& fileName): mapFilePath(new string(PATH_PREFIX + fileName)){}

/**
 * Copy Constructor
 */
MapLoader::MapLoader(MapLoader* mapLoader){
    mapFilePath = new string(mapLoader->getMapFilePath());
}

/**
 * Assignment operator
 */
MapLoader& MapLoader::operator=(MapLoader& mapLoader){
    mapFilePath = new string(mapLoader.getMapFilePath());
    return *this;
}

/**
 * Destructor
 */
MapLoader::~MapLoader(){
    delete mapFilePath;
    mapFilePath = nullptr;
}

//SETTER
void MapLoader::setMapFilePath(const string& fileName) {
    delete mapFilePath;
    mapFilePath = new string(PATH_PREFIX + fileName);
}

/**
 * Generates a GameMap object from a text file.
 *
 * The text file has the following syntax:
 *
 * $CONTINENT
 * KEY:COUNTRY
 * &
 * STARTKEY ENDKEY ENDKEY w WATERENDKEY
 * %
 * Image Text
 *
 * @return A GameMap pointer to a map.
 */
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

/**
 * Adds countries to a map object.
 *
 * @param map A GameMap pointer to a map object.
 * @param mapFile A ifstream pointer the map text file
 * @param nameMap An unordeder_map pointer of an object mapping vertex keys to name
 * @return A GameMap pointer to a map.
 */
bool MapLoader::loadCountries(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap) {

    string continent;
    string line;

    getline(*mapFile, line);

    if (line == "" || line.at(0) != '$') {
        cout << "[ ERROR! ] Invalid file. First line must be a continent in the form of \"$CONTINENT\"." << endl;
        return false;
    }

    cout << "[ MAP LOADER ] Getting names of countries." << endl;

    while (line != "&") {

        if (line.at(0) != '$') {
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

/**
 * Adds edges to a map object.
 *
 * @param map A GameMap pointer to a map object.
 * @param mapFile A ifstream pointer the map text file
 * @param nameMap An unordeder_map pointer of an object mapping vertex keys to name
 * @return A GameMap pointer to a map.
 */
bool MapLoader::loadEdges(GameMap* map, ifstream* mapFile, unordered_map<string, string>* nameMap) {

    string line;

    cout << "[ MAP LOADER ] Getting edges." << endl;

    //Move file pointer past the break line "&" from the previous step.
    getline(*mapFile, line);

    while(line != "%") {
        //Split line into a vector of strings
        string text = line;
        istringstream iss(text);
        vector<string> edgeKeys((istream_iterator<string>(iss)), istream_iterator<string>());

        //Start vertex key is always the first string.
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

/**
 * Adds Image to a map object.
 *
 * @param map A GameMap pointer to a map object.
 * @param mapFile A ifstream pointer the map text file
 * @return A GameMap pointer to a map.
 */
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