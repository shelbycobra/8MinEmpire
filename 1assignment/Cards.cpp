#include "Cards.h"

#include <set>
#include <map>

int generateRandomInt(set<int>* nums);

Deck::Deck(){
    string resources[] = {"gem", "iron", "stone", "wood", "carrot"};
    string actions[] = {"Add 3 armies", "Add 2 armies", "Move 6 armies", "Move 5 armies",
    "Move 4 armies", "Move 5 armies over water", "Move 4 armies over water", "Build city", "Destroy army"};

    int numResources = 5;
    int numActions = 9;

    int resourceIndex = 0;
    int actionIndex = 0;
    int count = 0;
    int joinedCard = 0;

    map<int, Card*>* cards = new map<int,Card*>();

    for(int i = 0; i < 42; i++) {

        string good = resources[resourceIndex];
        string action = actions[actionIndex];

        // if count divisible by 14, make OR with actionIndex and 7 or 8
        if (count % 14 == 0) {
            action = actions[actionIndex] + " OR " + actions[7 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        // if count divisible by 10, make AND actionIndex and 7 or 8
        if (count % 10 == 0) {
            action = actions[actionIndex] + " AND " + actions[7 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        Card* newCard = new Card(count, good, action);
        cards->insert(pair<int, Card*> (count, newCard));

        actionIndex = (actionIndex + 1) % numActions;
        resourceIndex = (resourceIndex + 1) % numResources;
        count++;
    }

    set<int>* nums = new set<int>();

    while(nums->size() < 42) {
        int rand = generateRandomInt(nums);
        cardDeck->push(cards->find(rand)->second);
    }

}

Deck::~Deck(){
    delete cardDeck;
    cardDeck = NULL;
}

Card* Deck::draw(){
    Card* card = cardDeck->front();
    cardDeck->pop();
    return card;
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

void Hand::exchange(int position, Player* player){

    int values[] = {0, 1, 1, 2, 2, 3};
    int cardValue = values[position];
    bool playerCanPay = player->payCoins(cardValue);

    if (playerCanPay) {
        Card* card = hand->at(position);

        vector<Card*>::iterator it;
        for(it = hand->begin(); it != hand->end(); ++it) {
            if (*it == card) {
                hand->erase(it);
                cout << "[ GAME HAND ] Removed card { " << (*it)->good << " : \"" << (*it)->action << "\" } from game hand."<< endl;
            }
        }

        player->addCard(card);
        hand->push_back(deck->draw());
    }
}

void Hand::printHand() {
    cout << "[ GAME HAND ] C U R R E N T   H A N D" << endl;
    int values[] = {0, 1, 1, 2, 2, 3};
    int count= 0;
    for(Card* c : *hand) {
        cout << "[ " << values[count] << " ] Card ID: " << c->id << "; Good: " << c->good << "; Action: " << c->action << endl;
        count++;
    }
}

//Random number generator taken from
//https://www.learncpp.com/cpp-tutorial/59-random-number-generation/
int generateRandomInt(set<int>* nums){
    while (true){
        static unsigned int seed = 5323;
        seed = 8253729 * seed + 2396403;
        int rand = seed  % 42;
        if(nums->find(rand) == nums->end()) {
            nums->insert(rand);
            return rand;
        }
    }
}
