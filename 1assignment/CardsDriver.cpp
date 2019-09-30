#include "Cards.h"
#include "MapUtil.h"
#include "Map.h"

int main() {

    GameMap* map = generateValidMap();

    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    // Deck* deckObj = new Deck();
    Hand* gameHand = new Hand();
    gameHand->printHand();

    string name1 = "Player 1";
    Player* player1 = new Player(name1, 11);

    player1->placeNewArmies(3, startVertex, startName);

    int values[] = {0, 1, 1, 2, 2, 3};
    //Player chooses card at position 5
    int position = 5;
    Card* card = gameHand->getHand()->at(position);

    if (player1->payCoins(values[position])) {
        Card* card = gameHand->exchange(5);
        player1->addCardToHand(card);
        bool result = performCardAction(player1, card->action, map->getVertices());
    }
}