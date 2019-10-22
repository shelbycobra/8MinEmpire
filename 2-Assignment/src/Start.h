#ifndef START_H
#define START_H

#include <dirent.h>
#include <sys/types.h>

#include "Map.h"
/*

Provide a group of C++ classes that implements a user interaction mechanism to start the game
by allowing the player to:
1) Select a map from a list of files as stored in a directory.
2) Select the number of players in the game (2-5 players).
The code should load all the game pieces and use the map loader to load the selected and
appropriate map, create all the players, assign biding facility to the players, create a deck of
cards, and assign an empty hand of cards to each player.
Requirements part 1: You must deliver a driver that demonstrates:
 The different valid maps can be loaded and their validity is verified (i.e. it is a connected
graph, etc.), and invalid maps are rejected without the program crashing.
 The right number of players is created.
 An assigned deck with the right number of cards is created.
 An assigned biding facility to each player.
 An assigned empty hand of cards to each player. 

*/


class Start {

public:
    static string selectMap();
    static int selectNumPlayers();
    static GameMap* initializeMap();
    static Player* createPlayer();
    static void initializeGame();

//The code should load all the game pieces and use the map loader to load the selected and
// appropriate map, create all the players, assign biding facility to the players, create a deck of
// cards, and assign an empty hand of cards to each player.

};

#endif