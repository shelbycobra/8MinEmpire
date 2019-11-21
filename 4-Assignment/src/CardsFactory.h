#ifndef CARDS_FACTORY_H
#define CARDS_FACTORY_H

#include "Cards.h"

class CardsFactory {

public:
    static Card* create(const int& id, const string& good, const string& action);

private:
    CardsFactory(){};
};

class StoneCard: public Card {
public:
    StoneCard();
    StoneCard(const int& id, const string& good, const string& action);
    string getType() { return STONE; }
};

class GemCard: public Card {
public:
    GemCard();
    GemCard(const int& id, const string& good, const string& action);
    string getType() { return GEM; }
};

class CarrotCard: public Card {
public:
    CarrotCard();
    CarrotCard(const int& id, const string& good, const string& action);
    string getType() { return CARROT; }
};

class IronCard: public Card {
public:
    IronCard();
    IronCard(const int& id, const string& good, const string& action);
    string getType() { return IRON; }
};

class WildCard: public Card {
public:
    WildCard();
    WildCard(const int& id, const string& good, const string& action);
    string getType() { return WILD; }
};

class WoodCard: public Card {
public:
    WoodCard();
    WoodCard(const int& id, const string& good, const string& action);
    string getType() { return WOOD; }
};

#endif