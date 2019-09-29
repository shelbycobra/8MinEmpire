#include "Player.h"
#include "Map.h"

Player::Player(string& playerName, int startCoins): name(new string(playerName)),
    armies(new int(14)), cities(new int(3)), coins(new int(startCoins)),
    countries(new set<Vertex*>()), hand(new Hand()){
        cout << "[ " << *name << " ] CREATED. (Purse = " << startCoins << ")." << endl;
    }

Player::~Player(){
    delete name;
    delete countries;
    delete armies;
    delete cities;
    delete coins;
    delete hand;

    name = NULL;
    countries = NULL;
    armies = NULL;
    cities = NULL;
    coins = NULL;
    hand = NULL;
}

bool Player::payCoins(int amount){
    if (amount <= *coins && amount >= 0) {
        *coins -= amount;
        cout << "[ "<< *name <<" ] Paid " << amount << " coins. (Purse = " << *coins << ")." << endl;
        return true;
    }
    cout << "[ "<< *name <<" ] Doesn't have enough coins to pay " << amount << ". (Purse = " << *coins << ")." << endl;
    return false;
}

bool Player::placeNewArmies(int newArmies, Vertex* country, string& start){
    //Note: player has already chosen which armies and where to place
    //Assumes country is a valid country on the map

    if (country->name == start || country->cities.find(*name) != country->cities.end()){
        if (newArmies > *armies) {
            cout << "[ "<< name <<" ] Doesn't have enough armies to place " << newArmies << " new armies on < " << country->name << " >." << endl;
            return false;
        }

        *armies -= newArmies;
        addArmiesToCountry(country, newArmies);

        cout << "[ " << *name << " ] Added " << newArmies << " new armies to < "<< country->name << " >." << endl;
        return true;
    }
    cout << "[ " << *name << " ] Chose an invalid country. It must be either START or contain one of the player's cities." << endl;
    return false;
}

bool Player::isAdjacent(Vertex* target, bool overWaterAllowed){
    typedef pair<Vertex*, bool> Edge;

    set<Vertex*>::iterator it = countries->begin();
    set<string> visited;

    while(it != countries->end()) {

        if ((*it)->name == target->name)
            return true;

        if (visited.find((*it)->name) == visited.end())
            for(Edge& e : (*it)->edges)
                if (e.first == target && (!e.second || (e.second && overWaterAllowed))) 
                    return true;

        visited.insert((*it)->name);
        ++it;
    }

    return false;
}

int Player::getArmiesOnCountry(Vertex* country) {
    set<Vertex*>::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        if ((*it)->name == country->name) {
            int numArmies = (*it)->armies.find(*name)->second;
            string army = numArmies == 1 ? "army" : "armies";
            cout << "[ " << *name << " ] Has " << numArmies << " " << army << " on country < " << country->name << " >." << endl;
            return numArmies;
        }
    }
    cout << "[ " << *name << " ] Has ZERO armies on country < " << country->name << " >." << endl;
    return 0;
}

int Player::getCitiesOnCountry(Vertex* country){
    set<Vertex*>::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        if ((*it)->name == country->name) {
            int numCities = (*it)->cities.find(*name)->second;
            string city = numCities == 1 ? "city" : "cities";
            cout << "[ " << *name << " ] Has " << numCities << " " << city << " on country < " << country->name << " >." << endl;
            return numCities;
        }
    }
    cout << "[ " << *name << " ] Has ZERO cities on country < " << country->name << " >." << endl;
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
        if (start->armies.find(*name) != start->armies.end()) {
            int currentArmies = start->armies.find(*name)->second;
            if (numArmies > currentArmies) {
                cout << "[ " << *name << " ] Doesn't have enough armies on < " << start->name
                     << " > to move " << numArmies << "." << endl;
                return false;
            }

            addArmiesToCountry(end, numArmies);
            removeArmiesFromCountry(start, numArmies);

            cout << "[ " << *name << " ] Moved " << numArmies 
                 << " from < " << start->name << " > to < " << end->name << " >." << endl;
            return true;
        }

        cout << "[ " << *name << " ] Doesn't have any armies on < " << start->name << " > to move." << endl;
        return false;
    }

    cout << "[ "<< *name <<" ] < " << end->name << " > is not an adjacent country." << endl;
    return false;
}

