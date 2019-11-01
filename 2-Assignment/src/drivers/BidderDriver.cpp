#include "../Bidder.h"
#include "../util/TestUtil.h"

int main() {

    Players players = createDummyPlayers(3);

    Player* winner = Bidder::startBid(&players);
    Bidder::getFirstPlayer(winner, &players);

    for(pair<string, Player*> pl: players){
        delete pl.second;
    }

    winner = nullptr;
}