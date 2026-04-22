#include "Game.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor — PROVIDED
Game::Game() : player(NULL), current_room(NULL), 
               game_over(false), victory(false) {
}


// TODO 1: Implement Game destructor
// - Delete player if it exists
// - Iterate through the world map and delete every Room*
// - Clear the map
//
// Hint (C++98 map iteration):
//
//   if (player != NULL) delete player;
//   for (std::map<std::string, Room*>::iterator it = world.begin();
//        it != world.end(); ++it) {
//       delete it->second;
//   }
//   world.clear();
//
Game::~Game() {
    // TODO: clean up player and all rooms
}


// initializeWorld — PROVIDED
// Creates 5 rooms, connects them, places monsters and items.
//
// MAP LAYOUT:
//                [Throne Room]       <- Dragon boss
//                     |
//     [Armory] - [Long Hallway] - [Treasury]
//                     |
//                 [Entrance]         <- start here
//
void Game::initializeWorld() {
    // Create rooms
    Room* entrance = new Room("Dungeon Entrance", 
        "A dark stone corridor leads deeper into the dungeon. "
        "Torches flicker on the walls.");
    
    Room* hallway = new Room("Long Hallway",
        "A long hallway with cracked stone walls. You hear strange noises ahead.");
    
    Room* armory = new Room("Armory",
        "An old armory with rusty weapons scattered about.");
    
    Room* treasury = new Room("Treasury",
        "A room filled with gold and treasure! But something guards it...");
    
    Room* throne_room = new Room("Throne Room",
        "The final chamber. A massive dragon sits upon a throne of bones.");
    
    // Add rooms to world map
    addRoom(entrance);
    addRoom(hallway);
    addRoom(armory);
    addRoom(treasury);
    addRoom(throne_room);
    
    // Connect rooms bidirectionally
    connectRooms("Dungeon Entrance", "north", "Long Hallway");
    connectRooms("Long Hallway", "west", "Armory");
    connectRooms("Long Hallway", "east", "Treasury");
    connectRooms("Long Hallway", "north", "Throne Room");
    
    // Place monsters
    hallway->setMonster(new Goblin());
    armory->setMonster(new Skeleton());
    treasury->setMonster(new Skeleton());
    throne_room->setMonster(new Dragon());  // Boss fight!
    
    // Place items
    entrance->addItem(new Consumable("Small Potion", "Restores 10 HP", 10));
    armory->addItem(new Weapon("Iron Sword", "A sturdy iron blade", 5));
    armory->addItem(new Armor("Chain Mail", "Light but protective armor", 3));
    treasury->addItem(new Consumable("Health Potion", "Restores 30 HP", 30));
    
    // Set starting room
    current_room = entrance;
}


// createStartingInventory — PROVIDED
void Game::createStartingInventory() {
    player->addItem(new Weapon("Rusty Dagger", "A dull, rusty dagger", 2));
    player->addItem(new Consumable("Bread", "A stale piece of bread", 5));
}


// TODO 2: Implement addRoom
// Add room to the world map using the room's name as the key.
//
// Hint:
//   if (room != NULL) { world[room->getName()] = room; }
//
void Game::addRoom(Room* room) {
    // TODO: add room to world map
}


// TODO 3: Implement connectRooms
// Look up both rooms in the world map.  If both exist:
//   1. room1->addExit(direction, room2)
//   2. Determine the reverse direction (north<->south, east<->west)
//   3. room2->addExit(reverse, room1)
//
// Starter code — fill in the blanks:
//
void Game::connectRooms(const std::string& room1_name, const std::string& direction,
                       const std::string& room2_name) {
    Room* room1 = world[room1_name];
    Room* room2 = world[room2_name];

    if (room1 != NULL && room2 != NULL) {
        room1->addExit(direction, room2);

        // TODO: determine reverse direction and add reverse exit
        std::string reverse;
        if (direction == "north")      reverse = "south";
        else if (direction == "south") reverse = "north";
        else if (direction == "east")  reverse = "west";
        else if (direction == "west")  reverse = "east";

        // TODO: add the reverse exit
        // hint: room2->addExit(reverse, room1);
    }
}


