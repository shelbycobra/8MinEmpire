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
    Bidder();
    Bidder(Player* player);
    Bidder(Bidder* bidder);
    Bidder& operator=(Bidder& bidder);
    ~Bidder();

    int bid();
    static Player* startBid(Players* players);

    bool getMadeBid() { return *madeBid; }
    Player* getPlayer() { return player; }

private:
    static Player* calculateWinner(unordered_map<Player*, int>* bids, Players* players);
};

#endif