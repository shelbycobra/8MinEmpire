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

PhaseObserver::PhaseObserver() {

}

PhaseObserver::PhaseObserver(MainGameEngine* mainEngine) {
    this->mainEngine = mainEngine;
    this->mainEngine->Attach(this);
}

PhaseObserver::~PhaseObserver() {
    this->mainEngine->Detach(this);
}

void PhaseObserver::Update() {
    Player* currentPlayer = mainEngine->getCurrentPlayer();
    Card* currentCard = mainEngine->getCurrentCard();

    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "{ " << currentPlayer->getName() << " } Has chosen card [ " << currentCard->getGood() << " : \""
         << currentCard->getAction() << "\" ]" << endl << "\tat position " << currentCard->getPosition()
         << " which costs " << currentCard->getCost() << " coins." << endl;
    cout << "---------------------------------------------------------------------\n" << endl;
}

StatsObserver::StatsObserver() {}

StatsObserver::StatsObserver(MainGameEngine* mainEngine) {
    this->mainEngine = mainEngine;
    this->mainEngine->Attach(this);
}

StatsObserver::~StatsObserver() {
    this->mainEngine->Detach(this);
}

void StatsObserver::Update() {
    GameMap* map = mainEngine->getMap();
    map->printMap();
    printStats();
    printVictoryPoints();
}

void StatsObserver::printStats() {
    GameMap* map = mainEngine->getMap();
    vector<set<string>* > continents = map->getContinents();

    vector<set<string>* >::iterator it;

    cout << "---------------------------------------------------------------------" << endl;

    for (it = continents.begin(); it != continents.end(); ++it) {
        set<string>* continent = (*it);

        string continentName = map->getVertices()->find(*continent->begin())->second->getContinent();
        string owner = map->getContinentOwner(continent);

        printf("%13s %s\n\n", continentName.c_str(), ("{{ " + owner + " }}").c_str());

        for(string vertex: *continent) {
            if (map->getVertices()->find(vertex) != map->getVertices()->end()) {
                Vertex* region = map->getVertices()->find(vertex)->second;

                if (region->getArmies()->size() > 0 || region->getCities()->size() > 0)
                    region->print();
            }
        }
        cout << "---------------------------------------------------------------------" << endl;
    }
    cout << endl;
}

void StatsObserver::printVictoryPoints() {
    Players* players = mainEngine->getPlayers();

    cout << "---------------------------------------------------------------------" << endl;
    cout << "=====================================================================" << endl;

    Players::iterator it;

    for(it = players->begin(); it != players->end(); ++it) {
        Player* pl = it->second;

        // Make it so stuff doesnt print out from player methods when this is called

        int vpRegions = pl->getVPFromRegions();
        int vpContinents = pl->getVPFromContinents(mainEngine->getMap());
        int vpGoods = pl->getVPFromGoods();

        int totalVP = vpRegions + vpContinents + vpGoods;

        cout << "{ " << pl->getName() << " } Victory Points:" << endl;
        cout << "\t VP from Regions: " << vpRegions << endl;
        cout << "\t VP from Continents: " << vpContinents << endl;
        cout << "\t VP from Goods: " << vpGoods << endl;
        cout << "\t TOTAL:\t" << totalVP << endl << endl;

    }
}