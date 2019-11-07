#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

class Card;
class GameMap;
class Players;

class Strategy {

public:
    virtual void execute(Card*, GameMap*, Players*) = 0;
};

class GreedyStrategy: public Strategy {
// greedy computer player that focuses on building cities or destroying opponents,
public:
    GreedyStrategy();
    ~GreedyStrategy();

    void execute(Card*, GameMap*, Players*);
};

class ModerateStrategy: public Strategy {
// a moderate computer player that control a region in which it just needs to occupy it with more armies than the
// opponents. 
public:
    ModerateStrategy();
    ~ModerateStrategy();

    void execute(Card*, GameMap*, Players*);
};

#endif