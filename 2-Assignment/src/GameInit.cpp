#include <dirent.h>
#include <algorithm>

#include "GameInit.h"
#include "MapLoader.h"
#include "util/MapUtil.h"

/**
 * Default Constructor
 */
GameInitEngine::GameInitEngine(): 
    map(new GameMap()), players(new Players()),
    hand(new Hand()), numPlayers(new int()) {
        colours = new list<string>();
        colours->push_front("BLUE");
        colours->push_front("GREEN");
        colours->push_front("YELLOW");
        colours->push_front("RED");
        colours->push_front("WHITE");
    }

/**
 * Copy constructor
 */
GameInitEngine::GameInitEngine(GameInitEngine* otherInitEngine) {
    map = new GameMap(otherInitEngine->getMap());
    players = new Players(*otherInitEngine->getPlayers());
    hand = new Hand(otherInitEngine->getHand());
    numPlayers = new int(otherInitEngine->getNumPlayers());
    colours = new list<string>(*otherInitEngine->getColours());
}

/**
 * Assignment operator
 */
GameInitEngine& GameInitEngine::operator=(GameInitEngine& otherInitEngine) {
    map = new GameMap(otherInitEngine.getMap());
    players = new Players(*otherInitEngine.getPlayers());
    hand = new Hand(otherInitEngine.getHand());
    numPlayers = new int(otherInitEngine.getNumPlayers());
    colours = new list<string>(*otherInitEngine.getColours());
    return *this;
}

/**
 * Destructor
 */
GameInitEngine::~GameInitEngine() {
    for (pair<string, Player*> player: *players) {
        delete player.second;
    }
    delete map;
    delete players;
    delete hand;
    delete numPlayers;
    delete colours;

    map = nullptr;
    players = nullptr;
    hand = nullptr;
    numPlayers = nullptr;
    colours = nullptr;
}

/**
 *
 */
void GameInitEngine::initGame() {
    // Only initialize the game once.
    if (players->size() == 0 && map->getVertices()->size() == 0) {

        cout << "\n---------------------------------------------------------------------" << endl;
        cout << "---------------------------------------------------------------------" << endl;
        cout << "                            W E L C O M E  T O " << endl;
        cout << "                      8  M I N U T E  E M P I R E !" << endl;
        cout << "---------------------------------------------------------------------" << endl;
        cout << "---------------------------------------------------------------------\n" << endl;

        initializeMap();
        selectNumPlayers();
        createPlayers();
        hand->fill();
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
    vector<Player*> playerList;
    cout << "[ INIT ] Creating " << *numPlayers << " players." << endl;

    for (int i = 0; i < *numPlayers; i++) {
        Player* player = createPlayer(&playerList);
        playerList.push_back(player);
    }

    for (int i = playerList.size() - 1; i >= 0; i--) {
        players->insert(pair<string, Player*>(playerList.at(i)->getName(), playerList.at(i)));
    }
}

Player* GameInitEngine::createPlayer(vector<Player*>* playerList){
    string name;
    string colour;

    cout << "\n[ INIT ] Creating a new player" << endl;

    while(true) {
        cout << "[ INIT ] Enter name of the player:" << endl;
        cout << "[ INIT ] > ";
        getline(cin, name);

        if(name != "") {
            bool nameIsFree = true;

            vector<Player*>::iterator it;
            for(it = playerList->begin(); it != playerList->end(); ++it) {
                // Look through all players created previously and find matching name.

                if ((*it)->getName() == name) {
                    cout << "\n[ ERROR! ] That name is already taken. Please choose another.\n" << endl;
                    nameIsFree = false;
                    break;
                }
            }

            if (nameIsFree)
                break;
        } else {
            cout << "[ ERROR! ] Name cannot be empty. Please try again." << endl;
        }
    }

    while(true) {

        cout << "\n[ INIT ] Choose a colour for the player:" << endl;
        printColours();
        cout << "[ INIT ] > ";

        getline(cin, colour);
        transform(colour.begin(), colour.end(), colour.begin(), ::toupper);

        bool colourMatched = 0;

        for(list<string>::iterator it = colours->begin(); it != colours->end(); ++it) {
            if ((*it) == colour) {
                cout << "[ INIT ] You chose " << (*it) << "." << endl;
                colourMatched = 1;
                colours->remove(colour);
                break;
            }
        }

        if (colourMatched)
            break;

        cout << "[ ERROR! ] You must choose a colour from the list." << endl;
    }

    return new Player(name, colour);
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

        cout << "\n[ INIT ] Please select a map [ 1 - " << NUM_MAPS << " ]:\n" << endl;
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

 void GameInitEngine::printColours() {
     cout << "[ INIT ]  AVAILABLE COLOURS\n" << endl;
     for(list<string>::iterator it = colours->begin(); it != colours->end(); ++it) {
         cout << (*it) << endl;
     }
     cout << endl;
 }