bool Player::buildCity(Vertex* country){
    unordered_map<string, int> armies = country->armies;

    //does country belong to the player and does it contain at least one army?
    if (armies.find(*name) != armies.end()) {
        if (armies.find(*name)->second > 0) {

            //If country contains a player owned city, increase the count
            int currentCities = 1;
            if (country->cities.find(*name) != country->cities.end()) {
                currentCities += country->cities.find(*name)->second;
                country->cities.erase(*name);
                country->cities.insert(pair<string,int>(*name, currentCities));
            } else //Else insert new record
                country->cities.insert(pair<string, int> (*name, currentCities));

            cout << "[ " << *name << " ] Added an city to < " << country->name << " >. (New city count = " << currentCities << ")." << endl; 
            return true;
        }
    }

    cout << "[ " << *name << " ] Can't place city on < " << country->name << " > because the player has no armies on it." << endl; 
    return false;
}

bool Player::destroyArmies(Vertex* country, Player* opponent){

    if (opponent->getName() == *name) {
        cout << "[ " << *name << " ] Can't destroy own army." << endl;
        return false;
    }

    if (country->armies.find(opponent->getName()) != country->armies.end() && country->armies.find(opponent->getName())->second > 0) {
        int currentArmies = country->armies.find(opponent->getName())->second;
        currentArmies--;

        //Add destroyed army back in opponents available armies
        opponent->increaseAvailableArmies(1);

        country->armies.erase(opponent->getName());

        if (currentArmies != 0)
            country->armies.insert(pair<string, int> (opponent->getName(), currentArmies));
        else {
            opponent->removeCountry(country);
        }

        cout << "[ " << *name << " ] Destroyed one of " << opponent->getName() << "'s armies on < " << country->name << " >." << endl;
        return true;
    }


    cout << "[ " << *name << " ] " << opponent->getName() << " doesn't have any armies to destroy on < " << country->name << " >." << endl;
    return false;
}

void Player::addCard(Card* card) {
    hand->push_back(card);

    string action = card->action;
    //
}


void Player::addCountry(Vertex* country){
    Vertex* v;
    v = country;
    countries->insert(v);
    cout << "[ " << *name << " ] " << "Added country < " << country->name << " > to player's countries." << endl;
}

void Player::removeCountry(Vertex* country) {
    string countryName = country->name;
    for (Vertex* v : *countries) {
        if (v->name == countryName) {
            int numArmies = 0, numCities = 0;

            if (v->armies.find(*name) != v->armies.end())
                numArmies = v->armies.find(*name)->second;
            if (v->cities.find(*name) != v->cities.end())
                numCities = v->cities.find(*name)->second;

            if (numArmies == 0 && numCities == 0) {
                v->armies.erase(*name);
                v->cities.erase(*name);
                countries->erase(v);
                cout << "[ "<< *name <<" ] " << "Removed < " << country->name << " >." << endl;
            } else
                cout << "[ "<< *name <<" ] " << "Can't remove < " << country->name << " >. The player still owns "
                    << numArmies << " armies and " << numCities << " cities on it." << endl;

            return;
        }
    }
    cout << "[ " << *name << " ] " << "Doesn't have the country < " << country->name << " > available to remove." << endl;
}

bool Player::isCountryInCountriesSet(Vertex* country) {
    set<Vertex*>::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it)
        if ((*it)->name == country->name)
            return true;
    return false;
}

// Private
void Player::addArmiesToCountry(Vertex* country, int numArmies) {
    if(!isCountryInCountriesSet(country))
        addCountry(country);
    if (country->armies.find(*name) == country->armies.end()) {
        country->armies.insert(pair<string, int> (*name, numArmies));
    } else {
        int currentArmies = country->armies.find(*name)->second;
        country->armies.erase(*name);
        country->armies.insert(pair<string, int> (*name, currentArmies + numArmies));
    }
}

// Private
void Player::removeArmiesFromCountry(Vertex* country, int numArmies) {
    int currentArmies = country->armies.find(*name)->second;
    if (numArmies == currentArmies && country->cities.find(*name) == country->cities.end())
        removeCountry(country);
    country->armies.erase(*name);
    country->armies.insert(pair<string, int> (*name, currentArmies - numArmies));
}

string Player::getName(){return *name;}

set<Vertex*>* Player::getCountries(){return countries;}

int Player::getAvailableArmies(){return *armies;}

int Player::getAvailableCities(){return *cities;}

int Player::getCoins(){return *coins;}

Hand* Player::getHand(){return hand;}

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