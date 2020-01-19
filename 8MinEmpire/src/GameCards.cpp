#include "CardsFactory.h"
#include <iostream>

using namespace std;

/**
 * Factory method that creates a specific card object depending on the good.
 *
 * @param id The id of the card.
 * @param good The good of the card. Either STONE, GEM, CARROT, IRON, WILD, or WOOD.
 * @param action The action that the card performs.
 * @return A pointer to a Card.
 */
Card* CardsFactory::create(const int& id, const string& good, const string& action) {

    if (good.find(STONE) != size_t(-1))
        return new StoneCard(id, good, action);
    if (good.find(GEM) != size_t(-1))
        return new GemCard(id, good, action);
    if (good.find(CARROT) != size_t(-1))
        return new CarrotCard(id, good, action);
    if (good.find(IRON) != size_t(-1))
        return new IronCard(id, good, action);
    if (good.find(WILD) != size_t(-1))
        return new WildCard(id, good, action);
    if (good.find(WOOD) != size_t(-1))
        return new WoodCard(id, good, action);

    return nullptr;
}

/**
 * Default Constructor
 */
StoneCard::StoneCard(): Card(0, STONE, NONE) {}

/**
 * Parameterized Constructor
 */
StoneCard::StoneCard(const int& id, const string& good, const string& action): Card(id, good, action) {}

/**
 * Default Constructor
 */
GemCard::GemCard(): Card(0, GEM, NONE)  {}

/**
 * Parameterized Constructor
 */
GemCard::GemCard(const int& id, const string& good, const string& action): Card(id, good, action) {}

/**
 * Default Constructor
 */
CarrotCard::CarrotCard(): Card(0, CARROT, NONE) {}

/**
 * Parameterized Constructor
 */
CarrotCard::CarrotCard(const int& id, const string& good, const string& action): Card(id, good, action) {}

/**
 * Default Constructor
 */
IronCard::IronCard(): Card(0, IRON, NONE)  {}

/**
 * Parameterized Constructor
 */
IronCard::IronCard(const int& id, const string& good, const string& action): Card(id, good, action) {}

/**
 * Default Constructor
 */
WildCard::WildCard(): Card(0, WILD, NONE) {}

/**
 * Parameterized Constructor
 */
WildCard::WildCard(const int& id, const string& good, const string& action): Card(id, good, action){}

/**
 * Default Constructor
 */
WoodCard::WoodCard(): Card(0, WOOD, NONE) {}

/**
 * Parameterized Constructor
 */
WoodCard::WoodCard(const int& id, const string& good, const string& action): Card(id, good, action) {}
