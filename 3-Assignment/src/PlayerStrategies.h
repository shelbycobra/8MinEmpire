#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include "Cards.h"
#include "Map.h"
#include "Player.h"


enum ActionType { MOVE_OVER_LAND, ADD_ARMY, DESTROY_ARMY, MOVE_OVER_WATER, BUILD_CITY, NONE };

class Card;
class Vertex;
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

    virtual Vertex* chooseStartVertex(Player* player) = 0;
    virtual Vertex* chooseEndVertex(Player* player, const ActionType& type) = 0;
    virtual int chooseArmies(Player* player, const int&, const int&, int, const string&) = 0;
    virtual string chooseORAction(Player* player, const string action) = 0;
    virtual Player* chooseOpponent(Player* player, Players* players)= 0;
};

class GreedyStrategy: public Strategy {
// greedy computer player that focuses on building cities or destroying opponents,
public:
    GreedyStrategy();
    ~GreedyStrategy();

    Vertex* chooseStartVertex(Player* player);
    Vertex* chooseEndVertex(Player* player, const ActionType& type);
    int chooseArmies(Player* player, const int&, const int&, int, const string&);
    string chooseORAction(Player* player, const string action);
    Player* chooseOpponent(Player* player, Players* players);
};

class ModerateStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents.
public:
    ModerateStrategy();
    ~ModerateStrategy();

    Vertex* chooseStartVertex(Player* player);
    Vertex* chooseEndVertex(Player* player, const ActionType& type);
    int chooseArmies(Player* player, const int&, const int&, int, const string&x);
    string chooseORAction(Player* player, const string action);
    Player* chooseOpponent(Player* player, Players* players);
};

class HumanStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents.
public:
    HumanStrategy();
    ~HumanStrategy();

    Vertex* chooseStartVertex(Player* player);
    Vertex* chooseEndVertex(Player* player, const ActionType& type);
    int chooseArmies(Player* player, const int&, const int&, int, const string&);
    string chooseORAction(Player* player, const string action);
    Player* chooseOpponent(Player* player, Players* players);
};

#endif