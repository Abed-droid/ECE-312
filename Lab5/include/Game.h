#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Room.h"
#include <map>
#include <string>

/**
 * Game class - Main game controller
 * 
 * Manages:
 * - Player character
 * - Game world (all rooms)
 * - Current room location
 * - Game state (game over, victory)
 * - Command processing
 * - Combat system
 * 
 * LEARNING OBJECTIVES:
 * - Complex object lifetime management
 * - Map for world structure
 * - Command parsing and dispatch
 * - Game loop implementation
 */
class Game {
private:
    Player* player;
    Room* current_room;
    std::map<std::string, Room*> world;  // All rooms keyed by name - Game owns these!
    bool game_over;
    bool victory;
    
    // Private helper methods - command handlers
    // TODO: Implement these in Game.cpp
    void processCommand(const std::string& command);
    void move(const std::string& direction);
    void look();
    void attack();
    void pickupItem(const std::string& item_name);
    void inventory();
    void useItem(const std::string& item_name);
    void equip(const std::string& item_name);
    void help();
    
    // Combat system
    // TODO: Implement in Game.cpp
    void combat(Monster* monster);
    
public:
    // Constructor
    // TODO: Implement in Game.cpp
    Game();
    
    // Destructor - CRITICAL for memory management!
    // TODO: Implement in Game.cpp
    // HINTS:
    // - Must delete player
    // - Must delete all rooms in world map
    ~Game();
    
    // Game initialization
    // TODO: Implement these in Game.cpp
    void initializeWorld();
    void createStartingInventory();
    
    // Main game loop
    // TODO: Implement in Game.cpp
    void run();
    
    // World building helpers
    // TODO: Implement these in Game.cpp
    void addRoom(Room* room);
    void connectRooms(const std::string& room1_name, const std::string& direction,
                     const std::string& room2_name);
};

#endif // GAME_H
