#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "util/MapUtil.h"

#include <algorithm>

class GameMap;
/**
 * Default constructor
 */
Player::Player():
    name(new string("No name")),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)) {}

Player::Player(const string &playerName): 
    name(new string(playerName)),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)) 
{
    cout << "{ " << *name << " } CREATED. (Purse = 0)." << endl;
}
/**
 * Initializes a Player object.
 * 
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of countries, an empty hand and a Bidder object.
 * 
 * @param playerName The name of the player.
 * @param startCoins The number of coins the player starts with.
 */
Player::Player(const string& playerName, const int& startCoins):
    name(new string(playerName)),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(startCoins)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this))
{
    cout << "{ " << *name << " } CREATED. (Purse = " << startCoins << ")." << endl;
}

/**
 * Copy Constructor
 */
Player::Player(Player* player){
    name = new string(player->getName());
    countries = new Vertices(*player->getCountries());
    armies = new int(player->getArmies());
    cities = new int(player->getCities());
    coins = new int(player->getCoins());
    hand = new vector<Card*>(*player->getHand());
    bidder = new Bidder(player->getBidder());
}

/**
 * Assignment operator
 */
Player& Player::operator=(Player& player) {
    name = new string(player.getName());
    countries = new Vertices(*player.getCountries());
    armies = new int(player.getArmies());
    cities = new int(player.getCities());
    coins = new int(player.getCoins());
    hand = new vector<Card*>(*player.getHand());
    bidder = new Bidder(player.getBidder());
    return *this;
}

/**
 * Destructor
 */
Player::~Player(){
    //Delete all the cards the player holds.
    for(vector<Card*>::iterator it = hand->begin(); it != hand->end(); ++it)
        delete *it;

    delete name;
    delete countries;
    delete armies;
    delete cities;
    delete coins;
    delete hand;
    delete bidder;

    name = nullptr;
    countries = nullptr;
    armies = nullptr;
    cities = nullptr;
    coins = nullptr;
    hand = nullptr;
    bidder = nullptr;
}

/**
 * Takes coins out of the Player's purse.
 *
 * Checks if the player has enough coins to pay "amount" of coins
 *
 * @param amount Integer amount of coins to pay.
 * @return a boolean that shows the action was successful.
 */
bool Player::PayCoins(const int& amount){
    if (amount <= *coins && amount >= 0) {
        string coinStr = amount == 1 ? "coin" : "coins";
        *coins -= amount;
        cout << "\n{ " << *name << " } Paid " << amount << " " << coinStr << ". (Purse = " << *coins << ").\n" << endl;
        return true;
    }
    cout << "[ ERROR! ] " << *name << " doesn't have enough coins to pay " << amount << ". (Purse = " << *coins << ")." << endl;
    return false;
}

