#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include <algorithm>
#include <cstdlib>
#include <map>

typedef map<string, Vertex*> Vertices;

/**
 * Default Constructor
 */
Strategy::Strategy(): type(new string("none")) {}

/**
 * Constructor
 *
 * @param type The type of strategy.Either HUMAN, MODERATE or GREEDY.
 */
Strategy::Strategy(const string& type): type(new string(type)) {}

/**
 * Destructor
 */
Strategy::~Strategy() {
    delete type;
}

/**
 * Constructor
 */
GreedyStrategy::GreedyStrategy(): Strategy(GREEDY) {}

/**
 * Destructor
 */
GreedyStrategy::~GreedyStrategy() {
}

void GreedyStrategy::PlaceNewArmies(Player* player, const string action, Players* players) {
    int maxArmies = stoi(action.substr(4, 5));
    Vertices* playerRegions = player->getOccupiedRegions();
    Vertex* addVertex = GameMap::instance()->getStartVertex();
    PlayerEntry* entry = player->getPlayerEntry();

    cout << "\n\n[[ ACTION ]] " << action << ".\n\n" << endl;
    cout << "{ " << player->getName() << " } [ GREEDY ] has the choice of adding " << maxArmies << " armies on the board." << endl;

    for(Vertices::iterator it = playerRegions->begin(); it != playerRegions->end(); ++it) {
        // Find vertex with a city
        Vertex* vertex = it->second;
        if (vertex->getCities()->find(entry) != vertex->getCities()->end()) {
            addVertex = vertex;
            break;
        }
    }

    if (player->executeAddArmies(maxArmies, addVertex)) {
        cout << "{ " << player->getName() << " } [ GREEDY ] Chose to place " << maxArmies
        << " armies on < " << addVertex->getName() << " >." << endl;
    }

    player->printRegions();
}

void GreedyStrategy::MoveArmies(Player* player, const string action, Players* players) {
    int maxArmies = stoi(action.substr(5, 6));
    bool overWaterAllowed = action.find("water") != size_t(-1);

    string actionSuffix = overWaterAllowed ? " armies over water.\n" : " armies over land.\n";

    cout << "\n\n[[ ACTION ]] Move " << maxArmies << actionSuffix << "\n\n" << endl;
    cout << "{ " << player->getName() << " } [ GREEDY ] Can move " << maxArmies << " armies around the board." << endl;

    PlayerEntry* entry = player->getPlayerEntry();
    Vertices* vertices = player->getOccupiedRegions();

    string previousStartVertex = "";

    Vertices::reverse_iterator r = vertices->rbegin();

    while(maxArmies > 0) {
        Vertex* currentVertex = r->second;

        // Skip vertex if there are no armies on it.
        if (currentVertex->getArmies()->find(entry) == currentVertex->getArmies()->end()) {
            r++;
            continue;
        }

        vector<Edge>* edges = currentVertex->getEdges();
        vector<Edge>::iterator e = edges->begin();

        while(e != edges->end() && maxArmies > 0) {

            bool isWaterEdge = e->second;
            if((!isWaterEdge || (overWaterAllowed && isWaterEdge)) && previousStartVertex != e->first->getName()) {
                if (player->executeMoveArmies(1, currentVertex, e->first, overWaterAllowed)) {
                    maxArmies--;
                    previousStartVertex = currentVertex->getName();
                    r = vertices->rbegin();
                    break;
                }
            }
            e++;
        }
        r++;

        if(r == vertices->rend() && maxArmies > 0) {
            r = vertices->rbegin();
        }
    }

    player->printRegions();
}

void GreedyStrategy::BuildCity(Player* player) {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertices* playerRegions = player->getOccupiedRegions();
    Vertex* buildVertex = GameMap::instance()->getStartVertex();

    for(Vertices::iterator it = playerRegions->begin(); it != playerRegions->end(); ++it) {
        // Find first vertex without a city. If none, defaults to the start vertex.
        Vertex* vertex = it->second;
        PlayerEntry* entry = player->getPlayerEntry();
        if (vertex->getCities()->find(entry) == vertex->getCities()->end() && vertex != GameMap::instance()->getStartVertex()) {
            buildVertex = vertex;
            break;
        }
    }

    cout << "{ " << player->getName() << " } [ GREEDY ] Chose < " << buildVertex->getName() << " > to build a city." << endl;
    player->executeBuildCity(buildVertex);

    player->printRegions();
}

