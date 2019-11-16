#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include "Cards.h"
#include "Map.h"
#include "Player.h"

class Card;
typedef unordered_map<string, Player*> Players;

const string HUMAN = "HUMAN";
const string GREEDY = "GREEDY";
const string MODERATE = "MODERATE";

class Strategy {

    string* type;

public:
    Strategy();
    Strategy(const string &type);
    virtual ~Strategy();
    string getType() { return *type; }
    virtual void execute(Card*, Players*) = 0;
};

class GreedyStrategy: public Strategy {
// greedy computer player that focuses on building cities or destroying opponents,
public:
    GreedyStrategy();
    ~GreedyStrategy();

    void execute(Card*, Players*);
};

class ModerateStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents.
public:
    ModerateStrategy();
    ~ModerateStrategy();

    void execute(Card*, Players*);
};

class HumanStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents.
public:
    HumanStrategy();
    ~HumanStrategy();

    void execute(Card*, Players*);
};

#endif