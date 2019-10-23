#include "GameStartUp.h"


/*Eight-Minute Empire game. This phase is composed of the following sequence:
1. Shuffling the cards of the deck, and draw six cards, that will be put face up along the top
of the board.
2. Each player takes a set of one color of cubes (14 armies) and discs (3 cities). Each player
places 3 armies on the starting region on the board. If playing with 2 players, each player
takes turns placing one army at a time of a third, non-player color in any region on the
board until ten armies have been placed.
3. Player places the coin tokens in a pile next to the board. This is the supply.
*if playing with five players, each player takes 8 coins
*if playing with four players, each player takes 9 coins
*if playing with three players, each player takes 11 coins
*if playing with two players, each player takes 14 coins.
4. Players bid (using the biding facility implemented in Assignemnt#1). The winner of the
bid will be chosen to start to play and the game will be going in clockwise order. If the
bids are tied for most, the youngest player wins the bid. If all bids are zero, the youngest
*/

/**
 * Default Constructor
 */
GameStartUpEngine::GameStartUpEngine(): 
    initPhase(new GameInitEngine()), nextTurn(new queue<Player*>()) {}

/**
 * Takes an GameInitEngine object.
 */
GameStartUpEngine::GameStartUpEngine(GameInitEngine* gameInit): 
    initPhase(gameInit), nextTurn(new queue<Player*>()) {}

/**
 * Copy Constructor
 */
GameStartUpEngine::GameStartUpEngine(GameStartUpEngine* otherStartUpEngine){
    initPhase = new GameInitEngine(otherStartUpEngine->getGameInitEngine());
    nextTurn = new queue<Player*>(*otherStartUpEngine->getNextTurnQueue());
}

/**
 * Assigment operator
 */
GameStartUpEngine& GameStartUpEngine::operator=(GameStartUpEngine& otherStartUpEngine) {
    initPhase = new GameInitEngine(otherStartUpEngine.getGameInitEngine());
    nextTurn = new queue<Player*>(*otherStartUpEngine.getNextTurnQueue());
    return *this;
}

/**
 * Deconstructor
 */
GameStartUpEngine::~GameStartUpEngine(){
    delete initPhase;
    delete nextTurn;

    initPhase = nullptr;
    nextTurn = nullptr;
}

/**
 * Assumes initPhase has already been initialized
 */
void GameStartUpEngine::startGame() {

    distributeCoins();
    placeArmiesOnStartVertex();

    Player* firstPlayer = Bidder::startBid(initPhase->getPlayers());
    setPlayerOrderInQueue(firstPlayer);

    //Each player places 3 armies on start region on board.
    //If 2 players, place 3 armies on start 
    //Distribute coins to players
    //Players bid
    //Rearrange player order to match bid.

}

void GameStartUpEngine::distributeCoins() {
    int coins = 18 - initPhase->getNumPlayers() * 2;
    if (initPhase->getNumPlayers() == 3 || initPhase->getNumPlayers() == 4)
        coins--;
}

void GameStartUpEngine::placeArmiesOnStartVertex() {

}

void GameStartUpEngine::setPlayerOrderInQueue(Player* firstPlayer) {

}

