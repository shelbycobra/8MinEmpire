#include "../Bidder.h"
#include "../util/TestUtil.h"

int main() {

    Players players = createDummyPlayers(3);

    Bidder::startBid(&players);

    for(pair<string, Player*> pl: players){
        delete pl.second;
    }
}