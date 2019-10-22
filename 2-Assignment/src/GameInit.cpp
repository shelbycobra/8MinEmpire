#include <dirent.h>
#include "GameInit.h"
#include "MapLoader.h"

GameInitEngine::GameInitEngine(): map(new GameMap()), players(new Players()),
    hand(new Hand()), mapName(new string()), numPlayers(new int()) {}

GameInitEngine::~GameInitEngine() {
    delete map;
    delete players;
    delete hand;
    delete mapName;
    delete numPlayers;
}

void GameInitEngine::selectMap() {
    vector<string>* maps = getMapFiles();
    const int NUM_MAPS = maps->size();

    while(true) {
        string answer;
        cout << "\n[START] Please select a map [1 - " << NUM_MAPS << "]:" << endl;
        for (int i = 0; i < NUM_MAPS; i++)
            cout << i+1 << " : " << maps->at(i) << endl;
        cout << "\n[START] > ";
        getline(cin, answer);
        try{
            int index = stoi(answer);
            if (index > 0 && index <= NUM_MAPS) {
                string chosenMap = maps->at(index-1);
                cout << "[START] You have chosen " << chosenMap << endl;
                *mapName = chosenMap;
                break;
            }
            cout << "\n[ERROR!] Invalid answer. Please choose among the available maps." << endl;
        } catch (invalid_argument& e) {
            cout << "\n[ERROR!] Invalid input. Please enter a number." << endl;
        }
    }
}

void GameInitEngine::selectNumPlayers() {
    const int MAX_NUM_PLAYERS = 5;
    const int MIN_NUM_PLAYERS = 2;

    while(true) {
        string answer;
        cout << "\n[START] Please enter the number of players from 2 to 5:" << endl;
        cout << "[START] > ";
        getline(cin, answer);
        try {
            int num = stoi(answer);
            if (num >= MIN_NUM_PLAYERS && num <= MAX_NUM_PLAYERS) {
                cout << "[START] You entered " << num << endl;
                *numPlayers = num;
                break;
            }
            cout << "\n[ERROR!] Invalid answer. Please enter a number from 2 to 5." << endl;
        } catch (invalid_argument& e) {
            cout << "\n[ERROR!] Invalid input. Please enter a number." << endl;
        }
    }
}

void GameInitEngine::initializeMap() {
    MapLoader loader(*mapName);
    map = loader.generateMap();
}

void GameInitEngine::createPlayers(){
    int coins = 18 - *numPlayers * 2;
    if (*numPlayers == 3 || *numPlayers == 4)
        coins--;

    cout << "[START] Creating " << *numPlayers << " players. Each will have " << coins << " coins." << endl;
    for (int i = 0; i < *numPlayers; i++) {
        createPlayer(coins);
    }
}

void GameInitEngine::createPlayer(const int& coins){
    string name;

    cout << "\n[START] Creating a new player" << endl;
    cout << "[START] Enter name of the player:" << endl;
    cout << "[START] > ";
    getline(cin, name);

    players->insert(pair<string,Player*>(name, new Player(name, coins)));
}

void GameInitEngine::initGame() {
    selectMap();
    selectNumPlayers();
    initializeMap();
    createPlayers();
}

vector<string>* GameInitEngine::getMapFiles() {
    vector<string>* files = new vector<string>();

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir ("maps")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name != "." && ent->d_name != "..")
                files->push_back(string(ent->d_name));
        }
        closedir (dir);
    }
}