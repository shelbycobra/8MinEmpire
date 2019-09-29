#ifndef CARDS_H
#define CARDS_H

#include <map>
#include <string>
#include <vector>
#include <queue>

#include "Player.h"

using namespace std;

struct Card {

    int id;
    string good;
    string action;

    Card(int id, string theGood, string theAction): id(id), good(theGood), action(theAction) {}
};

class Deck {
    queue<Card*>* cardDeck;

    public:
        Deck();
        ~Deck();
        Card* draw();
};

class Hand {

    vector<Card*>* hand;
    Deck* deck;

    public:
        Hand();
        ~Hand();

        void exchange(int position, Player* player);
        void printHand();
};

#endif