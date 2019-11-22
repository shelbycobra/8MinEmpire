#include "GameObservers.h"
#include "GameEngine.h"

#include <stdio.h>

using namespace std;

class GamegameEngine;

Observer::Observer() {}

Observer::~Observer() {}

/**
 * Default Constructor.
 */
Subject::Subject(){
    observers = new list<Observer*>;
}

/**
 * Destructor.
 */
Subject::~Subject(){
    delete observers;
}

/**
 * Attaches an Observer to the subject.
 */
void Subject::Attach(Observer* o){
    observers->push_back(o);
}

/**
 * Detaches an Observer to the subject.
 */
void Subject::Detach(Observer* o){
    observers->remove(o);
}

/**
 * Updates all Observers attached to the Subject.
 */
void Subject::Notify(){
    list<Observer *>::iterator i = observers->begin();
    for (; i != observers->end(); ++i)
        (*i)->Update();
}

/**
 * Copy Constructor
 */
PhaseObserver::PhaseObserver(GameEngine* gameEngine) {
    this->gameEngine = gameEngine;
    this->gameEngine->Attach(this);
}

/**
 * Destructor
 */
PhaseObserver::~PhaseObserver() {
    this->gameEngine->Detach(this);
}

/**
 * Prints out a header indicating the card the current player has chosen, the position
 * the card was situated, and how many coins the player paid for the card.
 */
void PhaseObserver::Update() {
    Player* currentPlayer = gameEngine->getCurrentPlayer();
    Card* currentCard = gameEngine->getCurrentCard();

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "{ " << currentPlayer->getName() << " } Has chosen card [ " << currentCard->getGood() << " : \""
         << currentCard->getAction() << "\" ]" << endl << "\tat position " << currentCard->getPosition()
         << " which costs " << currentCard->getCost() << " coins." << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;
}

/**
 * Copy Constructor.
 */
StatsObserver::StatsObserver(GameEngine* gameEngine) {
    this->gameEngine = gameEngine;
    this->gameEngine->Attach(this);
}

/**
 * Destructor.
 */
StatsObserver::~StatsObserver() {
    this->gameEngine->Detach(this);
}

/**
 * Prints out the current game statistics at the end of each turn.
 *
 * Once all the players have reached their max card capacity, a winner is declared
 * and the game exits.
 */
void StatsObserver::Update() {
    GameMap::instance()->printMap();
    printMapRegions();
    printVictoryPoints();
    printGoodsFromCards();

    MainGameEngine* mainEngine = dynamic_cast<MainGameEngine*>(gameEngine);
    if (!InitGameEngine::instance()->isTournament() && !mainEngine->continueGame()) {
        mainEngine->declareWinner();
        exit(EXIT_SUCCESS);
    }
}

/**
* Prints out the occupied regions of the game inidicating which player currently owns each region and each continent.
 */
void StatsObserver::printMapRegions() {
    vector<set<string>* > continents = GameMap::instance()->getContinents();

    vector<set<string>* >::iterator it;

    cout << "---------------------------------------------------------------------------" << endl;

    for (it = continents.begin(); it != continents.end(); ++it) {
        set<string>* continent = (*it);

        string continentName = GameMap::instance()->getVertices()->find(*continent->begin())->second->getContinent();
        string owner = GameMap::instance()->getContinentOwner(continent);

        printf("%13s %s\n\n", continentName.c_str(), ("{{ " + owner + " }}").c_str());

        for(string vertex: *continent) {
            if (GameMap::instance()->getVertices()->find(vertex) != GameMap::instance()->getVertices()->end()) {
                Vertex* region = GameMap::instance()->getVertices()->find(vertex)->second;

                if (region->getArmies()->size() > 0 || region->getCities()->size() > 0)
                    region->print();
            }
        }
        cout << "---------------------------------------------------------------------------" << endl;
    }
    cout << endl;
}

/**
 * Prints out the current victory points for each player from owned regions, owned continents and card goods.
 */
