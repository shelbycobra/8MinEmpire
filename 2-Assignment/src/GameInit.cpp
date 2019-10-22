#include <dirent.h>
#include "GameInit.h"
#include "MapLoader.h"
#include "util/MapUtil.h"


GameInitEngine::GameInitEngine(): map(new GameMap()), players(new Players()),
    hand(new Hand()), numPlayers(new int()) {}

GameInitEngine::~GameInitEngine() {
    delete map;
    delete players;
    delete hand;
    delete numPlayers;
}

void GameInitEngine::initGame() {
    initializeMap();
    selectNumPlayers();
    createPlayers();
}

void GameInitEngine::initializeMap() {
    vector<string>* mapFiles = getMapFiles();
    
    while(true) {
        string mapName = selectMap(mapFiles);
        MapLoader loader(mapName);

        map = loader.generateMap();

        // Map file is invalid and returned no map object.
        if (map == nullptr)
            continue;

        // Map file is valid and created a valid map object.
        if(isConnectedMap(map) && validateContinents(map) && validateEdges(map))
            break;
        
        // Map file is valid but created a faulty map object.
        delete map;
    }

    delete mapFiles;
    mapFiles = nullptr;
}

void GameInitEngine::selectNumPlayers() {
    const int MAX_NUM_PLAYERS = 5;
    const int MIN_NUM_PLAYERS = 2;

    while(true) {
        string answer;

        cout << "\n[ START ] Please enter the number of players from 2 to 5:" << endl;
        cout << "[ START ] > ";

        getline(cin, answer);
        
        try {
            int num = stoi(answer);
        
            if (num >= MIN_NUM_PLAYERS && num <= MAX_NUM_PLAYERS) {
                cout << "[ START ] You entered " << num << endl;
                *numPlayers = num;
                break;
            }
        
            cout << "\n[ ERROR! ] Invalid answer. Please enter a number from 2 to 5." << endl;
        
        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number." << endl;
        }
    }
}

void GameInitEngine::createPlayers(){
    int coins = 18 - *numPlayers * 2;
    if (*numPlayers == 3 || *numPlayers == 4)
        coins--;

    cout << "[ START ] Creating " << *numPlayers << " players. Each will have " << coins << " coins." << endl;
    
    for (int i = 0; i < *numPlayers; i++) {
        createPlayer(coins);
    }
}

void GameInitEngine::createPlayer(const int& coins){
    string name;

    cout << "\n[ START ] Creating a new player" << endl;
    cout << "[ START ] Enter name of the player:" << endl;
    cout << "[ START ] > ";
    getline(cin, name);

    players->insert(pair<string,Player*>(name, new Player(name, coins)));
}

vector<string>* GameInitEngine::getMapFiles() {
    vector<string>* files = new vector<string>();

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir ("maps")) != NULL) {

        while ((ent = readdir (dir)) != NULL) {

            //Exclude . and .. from map files
            if(string(ent->d_name).compare(".") == string(ent->d_name).compare(".."))
                files->push_back(string(ent->d_name));
        }
        
        closedir (dir);
    }

    return files;
}

string GameInitEngine::selectMap(vector<string>* maps) {
    const int NUM_MAPS = maps->size();

    while(true) {
        string answer;

        cout << "\n[ START ] Please select a map [ 1 - " << NUM_MAPS << " ]:" << endl;
        for (int i = 0; i < NUM_MAPS; i++)
            cout << i+1 << " : " << maps->at(i) << endl;
        cout << "\n[ START ] > ";

        getline(cin, answer);
        
        try{
            int index = stoi(answer);
        
            if (index > 0 && index <= NUM_MAPS) {
                string chosenMap = maps->at(index-1);
                cout << "[ START ] You have chosen " << chosenMap << endl;
                return chosenMap;
            }
        
            cout << "\n[ ERROR! ] Invalid answer. Please choose among the available maps." << endl;
        
        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number." << endl;
        }
    }
}