#include "../Cards.h"
#include "../util/MapUtil.h"
#include "../util/TestUtil.h"
#include "../Map.h"
#include "../MapLoader.h"

int main() {
    MapLoader loader("../maps/gotmap/got.map");
    GameMap* map = loader.generateMap();
    Hand* gameHand = new Hand();
    Players players = createDummyPlayers(3);
    queue<Player*> nextTurn;

    //Set start vertex to "Crownlands" on the map
    string startName = "CL";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    //Add 3 armies from each player to the start vertex
    cout << "\n[ GAME ] Each player adds 3 armies to the start vertex.\n" << endl;

    for (pair<string, Player*> player: players) {
        player.second->placeNewArmies(3, startVertex, startName);
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

        Card currentCard = gameHand->exchange(currentPlayer);
        performCardAction(currentPlayer, currentCard.getAction(), map, &players);
        
        count++;

        cout << "CONTINUE?" << endl;
        cout << "'y' or 'Y' to continue > ";
        getline(cin, cont);
    }

    // Deallocate heap memory
    delete gameHand;
    delete map;
    delete startVertex;
    for (pair<string, Player*> player: players) {
        delete player.second;
    }

    //Set pointers to 0 (NULL)
    gameHand = 0;
    map = 0;
    startVertex = 0;

    while(!nextTurn.empty()){
        // player objects already deleted above.
        nextTurn.front() = 0;
        nextTurn.pop();
    }
}
