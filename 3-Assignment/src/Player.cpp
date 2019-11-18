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
    regions(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)),
    colour(new string("none")),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0)),
    strategy(new HumanStrategy()) {}

/**
 * Initializes a Player object.
 *
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of regions, an empty hand and a Bidder object.
 *
 * @param playerName The name of the player.
 * @param theColour The colour of game pieces.
 */
Player::Player(const string &playerName, const string& theColour):
    name(new string(playerName)),
    regions(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(0)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)),
    colour(new string(theColour)),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0)),
    strategy(new HumanStrategy())
{
    cout << "\n{ " << *name << " } CREATED. [ " << *colour << " ] (Purse = 0)." << endl;
}

/**
 * Initializes a Player object.
 *
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of regions, an empty hand and a Bidder object.
 *
 * @param playerName The name of the player.
 * @param startCoins The number of coins the player starts with.
 */
Player::Player(const string& playerName, const int& startCoins):
    name(new string(playerName)),
    regions(new Vertices()),
    armies(new int(14)),
    cities(new int(3)),
    coins(new int(startCoins)),
    hand(new vector<Card*>()),
    bidder(new Bidder(this)),
    colour(new string("")),
    playerEntry(new PlayerEntry(*name, *colour)),
    controlledRegions(new int(0)),
    strategy(new HumanStrategy())
{
    cout << "{ " << *name << " } CREATED. (Purse = " << startCoins << ")." << endl;
}

/**
 * Initializes a Player object.
 *
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of regions, an empty hand and a Bidder object.
 *
 * @param playerName The name of the player.
 * @param theColour The colour of game pieces.
 * @param theStrategy The strategy the player will use during game play.
 */
Player::Player(const string &playerName, const string& theColour, Strategy* theStrategy):
    name(new string(playerName)),
    regions(new Vertices()),
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
    regions = new Vertices(*player->getOccupiedRegions());
    armies = new int(player->getArmies());
    cities = new int(player->getCities());
    coins = new int(player->getCoins());
    hand = new vector<Card*>(*player->getHand());
    bidder = new Bidder(player->getBidder());
    colour = new string(player->getColour());
    playerEntry = new PlayerEntry(player->getPlayerEntry()->first, player->getPlayerEntry()->second);
    controlledRegions = new int(player->getControlledRegions());
    strategy = player->getStrategy();
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
        delete regions;
        delete armies;
        delete cities;
        delete coins;
        delete hand;
        delete bidder;
        delete colour;
        delete playerEntry;
        delete controlledRegions;
        delete strategy;

        name = new string(player.getName());
        regions = new Vertices(*player.getOccupiedRegions());
        armies = new int(player.getArmies());
        cities = new int(player.getCities());
        coins = new int(player.getCoins());
        hand = new vector<Card*>(*player.getHand());
        bidder = new Bidder(player.getBidder());
        colour = new string(player.getColour());
        playerEntry = new PlayerEntry(player.getPlayerEntry()->first, player.getPlayerEntry()->second);
        controlledRegions = new int(player.getControlledRegions());
        strategy = player.getStrategy();
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
    delete regions;
    delete armies;
    delete cities;
    delete coins;
    delete hand;
    delete bidder;
    delete colour;
    delete playerEntry;
    delete controlledRegions;
    delete strategy;

    name = nullptr;
    regions = nullptr;
    armies = nullptr;
    cities = nullptr;
    coins = nullptr;
    hand = nullptr;
    bidder = nullptr;
    colour = nullptr;
    playerEntry = nullptr;
    controlledRegions = nullptr;
    strategy = nullptr;
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
 * The player can build a city on a region where they currently have armies.
 *
 */
void Player::BuildCity() {
    strategy->BuildCity(this);
}

/**
 * Executes the action "Moves # armies".
 *
 * @param action The action to be executed.
 */
void Player::MoveOverLand(const string action, Players* players){
    MoveArmies(action, players);
}

/**
 * Executes the action "Move # armies over water".
 *
 * @param action The action to be executed.
 */
void Player::MoveOverWater(const string action, Players* players){
    MoveArmies(action, players);
}

