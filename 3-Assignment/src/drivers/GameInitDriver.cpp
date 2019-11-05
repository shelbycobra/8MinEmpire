#include <iostream>
#include <cassert>
#include "../GameInit.h"

int main() {
    cout << "\n=====================================================================" << endl;
    cout << "TEST: test_GameInitObject" << endl;
    cout << "=====================================================================" << endl;

    GameInitEngine initPhase;

    initPhase.initGame();

    Hand* hand;
    GameMap* map;
    Players* players;

    hand = initPhase.getHand();
    map = initPhase.getMap();
    players = initPhase.getPlayers();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify deck contains 42 cards." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    Deck* deck = hand->getDeck();
    int decksize = deck->getDeck()->size();
    int handsize = hand->getHand()->size();

    cout << decksize << "+" << handsize << endl;
    assert(decksize + handsize == 42);
    cout << "Size of deck is " << decksize + handsize << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify game hand contains same cards during initialization." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    vector<Card*>::iterator c;
    for(c = hand->getHand()->begin(); c != hand->getHand()->end(); ++c)
        cout << (*c)->getGood() << " " << (*c)->getAction() << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify map is same as during initialization." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    map->printMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify players are the same as during initialization." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    cout << "Number of players are: " << players->size() << endl;

    Players::iterator pl;
    for(pl = players->begin(); pl != players->end(); ++pl)
        cout << pl->first << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify players hands are empty." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;
    for(pl = players->begin(); pl != players->end(); ++pl) {
        cout << pl->first << ": Num cards in hand is " << pl->second->getHand()->size() << endl;

    }

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Verify player has bidding facility and has not made a bid yet." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;
    for(pl = players->begin(); pl != players->end(); ++pl) {
        cout << pl->first << " has made bid " << pl->second->getBidder()->getMadeBid() << endl;
    }

    return 0;

}