#include "Cards.h"
#include "util/MapUtil.h"

#include <stdio.h>
#include <set>
#include <map>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 * Default Constructor
 */
Card::Card():
    id(new int(0)), 
    good(new string("No good")), 
    action(new string("No action")) {}

/**
 * Initializes a Card object with an ID number, a good and an action.
 * 
 * @param theId The ID of the card.
 * @param theGood The good of the card.
 * @param theAction The action of the card.
 */
Card::Card(int theId, string theGood, string theAction): 
    id(new int(theId)), 
    good(new string(theGood)), 
    action(new string(theAction)) {}

/**
 * Copy Constructor
 */
Card::Card(Card* card) {
    id = new int(card->getID());
    good = new string(card->getGood());
    action = new string(card->getAction());
}

Card& Card::operator =(Card& card){
    id = new int(card.getID());
    good = new string(card.getGood());
    action = new string(card.getAction());
    return *this;
}

Card::~Card() {
    delete id;
    delete good;
    delete action;

    id = nullptr;
    good = nullptr;
    action = nullptr;
}

/**
 * Initliazes a Deck object that contains 42 Card objects.
 * The card order is generated randomly each time a Deck object is created,
 * meaning no two Deck object will have the same order of cards.
 */
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
        cardDeck->push(card);
    }

}

/**
 * Copy Constructor
 */
Deck::Deck(Deck* deck) {
    cardDeck = new queue<Card*>(*deck->getDeck());
}

Deck& Deck::operator =(Deck& deck) {
    cardDeck = new queue<Card*>(*deck.getDeck());
    return *this;
}

Deck::~Deck(){
    //Delete all the cards left over in the deck.
    while(!cardDeck->empty()){
        delete cardDeck->front();
        cardDeck->pop();
    }

    delete cardDeck;
    cardDeck = nullptr;
}

/**
 * Removes a Cards from the top of the Deck.
 * 
 * @return A Card pointer to the removed card.
 */
Card* Deck::draw(){
    Card* card = cardDeck->front();
    cardDeck->pop();
    cout << "[ DECK ] Drew card { " << card->getGood() << " : \"" << card->getAction() << "\" } from the deck." << endl;
    return card;
}

//PRIVATE
/**
 * Generates a random integer. Used for shuffling card order.
 */
int Deck::generateRandomInt(set<int>* nums){
    srand (time(0));

    while (true){
        int num = rand() % 42;
        if(nums->find(num) == nums->end()) {
            nums->insert(num);
            return num;
        }
    }
}

/**
 * Default Constructor
 * 
 * Each Hand object initilizes a Deck object and immediately draws 6 cards from the top
 * of the Deck and adds them to the hand.
 */
Hand::Hand(): hand(new vector<Card*>()), deck(new Deck()) {
    //Populate game hand
    for(int i = 0; i < 6; i++) {
        hand->push_back(deck->draw());
    }
}

/**
 * Copy Constructor
 */
Hand::Hand(Hand* otherGameHand) {
    hand = new vector<Card*>(*otherGameHand->getHand());
    deck = new Deck(otherGameHand->getDeck());
}

Hand& Hand::operator=(Hand& otherGameHand) {
    hand = new vector<Card*>(*otherGameHand.getHand());
    deck = new Deck(otherGameHand.getDeck());

    return *this;
}

Hand::~Hand(){
    //Delete all the cards in the game hand.
    for(vector<Card*>::iterator it = hand->begin(); it != hand->end(); ++it)
        delete *it;

    delete hand;
    delete deck;

    hand = nullptr;
    deck = nullptr;
}

/**
 * The current player is asked to choose a card from the Hand.
 * If the player has enough coins, the card will be removed from the 
 * hand and added to the Player's hand. The Player will pay the corresponding 
 * amount of coins for the card and the Hand object will draw another card from the 
 * top of the deck and place it in the back of the hand.
 * 
 * @param player A Player pointer to the current player. 
 * @return A Card pointer to the removed card.
 */
Card Hand::exchange(Player* player){
    int position;
    int values[] = {0, 1, 1, 2, 2, 3};

    while(true) {
        position = selectPositionOfCardFromGameHand();
        if (player->PayCoins(values[position])) {

            Card* card = hand->at(position);
			player->addCardToHand(card);

            vector<Card*>::iterator it;
            for(it = hand->begin(); it != hand->end(); ++it) {
                if (*it == card) {
                    cout << "[ GAME HAND ] Removed card { " << (*it)->getGood() << " : \"" << (*it)->getAction() << "\" } from game hand."<< endl;
                    hand->erase(it);
                    break;
                }
            }

            hand->push_back(deck->draw());

            return *card;
        }
    }
}

//PRIVATE
/**
 * Prompts the current player to select the position in the Hand of the
 * desired card.
 * 
 * @return The position of the card to be used in gameplay.
 */
int Hand::selectPositionOfCardFromGameHand(){
    string pos;
    int position;

    printHand();

    while (true) {
        cout << "[ GAME HAND ] Please choose a card from the game hand." << endl;
        cout << "[ GAME HAND ] > ";
        getline(cin, pos);

        stringstream toInt(pos);
        toInt >> position;

        if (position < 7 && position > 0)
            return position - 1;
        cout << "[ ERROR! ] You can only choose numbers between [1, 6]. Please try again." << endl;
    }
}

/**
 * Prints the cards in the hand "face up". 
 */
void Hand::printHand() {
    cout << "\n[ GAME HAND ] C U R R E N T   H A N D" << endl;
    int values[] = {0, 1, 1, 2, 2, 3};
    int count= 0;
    for(Card* c : *hand) {
        printf("%d [ %d ] Card ID: %-5d Good: %-10s Action: %s\n", count+1, values[count], c->getID(), c->getGood().c_str(), c->getAction().c_str());
        count++;
    }
    cout << endl;
}