/**
 * Executes the action "Move # armies" or "Move # armies over water".
 *
 * The player can move armies around the map to adjacent regions as many times as the card dictates.
 *
 * @param action The action to be executed.
 */
void Player::MoveArmies(const string action, Players* players) {
    strategy->MoveArmies(this, action, players);
}

/**
 * Executes the action "Add # armies".
 *
 * The player can add armies to any region the player currently has a city or to the start region.
 *
 * @param action The action to be executed.
 * @param players A pointer to the players in the game.
 */
void Player::PlaceNewArmies(const string action, Players* players) {
    strategy->PlaceNewArmies(this, action, players);
}

/**
 * Executes the action "Destroy army".
 *
 * The player chooses an opponent's army to destroy on a region where the oponnent has an army.
 *
 * @param players A list of players in the game.
 */
void Player::DestroyArmy(Players* players) {
    strategy->DestroyArmy(this, players);
}

/**
 * Executes the action "<ACTION> AND <ACTION>" or "<ACTION> OR <ACTION>".
 * If the action contains "OR", it asks the player which action to take.
 *
 * @param action The action that contains an AND/OR double action
 * @param players A pointer to a map of Player pointers and their names.
 */
void Player::AndOrAction(const string action, Players* players) {
    strategy->AndOrAction(this, action, players);
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
    vector<string>* ownedRegions = getRegions();

    for (vector<string>::iterator it = ownedRegions->begin(); it != ownedRegions->end(); ++it) {
        cout << "{ " << *name << " } Owns < " << (*it) << " >." << endl;
    }

    *controlledRegions = int(ownedRegions->size());

    cout << "{ " << *name << " } Owns " << *controlledRegions << " regions." << endl;

    return *controlledRegions;
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

/**
 * Gets a list of regions names that the player owns.
 * @return A pointer to a vector of strings.
 */
vector<string>* Player::getRegions() {
    vector<string>* ownedRegions = new vector<string>();

    Vertices::iterator it;
    for (it = regions->begin(); it != regions->end(); ++it) {
        Vertex* region = it->second;

        if (region->getRegionOwner() == *name) {
            ownedRegions->push_back(region->getName());
        }
    }

    return ownedRegions;
}

/**
 * Gets a list of continent names that the player owns.
 * @return A pointer to a vector of strings.
 */
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
 * Returns the number of victory points calculated from the cards the player holds in their hand.
 *
 * @param goodsCount An unordered_map<string,int> pointer that maps the name of the good to how many.
 * @return The number of victory points.
 */
int Player::getVPFromGoods(unordered_map<string, int>* goodsCount) {
                                    // 0 1 2 3 4 5 6 7 8 9 0 1 2 3
    int woodValuePerCardCount[]     = {0,0,1,1,2,3,5,5,5,5,5,5,5,5};
    int ironValuePerCardCount[]     = {0,0,1,1,2,2,3,5,5,5,5,5,5,5};
    int carrotValuePerCardCount[]   = {0,0,0,1,1,2,2,3,5,5,5,5,5,5};
    int stoneValuePerCardCount[]    = {0,0,1,2,3,5,5,5,5,5,5,5,5,5};
    int gemValuePerCardCount[]      = {0,1,2,3,5,5,5,5,5,5,5,5,5,5};

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
        else if(it->first == NONE)
            ;//Do nothing
        else
            cout << "[ ERROR! ] Invalid card type. " << it->first << endl;
    }

    return points;
}

/**
 * Returns a mapping of the type of good to how many the player has in their hand.
 * @return unordered_map<string, int> pointer.
 */
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
 * Detects if a region is adjacent to at least one the player's currently
 * occupied regions.
 *
 * @param target A Vertex pointer to the target region.
 * @param overWaterAllowed A boolean representing if the region is allowed to be across a water edge.
 * @return a boolean representing if the region is adjacent.
 */
bool Player::isAdjacent(Vertex* target, const bool& overWaterAllowed){
    return isAdjacent(target->getName(), overWaterAllowed);
}

/**
 * Detects if a region is adjacent to at least one the player's currently
 * occupied regions.
 *
 * @param target The target region name.
 * @param overWaterAllowed A boolean representing if the region is allowed to be across a water edge.
 * @return a boolean representing if the region is adjacent.
 */
