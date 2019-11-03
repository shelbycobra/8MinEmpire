#include "Cards.h"
#include "util/MapUtil.h"

#include <time.h>
#define DECK_SIZE 42

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
Card::Card(const int& theId, const string& theGood, const string& theAction):
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

/**
 * Assignment operator
 */
Card& Card::operator =(Card& card){
    id = new int(card.getID());
    good = new string(card.getGood());
    action = new string(card.getAction());
    return *this;
}

/**
 * Destructor
 */
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

    typedef pair<string, string>  CardInfo;

    CardInfo cardInfo[] = {
        CardInfo(WOOD,   "Add 2 armies OR Move 3 armies"),
        CardInfo(WOOD,   "Destroy an army OR Build a city"),
        CardInfo(WOOD,   "Add 3 armies"),
        CardInfo(WOOD,   "Move 6 armies"),
        CardInfo(WOOD,   "Move 4 armies over water"),
        CardInfo(WOOD,   "Build a city"),
        CardInfo(WOOD,   "Move 3 armies over water"),
        CardInfo(WOOD,   "Move 3 armies"),
        CardInfo(IRON,   "Add 3 armies"),
        CardInfo(IRON,   "Add 3 armies"),
        CardInfo(IRON,   "Move 3 armies over water"),
        CardInfo(IRON,   "Build a city"),
        CardInfo(IRON,   "Add 3 armies OR Move 3 armies"),
        CardInfo(IRON,   "Move 4 armies"),
        CardInfo(IRON,   "Add 3 armies OR Move 4 armies"),
        CardInfo(IRON,   "Move 5 armies"),
        CardInfo(IRON + " " + IRON, "Move 4 armies"),
        CardInfo(CARROT, "Build a city"),
        CardInfo(CARROT, "Move 4 armies"),
        CardInfo(CARROT, "Add 4 armies OR Move 2 armies"),
        CardInfo(CARROT, "Move 3 armies over water"),
        CardInfo(CARROT, "Add 3 armies"),
        CardInfo(CARROT, "Move 5 armies"),
        CardInfo(CARROT, "Move 4 armies"),
        CardInfo(CARROT, "Add 3 armies"),
        CardInfo(CARROT, "Build a city"),
        CardInfo(CARROT, "Destroy an army AND Add 1 army"),
        CardInfo(GEM,    "Add 2 armies"),
        CardInfo(GEM,    "Add 2 armies"),
        CardInfo(GEM,    "Add 2 armies"),
        CardInfo(GEM,    "Add 1 army"),
        CardInfo(GEM,    "Move 2 armies"),
        CardInfo(STONE,  "Add 3 armies"),
        CardInfo(STONE,  "Move 2 armies over water"),
        CardInfo(STONE,  "Move 2 armies over water"),
        CardInfo(STONE,  "Move 3 armies over water"),
        CardInfo(STONE,  "Move 2 armies"),
        CardInfo(STONE,  "Add 2 armies OR Build a city"),
        CardInfo(STONE,  "Add 3 armies"),
        CardInfo(WILD,   "Move 2 armies over water"),
        CardInfo(WILD,   "Move 2 armies over water"),
        CardInfo(WILD,   "Add 2 armies")
    };

    cardDeck = new queue<pair<int, Card*>>();
    cardMap = new map<int,Card*>();

    for(int i = 0; i < 42; i++) {

        string good = cardInfo[i].first;
        string action = cardInfo[i].second;

        Card* newCard = new Card(i+1, good, action);
        pair<int, Card*> cardEntry(i+1, newCard);

        cardMap->insert(cardEntry);
        cardDeck->push(cardEntry);
    }
}

/**
 * Copy Constructor
 */
Deck::Deck(Deck* deck) {
    cardDeck = new queue<pair<int, Card*>>(*deck->getDeck());
}

/**
 * Assignment operator
 */
Deck& Deck::operator =(Deck& deck) {
    cardDeck = new queue<pair<int, Card*>>(*deck.getDeck());
    return *this;
}

/**
 * Destructor
 */
Deck::~Deck(){
    //Delete all the cards left over in the deck.
    while(!cardDeck->empty()){
        delete cardDeck->front().second;
        cardDeck->pop();
    }

    delete cardDeck;
    delete cardMap;

    cardDeck = nullptr;
    cardMap = nullptr;

}

/**
 * Shuffles the deck.
 */
