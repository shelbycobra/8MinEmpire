#include "Cards.h"
#include "MapUtil.h"
#include "Map.h"

typedef unordered_map<string, Player*> Players;

int main() {

    GameMap* map = generateValidMap();

    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    // Deck* deckObj = new Deck();
    Hand* gameHand = new Hand();
    gameHand->printHand();

    string name1 = "Player 1", name2 = "Player 2", name3 = "Player 3";

    Players* players = new Players();
    Player* player1 = new Player(name1, 11);
    Player* player2 = new Player(name2, 11);
    Player* player3 = new Player(name3, 11);

    players->insert(pair<string, Player*>(name1, player1));
    players->insert(pair<string, Player*>(name2, player2));
    players->insert(pair<string, Player*>(name3, player3));

    player1->placeNewArmies(3, startVertex, startName);
    player2->placeNewArmies(3, startVertex, startName);
    player3->placeNewArmies(3, startVertex, startName);

    int values[] = {0, 1, 1, 2, 2, 3};
    //Player chooses card at position 5
    int position = 5;
    Card* card = gameHand->getHand()->at(position);

    if (player1->payCoins(values[position])) {
        Card* card = gameHand->exchange(5);
        player1->addCardToHand(card);
        bool result = performCardAction(player1, card->action, map, players);
    }
}