// TODO 4: Implement run — main game loop
// This is the heart of the game.  Follow these steps:
//
//   1. Print welcome banner
//   2. Get player name from std::getline
//   3. Create player:  player = new Player(player_name);
//   4. Call initializeWorld() and createStartingInventory()
//   5. Display starting room and mark it visited
//   6. Main loop — while (!game_over):
//        a. Print prompt "> "
//        b. Read command with std::getline
//        c. Convert to lowercase
//        d. Call processCommand(command)
//        e. Check victory / defeat conditions
//
// Starter code (nearly complete — fill in the marked spots):
//
void Game::run() {
    std::string player_name;
    std::cout << "\n=== DUNGEON CRAWLER RPG ===" << std::endl;
    std::cout << "Enter your name, brave adventurer: ";
    std::getline(std::cin, player_name);

    player = new Player(player_name);
    initializeWorld();
    createStartingInventory();

    std::cout << "\nWelcome, " << player_name << "!" << std::endl;
    std::cout << "Your quest: Defeat the dragon in the throne room!" << std::endl;
    std::cout << "Type 'help' for commands.\n" << std::endl;

    current_room->display();
    current_room->markVisited();

    // TODO: main game loop
    // hint:
    //   while (!game_over) {
    //       std::cout << "\n> ";
    //       std::string command;
    //       std::getline(std::cin, command);
    //       std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    //       if (!command.empty()) processCommand(command);
    //
    //       if (victory) {
    //           std::cout << "\nVICTORY! You have defeated the dragon!" << std::endl;
    //           game_over = true;
    //       }
    //       if (!player->isAlive()) {
    //           std::cout << "\nYou have died... Game Over." << std::endl;
    //           game_over = true;
    //       }
    //   }
}


// processCommand — PROVIDED
// Parses the command string and dispatches to the appropriate handler.
//
void Game::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string verb, object;
    iss >> verb;

    // Get rest of line as the object (e.g., item name)
    std::getline(iss, object);
    if (!object.empty() && object[0] == ' ') {
        object = object.substr(1);
    }

    if (verb == "go" || verb == "move") {
        if (object.empty()) {
            std::cout << "Go where? (north/south/east/west)" << std::endl;
        } else {
            move(object);
        }
    }
    else if (verb == "look" || verb == "l") {
        look();
    }
    else if (verb == "attack" || verb == "fight") {
        attack();
    }
    else if (verb == "pickup" || verb == "get" || verb == "take") {
        if (object.empty()) {
            std::cout << "Pick up what?" << std::endl;
        } else {
            pickupItem(object);
        }
    }
    else if (verb == "inventory" || verb == "i") {
        inventory();
    }
    else if (verb == "use") {
        if (object.empty()) {
            std::cout << "Use what?" << std::endl;
        } else {
            useItem(object);
        }
    }
    else if (verb == "equip" || verb == "wield" || verb == "wear") {
        if (object.empty()) {
            std::cout << "Equip what?" << std::endl;
        } else {
            equip(object);
        }
    }
    else if (verb == "stats") {
        player->displayStats();
    }
    else if (verb == "help" || verb == "h" || verb == "?") {
        help();
    }
    else if (verb == "quit" || verb == "exit") {
        std::cout << "Thanks for playing!" << std::endl;
        game_over = true;
    }
    else {
        std::cout << "Unknown command. Type 'help' for commands." << std::endl;
    }
}


// TODO 5: Implement move
// 1. If current room has a monster, print "You can't leave while a monster
//    blocks your path!" and return.
// 2. Get the exit in the given direction.
// 3. If it exists, update current_room, display, and mark visited.
// 4. Otherwise print "You can't go that way!"
//
void Game::move(const std::string& direction) {
    if (current_room->hasMonster()) {
        std::cout << "You can't leave while a monster blocks your path!" << std::endl;
        return;
    }

    Room* next = current_room->getExit(direction);
    // TODO: if next is not NULL, move there; otherwise print error
    // hint:
    //   if (next != NULL) {
    //       current_room = next;
    //       current_room->display();
    //       current_room->markVisited();
    //   } else {
    //       std::cout << "You can't go that way!" << std::endl;
    //   }
    (void)next;  // suppress warning until you implement the TODO above
}


// look — PROVIDED
void Game::look() {
    current_room->display();
}


// TODO 6: Implement attack
// If no monster in room, print a message.  Otherwise call combat().
//
void Game::attack() {
    if (!current_room->hasMonster()) {
        std::cout << "There's nothing to fight here." << std::endl;
        return;
    }
    combat(current_room->getMonster());
}


