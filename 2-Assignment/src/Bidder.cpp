#include "Bidder.h"

/**
 * The Bidder object is initialized with a pointer to the Player who owns the Bidder object
 * and the boolean madeBid to false.
 * 
 * The boolean madeBid is used to prevent additional bids after the first one is made by the Player.
 */
Bidder::Bidder(Player* player): 
    madeBid(new bool(false)), 
    player(player) {}

Bidder::Bidder(): 
    madeBid(new bool(false)),
    player(new Player()) {}

Bidder::Bidder(Bidder* bidder) {
    madeBid = new bool(bidder->getMadeBid());
    player = new Player(bidder->getPlayer());
}

Bidder& Bidder::operator=(Bidder& bidder) {
    madeBid = new bool(bidder.getMadeBid());
    player = new Player(bidder.getPlayer());
    return *this;
}

Bidder::~Bidder(){
    delete madeBid;
    delete player;

    madeBid = nullptr;
    player = nullptr;
}

//GETTERS
bool Bidder::getMadeBid(){return *madeBid;}
Player* Bidder::getPlayer(){return player;}

/**
 * Prompts the Player to make a bid from their purse.
 * If the Player has enough coins to make the bid, the
 * madeBid boolean is set to true.
 */
int Bidder::bid() {
    int bid = -1;
    string bidStr;

    if (!*madeBid) {

        while(true) {

            cout << "[ BIDDING ] " << player->getName() << ", please select your bid."
                    << " You have " << player->getCoins() << " coins." << endl;
            cout << "[ BIDDING ] > ";
            getline(cin, bidStr);

            stringstream toInt(bidStr);
            toInt >> bid;

            if (bid <= player->getCoins()) 
                break;

            cout << "[ ERROR! ] You don't have enough coins to make that big. Please try again." << endl; 

        }
    } else {
        cout << "[ ERROR! ] A bid has already been made for " << player->getName() << endl;
    }

    *madeBid = true;

    return bid;
}

/**
 * A class function that starts the bidding process for a group of players.
 * 
 * @param players A pointer to a vector of Player pointers representing all the players in the game.
 */
void Bidder::startBid(Players* players) {
    cout << "\n[ BIDDER ] STARTING BID!\n" << endl;

    unordered_map<Player*, int>* bids = new unordered_map<Player*, int>();

    Players::iterator it;
    for(it = players->begin(); it != players->end(); ++it){
        int bid = it->second->getBidder()->bid();

        bids->insert(pair<Player*, int>(it->second, bid));
    }

    Player* winner = Bidder::calculateWinner(bids, players);

    winner->payCoins(bids->find(winner)->second);

    delete bids;
    bids = nullptr;
 }

/**
 * A class function that calculates the winner from the bids of the players in the game.
 * 
 * @param bids An unordered_map pointer that maps Players to their bids.
 * @param players A pointer to a vector of Player pointers representing all the players in the game.
 * @return The player who won the bid.
 */
Player* Bidder::calculateWinner(unordered_map<Player*, int>* bids, Players* players) {

    cout << "\n[ BIDDER ] Finding winning bid ... \n\n";

    int max = -1;
    Player* winner;

    unordered_map<Player*, int>::iterator it;
    for(it = bids->begin(); it != bids->end(); ++it) {
        if (it->second > max) {
            winner = it->first;
            max = it->second;
        }
    }

    //check for duplicate bids
    for(it = bids->begin(); it != bids->end(); ++it) {

        if (it->second == max && it->first != winner) {
            cout << "\n[ BIDDER ] There is a tie between " << winner->getName() << " and "
                    << it->first->getName() << " for the highest bid of " << max << ".\n";
            while(true) {
                cout << "[ BIDDER ] Who is the younger player?\n";
                cout << "[ BIDDER ] > ";

                string youngerPlayer;
                getline(cin, youngerPlayer);

                if (youngerPlayer == winner->getName() || youngerPlayer == it->first->getName()) {
                    winner = players->find(youngerPlayer)->second;
                    break;
                } else {
                    cout << "[ BIDDER ] That name is invalid. Please choose between " << winner->getName() << " and "
                    << it->first->getName() << " for the highest bid of " << max << ".\n";
                }
            }
        }
    }

    cout << "\n[ BIDDER ] " << winner->getName() << " has won the bid!\n" << endl;

    return winner;
}