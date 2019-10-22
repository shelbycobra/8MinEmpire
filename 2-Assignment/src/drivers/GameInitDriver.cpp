#include <iostream>

#include "../GameInit.h"


void test_GameInit();

int main() {

    test_GameInit();

    return 0;
}

void test_GameInit(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_GameInitObject" << endl;
    cout << "=======================================================" << endl;

    GameInitEngine initPhase;

    initPhase.initGame();

    Hand* hand;
    GameMap* map;
    Players* players;

    hand = initPhase.getHand();
    map = initPhase.getMap();
    players = initPhase.getPlayers();

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

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Verify players hands are empty." << endl;
    cout << "--------------------------------------------------------\n" << endl;
    for(pl = players->begin(); pl != players->end(); ++pl) {
        cout << pl->first << ": Num cards in hand is " << pl->second->getHand()->size() << endl;

    }
        
}