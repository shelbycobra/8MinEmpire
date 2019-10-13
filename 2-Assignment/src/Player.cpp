#include "Player.h"
#include "Map.h"
#include "Cards.h"

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

/**
 * Initializes a Player object.
 * 
 * Each player starts with 14 free armies and 3 free cities to place on the map during game play.
 * The player is initialized with an empty list of countries, an empty hand and a Bidder object.
 * 
 * @param playerName The name of the player.
 * @param startCoins The number of coins the player starts with.
 */
Player::Player(string& playerName, int startCoins):
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

    name = nullptr;
    countries = nullptr;
    armies = nullptr;
    cities = nullptr;
    coins = nullptr;
    hand = nullptr;
}

// GETTERS
string Player::getName(){return *name;}
Vertices* Player::getCountries(){return countries;}
int Player::getArmies(){return *armies;}
int Player::getCities(){return *cities;}
int Player::getCoins(){return *coins;}
vector<Card*>* Player::getHand(){return hand;}
Bidder* Player::getBidder() {return bidder;}

/**
 * Takes coins out of the Player's purse.
 *
 * Checks if the player has enough coins to pay "amount" of coins
 *
 * @param amount Integer amount of coins to pay.
 * @return a boolean that shows the action was successful.
 */
bool Player::payCoins(int amount){
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
bool Player::placeNewArmies(int newArmies, Vertex* country, string start){
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
 * Detects if a country is adjacent to at least one the player's currently
 * occupied countries.
 *
 * @param target A Vertex pointer to the target country.
 * @param overWaterAllowed A boolean representing if the country is allowed to be across a water edge.
 * @return a boolean representing if the country is adjacent.
 */
bool Player::isAdjacent(Vertex* target, bool overWaterAllowed){
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
bool Player::isAdjacent(string target, bool overWaterAllowed){
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
 * Moves a certain number of armies over to an adjacent country not separated by water.
 *
 * Detects whether or not the country contains the Player's armies and if the country has
 * enough armies on it to move.
 *
 * @param numArmies Amount of armies to move.
 * @param start A Vertex pointer to the start country.
 * @param end A Vertex pointer to the end country.
 * @return a boolean that shows the action was successful.
 */
bool Player::moveOverLand(int numArmies, Vertex* start, Vertex* end){
    return moveArmies(numArmies, start, end, false);
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
bool Player::moveArmies(int numArmies, Vertex* start, Vertex* end, bool moveOverWater){
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
bool Player::buildCity(Vertex* country){
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
bool Player::destroyArmy(Vertex* country, Player* opponent){

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

    cout << "--------------------------------------------------------" << endl;
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
void Player::addArmiesToCountry(Vertex* country, int numArmies) {
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
void Player::removeArmiesFromCountry(Vertex* country, int numArmies) {
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
void Player::increaseAvailableArmies(int numArmies) {
    *armies += numArmies;
}

//PRIVATE
/**
 * Removes free armies available to a Player.
 *
 * @param numArmies The number of armies to remove.
 */
void Player::decreaseAvailableArmies(int numArmies) {
    if (*armies - numArmies <= 0) {
        *armies = 0;
    } else {
        *armies -= numArmies;
    }
}