/**
 * Executes the action "Build city".
 * 
 * The player can build a city on a country where they currently have armies.
 * 
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void Player::BuildCity(const string action, GameMap* map) {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertex* endVertex;

    while (true) {
        cout << "{ " << *name << " } Please choose a country to build a city on." << endl;
        endVertex = chooseEndVertex(ActionType::BUILD_CITY, map);
        if (!executeBuildCity(endVertex)) {
            continue;
        }
        break;
    }
    printCountries();
}

void Player::MoveOverLand(const string action, GameMap* map){
    return MoveArmies(action, map);
}

void Player::MoveOverWater(const string action, GameMap* map){
    return MoveArmies(action, map);
}

/**
 * Executes the action "Move # armies" or "Move # armies over water".
 * 
 * The player can move armies around the map to adjacent countries as many times as the card dictates.
 * 
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void Player::MoveArmies(const string action, GameMap* map) {
    Vertex* startVertex;
    Vertex* endVertex;
    int maxArmies;
    int remainderArmies;

    stringstream toInt(action.substr(5, 6));
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    bool overWater = action.find("water") != size_t(-1);
    ActionType type = overWater ? ActionType::MOVE_OVER_WATER : ActionType::MOVE_OVER_LAND;
    string actionSuffix = overWater ? " armies over water.\n" : " armies over land.\n";


    cout << "\n\n[[ ACTION ]] Move " << maxArmies << actionSuffix << "\n\n" << endl;
    cout << "{ " << *name << " } You can move " << maxArmies << " armies around the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        startVertex = chooseStartVertex();
        cout << "{ " << *name << " } Please choose a country to move armies to." << endl;
        endVertex = chooseEndVertex(type, map);
        
        int startVertexArmies = startVertex->getArmies()->find(*name)->second;
        armies = chooseArmies(maxArmies, remainderArmies, startVertexArmies, startVertex->getName());

        if (!executeMoveArmies(armies, startVertex, endVertex, overWater)) {
            continue;
        }

        remainderArmies -= armies;
    }
    printCountries();
}

/**
 * Executes the action "Add # armies".
 * 
 * The player can add armies to any country the player currently has a city or to the start country.
 * 
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 */
void Player::PlaceNewArmies(const string action, GameMap* map) {
    Vertex* endVertex;

    stringstream toInt(action.substr(4, 5));
    int maxArmies;
    int remainderArmies;
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    cout << "\n\n[[ ACTION ]] Add " << maxArmies << " armies.\n\n" << endl;
    cout << "{ " << *name << " } You have the choice of adding " << maxArmies << " armies on the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        cout << "{ " << *name << " } Please choose a country to add new armies to." << endl;
        endVertex = chooseEndVertex(ActionType::ADD_ARMY, map);
        armies = chooseArmies(maxArmies, remainderArmies, 0, "none");

        if (!executeAddArmies(armies, endVertex, map->getStartVertex())) {
            continue;
        }

        remainderArmies -= armies;
    }

    printCountries();
}

/**
 * Executes the action "Destroy army".
 * 
 * The player chooses an opponent's army to destroy on a country where the oponnent has an army.
 *
 * @param action The action to be executed.
 * @param map A GameMap pointer to the map.
 * @param players A list of players in the game.
 */
void Player::DestroyArmy(const string action, GameMap* map, Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertex* endVertex;
    string opponentName;
    Player* opponent;

    opponent = chooseOpponent(players);

    while(true) {
        cout << "{ " << *name << " } Please choose an opponent occupied country." << endl;
        endVertex = chooseEndVertex(ActionType::DESTROY_ARMY, map);
        if (!executeDestroyArmy(endVertex, opponent)) {
            continue;
        }
        break;
    }

    opponent->printCountries();
}

void Player::AndOrAction(const string action, GameMap* map, Players* players) {
    vector<string> actionArr;

    if (action.find("OR") != size_t(-1)) {
        actionArr.push_back(chooseORAction(action));
    }
    else {
        size_t andPos = action.find("AND");
        actionArr.push_back(action.substr(0, andPos));
        actionArr.push_back(action.substr(andPos));
    }

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it).find("Move") != size_t(-1)) {
            if ((*it).find("water") != size_t(-1))
                MoveOverLand(action, map);
            else
                MoveOverWater(action, map);
        }
        else if ((*it).find("Add") != size_t(-1))
            PlaceNewArmies(action, map);
        else if ((*it).find("Destroy") != size_t(-1))
            DestroyArmy(action, map, players);
        else if ((*it).find("Build") != size_t(-1))
            BuildCity(action, map);
    }
}

void Player::Ignore() {
cout << "\n{ " << *name << " } Ignoring card action ... \n" << endl;
}

/**
 * Detects if a country is adjacent to at least one the player's currently
 * occupied countries.
 *
 * @param target A Vertex pointer to the target country.
 * @param overWaterAllowed A boolean representing if the country is allowed to be across a water edge.
 * @return a boolean representing if the country is adjacent.
 */
bool Player::isAdjacent(Vertex* target, const bool& overWaterAllowed){
    return isAdjacent(target->getName(), overWaterAllowed);
}

/**
 * Detects if a country is adjacent to at least one the player's currently
 * occupied countries.
 *
 * @param target The target country name.
 * @param overWaterAllowed A boolean representing if the country is allowed to be across a water edge.
 * @return a boolean representing if the country is adjacent.
 */
