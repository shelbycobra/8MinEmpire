#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <set>

using namespace std;

class Card {

    int* id;
    string* good;
    string* action;

public:
    Card(int theId, string theGood, string theAction);
    ~Card();

    int getID();
    string getGood();
    string getAction();
};

class Deck {
    queue<Card*>* cardDeck;

    public:
        Deck();
        ~Deck();
        Card* draw();
        queue<Card*>* getDeck();
    private:
        int generateRandomInt(set<int>* nums);
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