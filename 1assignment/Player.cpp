#include "Player.h"
#include "Map.h"
#include "Cards.h"

Player::Player(string& playerName, int startCoins): name(new string(playerName)), countries(new Vertices()),
    armies(new int(14)), cities(new int(3)), coins(new int(startCoins)), hand(new vector<Card*>()){
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
    if (country->vertexKey == start || country->cities.find(*name) != country->cities.end()){
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
    return isAdjacent(target->name, overWaterAllowed);
}

bool Player::isAdjacent(string target, bool overWaterAllowed){
    typedef pair<Vertex*, bool> Edge;

    Vertices::iterator it = countries->begin();
    set<string> visited;

    while(it != countries->end()) {

        if (it->second->name == target)
            return true;

        if (visited.find(it->first) == visited.end())
            for(Edge& e : it->second->edges)
                if (e.first->name == target && (!e.second || (e.second && overWaterAllowed))) 
                    return true;

        visited.insert(it->first);
        ++it;
    }

    return false;
}

int Player::getArmiesOnCountry(Vertex* country) {
    if(countries->find(country->vertexKey) != countries->end()) {
        int numArmies = country->armies.find(*name)->second;
        string army = numArmies == 1 ? "army" : "armies";
        cout << "[ " << *name << " ] Has " << numArmies << " " << army << " on country < " << country->name << " >." << endl;
        return numArmies;
    }
    cout << "[ " << *name << " ] Has ZERO armies on country < " << country->name << " >." << endl;
    return 0;
}

int Player::getCitiesOnCountry(Vertex* country){
    if(countries->find(country->vertexKey) != countries->end()) {
        int numCities = country->cities.find(*name)->second;
        string city = numCities == 1 ? "city" : "cities";
        cout << "[ " << *name << " ] Has " << numCities << " " << city << " on country < " << country->name << " >." << endl;
        return numCities;
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
                 << " armies from < " << start->name << " > to < " << end->name << " >." << endl;
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

bool Player::destroyArmy(Vertex* country, Player* opponent){

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

void Player::addCardToHand(Card* card) {
    hand->push_back(card);
}


void Player::addCountry(Vertex* country){
    Vertex* v;
    v = country;
    countries->insert(pair<string, Vertex*> (country->vertexKey, v));
    cout << "[ " << *name << " ] " << "Added country < " << country->name << " > to player's countries." << endl;
}

void Player::removeCountry(Vertex* country) {
    if(countries->find(country->vertexKey) != countries->end()) {
        int numArmies = 0, numCities = 0;

        if (country->armies.find(*name) != country->armies.end())
            numArmies = country->armies.find(*name)->second;
        if (country->cities.find(*name) != country->cities.end())
            numCities = country->cities.find(*name)->second;

        if (numArmies == 0 && numCities == 0) {
            country->armies.erase(*name);
            country->cities.erase(*name);
            countries->erase(country->vertexKey);
            cout << "[ "<< *name <<" ] " << "Removed < " << country->name << " >." << endl;
        } else
            cout << "[ "<< *name <<" ] " << "Can't remove < " << country->name << " >. The player still owns "
                << numArmies << " armies and " << numCities << " cities on it." << endl;
    } else {
        cout << "[ " << *name << " ] " << "Doesn't have the country < " << country->name << " > available to remove." << endl;
    }
}


// Private
void Player::addArmiesToCountry(Vertex* country, int numArmies) {
    if(countries->find(country->vertexKey) == countries->end())
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

    // erase current record to be updated
    country->armies.erase(*name);

    // only update army count if there are still armies left on country
    if (currentArmies - numArmies > 0)
        country->armies.insert(pair<string, int> (*name, currentArmies - numArmies));
    // remove country if resulting num armies is 0 and no cities exist
    else if (numArmies == currentArmies && country->cities.find(*name) == country->cities.end()) 
        removeCountry(country);
}

string Player::getName(){return *name;}

Vertices* Player::getCountries(){return countries;}

int Player::getAvailableArmies(){return *armies;}

int Player::getAvailableCities(){return *cities;}

int Player::getCoins(){return *coins;}

vector<Card*>* Player::getHand(){return hand;}

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
    cout << "\n[ " << *name << " ] Occupied Countries:" << endl;
    Vertices::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        int numArmies = 0;
        int numCities = 0;
        if (it->second->armies.find(*name) != it->second->armies.end())
            numArmies = it->second->armies.find(*name)->second;
        if (it->second->cities.find(*name) != it->second->cities.end())
            numCities = it->second->cities.find(*name)->second;

        printf("\t%-15s ARMIES: %-5d CITIES: %-5d\n", it->second->name.c_str(), numArmies, numCities);
    }
}