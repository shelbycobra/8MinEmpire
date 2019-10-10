#include "Player.h"
#include "Map.h"
#include "Cards.h"

Player::Player():
    name(new string("No name")),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)) {}

Player::Player(string& playerName, int startCoins):
    name(new string(playerName)),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(startCoins)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this))
{
    cout << "{ " << *name << " } CREATED. (Purse = " << startCoins << ")." << endl;
}

Player::Player(Player* player){
    name = new string(player->getName());
    countries = new Vertices(*player->getCountries());
    armies = new int(player->getArmies());
    cities = new int(player->getCities());
    coins = new int(player->getCoins());
    hand = new vector<Card*>(*player->getHand());
    bidder = new Bidder(player->getBidder());
}

Player::~Player(){
    //Delete all the cards the player holds.
    for(vector<Card*>::iterator it = hand->begin(); it != hand->end(); ++it)
        delete *it;

    delete name;
    delete countries;
    delete armies;
    delete cities;
    delete coins;
    delete hand;

    name = 0;
    countries = 0;
    armies = 0;
    cities = 0;
    coins = 0;
    hand = 0;
}

bool Player::payCoins(int amount){
    if (amount <= *coins && amount >= 0) {
        string coinStr = amount == 1 ? "coin" : "coins";
        *coins -= amount;
        cout << "\n{ " << *name << " } Paid " << amount << " " << coinStr << ". (Purse = " << *coins << ").\n" << endl;
        return true;
    }
    cout << "[ ERROR! ] " << *name << " doesn't have enough coins to pay " << amount << ". (Purse = " << *coins << ")." << endl;
    return false;
}

bool Player::placeNewArmies(int newArmies, Vertex* country, string start){
    if (country->getKey() == start || country->getCities()->find(*name) != country->getCities()->end()){
        if (newArmies > *armies) {
            cout << "[ ERROR! ] " << *name << " doesn't have enough armies to place " << newArmies << " new armies on < " << country->getName() << " >." << endl;
            return false;
        }

        *armies -= newArmies;
        addArmiesToCountry(country, newArmies);

        cout << "{ " << *name << " } Added " << newArmies << " new armies to < "<< country->getName() << " >." << endl;
        return true;
    }
    cout << "[ ERROR! ] " << *name << " Chose an invalid country. It must be either START or contain one of the player's cities->" << endl;
    return false;
}

bool Player::isAdjacent(Vertex* target, bool overWaterAllowed){
    return isAdjacent(target->getName(), overWaterAllowed);
}

bool Player::isAdjacent(string target, bool overWaterAllowed){
    typedef pair<Vertex*, bool> Edge;

    Vertices::iterator it = countries->begin();
    set<string> visited;

    while(it != countries->end()) {

        if (it->second->getName() == target)
            return true;

        if (visited.find(it->first) == visited.end())
            for(Edge& e : *it->second->getEdges())
                if (e.first->getName() == target && (!e.second || (e.second && overWaterAllowed)))
                    return true;

        visited.insert(it->first);
        ++it;
    }

    return false;
}

int Player::getArmiesOnCountry(Vertex* country) {
    if(countries->find(country->getKey()) != countries->end()) {
        int numArmies = country->getArmies()->find(*name)->second;
        string army = numArmies == 1 ? "army" : "armies";
        cout << "{ " << *name << " } Has " << numArmies << " " << army << " on country < " << country->getName() << " >." << endl;
        return numArmies;
    }
    cout << "{ " << *name << " } Has ZERO armies on country < " << country->getName() << " >." << endl;
    return 0;
}

int Player::getCitiesOnCountry(Vertex* country){
    if(countries->find(country->getKey()) != countries->end()) {
        int numCities = country->getCities()->find(*name)->second;
        string city = numCities == 1 ? "city" : "cities";
        cout << "{ " << *name << " } Has " << numCities << " " << city << " on country < " << country->getName() << " >." << endl;
        return numCities;
    }
    cout << "{ " << *name << " } Has ZERO cities on country < " << country->getName() << " >." << endl;
    return 0;
}

bool Player::moveOverLand(int numArmies, Vertex* start, Vertex* end){
    return moveArmies(numArmies, start, end, false);
}

bool Player::moveArmies(int numArmies, Vertex* start, Vertex* end, bool moveOverWater){
    //Assumes: country is a valid vertex on the map
    //is country an adjacent country to an occupied country?
    if (isAdjacent(end, moveOverWater)) {
        //does start vertex even have armies on it?
        if (start->getArmies()->find(*name) != start->getArmies()->end()) {
            int currentArmies = start->getArmies()->find(*name)->second;
            if (numArmies > currentArmies) {
                cout << "[ ERROR! ] " << *name << " doesn't have enough armies on < " << start->getName()
                     << " > to move " << numArmies << "." << endl;
                return false;
            }

            addArmiesToCountry(end, numArmies);
            removeArmiesFromCountry(start, numArmies);

            cout << "{ " << *name << " } Moved " << numArmies
                 << " armies from < " << start->getName() << " > to < " << end->getName() << " >." << endl;
            return true;
        }

        cout << "[ ERROR! ] " << *name << " doesn't have any armies on < " << start->getName() << " > to move." << endl;
        return false;
    }

    cout << "[ ERROR! ] " << end->getName() << " > is not an adjacent country." << endl;
    return false;
}

