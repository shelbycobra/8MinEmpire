#include <dirent.h>
#include "GameInit.h"
#include "MapLoader.h"
#include "util/MapUtil.h"

/**
 * Default Constructor
 */
GameInitEngine::GameInitEngine(): 
    map(new GameMap()), players(new Players()),
    hand(new Hand()), numPlayers(new int()) {}

/**
 * Copy constructor
 */
GameInitEngine::GameInitEngine(GameInitEngine* otherInitEngine) {
    map = new GameMap(otherInitEngine->getMap());
    players = new Players(*otherInitEngine->getPlayers());
    hand = new Hand(otherInitEngine->getHand());
    numPlayers = new int(otherInitEngine->getNumPlayers());
}

/**
 * Assignment operator
 */
GameInitEngine& GameInitEngine::operator=(GameInitEngine& otherInitEngine) {
    map = new GameMap(otherInitEngine.getMap());
    players = new Players(*otherInitEngine.getPlayers());
    hand = new Hand(otherInitEngine.getHand());
    numPlayers = new int(otherInitEngine.getNumPlayers());
    return *this;
}

/**
 * Destructor
 */
GameInitEngine::~GameInitEngine() {
    delete map;
    delete players;
    delete hand;
    delete numPlayers;

    map = nullptr;
    players = nullptr;
    hand = nullptr;
    numPlayers = nullptr;
}

/**
 * 
 */
void GameInitEngine::initGame() {
    // Only initialize the game once.
    if (players->size() == 0 && map->getVertices()->size() == 0) {
        initializeMap();
        selectNumPlayers();
        createPlayers();
    }
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

        cout << "\n[ INIT ] Please enter the number of players from 2 to 5:" << endl;
        cout << "[ INIT ] > ";

        getline(cin, answer);
        
        try {
            int num = stoi(answer);
        
            if (num >= MIN_NUM_PLAYERS && num <= MAX_NUM_PLAYERS) {
                cout << "[ INIT ] You entered " << num << endl;
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
    cout << "[ INIT ] Creating " << *numPlayers << " players." << endl;
    
    for (int i = 0; i < *numPlayers; i++) {
        createPlayer();
    }
}

void GameInitEngine::createPlayer(){
    string name;

    cout << "\n[ INIT ] Creating a new player" << endl;
    cout << "[ INIT ] Enter name of the player:" << endl;
    cout << "[ INIT ] > ";
    getline(cin, name);

    players->insert(pair<string,Player*>(name, new Player(name)));
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

        cout << "\n[ INIT ] Please select a map [ 1 - " << NUM_MAPS << " ]:" << endl;
        for (int i = 0; i < NUM_MAPS; i++)
            cout << i+1 << " : " << maps->at(i) << endl;
        cout << "\n[ INIT ] > ";

        getline(cin, answer);
        
        try{
            int index = stoi(answer);
        
            if (index > 0 && index <= NUM_MAPS) {
                string chosenMap = maps->at(index-1);
                cout << "[ INIT ] You have chosen " << chosenMap << endl;
                return chosenMap;
            }
        
            cout << "\n[ ERROR! ] Invalid answer. Please choose among the available maps." << endl;
        
        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number." << endl;
        }
    }
}