#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game {

    int* numPlayers;
    Players* players;

public:
    int getNumPlayers() { return *numPlayers; }
    Players* getPlayers() { return players; }
};

#endif