void GreedyStrategy::DestroyArmy(Player* player, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertices* vertices = player->getOccupiedRegions();

    for(Vertices::iterator it = vertices->begin(); it != vertices->end(); ++it) {
        Vertex* currentVertex = it->second;

        unordered_map<PlayerEntry*, int>* armies = currentVertex->getArmies();
        unordered_map<PlayerEntry*, int>::iterator a;

        string opponentName = "";

        for (a = armies->begin(); a != armies->end(); ++a) {
            if (a->first != player->getPlayerEntry()) {
                opponentName = a->first->first;
                break;
            }
        }

        if (opponentName != "") {
            Player* opponent = players->find(opponentName)->second;
            player->executeDestroyArmy(currentVertex, opponent);
            break;
        }
    }

}
void GreedyStrategy::AndOrAction(Player* player, const string action, Players* players) {
    vector<string> actionArr;

    if (action.find("OR") != size_t(-1)) {
        int orPos = action.find("OR");
        string firstChoice = action.substr(0, orPos - 1);
        string secondChoice = action.substr(orPos + 3);
        string chosenAction;

        if (firstChoice.find("Build") != size_t(-1) || firstChoice.find("Destroy") != size_t(-1))
            chosenAction = firstChoice;
        else
            chosenAction = secondChoice;

        actionArr.push_back(chosenAction);
    }
    else {
        size_t andPos = action.find("AND");
        actionArr.push_back(action.substr(0, andPos));
        actionArr.push_back(action.substr(andPos+4));
    }

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it).find("Move") != size_t(-1))
            MoveArmies(player, *it, players);
        else if ((*it).find("Add") != size_t(-1))
            PlaceNewArmies(player, *it, players);
        else if ((*it).find("Destroy") != size_t(-1))
            DestroyArmy(player, players);
        else if ((*it).find("Build") != size_t(-1))
            BuildCity(player);
    }
}

/**
 * A greedy player to choose a card position from the game hand.
 *
 * @param player A player pointer to the is using this strategy.
 * @param hand A pointer to the game hand (not used in human strategy).
 * @return The position of the chosen card.
 */
int GreedyStrategy::chooseCardPosition(Player* player, Hand* hand) {
    // BUILD CITY
    // Find first region without a city and not start vertex
    // Else place on start vertex.

    // DESTROY ARMY
    // Find region with only one army, else
    // Random

    vector<Card*>::iterator it = hand->getHand()->begin();
    int count = 0;

    while(it != hand->getHand()->end() && count <= player->getCoins()) {
        if ((*it)->getAction().find("Build") != size_t(-1) || (*it)->getAction().find("Destroy") != size_t(-1)) {
            cout << "{ " << player->getName() << " } [ GREEDY ] Chose position " << count << "." << endl;
            return count;
        }
        count++;
        it++;
    }

    return 0;
}

/**
 * Constructor
 */
ModerateStrategy::ModerateStrategy(): Strategy(MODERATE) {}

/**
 * Destructor
 */
ModerateStrategy::~ModerateStrategy() {
}