void StatsObserver::printVictoryPoints() {
    Players* players = StartUpGameEngine::instance()->getPlayers();

    string divider = "========================================================================";

    cout << endl << divider << endl;
    cout << "                       G A M E  S T A T I S T I C S" << endl;
    cout << divider << endl << endl;

    string nameline = "";
    string regionline = "";
    string continentsline = "";
    string goodsline = "";
    string totalsline = "";

    int count = 0;

    for(Players::iterator it= players->begin(); it != players->end(); ++it) {

        char namebuff[50];
        char regionbuff[50];
        char continentsbuff[50];
        char goodsbuff[50];
        char totalsbuff[50];

        Player* pl = it->second;

        int vpRegions = pl->getRegions()->size();
        int vpContinents = pl->getOwnedContinents()->size();
        int vpGoods = pl->getVPFromGoods(pl->getGoodsCount());

        int totalVP = vpRegions + vpContinents + vpGoods;

        snprintf(namebuff, sizeof(namebuff), "\t%-15s    ",("{ " + pl->getName() + " }").c_str());
        snprintf(regionbuff, sizeof(regionbuff), "%-20s %d   ", "VP from Regions: ", vpRegions );
        snprintf(continentsbuff, sizeof(continentsbuff), "%-20s %d   ", "VP from Continents: ", vpContinents );
        snprintf(goodsbuff, sizeof(goodsbuff), "%-20s %d   ", "VP from Goods: ", vpGoods );
        snprintf(totalsbuff, sizeof(totalsbuff), "%-20s %-2d  ", "TOTAL:", totalVP );

        nameline += namebuff;
        regionline += regionbuff;
        continentsline += continentsbuff;
        goodsline += goodsbuff;
        totalsline += totalsbuff;

        Players::iterator next = it;
        next++;
        count++;

        // Prints out the first 3 players on the same line to save space.
        if (count == 3 || next == players->end()) {
            cout << nameline.c_str() << endl;
            cout << endl;
            cout << regionline.c_str() << endl;
            cout << continentsline.c_str() << endl;
            cout << goodsline.c_str() << endl;
            cout << endl;
            cout << totalsline.c_str() << endl << endl;

            nameline = "";
            regionline = "";
            continentsline = "";
            goodsline = "";
            totalsline = "";
        }
    }

    cout << divider << endl << endl;
}

/**
 * Prints out the number of goods each player has from their current hand.
 */
void StatsObserver::printGoodsFromCards() {

    Players* players = StartUpGameEngine::instance()->getPlayers();

    string divider = "========================================================================";

    cout << endl << divider << endl;
    cout << "                       P L A Y E R  C A R D S" << endl;
    cout << divider << endl << endl;

    string nameline = "";
    string woodline = "";
    string ironline = "";
    string carrotline = "";
    string gemline = "";
    string stoneline = "";
    string wildline = "";

    int count = 0;

    for(Players::iterator it = players->begin(); it != players->end(); ++it) {

        char namebuff[50];
        char woodbuff[50];
        char ironbuff[50];
        char carrotbuff[50];
        char gembuff[50];
        char stonebuff[50];
        char wildbuff[50];

        snprintf(woodbuff, sizeof(woodbuff), "%-17s%-10d", WOOD.c_str(), 0);
        snprintf(ironbuff, sizeof(ironbuff), "%-17s%-10d", IRON.c_str(), 0);
        snprintf(carrotbuff, sizeof(carrotbuff), "%-17s%-10d", CARROT.c_str(), 0);
        snprintf(gembuff, sizeof(gembuff), "%-17s%-10d", GEM.c_str(), 0);
        snprintf(stonebuff, sizeof(stonebuff), "%-17s%-10d", STONE.c_str(), 0);
        snprintf(wildbuff, sizeof(wildbuff), "%-17s%-10d", WILD.c_str(), 0);

        Player* pl = it->second;

        snprintf(namebuff, sizeof(namebuff), "\t%-15s    ",("{ " + pl->getName() + " }").c_str());

        unordered_map<string, int> * goodsCount = pl->getGoodsCount();
        unordered_map<string, int>::iterator good;

        for (good = goodsCount->begin(); good != goodsCount->end(); ++good) {
            if (good->first == WOOD)
                snprintf(woodbuff, sizeof(woodbuff), "%-17s%-10d", WOOD.c_str(), good->second);
            else if (good->first == IRON)
                snprintf(ironbuff, sizeof(ironbuff), "%-17s%-10d", IRON.c_str(), good->second);
            else if (good->first == CARROT)
                snprintf(carrotbuff, sizeof(carrotbuff), "%-17s%-10d", CARROT.c_str(), good->second);
            else if (good->first == GEM)
                snprintf(gembuff, sizeof(gembuff), "%-17s%-10d", GEM.c_str(), good->second);
            else if (good->first == STONE)
                snprintf(stonebuff, sizeof(stonebuff), "%-17s%-10d", STONE.c_str(), good->second);
            else if (good->first == WILD)
                snprintf(wildbuff, sizeof(wildbuff), "%-17s%-10d", WILD.c_str(), good->second);
        }

        nameline += namebuff;
        carrotline += carrotbuff;
        woodline += woodbuff;
        ironline += ironbuff;
        gemline += gembuff;
        stoneline += stonebuff;
        wildline += wildbuff;

        Players::iterator next = it;
        next++;
        count++;

        if (count == 3 || next == players->end()) {
            cout << nameline << endl;
            cout << endl;
            cout << woodline << endl;
            cout << carrotline << endl;
            cout << ironline << endl;
            cout << gemline << endl;
            cout << stoneline << endl;
            cout << wildline << endl << endl;

            nameline = "";
            woodline = "";
            ironline = "";
            carrotline = "";
            gemline = "";
            stoneline = "";
            wildline = "";
        }
    }

    cout << divider << endl << endl;
}