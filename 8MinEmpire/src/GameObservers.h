#ifndef GAME_OBSERVERS_H
#define GAME_OBSERVERS_H

#include <list>

using namespace std;

class GameEngine;

class Observer {
public:
    ~Observer();
    virtual void Update() = 0;
protected:
    Observer();
};

class Subject {
public:
    virtual void Attach(Observer* o);
    virtual void Detach(Observer* o);
    virtual void Notify();
    Subject();
    virtual ~Subject();
private:
    list<Observer*> *observers;
};

// This needs to be included AFTER the Subject get defined to avoid errors.
#include "GameEngine.h"

class PhaseObserver : public Observer {
    GameEngine* gameEngine;
public:
    PhaseObserver();
    PhaseObserver(GameEngine* gameEngine);
    ~PhaseObserver();
    void Update();
};

class StatsObserver : public Observer {
    GameEngine* gameEngine;
public:
    StatsObserver();
    StatsObserver(GameEngine* gameEngine);
    ~StatsObserver();
    void Update();
    void printMapRegions();
    void printVictoryPoints();
    void printGoodsFromCards();
};

#endif