void ModerateStrategy::MoveArmies(Player* player, const string action, Players* players) {
    int maxArmies = stoi(action.substr(5, 6));
    bool overWaterAllowed = action.find("water") != size_t(-1);

    string actionSuffix = overWaterAllowed ? " armies over water.\n" : " armies over land.\n";

    cout << "\n\n[[ ACTION ]] Move " << maxArmies << actionSuffix << "\n\n" << endl;
    cout << "{ " << player->getName() << " } Can move " << maxArmies << " armies around the board." << endl;

    PlayerEntry* entry = player->getPlayerEntry();
    Vertices* vertices = player->getOccupiedRegions();

    string previousStartVertex = "";

    // For each occupied region, find first adjacent region that moving armies would change ownership.
    Vertices::iterator it = vertices->begin();
    while(it != vertices->end() && maxArmies > 0) {
        cout << "before currentVertex" << endl;
        Vertex* currentVertex = it->second;
        cout << "before edge " << currentVertex->getName() << endl;
        vector<Edge>* edges = currentVertex->getEdges();

        // Skip this vertex if there are no armies on it
        cout << "before checking armies" << endl;
        if (currentVertex->getArmies()->find(entry) == currentVertex->getArmies()->end()) {
            it++;
            continue;
        }

        // Cycle through all adjacent vertices
        cout << "before iterater e" << endl;
        vector<Edge>::iterator e = edges->begin();
        while(e != edges->end() && maxArmies > 0) {
            bool isWaterEdge = e->second;
            if ((!isWaterEdge || (overWaterAllowed && isWaterEdge)) && previousStartVertex != e->first->getName()) {
                Vertex* endVertex = e->first;

                // If change of ownership is possible, then execute move armies and continue to next adjacent vertex.
                if(changeOwnership(currentVertex, endVertex, player, maxArmies, players, overWaterAllowed)) {
                    previousStartVertex = currentVertex->getName();
                    cout << "MANY ARMIES" << endl;
                    it = vertices->begin();
                    break;
                }
            }
            cout << "before e++" << endl;
            e++;
        }
        it++;
    }

    // If after cycling through all adjacent vertices and there are still armies left over,
    // move one army from current vertex to every adjacent vertex until maxArmies is 0.
    cout << "before reverse i" << endl;
    Vertices::reverse_iterator r = vertices->rbegin();

    while(maxArmies > 0) {
        cout << "before currentVertex" << endl;
        Vertex* currentVertex = r->second;

        // Skip vertex if there are no armies on it.
        if (currentVertex->getArmies()->find(player->getPlayerEntry()) == currentVertex->getArmies()->end()) {
            r++;
            continue;
        }

        cout << "before one edges" << endl;
        vector<Edge>* edges = currentVertex->getEdges();
        vector<Edge>::iterator e = edges->begin();

        while(e != edges->end() && maxArmies > 0) {

            bool isWaterEdge = e->second;
            if((!isWaterEdge || (overWaterAllowed && isWaterEdge)) && previousStartVertex != e->first->getName()) {
                if (player->executeMoveArmies(1, currentVertex, e->first, overWaterAllowed)) {
                    maxArmies--;
                    previousStartVertex = currentVertex->getName();
                    cout << "ONE ARMY" << endl;
                    break;
                }
            }
            e++;
        }
        if (r->second == nullptr)
            r = vertices->rbegin();
        else
            r++;

        if(r == vertices->rend() && maxArmies > 0) {
            r = vertices->rbegin();
            cout << "RESTARTING ONE" << endl;
        }
    }

    player->printRegions();
}

bool ModerateStrategy::changeOwnership(Vertex* startVertex, Vertex* endVertex, Player* currentPlayer, int& maxArmies, Players* players, bool overWaterAllowed) {
    int playerStartArmies = startVertex->getArmies()->find(currentPlayer->getPlayerEntry())->second;
    int maxMovableArmies = playerStartArmies < maxArmies ? playerStartArmies : maxArmies;

    string ownerName = endVertex->getRegionOwner();
    int ownerEndArmies = 0;

    if (ownerName != "" && ownerName != currentPlayer->getName()) {
        Player* owner = players->find(ownerName)->second;
        ownerEndArmies = endVertex->getArmies()->find(owner->getPlayerEntry())->second;
    }

    int diff = abs(ownerEndArmies - maxMovableArmies) + 1;
    if(diff <= maxMovableArmies) {
        if (currentPlayer->executeMoveArmies(diff, startVertex, endVertex, overWaterAllowed)) {
            maxArmies -= diff;
            return true;
        }
    }
    return false;
}

void ModerateStrategy::DestroyArmy(Player* player, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertices* vertices = player->getOccupiedRegions();

    for(Vertices::iterator it = vertices->begin(); it != vertices->end(); ++it) {
        Vertex* currentVertex = it->second;

        unordered_map<PlayerEntry*, int>* armies = currentVertex->getArmies();
        unordered_map<PlayerEntry*, int>::iterator a;

        string opponentName = "";

        for (a = armies->begin(); a != armies->end(); ++a) {
            if (a->first != player->getPlayerEntry()) {
                opponentName = a->first->first;
                break;
            }
        }

        if (opponentName != "") {
            Player* opponent = players->find(opponentName)->second;
            player->executeDestroyArmy(currentVertex, opponent);
            break;
        }
    }
}

