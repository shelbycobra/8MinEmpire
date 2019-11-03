#include "../Cards.h"
#include "../util/MapUtil.h"
#include "../util/TestUtil.h"
#include "../Map.h"
#include "../MapLoader.h"
#include <cassert>

void test_DeckObject();
void test_HandObject();
void test_FullGame();

int main() {
    test_DeckObject();
    test_HandObject();
    test_FullGame();

    return 0;
}

void test_DeckObject() {
    cout << "\n\n========================================================" << endl;
    cout << "TEST: test_DeckObject" << endl;
    cout << "======================================================================\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Deck contains 42 cards and each card has a good and an action." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    Deck deck;
    queue<pair<int,Card*>>* cardDeck = deck.getDeck();

    int count = 0;
    Card* card;
    while(!cardDeck->empty()) {
        card = cardDeck->front().second;
        assert(card->getGood() != "" && card->getAction() != "");
        cout << ++count << ": { " << card->getGood() << " : \"" << card->getAction() << "\" }" << endl;
        cardDeck->pop();
        delete card;
    }

    assert(count == 42);
    cout << "\nDeck contains 42 cards!\n" << endl;
}

void test_HandObject() {
    cout << "\n\n========================================================" << endl;
    cout << "TEST: test_HandObject" << endl;
    cout << "======================================================================\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Hand object creates deck and fills hand with cards from the deck." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    Hand* gameHand = new Hand();
    gameHand->fill();
    gameHand->fill();
    string name = "Player 1";
    Player player(name, 9);

    Card currentCard = gameHand->exchange(&player);

    delete gameHand;
    gameHand = nullptr;
}

void test_FullGame() {
    cout << "\n\n========================================================" << endl;
    cout << "TEST: test_CardActions" << endl;
    cout << "======================================================================\n" << endl;

    MapLoader loader("largeValid.map");
    GameMap* map = loader.generateMap();
    Hand* gameHand = new Hand();
    gameHand->fill();
    Players players = createDummyPlayers(3);
    queue<Player*> nextTurn;

    //Set start vertex
    string startName = "CL";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    //Add 3 armies from each player to the start vertex
    cout << "\n[ GAME ] Each player adds 3 armies to the start vertex.\n" << endl;

    for (pair<string, Player*> player: players) {
        player.second->executeAddArmies(3, startVertex, startName);
        nextTurn.push(player.second);
    }

    //Cycle through the nextTurn queue 6 times (2 rounds per player for demo purposes) or until the user stops execution,
    //either by entering any input other than 'y/Y' when asked "CONTINUE?", or by killing the process.

    string cont = "y";
    int count = 1;

    while(count <= 6 && (cont == "y" || cont == "Y")) {
        //Get next player, remove from front of queue and place in back.
        Player* currentPlayer = nextTurn.front();
        nextTurn.pop();
        nextTurn.push(currentPlayer);

        cout << "\n\n\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;

        Card* currentCard = gameHand->exchange(currentPlayer);
        performCardAction(currentPlayer, currentCard->getAction(), map, &players);
        gameHand->drawCardFromDeck();

        count++;

        cout << "CONTINUE DEMO?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
    }

    // Deallocate heap memory
    delete gameHand;
    delete map;
    for (pair<string, Player*> player: players) {
        delete player.second;
    }

    //Set pointers to 0 (NULL)
    gameHand = nullptr;
    map = nullptr;
    startVertex = nullptr;
}