bool Player::isAdjacent(const string& target, const bool& overWaterAllowed){
    typedef pair<Vertex*, bool> Edge;

    Vertices::iterator it = regions->begin();
    set<string> visited;

    while(it != regions->end()) {

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
 * Gets the number of armies on a region occupied by the player.
 *
 * @param region A Vertex pointer to the target region.
 * @return the number of armies on the region.
 */
int Player::getArmiesOnRegion(Vertex* region) {
    if(regions->find(region->getKey()) != regions->end()) {
        int numArmies = region->getArmies()->find(playerEntry)->second;
        string army = numArmies == 1 ? "army" : "armies";
        cout << "{ " << *name << " } Has " << numArmies << " " << army << " on region < " << region->getName() << " >." << endl;
        return numArmies;
    }
    cout << "{ " << *name << " } Has ZERO armies on region < " << region->getName() << " >." << endl;
    return 0;
}

/**
 * Gets the number of cities on a region occupied by the player.
 *
 * @param region A Vertex pointer to the target region.
 * @return the number of cities on the region.
 */
int Player::getCitiesOnRegion(Vertex* region){
    if(regions->find(region->getKey()) != regions->end()) {
        int numCities = region->getCities()->find(playerEntry)->second;
        string city = numCities == 1 ? "city" : "cities";
        cout << "{ " << *name << " } Has " << numCities << " " << city << " on region < " << region->getName() << " >." << endl;
        return numCities;
    }
    cout << "{ " << *name << " } Has ZERO cities on region < " << region->getName() << " >." << endl;
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
    cout << "{ " << *name << " } [ " << strategy->getType() << " ] Added card { " << card->getGood() << " : \"" << card->getAction() << "\" } to hand.\n" << endl;
    hand->push_back(card);
}

/**
 * Adds a region to the player's list of occupied regions.
 *
 * Preconditions: Region is a valid region and it satisfies all
 * required conditions specific to the card action chosen by the Player.
 *
 * @param region A Vertex pointer to the target region.
 */
void Player::addRegion(Vertex* region){
    regions->insert(pair<string, Vertex*> (region->getKey(), region));
    cout << "{ " << *name << " } [ " << strategy->getType() << " ] " << "Added region < " << region->getName() << " > to player's regions." << endl;
}

/**
 * Removes a region from the Player's list of occupied regions.
 *
 * Detects whether the region to be removed still contains armies or cities belonging to the Player.
 * If so, the region will not be removed and an error message will print out.
 *
 * @param region A Vertex pointer to the target region.
 */
void Player::removeRegion(Vertex* region) {
    if(regions->find(region->getKey()) != regions->end()) {
        int numArmies = 0, numCities = 0;

        //Get current number of armies and cities if they exist on the region.
        if (region->getArmies()->find(playerEntry) != region->getArmies()->end())
            numArmies = region->getArmies()->find(playerEntry)->second;
        if (region->getCities()->find(playerEntry) != region->getCities()->end())
            numCities = region->getCities()->find(playerEntry)->second;

        //Only remove the region if the player has 0 armies and 0 cities on the region.
        if (numArmies == 0 && numCities == 0) {
            region->getArmies()->erase(playerEntry);
            region->getCities()->erase(playerEntry);
            regions->erase(region->getKey());
            cout << "{ " << *name << " } " << "Removed < " << region->getName() << " >." << endl;
        } else
            cout << "[ ERROR! ] Can't remove < " << region->getName() << " >. " << *name << " still owns "
                << numArmies << " armies and " << numCities << " cities on it." << endl;
    } else {
        cout << "[ ERROR! ] " << *name << " doesn't have the region < " << region->getName() << " > available to remove." << endl;
    }
}

/**
 * Prints a list of the Player's occupied regions.
 */
void Player::printRegions(){

    cout << "\n{ " << *name << " } Occupied Regions: [ " << *colour << " ]\n" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    Vertices::iterator it;
    for(it = regions->begin(); it != regions->end(); ++it) {
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
 * Adds armies to a region.
 * Preconditions: The Player has enough free armies to add to this region and the region
 * has met the conditions of the card action chosen by the Player.
 *
 * Detects whether the region to be removed still contains armies or cities belonging to the Player.
 * If so, the region will not be removed and an error message will print out.
 *
 * @param region A Vertex pointer to the target region.
 * @param numArmies The number of armies to add.
 */
void Player::addArmiesToRegion(Vertex* region, const int& numArmies) {
    //Add region to the Player's list of occupied regions if it hasn't been added yet.
    if(regions->find(region->getKey()) == regions->end()) {
        addRegion(region);
    }

    if (region->getArmies()->find(playerEntry) == region->getArmies()->end()) {
        //Region doesn't have any of Player's armies. Create a new record.
        region->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, numArmies));
    } else {
        //Region already has some of Player's armies. Add to current number of armies.
        int currentArmies = region->getArmies()->find(playerEntry)->second;
        region->getArmies()->erase(playerEntry);
        region->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, currentArmies + numArmies));
    }
}

