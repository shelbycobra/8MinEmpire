#include <iostream>

#include "../GameInit.h"

Hand* hand;
GameMap* map;
Players* players;

void test_start();

int main() {

    test_start();

    return 0;
}

void test_start(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_StartObject" << endl;
    cout << "=======================================================" << endl;

    GameInitEngine startPhase;

    startPhase.getMapFiles();
    startPhase.initGame();

    hand = startPhase.getHand();
    map = startPhase.getMap();
    players = startPhase.getPlayers();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Verify game hand contains same cards during initialization." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    vector<Card*>::iterator c;
    for(c = hand->getHand()->begin(); c != hand->getHand()->end(); ++c)
        cout << (*c)->getAction() << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Verify map is same as during initialization." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    map->printMap();

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Verify players are the same as during initialization." << endl;
    cout << "--------------------------------------------------------\n" << endl;
    Players::iterator pl;
    for(pl = players->begin(); pl != players->end(); ++pl)
        cout << pl->first << endl;
}