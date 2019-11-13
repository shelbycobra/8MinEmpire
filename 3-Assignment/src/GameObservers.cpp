#include "GameObservers.h"
#include "GameMainLoop.h"

class GameMainEngine;

Observer::Observer() {

}

Observer::~Observer() {

}

Subject::Subject(){
    observers = new list<Observer*>;
}

Subject::~Subject(){
    delete observers;
}

void Subject::Attach(Observer* o){
    observers->push_back(o);
}

void Subject::Detach(Observer* o){
    observers->remove(o);
}

void Subject::Notify(){
    list<Observer *>::iterator i = observers->begin();
    for (; i != observers->end(); ++i)
        (*i)->Update();
}

GameObserver::GameObserver() {

}

GameObserver::GameObserver(MainGameEngine* mainEngine) {
    this->mainEngine = mainEngine;
    this->mainEngine->Attach(this);
}

GameObserver::~GameObserver() {
    this->mainEngine->Detach(this);
}

void GameObserver::Update() {
    cout << "IN GAME OBSERVER UPDATE METHOD" << endl;
}

StatsObserver::StatsObserver() {

}

StatsObserver::StatsObserver(MainGameEngine* mainEngine) {
    this->mainEngine = mainEngine;
    this->mainEngine->Attach(this);
}

StatsObserver::~StatsObserver() {
    this->mainEngine->Detach(this);
}

void StatsObserver::Update() {
    cout << "IN STATS OBSERVER UPDATE METHOD" << endl;
}