bool Player::isAdjacent(const string& target, const bool& overWaterAllowed){
    typedef pair<Vertex*, bool> Edge;

    Vertices::iterator it = countries->begin();
    set<string> visited;

    while(it != countries->end()) {

        if (it->second->getName() == target)
            return true;

        if (visited.find(it->first) == visited.end())
            for(Edge& e : *it->second->getEdges())
                if (e.first->getName() == target && (!e.second || (e.second && overWaterAllowed)))
                    return true;

        visited.insert(it->first);
        ++it;
    }

    return false;
}

/**
 * Gets the number of armies on a country occupied by the player.
 *
 * @param country A Vertex pointer to the target country.
 * @return the number of armies on the country.
 */
int Player::getArmiesOnCountry(Vertex* country) {
    if(countries->find(country->getKey()) != countries->end()) {
        int numArmies = country->getArmies()->find(*name)->second;
        string army = numArmies == 1 ? "army" : "armies";
        cout << "{ " << *name << " } Has " << numArmies << " " << army << " on country < " << country->getName() << " >." << endl;
        return numArmies;
    }
    cout << "{ " << *name << " } Has ZERO armies on country < " << country->getName() << " >." << endl;
    return 0;
}

/**
 * Gets the number of cities on a country occupied by the player.
 *
 * @param country A Vertex pointer to the target country.
 * @return the number of cities on the country.
 */
int Player::getCitiesOnCountry(Vertex* country){
    if(countries->find(country->getKey()) != countries->end()) {
        int numCities = country->getCities()->find(*name)->second;
        string city = numCities == 1 ? "city" : "cities";
        cout << "{ " << *name << " } Has " << numCities << " " << city << " on country < " << country->getName() << " >." << endl;
        return numCities;
    }
    cout << "{ " << *name << " } Has ZERO cities on country < " << country->getName() << " >." << endl;
    return 0;
}

/**
 * Adds a card to the player's hand.
 *
 * Preconditions: Player has successfuly paid for card.
 *
 * @param card A Card pointer. Used to create a deep copy of the card for the Player's hand.
 */
void Player::addCardToHand(Card* card) {
	Card *newCard = new Card(card);
    hand->push_back(newCard);
}

/**
 * Adds a country to the player's list of occupied countries.
 *
 * Preconditions: Country is a valid country and it satisfies all
 * required conditions specific to the card action chosen by the Player.
 *
 * @param country A Vertex pointer to the target country.
 */
void Player::addCountry(Vertex* country){
    countries->insert(pair<string, Vertex*> (country->getKey(), country));
    cout << "{ " << *name << " } " << "Added country < " << country->getName() << " > to player's countries." << endl;
}

/**
 * Removes a country from the Player's list of occupied countries.
 *
 * Detects whether the country to be removed still contains armies or cities belonging to the Player.
 * If so, the country will not be removed and an error message will print out.
 *
 * @param country A Vertex pointer to the target country.
 */
void Player::removeCountry(Vertex* country) {
    if(countries->find(country->getKey()) != countries->end()) {
        int numArmies = 0, numCities = 0;

        //Get current number of armies and cities if they exist on the country.
        if (country->getArmies()->find(*name) != country->getArmies()->end())
            numArmies = country->getArmies()->find(*name)->second;
        if (country->getCities()->find(*name) != country->getCities()->end())
            numCities = country->getCities()->find(*name)->second;

        //Only remove the country if the player has 0 armies and 0 cities on the country.
        if (numArmies == 0 && numCities == 0) {
            country->getArmies()->erase(*name);
            country->getCities()->erase(*name);
            countries->erase(country->getKey());
            cout << "{ " << *name << " } " << "Removed < " << country->getName() << " >." << endl;
        } else
            cout << "[ ERROR! ] Can't remove < " << country->getName() << " >. " << *name << " still owns "
                << numArmies << " armies and " << numCities << " cities on it." << endl;
    } else {
        cout << "[ ERROR! ] " << *name << " doesn't have the country < " << country->getName() << " > available to remove." << endl;
    }
}

/**
 * Prints a list of the Player's occupied countries.
 */
