#include "Cards.h"

#include <stdio.h>
#include <set>
#include <map>
#include <stdlib.h>
#include <time.h>

using namespace std;

int generateRandomInt(set<int>* nums);

Deck::Deck(){
    string resources[] = {"GEM", "IRON", "STONE", "WOOD", "CARROT"};
    string actions[] = {"Add 3 armies", "Add 2 armies", "Add 4 armies", "Move 3 armies", "Move 5 armies",
    "Move 4 armies", "Move 5 armies over water", "Move 4 armies over water", "Build city", "Destroy army"};

    int numResources = 5;
    int numActions = 10;

    int resourceIndex = 0;
    int actionIndex = 0;
    int count = 0;
    int joinedCard = 0;

    map<int, Card*>* cards = new map<int,Card*>();

    for(int i = 0; i < 42; i++) {

        string good = resources[resourceIndex];
        string action = actions[actionIndex];

        // if count divisible by 14, make OR with actionIndex and 8 or 9
        if (count % 7 == 0) {
            if (actionIndex == 7 + joinedCard) { // prevents the same action being or'd
                action = actions[actionIndex-2] + " OR " + actions[8 + joinedCard];
            } else
                action = actions[actionIndex] + " OR " + actions[8 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        // if count divisible by 10, make AND actionIndex and 7 or 8
        if (count % 10 == 0) {
            if (actionIndex == 7 + joinedCard) { // prevents the same action being and'd
                action = actions[actionIndex-2] + " AND " + actions[8 + joinedCard];
            } else
                action = actions[actionIndex] + " AND " + actions[8 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        Card* newCard = new Card(count, good, action);
        cards->insert(pair<int, Card*> (count, newCard));

        actionIndex = (actionIndex + 1) % numActions;
        resourceIndex = (resourceIndex + 1) % numResources;
        count++;
    }

    set<int>* nums = new set<int>();
    cardDeck = new queue<Card*>();

    while(nums->size() < 42) {
        int rand = generateRandomInt(nums);
        Card* card = cards->find(rand)->second;
        // cout << "[ DECK ] Card ID: " << card->id << ", Good: " << card->good << ", Action: \"" << card->action << "\"" << endl;

        cardDeck->push(card);
    }

}

Deck::~Deck(){
    delete cardDeck;
    cardDeck = NULL;
}

Card* Deck::draw(){
    Card* card = cardDeck->front();
    cardDeck->pop();
    cout << "[ DECK ] Drew card { " << card->good << " : \"" << card->action << "\" } from the deck." << endl;
    return card;
}

queue<Card*>* Deck::getDeck(){
    return cardDeck;
}

Hand::Hand() {
    hand = new vector<Card*>();
    deck = new Deck();

    //Populate game hand
    for(int i = 0; i < 6; i++) {
        hand->push_back(deck->draw());
    }
}

Hand::~Hand(){
    delete hand;
    hand = NULL;
}

Card* Hand::exchange(int position){

    // int values[] = {0, 1, 1, 2, 2, 3};
    // int cardValue = values[position];

    Card* card = hand->at(position);

    vector<Card*>::iterator it;
    for(it = hand->begin(); it != hand->end(); ++it) {
        if (*it == card) {
            cout << "[ GAME HAND ] Removed card { " << (*it)->good << " : \"" << (*it)->action << "\" } from game hand."<< endl;
            hand->erase(it);
            break;
        }
    }

    hand->push_back(deck->draw());

    return card;
}

void Hand::printHand() {
    cout << "\n[ GAME HAND ] C U R R E N T   H A N D" << endl;
    int values[] = {0, 1, 1, 2, 2, 3};
    int count= 0;
    for(Card* c : *hand) {
        printf("%d [ %d ] Card ID: %-5d Good: %-10s Action: %s\n", count+1, values[count], c->id, c->good.c_str(), c->action.c_str());
        count++;
    }
    cout << endl;
}

vector<Card*>* Hand::getHand(){return hand;}

int generateRandomInt(set<int>* nums){
    srand (time(NULL));

    while (true){
        int num = rand() % 42;
        if(nums->find(num) == nums->end()) {
            nums->insert(num);
            return num;
        }
    }
}
