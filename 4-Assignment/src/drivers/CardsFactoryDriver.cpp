#include "../CardsFactory.h"
#include <assert.h>

int main() {

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: CardsFactory creates a card of each type." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    Card* card;

    string action1 = "Move 4 armies";
    card = CardsFactory::create(0, STONE + " " + STONE, action1);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(STONE == card->getType() && STONE + " " + STONE == card->getGood() && action1 == card->getAction());

    delete card;

    string action2 = "Add 2 armies";
    card = CardsFactory::create(0, IRON, action2);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(IRON == card->getType() && IRON == card->getGood() && action2 == card->getAction());

    delete card;

    string action3 = "Destroy an army";
    card = CardsFactory::create(0, WOOD, action3);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(WOOD == card->getType() && WOOD == card->getGood() && action3 == card->getAction());

    delete card;

    string action4 = "Build a city";
    card = CardsFactory::create(0, CARROT, action4);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(CARROT == card->getType() && CARROT == card->getGood() && action4 == card->getAction());

    delete card;

    string action5 = "Move 4 armies OR Destroy an army";
    card = CardsFactory::create(0, GEM, action5);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(GEM == card->getType() && GEM == card->getGood() && action5 == card->getAction());

    delete card;

    string action6 = "Move 2 armies over water";
    card = CardsFactory::create(0, WILD, action6);
    cout << "Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
    assert(WILD == card->getType() && WILD == card->getGood() && action6 == card->getAction());

    delete card;

    string action7 = "Move 2 armies over water AND Build a city";
    card = CardsFactory::create(0, "sjfkdlsfdkl", action7);
    cout << "Card is NULLPTR " << card << endl;
    assert(card == 0);

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Deck contains 42 cards and each card has a good and an action." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete card;

    Deck deck;
    queue<pair<int,Card*>>* cardDeck = deck.getDeck();

    int count = 0;
    while(!cardDeck->empty()) {
        card = cardDeck->front().second;
        assert(card->getGood() != "" && card->getAction() != "");
        cout << ++count << ": Card Type = " << card->getType() << " { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
        cardDeck->pop();
        delete card;
    }

    assert(count == 42);

    return 0;
}