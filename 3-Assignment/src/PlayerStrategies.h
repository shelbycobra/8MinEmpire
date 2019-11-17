#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include "Cards.h"
#include "Map.h"
#include "Player.h"

enum ActionType { MOVE_OVER_LAND, ADD_ARMY, DESTROY_ARMY, MOVE_OVER_WATER, BUILD_CITY, NONE };

class Card;
class Hand;
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

    virtual void PlaceNewArmies(Player* player, const string action, Players* players) = 0;
    virtual void MoveArmies(Player* player, const string action, Players* players) = 0;
    virtual void BuildCity(Player* player) = 0;
    virtual void DestroyArmy(Player* player, Players* players) = 0;
    virtual void AndOrAction(Player* player, const string action, Players* players) = 0;
    virtual int chooseCardPosition(Player* player, Hand* hand) = 0;

};

class GreedyStrategy: public Strategy {
// greedy computer player that focuses on building cities or destroying opponents,
public:
    GreedyStrategy();
    ~GreedyStrategy();

    void PlaceNewArmies(Player* player, const string action, Players* players);
    void MoveArmies(Player* player, const string action, Players* players);
    void BuildCity(Player* player);
    void DestroyArmy(Player* player, Players* players);
    void AndOrAction(Player* player, const string action, Players* players);
    int chooseCardPosition(Player* player, Hand* hand);

};

class ModerateStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents.
public:
    ModerateStrategy();
    ~ModerateStrategy();

    void PlaceNewArmies(Player* player, const string action, Players* players);
    void MoveArmies(Player* player, const string action, Players* players);
    void BuildCity(Player* player);
    void DestroyArmy(Player* player, Players* players);
    void AndOrAction(Player* player, const string action, Players* players);
    int chooseCardPosition(Player* player, Hand* hand);
    bool changeOwnership(Vertex* startVertex, Vertex* endVertex, Player* currentPlayer, int& maxNumArmies, Players* players, bool overWaterAllowed);

};

class HumanStrategy: public Strategy {
public:
    HumanStrategy();
    ~HumanStrategy();

    void PlaceNewArmies(Player* player, const string action, Players* players);
    void MoveArmies(Player* player, const string action, Players* players);
    void BuildCity(Player* player);
    void DestroyArmy(Player* player, Players* players);
    void AndOrAction(Player* player, const string action, Players* players);

private:
    Vertex* chooseStartVertex(Player* player);
    Vertex* chooseEndVertex(Player* player, const ActionType& type);
    int chooseArmies(Player* player, const int&, const int&, int, const string&);
    string chooseORAction(Player* player, const string action);
    Player* chooseOpponent(Player* player, Players* players);
    int chooseCardPosition(Player* player, Hand* hand);
};

#endif