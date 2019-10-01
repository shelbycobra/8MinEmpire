#include "MapLoader.h"
#include "MapUtil.h"
#include <fstream>
#include <sstream>
#include <iterator>

GameMap* loadMap(const string& filePath) {
    ifstream mapFile(filePath);
    GameMap* map = new GameMap();
    unordered_map<string, string> nameMap;

    string line;
    getline(mapFile, line);

    //Add vertices
    string continent;
    while (line != "&") {
        // cout << line << endl;
        if (line.at(0) == '$') {
            continent=line.substr(1);
        }
        else {
            size_t pos = line.find(':');
            nameMap.insert(pair<string, string> (line.substr(0, pos), line.substr(pos+1)));
            map->addVertex(line.substr(pos+1), continent);
        }
        getline(mapFile, line);
    }

    getline(mapFile, line);

    //Add Edges
    while(line != "%") {
        // cout << line << endl;
        string text = line;
        istringstream iss(text);
        vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());

        string key = results.at(0);
        string startVertex = nameMap.find(key)->second;

        // cout << "Start vertex - " << key << ":" << startVertex << endl;
        // cout << "StartVertex " << startVertex << endl;
        auto i = results.size();
        for(i = 1; i < results.size(); i++) {
            // cout << results.at(i) << " ";
            bool isWaterEdge = false;
            if (results.at(i) == "w") {
                isWaterEdge = true;
                i++;
                // cout << results.at(i) << " ";
            }
            string endVertex = nameMap.find(results.at(i))->second;
            // cout << "Adding edge " << endVertex << ", waterEdge " << isWaterEdge << endl;
            map->addEdge(startVertex, endVertex, isWaterEdge);
        }
        // cout << endl;
        getline(mapFile, line);

    }

    string image = "";

    while(getline(mapFile, line)) {
        // cout << line << endl;
        image.append(line);
        image.append("\n");
    }

    // Load in map image at end of file.
    mapFile.close();

    map->setImage(image);
    map->printMap();

    return map;
}
