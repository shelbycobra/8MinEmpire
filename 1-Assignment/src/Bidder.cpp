/*
Implement a group of C++ classes that implement a biding facility to be used during start of the
game to see who will start first. There is only one bid per game. The biding consists of each player
picking up his coins and privately chooses a number to bid. When all players are ready, all players
reveal the amount they have chosen to bid at the same time. The player who bids the most coins
wins the bid and puts the coins he bid in the supply. Other players do not pay coins if they lost
the bid. If the bids are tied for most, the youngest player wins the bid and pays his coins. If all
bids are zero, the youngest player wins the bid. . You must deliver a driver that creates the biding
facility objects, with the following tests: 1) one can shows a player who bid the most coins wins,
2) that show bids that are tied and the youngest win the bid, 3) one that if all bids are zero the
youngest player win.
*/

#include "Bidder.h"
#include <unistd.h>

Bidder::Bidder(Player* player): madeBid(new bool(false)), player(player) {}

Bidder::~Bidder(){
    delete madeBid;
    delete player;

    madeBid = NULL;
    player = NULL;
}

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
 }

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

            break;
        }
    }

    cout << "\n[ BIDDER ] " << winner->getName() << " has won the bid!\n" << endl;

    return winner;

}