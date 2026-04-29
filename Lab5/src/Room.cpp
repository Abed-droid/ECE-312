#include "Room.h"
#include <iostream>
#include <algorithm>

// Helper: case-insensitive lowercase conversion
static std::string toLower(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Constructor — PROVIDED
Room::Room(const std::string& name, const std::string& description)
    : name(name), description(description), visited(false), monster(NULL) {
}


// TODO 1: Implement Room destructor
// - Delete monster if it exists (check for NULL first!)
// - Delete every Item* in items, then clear the vector
// - Do NOT delete rooms in exits map — Game owns those
//
// Hint:
//     if (monster != NULL) { delete monster; }
//     for (int i = 0; i < (int)items.size(); i++) { delete items[i]; }
//     items.clear();
//
Room::~Room() {
    if (monster != NULL) {
        delete monster;
        monster = NULL;
    }
    for (int i = 0; i < (int)items.size(); i++) {
        delete items[i];
    }
    items.clear();
}


// TODO 2: Implement display
// Print a formatted room description.  Example output:
//
//   ========================================
//   Dungeon Entrance
//   ========================================
//   A dark stone corridor leads deeper...
//
//   A Goblin blocks your path!
//
//   Items here:
//     - Small Potion
//
//   Exits: north, south
//   ========================================
//
// Starter code:
//
void Room::display() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << name << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << description << std::endl;

    if (hasMonster()) {
        std::cout << "\nA " << monster->getName() << " blocks your path!" << std::endl;
    }

    if (hasItems()) {
        std::cout << "\nItems here:" << std::endl;
        displayItems();
    }

    std::cout << std::endl;
    displayExits();
    std::cout << "========================================\n" << std::endl;
}


// TODO 3: Implement displayExits
// Iterate through the exits map and print each direction, comma-separated.
// Example: "Exits: north, south, east"
//
// Hint — map iteration with iterators (C++98 style):
//
//   std::cout << "Exits: ";
//   bool first = true;
//   for (std::map<std::string, Room*>::const_iterator it = exits.begin();
//        it != exits.end(); ++it) {
//       if (!first) std::cout << ", ";
//       std::cout << it->first;   // the direction string
//       first = false;
//   }
//   std::cout << std::endl;
//
void Room::displayExits() const {
    std::cout << "Exits: ";
    bool first = true;
    for (std::map<std::string, Room*>::const_iterator it = exits.begin();
         it != exits.end(); ++it) {
        if (!first) std::cout << ", ";
        std::cout << it->first;
        first = false;
    }
    std::cout << std::endl;
}


// addExit — PROVIDED
void Room::addExit(const std::string& direction, Room* room) {
    if (room != NULL) {
        exits[direction] = room;
    }
}


// TODO 4: Implement getExit
// Look up direction in the exits map.
// Return the Room* if found, NULL otherwise.
//
// Hint:
//   std::map<std::string, Room*>::const_iterator it = exits.find(direction);
//   if (it != exits.end()) return it->second;
//   return NULL;
//
Room* Room::getExit(const std::string& direction) const {
    std::map<std::string, Room*>::const_iterator it = exits.find(direction);
    if (it != exits.end()) return it->second;
    return NULL;
}


// TODO 5: Implement hasExit
// Return true if direction exists in exits map.
//
bool Room::hasExit(const std::string& direction) const {
    return exits.find(direction) != exits.end();
}


// TODO 6: Implement clearMonster
// If monster is not NULL, delete it and set the pointer to NULL.
//
void Room::clearMonster() {
    if (monster != NULL) {
        delete monster;
        monster = NULL;
    }
}


// addItem — PROVIDED
void Room::addItem(Item* item) {
    if (item != NULL) {
        items.push_back(item);
    }
}


// TODO 7: Implement removeItem
// Search items for a matching name (case-insensitive).
// If found, erase from vector but do NOT delete (ownership transfers to player).
//
void Room::removeItem(const std::string& item_name) {
    std::string target = toLower(item_name);
    for (int i = 0; i < (int)items.size(); i++) {
        if (toLower(items[i]->getName()) == target) {
            items.erase(items.begin() + i);
            return;  // don't delete — player now owns it
        }
    }
}


// displayItems — PROVIDED
void Room::displayItems() const {
    for (int i = 0; i < (int)items.size(); i++) {
        std::cout << "  - " << items[i]->getName() << std::endl;
    }
}


// TODO 8: Implement getItem
// Search items (case-insensitive). Return pointer if found, NULL otherwise.
//
Item* Room::getItem(const std::string& item_name) {
    std::string target = toLower(item_name);
    for (int i = 0; i < (int)items.size(); i++) {
        if (toLower(items[i]->getName()) == target) {
            return items[i];
        }
    }
    return NULL;
}
