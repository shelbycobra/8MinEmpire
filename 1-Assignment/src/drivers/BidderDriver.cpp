#include "../Bidder.h"
#include "../util/TestUtil.h"

int main() {

    Players players = createDummyPlayers(3);

    Bidder::startBid(&players);
}