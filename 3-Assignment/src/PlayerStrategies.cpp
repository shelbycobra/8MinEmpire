#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"

Strategy::Strategy(): type(new string("none")) {}

Strategy::Strategy(const string& type): type(new string(type)) {}


Strategy::~Strategy() {
    delete type;
}

GreedyStrategy::GreedyStrategy(): Strategy(GREEDY) {}

GreedyStrategy::~GreedyStrategy() {
}

void GreedyStrategy::execute(Card* card, Players* players) {

}

ModerateStrategy::ModerateStrategy(): Strategy(MODERATE) {}

ModerateStrategy::~ModerateStrategy() {
}

void ModerateStrategy::execute(Card* card, Players* players) {

}

HumanStrategy::HumanStrategy(): Strategy(HUMAN) {}

HumanStrategy::~HumanStrategy() {
}

void HumanStrategy::execute(Card*, Players*) {

}