#include "Player.h"
#include "Map.h"

Player::Player(const string& name, int cubes, int disks, int coins){
    *name = name;
    *cubes = cubes;
    *disks = disks;
    *coins = coins;
    *countries = new unordered_map<string, Vertex*>();
    *hand = new Hand();
}

Player::~Player(){
    delete name;
    delete countries;
    delete cubes;
    delete disks;
    delete coins;
    delete hand;

    name = NULL;
    countries = NULL;
    cubes = NULL;
    disks = NULL;
    coins = NULL;
    hand = NULL;
}

void Player::PayCoin(){}

void Player::PlaceNewArmies(){}

void Player::MoveArmies(){}

void Player::MoveOverLand(){}

void Player::BuildCity(){}

void Player::DestroyArmy(){}

string Player::getName(){return *name;}
set<Vertex>* Player::getCountries(){return countries;}
int Player::getCubes(){return *cubes;}
int Player::getDisks(){return *disks;}
int Player::getCoins(){return *coins;}
Hand* Player::getHand(){return hand;}

void Player::addCountry(Vertex* country){
    countries->insert(Country (country->name, country));
}