void Player::printCountries(){

    cout << "\n{ " << *name << " } Occupied Countries:\n" << endl;
    cout << "--------------------------------------------------------" << endl;
    Vertices::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        int numArmies = 0;
        int numCities = 0;
        if (it->second->getArmies()->find(*name) != it->second->getArmies()->end())
            numArmies = it->second->getArmies()->find(*name)->second;
        if (it->second->getCities()->find(*name) != it->second->getCities()->end())
            numCities = it->second->getCities()->find(*name)->second;

        printf("\t%-3s : %-15s ARMIES: %-5d CITIES: %-5d\n", it->second->getKey().c_str(), it->second->getName().c_str(), numArmies, numCities);
    }

    cout << "--------------------------------------------------------\n" << endl;
}

// PRIVATE
/**
 * Adds armies to a country.
 * Preconditions: The Player has enough free armies to add to this country and the country
 * has met the conditions of the card action chosen by the Player.
 *
 * Detects whether the country to be removed still contains armies or cities belonging to the Player.
 * If so, the country will not be removed and an error message will print out.
 *
 * @param country A Vertex pointer to the target country.
 * @param numArmies The number of armies to add.
 */
void Player::addArmiesToCountry(Vertex* country, const int& numArmies) {
    //Add country to the Player's list of occupied countries if it hasn't been added yet.
    if(countries->find(country->getKey()) == countries->end())
        addCountry(country);

    if (country->getArmies()->find(*name) == country->getArmies()->end()) {
        //Country doesn't have any of Player's armies. Create a new record.
        country->getArmies()->insert(pair<string, int> (*name, numArmies));
    } else {
        //Country already has some of Player's armies. Add to current number of armies.
        int currentArmies = country->getArmies()->find(*name)->second;
        country->getArmies()->erase(*name);
        country->getArmies()->insert(pair<string, int> (*name, currentArmies + numArmies));
    }
}

// PRIVATE
/**
 * Removes armies from a country.
 *
 * @param country A Vertex pointer to the target country.
 * @param numArmies The number of armies to remove from country.
 */
void Player::removeArmiesFromCountry(Vertex* country, const int& numArmies) {
    int currentArmies = country->getArmies()->find(*name)->second;

    // erase current record
    country->getArmies()->erase(*name);

    // only update army count if there are still armies left on country
    if (currentArmies - numArmies > 0)
        country->getArmies()->insert(pair<string, int> (*name, currentArmies - numArmies));
    // remove country if resulting num armies is 0 and no cities exist
    else if (numArmies == currentArmies && country->getCities()->find(*name) == country->getCities()->end())
        removeCountry(country);
}

//PRIVATE
/**
 * Increases number of free armies available to a Player.
 *
 * @param numArmies The number of armies to add.
 */
void Player::increaseAvailableArmies(const int& numArmies) {
    *armies += numArmies;
}

//PRIVATE
/**
 * Removes free armies available to a Player.
 *
 * @param numArmies The number of armies to remove.
 */
void Player::decreaseAvailableArmies(const int& numArmies) {
    if (*armies - numArmies <= 0) {
        *armies = 0;
    } else {
        *armies -= numArmies;
    }
}

/**
 * Prompts a player to choose a start vertex among the player's occupied countries.
 * 
 * 
 * @param player A pointer to the Player being prompted.
 * @return A Vertex pointer to the start country.
 */
Vertex* Player::chooseStartVertex(){

    string startName;

    while(true){
        cout << "{ " << *name << " } You must choose a country to take armies from." << endl;
        printCountries();
        cout << "{ " << *name << " } > ";
        getline(cin, startName);
        transform(startName.begin(), startName.end(),startName.begin(), ::toupper);

        if (getCountries()->find(startName) == getCountries()->end()) {
            cout << "[ ERROR! ] You chose an invalid country name. Please try again." << endl;
        } else {
            return getCountries()->find(startName)->second;
        }
    }
}

/**
 * Prompts a player to choose an end vertex on the map.
 * 
 * @param player A pointer to the Player being prompted.
 * @param type An ActionType enum representing the type of action being called.
 * @param map A GameMap pointer to the map.
 * @return A Vertex pointer to the end country.
 */
