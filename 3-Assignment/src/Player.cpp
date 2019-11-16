#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "util/MapUtil.h"
#include "PlayerStrategies.h"

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
    bidder(new Bidder(this)),
    colour(new string("none")),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0)) {}

/**
 * Initializes a Player object.
 *
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of countries, an empty hand and a Bidder object.
 *
 * @param playerName The name of the player.
 * @param theColour The colour of game pieces.
 */
Player::Player(const string &playerName, const string& theColour):
    name(new string(playerName)),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)),
    colour(new string(theColour)),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0))
{
    cout << "\n{ " << *name << " } CREATED. [ " << *colour << " ] (Purse = 0)." << endl;
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
    bidder(new Bidder(this)),
    colour(new string("")),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0))
{
    cout << "{ " << *name << " } CREATED. (Purse = " << startCoins << ")." << endl;
}

/**
 * Initializes a Player object.
 *
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of countries, an empty hand and a Bidder object.
 *
 * @param playerName The name of the player.
 * @param theColour The colour of game pieces.
 * @param theStrategy The strategy the player will use during game play.
 */
Player::Player(const string &playerName, const string& theColour, Strategy* theStrategy):
    name(new string(playerName)),
    countries(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)),
    colour(new string(theColour)),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0)),
    strategy(theStrategy)
{
    cout << "\n{ " << *name << " } CREATED. [ " << *colour << " ] (Purse = 0) { Strategy " << strategy->getType() << " }." << endl;
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
    colour = new string(player->getColour());
    playerEntry = new PlayerEntry(player->getPlayerEntry()->first, player->getPlayerEntry()->second);
    controlledRegions = new int(player->getControlledRegions());
}

/**
 * Assignment operator
 */
