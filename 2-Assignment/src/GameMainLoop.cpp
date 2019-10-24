#include "GameMainLoop.h"

GameMainEngine::GameMainEngine() {}
GameMainEngine::GameMainEngine(GameStartUpEngine* startUpEngine) {}
GameMainEngine::GameMainEngine(GameMainEngine* otherGameMainEngine) {}
GameMainEngine& GameMainEngine::operator=(GameMainEngine& otherGameMainEngine) {}
GameMainEngine::~GameMainEngine() {}

void GameMainEngine::runMainLoop() {}

/**
 * Executes the action of a card.
 * 
 * @param player A pointer to the Player object who is using the card.
 * @param action The action to be performed.
 * @param map A pointer to the GameMap object of the game.
 * @param players A pointer to the list of all players in the game.
 */
void GameMainEngine::performCardAction(Player* player, string& action, GameMap* map, Players* players) {
    if(action.find("OR") != size_t(-1) || action.find("AND") != size_t(-1))
        player->AndOrAction(action, map, players);
    if (action.find("Move") != size_t(-1)) {
        if (action.find("water") != size_t(-1))
            player->MoveOverLand(action, map);
        else
            player->MoveOverWater(action, map);
    }
    else if (action.find("Add") != size_t(-1))
        player->PlaceNewArmies(action, map);
    else if (action.find("Destroy") != size_t(-1))
        player->DestroyArmy(action, map, players);
    else if (action.find("Build") != size_t(-1))
        player->BuildCity(action, map);
    else 
        cout << "[ ERROR! ] Invalid action." << endl;
}