// TODO 7: Implement combat
// Turn-based combat loop.  Each iteration:
//   1. Display player and monster status
//   2. Prompt for action: attack / use <item> / flee
//   3. Handle player action
//   4. If monster still alive, monster attacks player
//
// Starter code (nearly complete — fill in marked spots):
//
void Game::combat(Monster* monster) {
    std::cout << "\n=== COMBAT BEGINS ===" << std::endl;

    while (player->isAlive() && monster->isAlive()) {
        // Display status
        std::cout << std::endl;
        player->displayStats();
        monster->displayStats();

        // Player turn
        std::cout << "\nYour turn! (attack / use <item> / flee): ";
        std::string action;
        std::getline(std::cin, action);
        std::transform(action.begin(), action.end(), action.begin(), ::tolower);

        if (action == "attack") {
            int damage = player->calculateDamage();
            std::cout << "\nYou attack for " << damage << " damage!" << std::endl;
            monster->takeDamage(damage);

            if (!monster->isAlive()) {
                std::cout << "\nVictory! You defeated the "
                          << monster->getName() << "!" << std::endl;
                player->gainExperience(monster->getExperienceReward());
                player->addGold(monster->getGoldReward());

                // Drop loot into the room
                std::vector<Item*> loot = monster->dropLoot();
                for (int i = 0; i < (int)loot.size(); i++) {
                    current_room->addItem(loot[i]);
                }

                // TODO: check if this was the Dragon — if so, set victory = true
                // hint: if (monster->getName() == "Dragon") victory = true;

                current_room->clearMonster();
                break;
            }
        }
        else if (action.find("use") == 0 && action.size() > 4) {
            std::string item_name = action.substr(4);
            player->useItem(item_name);
        }
        else if (action == "flee") {
            std::cout << "You flee from combat!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid action! Try: attack, use <item>, or flee" << std::endl;
            continue;  // don't give monster a free turn
        }

        // Monster turn (if still alive)
        if (monster->isAlive()) {
            std::cout << "\n" << monster->getAttackMessage() << std::endl;
            int mdmg = monster->calculateDamage();
            player->takeDamage(mdmg);
        }
    }

    std::cout << "\n=== COMBAT ENDS ===" << std::endl;
}


// TODO 8: Implement pickupItem
// Get the item from the current room.  If it exists, add to player
// inventory and remove from room.  Otherwise print error.
//
void Game::pickupItem(const std::string& item_name) {
    Item* item = current_room->getItem(item_name);
    // TODO: if item exists, add to player and remove from room
    // hint:
    //   if (item != NULL) {
    //       player->addItem(item);
    //       current_room->removeItem(item_name);
    //   } else {
    //       std::cout << "Item not found: " << item_name << std::endl;
    //   }
    (void)item;  // suppress warning until you implement the TODO above
}


// inventory — PROVIDED
void Game::inventory() {
    player->displayInventory();
}

// useItem — PROVIDED
void Game::useItem(const std::string& item_name) {
    player->useItem(item_name);
}


// TODO 9: Implement equip
// Get the item from player inventory.  Check its type:
//   "Weapon" → player->equipWeapon(item_name)
//   "Armor"  → player->equipArmor(item_name)
//   else     → print "You can't equip that!"
//
void Game::equip(const std::string& item_name) {
    Item* item = player->getItem(item_name);
    if (item == NULL) {
        std::cout << "You don't have that item!" << std::endl;
        return;
    }
    // TODO: check type and call appropriate equip method
}


// help — PROVIDED
void Game::help() {
    std::cout << "\n=== COMMANDS ===" << std::endl;
    std::cout << "  go <direction>    - Move (north/south/east/west)" << std::endl;
    std::cout << "  look              - Look around current room" << std::endl;
    std::cout << "  attack            - Attack monster in room" << std::endl;
    std::cout << "  pickup <item>     - Pick up item from room" << std::endl;
    std::cout << "  inventory         - Show your inventory" << std::endl;
    std::cout << "  equip <item>      - Equip weapon or armor" << std::endl;
    std::cout << "  use <item>        - Use consumable item" << std::endl;
    std::cout << "  stats             - Show your character stats" << std::endl;
    std::cout << "  help              - Show this help" << std::endl;
    std::cout << "  quit              - Exit game" << std::endl;
    std::cout << "================\n" << std::endl;
}