/**
 * Removes armies from a region.
 *
 * @param region A Vertex pointer to the target region.
 * @param numArmies The number of armies to remove from region.
 */
void Player::removeArmiesFromRegion(Vertex* region, const int& numArmies) {
    int currentArmies = region->getArmies()->find(playerEntry)->second;

    // erase current record
    region->getArmies()->erase(playerEntry);

    // only update army count if there are still armies left on region
    if (currentArmies - numArmies > 0)
        region->getArmies()->insert(pair<PlayerEntry*, int> (playerEntry, currentArmies - numArmies));
    // remove region if resulting num armies is 0 and no cities exist
    else if (numArmies == currentArmies && region->getCities()->find(playerEntry) == region->getCities()->end())
        removeRegion(region);
}

/**
 * Player places new armies on a region.
 *
 * Checks if the player has enough free armies to place on the region.
 * The region must either be the start region or contain a city.
 *
 * @param amount Number of armies to place.
 * @param region A Vertex pointer to the region on which to place armies.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeAddArmies(const int& newArmies, Vertex* region){
    string start = GameMap::instance()->getStartVertexName();

    if (region->getKey() == start || region->getCities()->find(playerEntry) != region->getCities()->end()){
        if (newArmies > *armies) {
            cout << "{ " << *name << " } [ " << strategy->getType() << " ] doesn't have enough armies to place " << newArmies << " new armies on < " << region->getName() << " >." << endl;
            cout << "{ " << *name << " } [ " << strategy->getType() << " ] placing " << *armies << " instead." << endl;
            addArmiesToRegion(region, *armies);
            *armies  = 0;
        } else {
            addArmiesToRegion(region, newArmies);
            *armies -= newArmies;
        }

        cout << "{ " << *name << " } [ " << strategy->getType() << " ] Added " << newArmies << " new armies to < "<< region->getName() << " >." << endl;
        return true;
    }
    cout << "[ ERROR! ] " << *name << " Chose an invalid region. It must be either START or contain one of the player's cities->" << endl;
    return false;
}

/**
 * Moves a certain number of armies over to an adjacent region not separated by water.
 *
 * Detects whether or not the region contains the Player's armies and if the region has
 * enough armies on it to move.
 *
 * @param numArmies Amount of armies to move.
 * @param start A Vertex pointer to the start region.
 * @param end A Vertex pointer to the end region.
 * @param moveOverWater A boolean representing if armies can move over water.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeMoveArmies(const int& numArmies, Vertex* start, Vertex* end, const bool& moveOverWater){
    //Assumes: region is a valid vertex on the map
    //is region an adjacent region to an occupied region?
    if (isAdjacent(end, moveOverWater)) {
        //does start vertex even have armies on it?
        if (start->getArmies()->find(playerEntry) != start->getArmies()->end()) {
            int currentArmies = start->getArmies()->find(playerEntry)->second;
            if (numArmies > currentArmies) {
                cout << "[ ERROR! ] " << *name << " doesn't have enough armies on < " << start->getName()
                     << " > to move " << numArmies << "." << endl;
                return false;
            }

            addArmiesToRegion(end, numArmies);
            removeArmiesFromRegion(start, numArmies);

            cout << "{ " << *name << " } [ " << strategy->getType() << " ] Moved " << numArmies
                 << " armies from < " << start->getName() << " > to < " << end->getName() << " >.\n" << endl;
            return true;
        }

        cout << "[ ERROR! ] " << *name << " doesn't have any armies on < " << start->getName() << " > to move." << endl;
        return false;
    }

    cout << "[ ERROR! ] < " << end->getName() << " > is not an adjacent region." << endl;
    return false;
}

/**
 * Builds a city on a region occupied by the Player.
 *
 * Detects whether or not the region contains any Player's armies.
 *
 * @param region A Vertex pointer to the region.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeBuildCity(Vertex* region){
    unordered_map<PlayerEntry*, int>* armies = region->getArmies();

    //does region belong to the player and does it contain at least one army?
    if (armies->find(playerEntry) != armies->end()) {
        if (armies->find(playerEntry)->second > 0) {

            //If region contains a player owned city, increase the count
            int currentCities = 1;
            if (region->getCities()->find(playerEntry) != region->getCities()->end()) {
                currentCities += region->getCities()->find(playerEntry)->second;
                region->getCities()->erase(playerEntry);
                region->getCities()->insert(pair<PlayerEntry*,int>(playerEntry, currentCities));
            } else //Else insert new record
                region->getCities()->insert(pair<PlayerEntry*, int> (playerEntry, currentCities));

            cout << "{ " << *name << " } [ " << strategy->getType() << " ] Added an city to < " << region->getName() << " >. (New city count = " << currentCities << ")." << endl;
            return true;
        }
    }

    cout << "[ ERROR! ] " << *name << " can't place city on < " << region->getName() << " > because the player has no armies on it." << endl;
    return false;
}

/**
 * Destroys an army belonging an opponent's region.
 *
 * Ensures the opponent is not the current player and that the chosen region
 * contains armies belonging to the opponent.
 *
 * @param region A Vertex pointer to the target region.
 * @param opponent A Player pointer to the opponent player.
 * @return a boolean that shows the action was successful.
 */