void ModerateStrategy::PlaceNewArmies(Player* player, const string action, Players* players) {
    // Find vertex with either a city or is start vertex that is not owned by player.
    // Find first vertex where number of added regions makes region owned by player,
    // Else return startVertex

    int maxArmies = stoi(action.substr(4, 5));
    Vertices* playerRegions = player->getOccupiedRegions();
    Vertex* addVertex = GameMap::instance()->getStartVertex();
    PlayerEntry* entry = player->getPlayerEntry();

    cout << "\n\n[[ ACTION ]] " << action << ".\n\n" << endl;
    cout << "{ " << player->getName() << " } has the choice of adding " << maxArmies << " armies on the board." << endl;

    while (maxArmies > 0) {
        int numArmiesToPlace = maxArmies;

        for(Vertices::iterator it = playerRegions->begin(); it != playerRegions->end(); ++it) {
            // Find vertex with a city
            Vertex* vertex = it->second;
            if (vertex->getCities()->find(entry) != vertex->getCities()->end()) {

                // Get number of armies on that vertex
                int playerArmies = 0;
                if (vertex->getArmies()->find(entry) != vertex->getArmies()->end()) {
                    playerArmies = vertex->getArmies()->find(entry)->second;
                }

                // Get owner of vertex.
                string ownerName = vertex->getRegionOwner();
                int ownerArmies = 0;

                // Get number of owner armies if the owner is not none and the owner is not the same as current player
                if (ownerName != "" && ownerName != player->getName()) {
                    Player* owner = players->find(ownerName)->second;
                    PlayerEntry* ownerEntry = owner->getPlayerEntry();
                    ownerArmies = vertex->getArmies()->find(ownerEntry)->second;

                    // If the difference between the current player's armies and the owner's armies + 1 is <= maxArmies
                    // then add the difference to that vertex
                    int numArmiesDifference = ownerArmies - playerArmies + 1;
                    if(numArmiesDifference <= maxArmies) {
                        numArmiesToPlace = numArmiesDifference;
                        addVertex = vertex;
                        break;
                    }
                }
            }
        }

        if (player->executeAddArmies(numArmiesToPlace, addVertex)) {
            cout << "{ " << player->getName() << " } [ MODERATE ] Chose to place " << numArmiesToPlace
            << " armies on < " << addVertex->getName() << " >." << endl;
            maxArmies -= numArmiesToPlace;
        }
    }

    player->printRegions();
}

void ModerateStrategy::AndOrAction(Player* player, const string action, Players* players) {
    vector<string> actionArr;

    if (action.find("OR") != size_t(-1)) {
        int orPos = action.find("OR");
        string firstChoice = action.substr(0, orPos - 1);
        string secondChoice = action.substr(orPos + 3);
        string chosenAction;

        if (firstChoice.find("Add") != size_t(-1) || firstChoice.find("Move") != size_t(-1))
            chosenAction = firstChoice;
        else
            chosenAction = secondChoice;

        actionArr.push_back(chosenAction);
    }
    else {
        size_t andPos = action.find("AND");
        actionArr.push_back(action.substr(0, andPos));
        actionArr.push_back(action.substr(andPos+4));
    }

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it).find("Move") != size_t(-1))
            MoveArmies(player, *it, players);
        else if ((*it).find("Add") != size_t(-1))
            PlaceNewArmies(player, *it, players);
        else if ((*it).find("Destroy") != size_t(-1))
            DestroyArmy(player, players);
        else if ((*it).find("Build") != size_t(-1))
            BuildCity(player);
    }
}

