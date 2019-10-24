/*
Provide a group of C++ classes that implements the main game loop following the rules of the
Eight-Minute Empire. During the main game loop, starting with the first player and going in
clockwise order, players take turns taking one of the face-up cards. When a player takes a card,
he must pay the appropriate coin cost for the card depending on where it is in the row (this cost
is listed at the top of the board). From left to right, these are the coin costs of the cards: 0, 1, 1, 2,
2, and 3. For example, if a player selected the third card from the left, he would pay one coin.
Each card gives a good and an action. The player takes the action immediately. Actions allow
players to build their empires and take control of the board.
Requirements part 3: You must deliver a driver that demonstrates:
* Running of a game loop with each player turn starting with the first and going clockwise
order.
* Each player takes one face-up card and plays the appropriate cost for the card depending on
where it is in the row.
* A display of each player card’s action.
*/
#include "Player.h"
#include "GameStartUp.h"

#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

class GameMainEngine {

public:
    GameMainEngine();
    GameMainEngine(GameStartUpEngine* startUpEngine);
    GameMainEngine(GameMainEngine* otherGameMainEngine);
    GameMainEngine& operator=(GameMainEngine& otherGameMainEngine);
    ~GameMainEngine();

    void playTurn(Players* players, queue<Player*>* nextTurn, GameMap* map, Hand* gameHand);
    void performCardAction(Player* player, const string action, GameMap* map, Players* players);
};

#endif