#ifndef BIDDER_H
#define BIDDER_H

#include <iostream>
#include <sstream>
#include "Player.h"

class Player;
typedef unordered_map<string, Player*> Players;

class Bidder {

    bool* madeBid;
    Player* player;

public:
    Bidder(Player* player);
    ~Bidder();
    int bid();
    static void startBid(Players* players);

private:
    static Player* calculateWinner(unordered_map<Player*, int>* bids, Players* players);
};

#endif