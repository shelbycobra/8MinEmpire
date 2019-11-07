#ifndef GAME_OBSERVERS_H
#define GAME_OBSERVERS_H

/*
Using the Observer design pattern, implement a view that displays information happening in the
current turn. It should first display a header showing what player and what is currently being
played, e.g. “Player 2: select the third card from the left, he would pay one coin” or “Player 3:
move armies the amount of movement the card gives him ” Then it should display important
information related to what is happening in this turn, which should be different depending on
which player turn. This should dynamically be updated as the game goes through different
players and be visible at all times during game play. The Observer and Observable classes code
must be implemented in a new GameObservers.cpp/GameObservers.h file duo (same as for
Part 3).


Using the Observer design pattern, implement a view that displays some useful statistics about
the game, the minimum being a “player #’ location, and his procession (e.g. victory points, cities,
continents, etc.) view” that shows using some kind of bar graph depicting what city/continent is
currently being controlled by each player. This should dynamically be updated as the map state
changes and be visible at all times during game play.
You must deliver a driver that demonstrates that (1) the game statistics view updates itself every
time a city has been conquered by a player; (2) the game statistics updates itself when a player for
example has been removed from any location by an opponent, or has built a city on the view; (3)
as soon as a player owns a required number of cards, the game statistics view updates itself and
displays a celebratory message with each player scoring..
The Observer and Observable classes code must be implemented in a new
GameObservers.cpp/GameObservers.h file duo (same as for Part 2).
*/

#endif