bool Player::executeDestroyArmy(Vertex* region, Player* opponent){

    if (opponent->getName() == *name) {
        cout << "\n[ ERROR! ] Can't destroy own army." << endl;
        return false;
    }

    PlayerEntry* opponentPlayerEntry = opponent->getPlayerEntry();
    //Does opponent region contain an army to destroy?
    if (region->getArmies()->find(opponentPlayerEntry) != region->getArmies()->end()
        && region->getArmies()->find(opponentPlayerEntry)->second > 0) {

        int currentArmies = region->getArmies()->find(opponentPlayerEntry)->second;
        currentArmies--;

        //Add destroyed army back in opponents available armies
        opponent->increaseAvailableArmies(1);

        //Remove old record of opponent's armies on region
        region->getArmies()->erase(opponentPlayerEntry);

        if (currentArmies != 0)
            //Insert new record with decremented army count
            region->getArmies()->insert(pair<PlayerEntry*, int> (opponentPlayerEntry, currentArmies));
        else {
            //Remove region from opponent's list of occupied regions because the last army was destroyed.
            opponent->removeRegion(region);
        }

        cout << "{ " << *name << " } [ " << strategy->getType() << " ] Destroyed one of " << opponent->getName() << "'s armies on < " << region->getName() << " >." << endl;
        return true;
    }

    cout << "[ ERROR! ] " << opponent->getName() << " doesn't have any armies to destroy on < " << region->getName() << " >." << endl;
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
/**
 * Finds any wild cards the user may have in their hand and asks them to increment
 * another good that they own.
 */
void Player::findAndDistributeWildCards(unordered_map<string, int>* goodsCount) {
    if (goodsCount->find(WILD) != goodsCount->end()) {
        int numWildCards = goodsCount->find(WILD)->second;
        goodsCount->erase(WILD);

        cout << "{ " << *name << " } You have " << numWildCards << " WILD cards!" << endl;

        for (int i = 0; i < numWildCards; i++) {
            // Only ask to move Wild cards around if there the player owns other goods.
            while(goodsCount->size() > 1) {
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

/**
 * Sets the a new strategy for the user and deletes the old one.
 */
void Player::setStrategy(Strategy* newStrategy) {
    delete strategy;
    strategy = newStrategy;
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