Vertex* Player::chooseEndVertex(const ActionType& type, GameMap* map){
    string endName;

    while(true){
        if (type == ActionType::ADD_ARMY || type == ActionType::BUILD_CITY)
            printCountries();
        else if (type == ActionType::MOVE_OVER_LAND || type == ActionType::MOVE_OVER_WATER || type == ActionType::DESTROY_ARMY)
            map->printMap();
        else {
            cerr << "[ ERROR! ] Invalid action type." << endl;
            return 0;
        }

        cout << "{ " << *name << " } > ";
        getline(cin, endName);
        transform(endName.begin(), endName.end(), endName.begin(), ::toupper);
        cout << "{ " << *name << " } You chose < " << endName << " >.\n" << endl;

        if (map->getVertices()->find(endName) == map->getVertices()->end()) {
            cerr << "[ ERROR! ] That country doesn't exist on the map." << endl;
            continue;
        }

        return map->getVertices()->find(endName)->second;
    }
}

/**
 * Prompts a player to choose how many armies to place on a country.
 * 
 * @param maxArmies The maximum number of armies the player can place. Determined by the card chosen.
 * @param remainderArmies The amount of armies left that the player can choose from.
 * @return The number of armies chosen to move.
 */
int Player::chooseArmies(const int& maxArmies, const int& remainderArmies, int startVertexArmies, const string& startVertexName) {
    int armies;
    string armiesStr;

    if (startVertexName == "none")
        startVertexArmies = remainderArmies;

    while (true) {
        string remainderArmiesStr = remainderArmies == 1 ? "army" : "armies";
        string startVertexArmiesStr = startVertexArmies == 1 ? "army" : "armies";

        cout << "{ " << *name << " } How many armies? ( Max armies " << remainderArmies << ")" << endl;
        cout << "{ " << *name << " } > ";
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
                cout << "[ ERROR! ] < " << startVertexName << " > only has " << startVertexArmies << " " << startVertexArmiesStr << " available." << endl;
            
        } catch(invalid_argument& e) {
            cout << "[ ERROR! ] Please enter a number." << endl;
        }
    }

    return armies;
}

/**
 * Prompts a player to choose one of the action in an OR'd card.
 * 
 * @param action The action containing an OR'd action.
 * @return The action chosen by the player.
 */