void ModerateStrategy::BuildCity(Player* player) {
    Vertices* playerRegions = player->getOccupiedRegions();
    Vertex* buildVertex = GameMap::instance()->getStartVertex();

    for(Vertices::iterator it = playerRegions->begin(); it != playerRegions->end(); ++it) {
        // Find first vertex with a city. If none, defaults to the start vertex.
        Vertex* vertex = it->second;
        PlayerEntry* entry = player->getPlayerEntry();
        if (vertex->getCities()->find(entry) == vertex->getCities()->end() && vertex != GameMap::instance()->getStartVertex()) {
            cout << "{ " << player->getName() << " } [ MODERATE ] Chose < " << vertex->getName() << " > to build a city." << endl;
            buildVertex = vertex;
            break;
        }
    }

    cout << "{ " << player->getName() << " } [ MODERATE ] Chose the start region < " << buildVertex->getName() << " > to build a city." << endl;
    player->executeBuildCity(buildVertex);

    player->printRegions();
}

/**
 * A moderate player to choose a card position from the game hand.
 *
 * @param player A player pointer to the is using this strategy.
 * @param hand A pointer to the game hand (not used in human strategy).
 * @return The position of the chosen card.
 */
int ModerateStrategy::chooseCardPosition(Player* player, Hand* hand) {
    vector<Card*>::iterator it = hand->getHand()->begin();
    int count = 0;

    while(it != hand->getHand()->end() && count <= player->getCoins()) {
        if ((*it)->getAction().find("Add") != size_t(-1) || (*it)->getAction().find("Move") != size_t(-1)) {
            cout << "{ " << player->getName() << " } [ MODERATE ] Chose position " << count << "." << endl;
            return count;
        }
        count++;
        it++;
    }

    return 0;
}

/**
 * Constructor
 */
HumanStrategy::HumanStrategy(): Strategy(HUMAN) {}

/**
 * Destructor
 */
HumanStrategy::~HumanStrategy() {}

/**
 * Executes the action "Build city".
 *
 * The player can build a city on a region where they currently have armies.
 *
 */
void HumanStrategy::BuildCity(Player* player) {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertex* endVertex;

    while (true) {
        cout << "{ " << player->getName() << " } Please choose a region to build a city on." << endl;
        endVertex = chooseEndVertex(player, ActionType::BUILD_CITY);
        if (!player->executeBuildCity(endVertex)) {
            continue;
        }
        break;
    }
    player->printRegions();
}

/**
 * Executes the action "Move # armies" or "Move # armies over water".
 *
 * The player can move armies around the map to adjacent regions as many times as the card dictates.
 *
 * @param action The action to be executed.
 */
void HumanStrategy::MoveArmies(Player* player, const string action, Players* players) {
    Vertex* startVertex;
    Vertex* endVertex;
    int maxArmies;
    int remainderArmies;

    stringstream toInt(action.substr(5, 6));
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    bool overWaterAllowed = action.find("water") != size_t(-1);
    ActionType type = overWaterAllowed ? ActionType::MOVE_OVER_WATER : ActionType::MOVE_OVER_LAND;
    string actionSuffix = overWaterAllowed ? " armies over water.\n" : " armies over land.\n";

    cout << "\n\n[[ ACTION ]] Move " << maxArmies << actionSuffix << "\n\n" << endl;
    cout << "{ " << player->getName() << " } You can move " << maxArmies << " armies around the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        startVertex = chooseStartVertex(player);
        cout << "{ " << player->getName() << " } Please choose a region to move armies to." << endl;
        endVertex = chooseEndVertex(player, type);

        int startVertexArmies = startVertex->getArmies()->find(player->getPlayerEntry())->second;
        armies = chooseArmies(player, maxArmies, remainderArmies, startVertexArmies, startVertex->getName());

        if (!player->executeMoveArmies(armies, startVertex, endVertex, overWaterAllowed)) {
            continue;
        }

        remainderArmies -= armies;
    }

    player->printRegions();
}

/**
 * Executes the action "Add # armies".
 *
 * The player can add armies to any region the player currently has a city or to the start region.
 *
 * @param action The action to be executed.
 */
void HumanStrategy::PlaceNewArmies(Player* player, const string action, Players* players) {
    Vertex* endVertex;

    stringstream toInt(action.substr(4, 5));
    int maxArmies;
    int remainderArmies;
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    cout << "\n\n[[ ACTION ]] " << action << ".\n\n" << endl;
    cout << "{ " << player->getName() << " } You have the choice of adding " << maxArmies << " armies on the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        cout << "{ " << player->getName() << " } Please choose a region to add new armies to." << endl;
        endVertex = chooseEndVertex(player, ActionType::ADD_ARMY);
        armies = chooseArmies(player, maxArmies, remainderArmies, 0, "none");

        if (!player->executeAddArmies(armies, endVertex)) {
            continue;
        }

        remainderArmies -= armies;
    }

    player->printRegions();
}

