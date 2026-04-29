#include "Monster.h"
#include <iostream>

// ============================================================================
// Base Monster class
// ============================================================================

// Constructor — PROVIDED (no changes needed)
Monster::Monster(const std::string& name, int hp, int attack, int defense,
                 int exp_reward, int gold_reward)
    : Character(name, hp, attack, defense),
      experience_reward(exp_reward), gold_reward(gold_reward) {
}


// TODO 1: Implement Monster destructor
// You MUST delete every Item* in loot_table to prevent memory leaks,
// then clear the vector.
//
// Hint:
//     for (int i = 0; i < (int)loot_table.size(); i++) {
//         delete loot_table[i];
//     }
//     loot_table.clear();
//
Monster::~Monster() {
    for (int i = 0; i < (int)loot_table.size(); i++) {
        delete loot_table[i];
    }
    loot_table.clear();
}


// TODO 2: Override displayStats
// Print: "MonsterName [HP: current/max]"
// Use getName(), getCurrentHP(), getMaxHP()
//
void Monster::displayStats() const {
    std::cout << getName() << " [HP: " << getCurrentHP() << "/" << getMaxHP() << "]" << std::endl;
}


// addLoot — PROVIDED (no changes needed)
void Monster::addLoot(Item* item) {
    if (item != NULL) {
        loot_table.push_back(item);
    }
}


// TODO 3: Implement dropLoot
// Create a copy of loot_table, then clear the original (transfers ownership
// to the caller).  Return the copy.
//
// Starter code — fill in the blank:
//
std::vector<Item*> Monster::dropLoot() {
    std::vector<Item*> dropped = loot_table;
    loot_table.clear();
    return dropped;
}


// TODO 4: Implement getAttackMessage (base version)
// Return: "MonsterName attacks!"
//
std::string Monster::getAttackMessage() const {
    return getName() + " attacks!";
}


// ============================================================================
// Goblin — weak but common enemy
// ============================================================================

// Constructor — PROVIDED (stats set, but you need to add loot)
Goblin::Goblin()
    : Monster("Goblin", 30, 5, 2, 10, 5) {
    addLoot(new Consumable("Small Potion", "Restores 10 HP", 10));
}

// TODO 6: Override getAttackMessage for Goblin
// Return: "The goblin swipes at you with its rusty dagger!"
//
std::string Goblin::getAttackMessage() const {
    return "The goblin swipes at you with its rusty dagger!";
}


// ============================================================================
// Skeleton — undead warrior
// ============================================================================

// Constructor — PROVIDED (stats set, but you need to add loot)
Skeleton::Skeleton()
    : Monster("Skeleton", 40, 8, 4, 20, 10) {
    addLoot(new Weapon("Old Sword", "A battered but usable blade", 3));
}

// TODO 8: Override getAttackMessage for Skeleton
// Return: "The skeleton rattles its bones and slashes with a sword!"
//
std::string Skeleton::getAttackMessage() const {
    return "The skeleton rattles its bones and slashes with a sword!";
}


// ============================================================================
// Dragon — boss enemy with bonus fire damage
// ============================================================================

// Constructor — PROVIDED (stats set, but you need to add legendary loot)
Dragon::Dragon()
    : Monster("Dragon", 150, 20, 10, 100, 50) {
    addLoot(new Weapon("Dragon Slayer", "A legendary blade", 10));
    addLoot(new Armor("Dragon Scale Armor", "Incredibly tough scales", 8));
    addLoot(new Consumable("Greater Health Potion", "Restores 100 HP", 100));
}

// TODO 10: Override getAttackMessage for Dragon
// Return: "The dragon breathes fire at you!"
//
std::string Dragon::getAttackMessage() const {
    return "The dragon breathes fire at you!";
}

// TODO 11: Override calculateDamage for Dragon
// Call the base Monster::calculateDamage() and add +5 fire bonus.
//
// Hint:
//     return Monster::calculateDamage() + 5;
//
int Dragon::calculateDamage() const {
    return Monster::calculateDamage() + 5;
}