string Player::chooseORAction(const string action) {
    string answer;
    string firstChoice;
    string secondChoice;

    int orPos = action.find("OR");

    while(true) {
        cout << "\n{ " << *name << " } Which action do you want? 1 or 2 ?" << endl;

        firstChoice = action.substr(0, orPos - 1);
        secondChoice = action.substr(orPos + 3);

        cout << "[ OPTION 1 ] " << firstChoice << endl;
        cout << "[ OPTION 2 ] " << secondChoice << endl;

        cout << "{ " << *name << " } > ";
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
 * @param players A pointer to the list of Players in the game.
 * @param currentPlayer A pointer to the Player being prompted.
 * @return A Player pointer to the chosen opponent.
 */
Player* Player::chooseOpponent(Players* players) {
    string opponentName;

    while (true) {
        cout << "{ " << *name << " } Choose an opponent." << endl;
        cout << "{ " << *name << " } > ";
        getline(cin, opponentName);

        if (opponentName == *name) {
            cerr << "[ ERROR! ] You can't destroy your own army." << endl;
            continue;
        }

        if (players->find(opponentName) != players->end())
            return players->find(opponentName)->second;

        cerr << "[ ERROR! ] " << opponentName << " doesn't exist. Try again." << endl;
    }
}

/**
 * Player places new armies on a country.
 *
 * Checks if the player has enough free armies to place on the country.
 * The country must either be the start country or contain a city.
 *
 * @param amount Number of armies to place.
 * @param country A Vertex pointer to the country on which to place armies.
 * @param start A string containing the name of the start country.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeAddArmies(const int& newArmies, Vertex* country, const string& start){
    if (country->getKey() == start || country->getCities()->find(*name) != country->getCities()->end()){
        if (newArmies > *armies || newArmies < 0) {
            cout << "[ ERROR! ] " << *name << " doesn't have enough armies to place " << newArmies << " new armies on < " << country->getName() << " >." << endl;
            return false;
        }

        *armies -= newArmies;
        addArmiesToCountry(country, newArmies);

        cout << "{ " << *name << " } Added " << newArmies << " new armies to < "<< country->getName() << " >." << endl;
        return true;
    }
    cout << "[ ERROR! ] " << *name << " Chose an invalid country. It must be either START or contain one of the player's cities->" << endl;
    return false;
}

/**
 * Moves a certain number of armies over to an adjacent country not separated by water.
 *
 * Detects whether or not the country contains the Player's armies and if the country has
 * enough armies on it to move.
 *
 * @param numArmies Amount of armies to move.
 * @param start A Vertex pointer to the start country.
 * @param end A Vertex pointer to the end country.
 * @param moveOverWater A boolean representing if armies can move over water.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeMoveArmies(const int& numArmies, Vertex* start, Vertex* end, const bool& moveOverWater){
    //Assumes: country is a valid vertex on the map
    //is country an adjacent country to an occupied country?
    if (isAdjacent(end, moveOverWater)) {
        //does start vertex even have armies on it?
        if (start->getArmies()->find(*name) != start->getArmies()->end()) {
            int currentArmies = start->getArmies()->find(*name)->second;
            if (numArmies > currentArmies) {
                cout << "[ ERROR! ] " << *name << " doesn't have enough armies on < " << start->getName()
                     << " > to move " << numArmies << "." << endl;
                return false;
            }

            addArmiesToCountry(end, numArmies);
            removeArmiesFromCountry(start, numArmies);

            cout << "{ " << *name << " } Moved " << numArmies
                 << " armies from < " << start->getName() << " > to < " << end->getName() << " >." << endl;
            return true;
        }

        cout << "[ ERROR! ] " << *name << " doesn't have any armies on < " << start->getName() << " > to move." << endl;
        return false;
    }

    cout << "[ ERROR! ] " << end->getName() << " > is not an adjacent country." << endl;
    return false;
}

/**
 * Builds a city on a country occupied by the Player.
 *
 * Detects whether or not the country contains any Player's armies.
 *
 * @param country A Vertex pointer to the country.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeBuildCity(Vertex* country){
    unordered_map<string, int>* armies = country->getArmies();

    //does country belong to the player and does it contain at least one army?
    if (armies->find(*name) != armies->end()) {
        if (armies->find(*name)->second > 0) {

            //If country contains a player owned city, increase the count
            int currentCities = 1;
            if (country->getCities()->find(*name) != country->getCities()->end()) {
                currentCities += country->getCities()->find(*name)->second;
                country->getCities()->erase(*name);
                country->getCities()->insert(pair<string,int>(*name, currentCities));
            } else //Else insert new record
                country->getCities()->insert(pair<string, int> (*name, currentCities));

            cout << "{ " << *name << " } Added an city to < " << country->getName() << " >. (New city count = " << currentCities << ")." << endl;
            return true;
        }
    }

    cout << "[ ERROR! ] " << *name << " can't place city on < " << country->getName() << " > because the player has no armies on it." << endl;
    return false;
}

/**
 * Destroys an army belonging an opponent's country.
 *
 * Ensures the opponent is not the current player and that the chosen country
 * contains armies belonging to the opponent.
 *
 * @param country A Vertex pointer to the target country.
 * @param opponent A Player pointer to the opponent player.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeDestroyArmy(Vertex* country, Player* opponent){

    if (opponent->getName() == *name) {
        cout << "\n{ " << *name << " } Can't destroy own army." << endl;
        return false;
    }

    //Does opponent country contain an army to destroy?
    if (country->getArmies()->find(opponent->getName()) != country->getArmies()->end()
        && country->getArmies()->find(opponent->getName())->second > 0) {

        int currentArmies = country->getArmies()->find(opponent->getName())->second;
        currentArmies--;

        //Add destroyed army back in opponents available armies
        opponent->increaseAvailableArmies(1);

        //Remove old record of opponent's armies on country
        country->getArmies()->erase(opponent->getName());

        if (currentArmies != 0)
            //Insert new record with decremented army count
            country->getArmies()->insert(pair<string, int> (opponent->getName(), currentArmies));
        else {
            //Remove country from opponent's list of occupied countries because the last army was destroyed.
            opponent->removeCountry(country);
        }

        cout << "{ " << *name << " } Destroyed one of " << opponent->getName() << "'s armies on < " << country->getName() << " >." << endl;
        return true;
    }


    cout << "[ ERROR! ] " << opponent->getName() << " doesn't have any armies to destroy on < " << country->getName() << " >." << endl;
    return false;
}