/**
 * Executes the action "Destroy army".
 *
 * The player chooses an opponent's army to destroy on a region where the oponnent has an army.
 *
 * @param players A list of players in the game.
 */
void HumanStrategy::DestroyArmy(Player* player, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertex* endVertex;
    string opponentName;
    Player* opponent;

    opponent = chooseOpponent(player, players);

    while(true) {
        cout << "{ " << player->getName() << " } Please choose an opponent occupied region." << endl;
        endVertex = chooseEndVertex(player, ActionType::DESTROY_ARMY);
        if (!player->executeDestroyArmy(endVertex, opponent)) {
            continue;
        }
        break;
    }

    opponent->printRegions();
}

/**
 * Executes the action "<ACTION> AND <ACTION>" or "<ACTION> OR <ACTION>".
 * If the action contains "OR", it asks the player which action to take.
 *
 * @param action The action that contains an AND/OR double action
 * @param players A pointer to a map of Player pointers and their names.
 */
void HumanStrategy::AndOrAction(Player* player, const string action, Players* players) {
    vector<string> actionArr;

    if (action.find("OR") != size_t(-1)) {
        actionArr.push_back(chooseORAction(player, action));
    }
    else {
        size_t andPos = action.find("AND");
        actionArr.push_back(action.substr(0, andPos));
        actionArr.push_back(action.substr(andPos+4));
    }

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it).find("Move") != size_t(-1))
            MoveArmies(player, *it, players);
        else if ((*it).find("Add") != size_t(-1))
            PlaceNewArmies(player, *it, players);
        else if ((*it).find("Destroy") != size_t(-1))
            DestroyArmy(player, players);
        else if ((*it).find("Build") != size_t(-1))
            BuildCity(player);
    }
}

/**
 * Prompts a player to choose how many armies to place on a region.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param maxArmies The maximum number of armies the player can place. Determined by the card chosen.
 * @param remainderArmies The amount of armies left that the player can choose from.
 * @param startVertexArmies The number of armies on the vertex where the armies are coming from.
 * @param startVertex The name of the vertex that the armies are coming from.
 * @return The number of armies chosen to move.
 */
int HumanStrategy::chooseArmies(Player* player, const int& maxArmies, const int& remainderArmies, int startVertexArmies, const string& startVertex) {
    int armies;
    string armiesStr;

    if (startVertex == "none")
        startVertexArmies = remainderArmies;

    while (true) {
        string remainderArmiesStr = remainderArmies == 1 ? "army" : "armies";
        string startVertexArmiesStr = startVertexArmies == 1 ? "army" : "armies";

        cout << "{ " << player->getName() << " } [ HUMAN ] How many armies? ( Max armies " << remainderArmies << ")" << endl;
        cout << "{ " << player->getName() << " } > ";
        getline(cin, armiesStr);

        try {
            armies = stoi(armiesStr);

            if (startVertexArmies - armies >= 0 && remainderArmies - armies >= 0 && remainderArmies - armies <= maxArmies) {
                cout << endl;
                break;
            }

            if (remainderArmies - armies)
                cout << "[ ERROR! ] You can only choose a maximum of " << remainderArmies << " " << remainderArmiesStr << "." << endl;
            else
                cout << "[ ERROR! ] < " << startVertex << " > only has " << startVertexArmies << " " << startVertexArmiesStr << " available." << endl;

        } catch(invalid_argument& e) {
            cout << "[ ERROR! ] Please enter a number." << endl;
        }
    }

    return armies;
}

/**
 * Prompts a player to choose a start vertex among the player's occupied regions.
 *
 * @param player A pointer to the player who is using this strategy.
 * @return A Vertex pointer to the start region.
 */
