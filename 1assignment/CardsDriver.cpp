#include "Cards.h"
#include "util/MapUtil.h"
#include "util/TestUtil.h"
#include "Map.h"

typedef unordered_map<string, Player*> Players;

void test_PlayerChoosesBuildCityCard();
void test_PlayerChoosesMoveArmiesOverLandCard();
void test_PlayerChoosesMoveArmiesOverWaterCard();
void test_PlayerChoosesAddArmiesCard();
void test_PlayerChoosesDestroyArmyCard();
void test_PlayerChoosesAnANDCard();
void test_PlayerChoosesAnORCard();

void test_DeckObject();

int main() {
    // test_PlayerChoosesBuildCityCard();
    // test_PlayerChoosesMoveArmiesOverLandCard();
    // test_PlayerChoosesMoveArmiesOverWaterCard();
    // test_PlayerChoosesAddArmiesCard();
    // test_PlayerChoosesDestroyArmyCard();
    // test_PlayerChoosesAnANDCard();
    // test_PlayerChoosesAnORCard();
    // test_DeckObject();


    GameMap* map = generateValidMap();

    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    // string name = "Player", name1 = "Player 1";
    // Player* player = new Player(name, 0);
     
    Hand* gameHand = new Hand();

    string name1 = "Player 1", name2 = "Player 2";

    Players* players = new Players();
    Player* player1 = new Player(name1, 11);
    Player* player2 = new Player(name2, 11);

    players->insert(pair<string, Player*>(name1, player1));
    players->insert(pair<string, Player*>(name2, player2));

    player1->placeNewArmies(3, startVertex, startName);
    player2->placeNewArmies(3, startVertex, startName);

    int values[] = {0, 1, 1, 2, 2, 3};

                int position;
    
    while(true) {
        position = selectPositionOfCardFromGameHand(gameHand);

        if (player1->payCoins(values[position])) {
            Card* card = gameHand->exchange(position);
            player1->addCardToHand(card);
            performCardAction(player1, card->action, map, players);
            break;
        }
    }  

}

void test_PlayerChoosesBuildCityCard(){


    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesBuildCityCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player builds a city on START.\n" << endl;
    cout << "\nTEST: Player fails to build a city on an invalid country.\n" << endl;
    cout << "\nTEST: Player builds a city on a valid country.\n" << endl;
}

void test_PlayerChoosesMoveArmiesOverLandCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesMoveArmiesOverLandCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player moves all armies from START to \"P\".\n" << endl;
    cout << "\nTEST: Player moves armies from START to \"P\", then \"P\" to \"Q\".\n" << endl;
    cout << "\nTEST: Player fails to move more armies that they have.\n" << endl;
    cout << "\nTEST: Player fails to move armies over water.\n" << endl;
}

void test_PlayerChoosesMoveArmiesOverWaterCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesMoveArmiesOverWaterCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player moves all armies from START to \"P\".\n" << endl;
    cout << "\nTEST: Player moves armies from START to \"P\", then \"P\" to \"Q\".\n" << endl;
    cout << "\nTEST: Player fails to move more armies that they have.\n" << endl;
    cout << "\nTEST: Player moves armies over water from START to \"E\".\n" << endl;
}

void test_PlayerChoosesAddArmiesCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesAddArmiesCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player adds all armies to START.\n" << endl;
    cout << "\nTEST: Player add some armies to START and some to \"P\".\n" << endl;
    cout << "\nTEST: Player fails add armies to country without a city.\n" << endl;
    cout << "\nTEST: Player adds all armies to \"P\".\n" << endl;
}

void test_PlayerChoosesDestroyArmyCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesDestroyArmyCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player destoys an opponent's army.\n" << endl;
    cout << "\nTEST: Player fails to destroy own army.\n" << endl;
    cout << "\nTEST: Player fails to destroy an opponent who doesn't exist.\n" << endl;
    cout << "\nTEST: Player fails to destroy an opponent's army if no armies exist on country.\n" << endl;
}

void test_PlayerChoosesAnANDCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesAnANDCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player performs \"Move 4 armies\" AND \"Destroy an army\".\n" << endl;
    cout << "\nTEST: Player performs \"Add 3 armies\" AND \"Build a city\".\n" << endl;
}

void test_PlayerChoosesAnORCard(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerChoosesAnORCard" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player is unable to pay for card.\n" << endl;
    cout << "\nTEST: Player chooses between \"Move 4 armies\" OR \"Destroy an army\".\n" << endl;
    cout << "\nTEST: Player chooses between \"Add 3 armies\" OR \"Build a city\".\n" << endl;
}

void test_DeckObject(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_DeckObject" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Deck contains 42 cards upon initialization.\n" << endl;
    cout << "\nTEST: No two Deck object contain the same sequence of cards (ie, Decks are shuffled).\n" << endl;
    cout << "\nTEST: Deck draws a card from top of the deck.\n" << endl;

}
