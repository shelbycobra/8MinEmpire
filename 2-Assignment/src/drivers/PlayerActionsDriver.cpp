#include "../GameMainLoop.h"
#include "../MapLoader.h"


int main() {
    GameMainEngine mainLoop;
    Card move4Card(1, "GEM", "Move 4 armies");
    Card orCard(2, "CARROT", "Add 2 armies OR Build city");
    Card andCard(3, "WOOD", "Move 3 armies over water AND Destroy army");
    Card buildCard(4, "GEM", "Build city");
    MapLoader loader("got.map");
    GameMap* map = loader.generateMap(); 
    Player* player1 = new Player(string("shelby"), string("RED"));
    Player* player2 = new Player(string("bob"), string("WHITE"));
    Player* player3 = new Player(string("billy"), string("GREEN"));
    Players* players = new Players();
    players->insert(pair<string, Player*>(player1->getName(), player1));
    players->insert(pair<string, Player*>(player2->getName(), player2));
    players->insert(pair<string, Player*>(player3->getName(), player3));

    string startName = "cl";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;
    player1->executeAddArmies(3, startVertex, startName);
    player2->executeAddArmies(3, startVertex, startName);
    player3->executeAddArmies(3, startVertex, startName);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Move armies over land." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    mainLoop.performCardAction(player1, move4Card.getAction(), map, players);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Or card (Add 2 armies)." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    mainLoop.performCardAction(player2, orCard.getAction(), map, players);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: And card (Move over water and destory an army)." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    mainLoop.performCardAction(player3, andCard.getAction(), map, players);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Build a city." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    mainLoop.performCardAction(player1, buildCard.getAction(), map, players);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Ignore action." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    mainLoop.performCardAction(player2, move4Card.getAction(), map, players);

    delete map;
    delete players;
    delete player1;
    delete player2;
    delete player3;

    return 0;
}