Vertex* HumanStrategy::chooseStartVertex(Player* player){

    string startName;

    while(true){
        cout << "{ " << player->getName() << " } [ HUMAN ] You must choose a region to take armies from." << endl;
        player->printRegions();
        cout << "{ " << player->getName() << " } > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (player->getOccupiedRegions()->find(startName) == player->getOccupiedRegions()->end()) {
            cout << "[ ERROR! ] You chose an invalid region name. Please try again." << endl;
        } else {
            return player->getOccupiedRegions()->find(startName)->second;
        }
    }
}

/**
 * Prompts a player to choose an end vertex on the map.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param type An ActionType enum representing the type of action being called.
 * @return A Vertex pointer to the end region.
 */
Vertex* HumanStrategy::chooseEndVertex(Player* player, const ActionType& type){
    string endName;

    while(true){
        if (type == ActionType::ADD_ARMY || type == ActionType::BUILD_CITY)
            player->printRegions();
        else if (type == ActionType::MOVE_OVER_LAND || type == ActionType::MOVE_OVER_WATER || type == ActionType::DESTROY_ARMY) {
            GameMap::instance()->printMap();
            GameMap::instance()->printOccupiedRegions();
        } else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return 0;
        }

        cout << "{ " << player->getName() << " } > ";
        getline(cin, endName);
        transform(endName.begin(), endName.end(), endName.begin(), ::toupper);
        cout << "{ " << player->getName() << " } [ HUMAN ] You chose < " << endName << " >.\n" << endl;

        if (GameMap::instance()->getVertices()->find(endName) == GameMap::instance()->getVertices()->end()) {
            cerr << "[ ERROR! ] That region doesn't exist on the map." << endl;
            continue;
        }

        return GameMap::instance()->getVertices()->find(endName)->second;
    }
}


/**
 * Prompts a player to choose one of the action in an OR'd card.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param action The action containing an OR'd action.
 * @return The action chosen by the player.
 */
string HumanStrategy::chooseORAction(Player* player, const string action) {
    string answer;
    string firstChoice;
    string secondChoice;

    int orPos = action.find("OR");

    while(true) {
        cout << "\n{ " << player->getName() << " } [ HUMAN ] Which action do you want? 1 or 2 ?" << endl;

        firstChoice = action.substr(0, orPos - 1);
        secondChoice = action.substr(orPos + 3);

        cout << "[ OPTION 1 ] " << firstChoice << endl;
        cout << "[ OPTION 2 ] " << secondChoice << endl;

        cout << "{ " << player->getName() << " } > ";
        getline(cin, answer);

        if (answer == "1")
            return firstChoice;
        if (answer == "2")
            return secondChoice;

        cout << "[ ERROR! ] Invalid choice. Please enter either '1' or '2'." << endl;
    }
}


/**
 * Prompts a player to choose an opponent.
 *
 * @param player A pointer to the player who is using this strategy.
 * @param players A pointer to the list of Players in the game.
 * @return A Player pointer to the chosen opponent.
 */
Player* HumanStrategy::chooseOpponent(Player* player, Players* players) {
    string opponentName;

    while (true) {
        cout << "{ " << player->getName() << " } [ HUMAN ] Choose an opponent." << endl;

        for (Players::iterator it = players->begin(); it != players->end(); ++it)
            if (it->second != player)
                cout << "[ OPPONENT ] " << it->first << endl;

        cout << "{ " << player->getName() << " } > ";
        getline(cin, opponentName);

        if (opponentName == player->getName()) {
            cerr << "[ ERROR! ] You can't destroy your own army." << endl;
            continue;
        }

        if (players->find(opponentName) != players->end())
            return players->find(opponentName)->second;

        cerr << "[ ERROR! ] " << opponentName << " doesn't exist. Try again." << endl;
    }
}

/**
 * Prompts a human player to choose a card position from the game hand.
 *
 * @param player A player pointer to the is using this strategy.
 * @param hand A pointer to the game hand (not used in human strategy).
 * @return The position of the chosen card.
 */
int HumanStrategy::chooseCardPosition(Player* player, Hand* hand) {
    string pos;
    int position;

    while (true) {
        cout << "[ GAME HAND ] Please choose a card from the game hand. { Purse = "
             << player->getCoins() << " } { Cards in hand "
             << player->getHand()->size() << " }." << endl;
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