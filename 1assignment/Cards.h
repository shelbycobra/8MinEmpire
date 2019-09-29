#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <queue>

#include "Player.h"

using namespace std;

class Player;

struct Card {

    int id;
    string good;
    string action;

    Card(int theId, string theGood, string theAction): id(theId), good(theGood), action(theAction) {}
};

class Deck {
    queue<Card*>* cardDeck;

    public:
        Deck();
        ~Deck();
        Card* draw();
        queue<Card*>* getDeck();
};

class Hand {

    vector<Card*>* hand;
    Deck* deck;

    public:
        Hand();
        ~Hand();

        Card* exchange(int position);
        void printHand();
        vector<Card*>* getHand();
};

#endif