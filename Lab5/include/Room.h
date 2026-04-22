#ifndef ROOM_H
#define ROOM_H

#include "Monster.h"
#include "Item.h"
#include <string>
#include <vector>
#include <map>

/**
 * Room class - Represents a location in the game world
 * 
 * Rooms can contain:
 * - A monster (blocking progress)
 * - Items on the ground
 * - Exits to other rooms (stored in a map)
 * 
 * LEARNING OBJECTIVES:
 * - Use std::map for key-value storage
 * - Manage complex object ownership
 * - Understand composition (Room HAS-A Monster, not IS-A Monster)
 */
class Room {
private:
    std::string name;
    std::string description;
    bool visited;
    
    // Room contents
    Monster* monster;          // NULL if no monster - Room owns this!
    std::vector<Item*> items;  // Items on ground - Room owns these!
    
    // Connections to other rooms
    // Key: direction ("north", "south", "east", "west")
    // Value: pointer to connected Room
    // NOTE: Room does NOT own these - Game owns all rooms!
    std::map<std::string, Room*> exits;
    
public:
    // Constructor
    // TODO: Implement in Room.cpp
    Room(const std::string& name, const std::string& description);
    
    // Destructor - CRITICAL for memory management!
    // TODO: Implement in Room.cpp
    // HINTS:
    // - Must delete monster if present
    // - Must delete all items
    // - DON'T delete rooms in exits map (Game owns those)
    ~Room();
    
    // Display room information
    // TODO: Implement these in Room.cpp
    void display() const;
    void displayExits() const;
    
    // Room connections
    // TODO: Implement these in Room.cpp
    void addExit(const std::string& direction, Room* room);
    Room* getExit(const std::string& direction) const;
    bool hasExit(const std::string& direction) const;
    
    // Monster management
    void setMonster(Monster* m) { monster = m; }
    Monster* getMonster() { return monster; }
    bool hasMonster() const { return monster != NULL && monster->isAlive(); }
    
    // TODO: Implement in Room.cpp
    void clearMonster();
    
    // Item management
    // TODO: Implement these in Room.cpp
    void addItem(Item* item);
    void removeItem(const std::string& item_name);
    void displayItems() const;
    Item* getItem(const std::string& item_name);
    bool hasItems() const { return !items.empty(); }
    
    // Getters/Setters
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    bool isVisited() const { return visited; }
    void markVisited() { visited = true; }
};

#endif // ROOM_H
