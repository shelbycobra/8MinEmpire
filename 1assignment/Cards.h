#ifndef CARDS_H
#define CARDS_H

#include <unordered_map>
#include <string>

using namespace std;

class Card {

    string* good;
    string* action;
};

class Deck {

    unordered_map<int, Card*>* deck;
    

public:
    Card* draw();
};


#endif