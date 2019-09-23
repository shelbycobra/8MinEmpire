#include "Player.h"
#include "Map.h"

Player::Player(const string& playerName, int startCoins){
    *name = playerName;
    *armies = 14;
    *cities = 3;
    *coins = startCoins;
    *countries = set<string>();
    *hand = Hand();
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

bool Player::PayCoin(int amount){
    if (amount <= *coins) {
        *coins -= amount;
        cout << "[ "<< name <<" ] paid " << amount << " coins." << endl;
        return true;
    }
    cout << "[ "<< name <<" ] doesn't have enough coins to pay " << amount << "." << endl;
    return false;
}

bool Player::PlaceNewArmies(int newArmies, Vertex* country, const string& start){
    //Note: player has already chosen which armies and where to place
    //Assumes country is a valid country on the map

    if (country->name == start || country->cities.find(*name) != country->cities.end()){
        if (newArmies > *armies) {
            cout << "[ "<< name <<" ] doesn't have enough armies to place " 
                 << newArmies << " new armies." << endl;
            return false;
        }

        *armies -= newArmies;
        addArmiesToCountry(country, newArmies);
        addCountry(country);
        cout << "[ "<< name <<" ] added " << newArmies << " new armies to "
             << country->name << "." << endl;
        return true;

    }
    cout << "[ "<< name <<" ] chose an invalid country. It must be either START or contain one of the player's cities." << endl;
    return false;
}

bool Player::isAdjacent(Vertex* country, bool overWaterAllowed){
    //will always start from START
    return true;
}

bool Player::MoveArmies(int numArmies, Vertex* start, Vertex* end){
    //Assumes: country is a valid vertex on the map
    //is country an adjacent country to an occupied country?
    if (isAdjacent(end, true)) {
        //does start even have armies on it?
        if (start->armies.find(*name) != start->armies.end()) {
            int currentArmies = start->armies.find(*name)->second;
            if (numArmies > currentArmies) {
                cout << "[ "<< name <<" ] doesn't have enough armies on " << start->name
                     << " to move." << endl;
                return false;
            }
            addArmiesToCountry(end, numArmies);
            removeArmiesFromCountry(start, numArmies);

            cout << "[ "<< name <<" ] moved " << numArmies 
                 << " from " << start->name << " to " << end->name << "." << endl;
            return true;
        }

        cout << "[ "<< name <<" ] doesn't have any armies on " << start->name << " to move." << endl;
        return false;
    }

    cout << "[ "<< name <<" ] " << end->name << " is not an adjacent country." << endl;
    return false;
}

bool Player::MoveOverLand(int numArmies, Vertex* start, Vertex* end){
    //is country adjacent to occupied country? (excluding water edges)
    //yes 
        // is numArmies <= current # armies?
            //yes -- add to country
            //no return false
    //no - return false
}

bool Player::BuildCity(Vertex* country){
    //does country contain at least one army?
    //yes
        // is numcities >0?``
            //yes -- add city
        // no return false
    //no return false
}

bool Player::DestroyArmy(Vertex* country){
    //does country contain an army to destroy?
    // yes -- remove one army and increase num avail armies
    //no return false
}

string Player::getName(){return *name;}

set<string>* Player::getCountries(){return countries;}

int Player::getArmies(){return *armies;}

int Player::getCities(){return *cities;}

int Player::getCoins(){return *coins;}

Hand* Player::getHand(){return hand;}

void Player::addCountry(Vertex* country){
    countries->insert(country->name);
}

void Player::removeCountry(Vertex* country) {
    countries->erase(country->name);
}

void Player::addArmiesToCountry(Vertex* country, int numArmies) {
    if (country->armies.find(*name) == country->armies.end()) {
        country->armies.insert(pair<string, int> (*name, numArmies));
    } else {
        int currentArmies = country->armies.find(*name)->second;
        country->armies.insert(pair<string, int> (*name, currentArmies + numArmies));
    }
}

void Player::removeArmiesFromCountry(Vertex* country, int numArmies) {
    int currentArmies = country->armies.find(*name)->second;

    if (numArmies == currentArmies && country->cities.find(*name) == country->cities.end())
        removeCountry(country);

    country->armies.insert(pair<string, int> (*name, currentArmies - numArmies));
}