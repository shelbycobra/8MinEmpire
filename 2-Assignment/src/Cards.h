#ifndef CARDS_H
#define CARDS_H

#include "Player.h"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <set>

using namespace std;

class Player;

class Card {
    int* id;
    string* good;
    string* action;

public:
    Card();
    Card(int theId, string theGood, string theAction);
    Card(Card* card);
    Card& operator=(Card& card);
    ~Card();

    int getID() { return *id; }
    string getGood() { return *good; }
    string getAction() { return *action; }
};

class Deck {
    queue<Card*>* cardDeck;

public:
    Deck();
    Deck(Deck* deck);
    Deck& operator=(Deck& deck);
    ~Deck();

    Card* draw();

    queue<Card*>* getDeck() { return cardDeck; }

private:
    int generateRandomInt(set<int>* nums);
};

class Hand {
    vector<Card*>* hand;
    Deck* deck;

public:
    Hand();
    Hand(Hand* hand);
    Hand& operator=(Hand& hand);
    ~Hand();

    Card exchange(Player* player);
    void printHand();

    vector<Card*>* getHand() { return hand; }
    Deck* getDeck() { return deck; }

private:
    int selectPositionOfCardFromGameHand();
};

#endif