Player& Player::operator=(Player& player) {
    if (&player != this) {
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
        delete colour;
        delete playerEntry;
        delete controlledRegions;

        name = new string(player.getName());
        countries = new Vertices(*player.getCountries());
        armies = new int(player.getArmies());
        cities = new int(player.getCities());
        coins = new int(player.getCoins());
        hand = new vector<Card*>(*player.getHand());
        bidder = new Bidder(player.getBidder());
        colour = new string(player.getColour());
        playerEntry = new PlayerEntry(player.getPlayerEntry()->first, player.getPlayerEntry()->second);
        controlledRegions = new int(player.getControlledRegions());
    }
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
    delete colour;
    delete playerEntry;
    delete controlledRegions;

    name = nullptr;
    countries = nullptr;
    armies = nullptr;
    cities = nullptr;
    coins = nullptr;
    hand = nullptr;
    bidder = nullptr;
    colour = nullptr;
    playerEntry = nullptr;
    controlledRegions = nullptr;
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
 */
void Player::BuildCity() {
    cout << "\n\n[[ ACTION ]] Build a city.\n\n" << endl;

    Vertex* endVertex;

    while (true) {
        cout << "{ " << *name << " } Please choose a country to build a city on." << endl;
        endVertex = strategy->chooseEndVertex(this, ActionType::BUILD_CITY);
        if (!executeBuildCity(endVertex)) {
            continue;
        }
        break;
    }
    printCountries();
}

/**
 * Executes the action "Moves # armies".
 *
 * @param action The action to be executed.
 */
void Player::MoveOverLand(const string action){
    MoveArmies(action);
}

/**
 * Executes the action "Move # armies over water".
 *
 * @param action The action to be executed.
 */
void Player::MoveOverWater(const string action){
    MoveArmies(action);
}

/**
 * Executes the action "Move # armies" or "Move # armies over water".
 *
 * The player can move armies around the map to adjacent countries as many times as the card dictates.
 *
 * @param action The action to be executed.
 */
void Player::MoveArmies(const string action) {
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

        startVertex = strategy->chooseStartVertex(this);
        cout << "{ " << *name << " } Please choose a country to move armies to." << endl;
        endVertex = strategy->chooseEndVertex(this, type);

        int startVertexArmies = startVertex->getArmies()->find(playerEntry)->second;
        armies = strategy->chooseArmies(this, maxArmies, remainderArmies, startVertexArmies, startVertex->getName());

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
 */
void Player::PlaceNewArmies(const string action) {
    Vertex* endVertex;

    stringstream toInt(action.substr(4, 5));
    int maxArmies;
    int remainderArmies;
    toInt >> maxArmies;
    remainderArmies = maxArmies;

    cout << "\n\n[[ ACTION ]] " << action << ".\n\n" << endl;
    cout << "{ " << *name << " } You have the choice of adding " << maxArmies << " armies on the board." << endl;

    while(remainderArmies > 0) {
        int armies;

        cout << "{ " << *name << " } Please choose a country to add new armies to." << endl;
        endVertex = strategy->chooseEndVertex(this, ActionType::ADD_ARMY);
        armies = strategy->chooseArmies(this, maxArmies, remainderArmies, 0, "none");

        if (!executeAddArmies(armies, endVertex, GameMap::instance()->getStartVertex())) {
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
 * @param players A list of players in the game.
 */
void Player::DestroyArmy(Players* players) {
    cout << "\n\n[[ ACTION ]] Destroy an army.\n\n" << endl;

    Vertex* endVertex;
    string opponentName;
    Player* opponent;

    opponent = strategy->chooseOpponent(this, players);

    while(true) {
        cout << "{ " << *name << " } Please choose an opponent occupied country." << endl;
        endVertex = strategy->chooseEndVertex(this, ActionType::DESTROY_ARMY);
        if (!executeDestroyArmy(endVertex, opponent)) {
            continue;
        }
        break;
    }

    opponent->printCountries();
}

/**
 * Executes the action "<ACTION> AND <ACTION>" or "<ACTION> OR <ACTION>".
 * If the action contains "OR", it asks the player which action to take.
 *
 * @param action The action that contains an AND/OR double action
 * @param players A pointer to a map of Player pointers and their names.
 */
void Player::AndOrAction(const string action, Players* players) {
    vector<string> actionArr;

    if (action.find("OR") != size_t(-1)) {
        actionArr.push_back(strategy->chooseORAction(this, action));
    }
    else {
        size_t andPos = action.find("AND");
        actionArr.push_back(action.substr(0, andPos));
        actionArr.push_back(action.substr(andPos+4));
    }

    for(vector<string>::iterator it = actionArr.begin(); it != actionArr.end(); ++it) {
        if ((*it).find("Move") != size_t(-1)) {
            if ((*it).find("water") != size_t(-1))
                MoveOverLand(*it);
            else
                MoveOverWater(*it);
        }
        else if ((*it).find("Add") != size_t(-1))
            PlaceNewArmies(*it);
        else if ((*it).find("Destroy") != size_t(-1))
            DestroyArmy(players);
        else if ((*it).find("Build") != size_t(-1))
            BuildCity();
    }
}

/**
 * Ignores the card action.
 */
void Player::Ignore() {
    cout << "\n{ " << *name << " } Ignoring card action ... \n" << endl;
}

/**
 * Calculates the total victory points of a player at the end of the game.
 * Totals the number of owned regions, owned continents and resource victory points.
 *.
 * @return The total victory points.
 */
int Player::ComputeScore() {
    cout << "---------------------------------------------------------------------" << endl;
    cout << "{ " << *name << " } CALCULATING SCORE ..." << endl;
    cout << "---------------------------------------------------------------------\n" << endl;

    int vpRegions = getVPFromRegions();
    int vpContinents = computeContinentScore();
    int vpGoods = computeGoodsScore();

    int totalPoints = vpRegions + vpContinents + vpGoods;

    cout << "\n{ " << *name << " } Final score : " << totalPoints << "\n" << endl;

    return totalPoints;
}

/**
 * Calculates the total owned regions for a player. A region is considered owned
 * if the player has the most armies and cities on that region. If two players have
 * the same count, then no one owns the region.
 *
 * @return The total number of owned regions.
 */
int Player::getVPFromRegions() {
    vector<string>* ownedRegions = getOwnedRegions();

    for (vector<string>::iterator it = ownedRegions->begin(); it != ownedRegions->end(); ++it) {
        cout << "{ " << *name << " } Owns < " << (*it) << " >." << endl;
    }

    *controlledRegions = int(ownedRegions->size());

    cout << "{ " << *name << " } Owns " << *controlledRegions << " regions." << endl;

    return *controlledRegions;
}

vector<string>* Player::getOwnedRegions() {
    vector<string>* ownedRegions = new vector<string>();

    Vertices::iterator it;
    for (it = countries->begin(); it != countries->end(); ++it) {
        Vertex* region = it->second;

        if (region->getRegionOwner() == *name) {
            ownedRegions->push_back(region->getName());
        }
    }

    return ownedRegions;
}

/**
 * Calculates the total owned continent for a player. A continent is considered owned
 * if the player has the most controlled regions on a continent. If two players have
 * the same count, then no one owns the continents.
 *
 * @return The total number of owned continents.
 */
int Player::computeContinentScore() {
    vector<string>* ownedContinents = getOwnedContinents();

    for(vector<string>::iterator it = ownedContinents->begin(); it != ownedContinents->end(); ++it) {
        cout << "{ " << *name << " } Owns continent " << (*it) << "." << endl;
    }

    cout << "{ " << *name << " } Owns " << ownedContinents->size() << " continents." << endl;

    int numOwnedContinents = ownedContinents->size();
    delete ownedContinents;

    return numOwnedContinents;
}

vector<string>* Player::getOwnedContinents() {

    vector<set<string>* > continents = GameMap::instance()->getContinents();
    vector<string>* ownedContinents = new vector<string>();

     //Iterate through each continent and get the owner.
    for(vector<set<string>* >::iterator it = continents.begin(); it != continents.end(); ++it) {

        set<string>* continent = *it;
        string continentName = GameMap::instance()->getVertices()->find(*continent->begin())->second->getContinent();
        string owner = GameMap::instance()->getContinentOwner(continent);

        if(owner == *name) {
            ownedContinents->push_back(continentName);
        }
    }

    for ( set<string>* c : continents) {
        delete c;
    }

    return ownedContinents;
}

/**
 * Computes the victory points for the player's set of cards.
 *
 * WILD goods cards can be added to any goods cards a player alreay owns for one
 * extra of that good per WILD card.
 *
 * @return The total victory points for each card type.
 */
int Player::computeGoodsScore() {
    unordered_map<string, int>* goodsCount = getGoodsCount();

    findAndDistributeWildCards(goodsCount);

    int points = getVPFromGoods(goodsCount);

    for(unordered_map<string, int>::iterator it = goodsCount->begin(); it != goodsCount->end(); ++it) {
        cout << "{ " << *name << " } Owns " << it->second << " " << it->first << " cards." << endl;
    }

    cout << "{ " << *name << " } Has " << points << " card points." << endl;

    return points;
}

int Player::getVPFromGoods(unordered_map<string, int>* goodsCount) {
                                    // 0 1 2 3 4 5 6 7 8
    int woodValuePerCardCount[]     = {0,0,1,1,2,3,5,5,5};
    int ironValuePerCardCount[]     = {0,0,1,1,2,2,3,5,5};
    int carrotValuePerCardCount[]   = {0,0,0,1,1,2,2,3,5};
    int stoneValuePerCardCount[]    = {0,0,1,2,3,5,5,5,5};
    int gemValuePerCardCount[]      = {0,1,2,3,5,5,5,5,5};

    int points = 0;

    for(unordered_map<string, int>::iterator it = goodsCount->begin(); it != goodsCount->end(); ++it) {
        if (it->first == GEM)
            points += gemValuePerCardCount[it->second];
        else if(it->first == WOOD)
            points += woodValuePerCardCount[it->second];
        else if(it->first == IRON)
            points += ironValuePerCardCount[it->second];
        else if(it->first == STONE)
            points += stoneValuePerCardCount[it->second];
        else if(it->first == CARROT)
            points += carrotValuePerCardCount[it->second];
        else if(it->first == WILD)
            ;//Do nothing
        else
            cout << "[ ERROR! ] Invalid card type. " << it->first << endl;
    }

    return points;
}

unordered_map<string, int>* Player::getGoodsCount() {
    unordered_map<string, int>* goodsCount = new unordered_map<string, int>();

    for(vector<Card*>::iterator it = hand->begin(); it != hand->end(); ++it) {
        string good = (*it)->getGood();

        // Check if card has double good
        size_t spaceCharIndex = good.find(" ");

        int countIncrement = 1;
        if (spaceCharIndex != size_t(-1)) {
            countIncrement = 2;
            good = good.substr(0, spaceCharIndex);
        }

        if (goodsCount->find(good) != goodsCount->end()) {
            int count = goodsCount->find(good)->second;
            goodsCount->erase(good);
            goodsCount->insert(pair<string,int>(good,count+countIncrement));
        } else
            goodsCount->insert(pair<string,int>(good,countIncrement));
    }

    return goodsCount;
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
        int numArmies = country->getArmies()->find(playerEntry)->second;
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
        int numCities = country->getCities()->find(playerEntry)->second;
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
    cout << "{ " << *name << " } Added card { " << card->getGood() << " : \"" << card->getAction() << "\" } to hand.\n" << endl;
    hand->push_back(card);
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
        if (country->getArmies()->find(playerEntry) != country->getArmies()->end())
            numArmies = country->getArmies()->find(playerEntry)->second;
        if (country->getCities()->find(playerEntry) != country->getCities()->end())
            numCities = country->getCities()->find(playerEntry)->second;

        //Only remove the country if the player has 0 armies and 0 cities on the country.
        if (numArmies == 0 && numCities == 0) {
            country->getArmies()->erase(playerEntry);
            country->getCities()->erase(playerEntry);
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

    cout << "\n{ " << *name << " } Occupied Countries: [ " << *colour << " ]\n" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    Vertices::iterator it;
    for(it = countries->begin(); it != countries->end(); ++it) {
        int numArmies = 0;
        int numCities = 0;
        if (it->second->getArmies()->find(playerEntry) != it->second->getArmies()->end())
            numArmies = it->second->getArmies()->find(playerEntry)->second;
        if (it->second->getCities()->find(playerEntry) != it->second->getCities()->end())
            numCities = it->second->getCities()->find(playerEntry)->second;

        printf("\t%-3s : %-20s ARMIES: %-5d CITIES: %-5d\n", it->second->getKey().c_str(), it->second->getName().c_str(), numArmies, numCities);
    }

    cout << "--------------------------------------------------------------------\n" << endl;
}

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

    if (country->getArmies()->find(playerEntry) == country->getArmies()->end()) {
        //Country doesn't have any of Player's armies. Create a new record.
        country->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, numArmies));
    } else {
        //Country already has some of Player's armies. Add to current number of armies.
        int currentArmies = country->getArmies()->find(playerEntry)->second;
        country->getArmies()->erase(playerEntry);
        country->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, currentArmies + numArmies));
    }
}

/**
 * Removes armies from a country.
 *
 * @param country A Vertex pointer to the target country.
 * @param numArmies The number of armies to remove from country.
 */
void Player::removeArmiesFromCountry(Vertex* country, const int& numArmies) {
    int currentArmies = country->getArmies()->find(playerEntry)->second;

    // erase current record
    country->getArmies()->erase(playerEntry);

    // only update army count if there are still armies left on country
    if (currentArmies - numArmies > 0)
        country->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, currentArmies - numArmies));
    // remove country if resulting num armies is 0 and no cities exist
    else if (numArmies == currentArmies && country->getCities()->find(playerEntry) == country->getCities()->end())
        removeCountry(country);
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
    if (country->getKey() == start || country->getCities()->find(playerEntry) != country->getCities()->end()){
        if (newArmies > *armies) {
            cout << "{ " << *name << " } doesn't have enough armies to place " << newArmies << " new armies on < " << country->getName() << " >." << endl;
            cout << "{ " << *name << " } placing " << *armies << " instead." << endl;
            addArmiesToCountry(country, *armies);
            *armies  = 0;
        } else {
            addArmiesToCountry(country, newArmies);
            *armies -= newArmies;
        }

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
        if (start->getArmies()->find(playerEntry) != start->getArmies()->end()) {
            int currentArmies = start->getArmies()->find(playerEntry)->second;
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

    cout << "[ ERROR! ] < " << end->getName() << " > is not an adjacent country." << endl;
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
    unordered_map<PlayerEntry*, int>* armies = country->getArmies();

    //does country belong to the player and does it contain at least one army?
    if (armies->find(playerEntry) != armies->end()) {
        if (armies->find(playerEntry)->second > 0) {

            //If country contains a player owned city, increase the count
            int currentCities = 1;
            if (country->getCities()->find(playerEntry) != country->getCities()->end()) {
                currentCities += country->getCities()->find(playerEntry)->second;
                country->getCities()->erase(playerEntry);
                country->getCities()->insert(pair<PlayerEntry*,int>(playerEntry, currentCities));
            } else //Else insert new record
                country->getCities()->insert(pair<PlayerEntry*, int> (playerEntry, currentCities));

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

    PlayerEntry* opponentPlayerEntry = opponent->getPlayerEntry();
    //Does opponent country contain an army to destroy?
    if (country->getArmies()->find(opponentPlayerEntry) != country->getArmies()->end()
        && country->getArmies()->find(opponentPlayerEntry)->second > 0) {

        int currentArmies = country->getArmies()->find(opponentPlayerEntry)->second;
        currentArmies--;

        //Add destroyed army back in opponents available armies
        opponent->increaseAvailableArmies(1);

        //Remove old record of opponent's armies on country
        country->getArmies()->erase(opponentPlayerEntry);

        if (currentArmies != 0)
            //Insert new record with decremented army count
            country->getArmies()->insert(pair<PlayerEntry*, int> (opponentPlayerEntry, currentArmies));
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

/**
 * Puts coins taken from the game coin supply into the player's purse.
 *
 * @param numCoins Number of coins to add to purse from the game coin supply.
 */
void Player::fillPurseFromSupply(const int& numCoins) {
    cout << "{ " << *name << " } Added " << numCoins << " to purse from coin supply." << endl;
    *coins += numCoins;
}

//PRIVATE
void Player::findAndDistributeWildCards(unordered_map<string, int>* goodsCount) {
    if (goodsCount->find(WILD) != goodsCount->end()) {
        int numWildCards = goodsCount->find(WILD)->second;
        goodsCount->erase(WILD);

        cout << "{ " << *name << " } You have " << numWildCards << " WILD cards!" << endl;

        for (int i = 0; i < numWildCards; i++) {
            while(true) {
                string resource;

                cout << "\n{ " << *name << " } Choose which resource to add the WILD card to ( " << numWildCards - i << " WILD cards left ):" << endl;

                for (unordered_map<string, int>::iterator it  = goodsCount->begin(); it != goodsCount->end(); ++it)
                    cout << "{ " << *name << " } You have " << it->second << " " << it->first << " cards." << endl;

                cout << "{ " << *name << " } > ";

                getline(cin, resource);
                transform(resource.begin(), resource.end(), resource.begin(), ::toupper);

                if (goodsCount->find(resource) != goodsCount->end()) {
                    cout << "{ " << *name << " } Adding 1 count of " << resource << "." << endl;
                    int count = goodsCount->find(resource)->second;
                    goodsCount->erase(resource);
                    goodsCount->insert(pair<string,int>(resource,count+1));
                    break;
                }

                cout << "\n[ ERROR! ] Invalid resource name. Please choose a resource that you own.\n" << endl;
            }
        }
    }
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

void Player::setStrategy(Strategy* newStrategy) {
    delete strategy;
    strategy = newStrategy;
}