void Deck::shuffle() {
    cout << "\n[ DECK ] Shuffling deck.\n" << endl;

    delete cardDeck;

    cardDeck = new queue<pair<int, Card*>>();
    set<int>* nums = new set<int>();

    while(nums->size() < DECK_SIZE) {
        int rand = generateRandomInt(nums);
        if (cardMap->find(rand) != cardMap->end()) {
            pair<int, Card*> cardEntry = *cardMap->find(rand);
            cardDeck->push(cardEntry);
        }
    }
}

/**
 * Removes a Cards from the top of the Deck.
 *
 * @return A Card pointer to the removed card.
 */
Card* Deck::draw(){
    pair<int, Card*> cardEntry = cardDeck->front();
    cardDeck->pop();

    cardMap->erase(cardEntry.first);

    cout << "[ DECK ] Drew card { " << cardEntry.second->getGood() << " : \"" << cardEntry.second->getAction() << "\" } from the deck." << endl;
    return cardEntry.second;
}

//PRIVATE
/**
 * Generates a random integer. Used for shuffling card order.
 */
int Deck::generateRandomInt(set<int>* nums){
    srand (time(0));

    while (true){
        int num = rand() % DECK_SIZE;
        if(nums->find(num) == nums->end()) {
            nums->insert(num);
            return num;
        }
    }
}

/**
 * Default Constructor
 */
Hand::Hand(): hand(new vector<Card*>()), deck(new Deck()) {}

/**
 * Copy Constructor
 */
Hand::Hand(Hand* otherGameHand) {
    hand = new vector<Card*>(*otherGameHand->getHand());
    deck = new Deck(otherGameHand->getDeck());
}

/**
 * Assignment operator
 */
Hand& Hand::operator=(Hand& otherGameHand) {
    hand = new vector<Card*>(*otherGameHand.getHand());
    deck = new Deck(otherGameHand.getDeck());

    return *this;
}

/**
 * Destructor
 */
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
 * Shuffles the Deck and draws 6 cards from the top
 * of the Deck and adds them to the hand.
 */
void Hand::fill() {

    if(hand->size() != 6) {
        deck->shuffle();

        //Populate game hand
        for(int i = 0; i < 6; i++) {
            hand->push_back(deck->draw());
        }
    } else {
        cout << "[ ERROR! ] Hand has already been populated." << endl;
    }
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
Card* Hand::exchange(Player* player){
    int position;
    int values[] = {0, 1, 1, 2, 2, 3};

    while(true) {
        position = selectCardPosition(player);
        if (player->PayCoins(values[position])) {

            Card* card = hand->at(position);
			player->addCardToHand(card);

            vector<Card*>::iterator it;
            for(it = hand->begin(); it != hand->end(); ++it) {
                if (*it == card) {
                    cout << "[ GAME HAND ] Removed card { " << (*it)->getGood() << " : \"" << (*it)->getAction() << "\" } from game hand.\n"<< endl;
                    hand->erase(it);
                    break;
                }
            }

            return card;
        }
    }
}

void Hand::drawCardFromDeck() {
    Card* card = deck->draw();
    cout << "\n[ GAME HAND ] Drew card { " << card->getGood() << " : \"" << card->getAction() << "\" } from deck." << endl;
    cout << "[ GAME HAND ] Adding it to the right side of the game hand.\n" << endl;
    hand->push_back(card);
}

//PRIVATE
/**
 * Prompts the current player to select the position in the Hand of the
 * desired card.
 *
 * @return The position of the card to be used in gameplay.
 */
int Hand::selectCardPosition(Player* player){
    string pos;
    int position;

    printHand();

    while (true) {
        cout << "[ GAME HAND ] Please choose a card from the game hand. { Purse = " << player->getCoins() << " } { Cards in hand " << player->getHand()->size() << " }." << endl;
        cout << "[ GAME HAND ] > ";

        try {
        getline(cin, pos);

        position = stoi(pos);

        if (position < 7 && position > 0)
            return position - 1;
        cout << "[ ERROR! ] You can only choose numbers between [1, 6]. Please try again." << endl;

        } catch (invalid_argument& e) {
            cout << "[ ERROR! ] You entered garbage. Please try again." << endl;
        }
    }
}

/**
 * Prints the cards in the hand "face up".
 */
void Hand::printHand() {
    cout << "\n[ GAME HAND ] C U R R E N T   H A N D" << endl;
    cout << "---------------------------------------------------------------------" << endl;

    int values[] = {0, 1, 1, 2, 2, 3};
    int count= 0;
    for(Card* c : *hand) {
        printf("%d [ %d ] Card ID: %-5d Good: %-10s Action: %s\n", count+1, values[count], c->getID(), c->getGood().c_str(), c->getAction().c_str());
        count++;
    }

    cout << "---------------------------------------------------------------------" << endl;
    cout << endl;
}