bool Player::buildCity(Vertex* country){
    unordered_map<string, int>* armies = country->getArmies();

    //does country belong to the player and does it contain at least one army?
    if (armies->find(*name) != armies->end()) {
        if (armies->find(*name)->second > 0) {

            //If country contains a player owned city, increase the count
            int currentCities = 1;
            if (country->getCities()->find(*name) != country->getCities()->end()) {
                currentCities += country->getCities()->find(*name)->second;
                country->getCities()->erase(*name);
                country->getCities()->insert(pair<string,int>(*name, currentCities));
            } else //Else insert new record
                country->getCities()->insert(pair<string, int> (*name, currentCities));

            cout << "{ " << *name << " } Added an city to < " << country->getName() << " >. (New city count = " << currentCities << ")." << endl;
            return true;
        }
    }

    cout << "[ ERROR! ] " << *name << " can't place city on < " << country->getName() << " > because the player has no armies on it." << endl;
    return false;
}

bool Player::destroyArmy(Vertex* country, Player* opponent){

    if (opponent->getName() == *name) {
        cout << "\n{ " << *name << " } Can't destroy own army." << endl;
        return false;
    }

    if (country->getArmies()->find(opponent->getName()) != country->getArmies()->end()
        && country->getArmies()->find(opponent->getName())->second > 0) {
        int currentArmies = country->getArmies()->find(opponent->getName())->second;
        currentArmies--;

        //Add destroyed army back in opponents available armies
        opponent->increaseAvailableArmies(1);

        country->getArmies()->erase(opponent->getName());

        if (currentArmies != 0)
            country->getArmies()->insert(pair<string, int> (opponent->getName(), currentArmies));
        else {
            opponent->removeCountry(country);
        }

        cout << "{ " << *name << " } Destroyed one of " << opponent->getName() << "'s armies on < " << country->getName() << " >." << endl;
        return true;
    }


    cout << "[ ERROR! ] " << opponent->getName() << " doesn't have any armies to destroy on < " << country->getName() << " >." << endl;
    return false;
}

void Player::addCardToHand(Card* card) {
    hand->push_back(card);
}


void Player::addCountry(Vertex* country){
    Vertex* v;
    v = country;
    countries->insert(pair<string, Vertex*> (country->getKey(), v));
    cout << "{ " << *name << " } " << "Added country < " << country->getName() << " > to player's countries." << endl;
}

void Player::removeCountry(Vertex* country) {
    if(countries->find(country->getKey()) != countries->end()) {
        int numArmies = 0, numCities = 0;

        if (country->getArmies()->find(*name) != country->getArmies()->end())
            numArmies = country->getArmies()->find(*name)->second;
        if (country->getCities()->find(*name) != country->getCities()->end())
            numCities = country->getCities()->find(*name)->second;

        if (numArmies == 0 && numCities == 0) {
            country->getArmies()->erase(*name);
            country->getCities()->erase(*name);
            countries->erase(country->getKey());
            cout << "{ " << *name << " } " << "Removed < " << country->getName() << " >." << endl;
        } else
            cout << "[ ERROR! ] Can't remove < " << country->getName() << " >. " << *name << " still owns "
                << numArmies << " armies and " << numCities << " cities on it." << endl;
    } else {
        cout << "[ ERROR! ] " << *name << " doesn't have the country < " << country->getName() << " > available to remove." << endl;
    }
}


// Private
void Player::addArmiesToCountry(Vertex* country, int numArmies) {
    if(countries->find(country->getKey()) == countries->end())
        addCountry(country);
    if (country->getArmies()->find(*name) == country->getArmies()->end()) {
        country->getArmies()->insert(pair<string, int> (*name, numArmies));
    } else {
        int currentArmies = country->getArmies()->find(*name)->second;
        country->getArmies()->erase(*name);
        country->getArmies()->insert(pair<string, int> (*name, currentArmies + numArmies));
    }
}

// Private
void Player::removeArmiesFromCountry(Vertex* country, int numArmies) {
    int currentArmies = country->getArmies()->find(*name)->second;

    // erase current record to be updated
    country->getArmies()->erase(*name);

    // only update army count if there are still armies left on country
    if (currentArmies - numArmies > 0)
        country->getArmies()->insert(pair<string, int> (*name, currentArmies - numArmies));
    // remove country if resulting num armies is 0 and no cities exist
    else if (numArmies == currentArmies && country->getCities()->find(*name) == country->getCities()->end())
        removeCountry(country);
}

string Player::getName(){return *name;}

Vertices* Player::getCountries(){return countries;}

int Player::getArmies(){return *armies;}

int Player::getCities(){return *cities;}

int Player::getCoins(){return *coins;}

vector<Card*>* Player::getHand(){return hand;}

Bidder* Player::getBidder() {return bidder;}

void Player::increaseAvailableArmies(int amount) {
    *armies += amount;
}

void Player::decreaseAvailableArmies(int amount) {
    if (*armies - amount <= 0) {
        *armies = 0;
    } else {
        *armies -= amount;
    }
}

void Player::printCountries(){

    cout << "\n{ " << *name << " } Occupied Countries:\n" << endl;
    cout << "--------------------------------------------------------" << endl;
    Vertices::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        int numArmies = 0;
        int numCities = 0;
        if (it->second->getArmies()->find(*name) != it->second->getArmies()->end())
            numArmies = it->second->getArmies()->find(*name)->second;
        if (it->second->getCities()->find(*name) != it->second->getCities()->end())
            numCities = it->second->getCities()->find(*name)->second;

        printf("\t%-3s : %-15s ARMIES: %-5d CITIES: %-5d\n", it->second->getKey().c_str(), it->second->getName().c_str(), numArmies, numCities);
    }

    cout << "--------------------------------------------------------" << endl;
}