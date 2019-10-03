#ifndef BIDDER_H
#define BIDDER_H

#include <iostream>
#include <sstream>
#include "Player.h"

class Player;
typedef unordered_map<string, Player*> Players;

void startBid(Players* players);
Player* calculateWinner(unordered_map<Player*, int>* bids, Players* players);

class Bidder {

    bool* madeBid;
    int* amount;
    Player* player;

public:
    Bidder(Player* player);
    ~Bidder();
    void bid();
};

#endif