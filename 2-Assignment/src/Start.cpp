/*

Provide a group of C++ classes that implements a user interaction mechanism to start the game
by allowing the player to:
1) Select a map from a list of files as stored in a directory.
2) Select the number of players in the game (2-5 players).
The code should load all the game pieces and use the map loader to load the selected and
appropriate map, create all the players, assign biding facility to the players, create a deck of
cards, and assign an empty hand of cards to each player.

class Start {
    int* numPlayers;
    Players* players;
    

public:
    Start();
    ~Start();

    static string selectMap();
    static int selectNumPlayers();
    static GameMap* initializeMap();

    int getNumPlayers() { return *numPlayers; }
    Players* getPlayers() { return players; }
};
*/
#include "Start.h"


string Start::selectMap() {
    //print map files
     

}

int Start::selectNumPlayers() {

}

GameMap* Start::initializeMap() {

}

Player* Start::createPlayer(){

}

void Start::initializeGame(){

}