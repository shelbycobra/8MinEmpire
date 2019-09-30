#include "Map.h"

using namespace std;

GameMap::GameMap(): start(new string("none")) {
    vertices = new Vertices();
}

GameMap::~GameMap() {
    delete vertices;
    vertices = NULL;
}

bool GameMap::setStartVertex(string& startVertexName){
    Vertices::iterator it;

    if (*start != "none"){
        cout << "[ MAP ] Start vertex was already set to < " << *start << " >. Start vertex can only be set once." << endl;
        return false;
    }

    cout << "[ MAP ] Setting start vertex. Searching for \"" << startVertexName << "\"." << endl;

    for(it = vertices->begin(); it != vertices->end(); ++it) {
        if (startVertexName == it->first) {
            *start = it->first;
            cout << "[ MAP ] Start vertex is now < " << it->first << " >." << endl;
            return true;
        }
    }
    cout << "[ MAP ] The vertex < " << startVertexName << " > doesn't exist on the map." << endl;
    return false;
}

string* GameMap::getStartVertex(){return start;}
Vertices* GameMap::getVertices(){return vertices;}

void GameMap::addVertex(const string& name, const string& continent) {
    Vertices::iterator it = vertices->find(name);

    if (it == vertices->end()) {
        Vertex * vertex = new Vertex (name, continent);
        typedef pair<string, Vertex*> node;
        vertices->insert(node (name, vertex));
        return;
    }

    Vertex* v = vertices->find(name)->second;
    cout << "[ MAP ] Vertex " << v->name << " on continent " << v->continent << " already exists on the map.\n" << endl;
}

void GameMap::addEdge(const string& startVertex, const string& endVertex, bool isWaterEdge) {
    Vertex * v1 = vertices->find(startVertex)->second;
    Vertex * v2 = vertices->find(endVertex)->second;
    v1->addEdge(v2, isWaterEdge);
    v2->addEdge(v1, isWaterEdge);
}

bool GameMap::performCardAction(Player* player, string action, Players* players) {
    //if action contains OR -> ask player which to use
    int orPos = action.find("OR");
    if ( orPos != -1) {
        while(true) {
            cout << "[ GAME ] Which action do you want? 1 or 2 ?" << endl;
            string firstChoice = action.substr(0, orPos -1);
            string secondChoice = action.substr(orPos + 2);
            cout << "[ OPTION 1 ] " << firstChoice << endl;
            cout << "[ OPTION 2 ] " << secondChoice << endl;
            string answer;
            cin >> answer;

            if (answer == "1") {
                action = firstChoice;
                break;
            } else if (answer == "2") {
                action = secondChoice;
                break;
            } else {
                cout << "[ GAME ] Invalid choice. Please use either 1 or 2." << endl;
            }
        }

    }

    vector<string>* actionArr = split(action, ' ');

    for(vector<string>::iterator it = actionArr->begin(); it != actionArr->end(); ++it) {
        if ((*it) == "Move") {
            ++it;
            stringstream toInt(*it);
            int numArmies;
            toInt >> numArmies;

            bool overWater = action.find("water") != -1;
            actionType type = overWater ? actionType::MOVE_OVER_WATER : actionType::MOVE_OVER_LAND;

            Vertex* startVertex = chooseStartVertex(player);
            Vertex* endVertex = chooseEndVertex(player, type);

            player->moveArmies(numArmies, startVertex, endVertex, overWater);
        }
        if ((*it) ==  "Add") {
            ++it;
            stringstream toInt(*it);
            int numArmies;
            toInt >> numArmies;

            Vertex* endVertex = chooseEndVertex(player, actionType::ADD_ARMY);

            player->placeNewArmies(numArmies, endVertex);
        }
        if ((*it) ==  "Destroy") {
            Vertex* endVertex = chooseEndVertex(player, actionType::DESTROY_ARMY);

            string opponentName;
            Player* opponent;

            while (true) {
                cout << "[ GAME ] Choose an opponent." << endl;
                cout << "[ GAME ] > ";
                cin >> opponentName;
                if (players->find(opponentName) == players->end();)
                    cerr << "[ ERROR! ] Player doesn't exist. Try again." << endl;
                else {
                    opponent = players->find(opponentName);
                    break;
                }
            }

            player->destroyArmy(endVertex, opponent);

        }
        if ((*it) == "Build") {
            Vertex* endVertex = chooseEndVertex(player, actionType::DESTROY_ARMY);

            player->buildCity(endVertex);
        }
    }
    cout << "[ " << player->getName() << " ] Does action \"" << action << "\"" << endl;

    return true;
}

Vertex* GameMap::chooseStartVertex(Player* player){

    string startName;
    Vertex* startVertex;

    while(true){
        cout << "[ GAME ] You must choose a country to take armies from." << endl;
        player->printCountries();
        cin >> startName;

        if (player->getCountries()->find(startName) == player->getCountries()->end()) {
            cout << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            startVertex = player->getCountries()->find(startName)->second;
            break;
        }
    }
    return startVertex;
}

Vertex* GameMap::chooseEndVertex(Player* player, actionType type){
    string endName;
    Vertex* endVertex;

    while(true){
        cout << "[ GAME ] You must choose a country to add armies to." << endl;

        if (type == actionType::ADD_ARMY || type == actionType::BUILD_CITY) {
            player->printCountries();
        } else if (type == actionType::MOVE_OVER_LAND || type == actionType::MOVE_OVER_WATER) {
            printMap();
            cout <<"\nW---Z   A---B---E===O---P---Q"
            <<"\n \\  |   |\\  |  /     \\  |  /"
            <<"\n  \\ |   | \\ | /       \\ | /"
            <<"\n   \\|   |  \\|/         \\|/"
            <<"\nY---X===C   D           R\n" << endl;
        } else if (type == actionType::DESTROY_ARMY) {
            printMap();
            cout << "\nW---Z   A---B---E===O---P---Q"
            <<"\n \\  |   |\\  |  /     \\  |  /"
            <<"\n  \\ |   | \\ | /       \\ | /"
            <<"\n   \\|   |  \\|/         \\|/"
            <<"\nY---X===C   D           R\n" << endl;
        } else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return;
        }

        cout << "[ GAME ] > ";
        cin >> endName;
        cout << "[ GAME ] You chose < " << endName << " >." << endl;

        endVertex = vertices->find(endName)->second;

        if (((actionType::MOVE_OVER_LAND || actionType::MOVE_OVER_WATER) && !player->isAdjacent(endVertex, type == actionType::MOVE_OVER_WATER))
            || (actionType::ADD_ARMY && endVertex->cities.find(player->getName()) ==  endVertex->cities.end())
            || (actionType::BUILD_CITY && endVertex->armies.find(player->getName()) == endVertex->armies.end())
            || (actionType::DESTROY_ARMY && endVertex->armies.find(opponent) == endVertex->armies.end())) {
            cerr << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            break;
        }
    }
    return endVertex;
}

void GameMap::printMap(){

}