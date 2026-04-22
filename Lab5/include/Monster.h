#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"
#include "Item.h"
#include <vector>

/**
 * Monster class - Base class for all enemy monsters
 * 
 * Inherits from Character and adds:
 * - Experience and gold rewards
 * - Loot table (items dropped when defeated)
 * - Attack messages (flavor text)
 * 
 * LEARNING OBJECTIVES:
 * - Multi-level inheritance (Goblin -> Monster -> Character)
 * - Virtual function overriding
 * - Polymorphism in action
 */
class Monster : public Character {
private:
    int experience_reward;
    int gold_reward;
    std::vector<Item*> loot_table;  // Monster owns these items!
    
public:
    // Constructor
    // TODO: Implement in Monster.cpp
    Monster(const std::string& name, int hp, int attack, int defense, 
            int exp_reward, int gold_reward);
    
    // Destructor - CRITICAL for memory management!
    // TODO: Implement in Monster.cpp
    // HINT: Must delete all items in loot_table
    ~Monster();
    
    // Override displayStats from Character
    // TODO: Implement in Monster.cpp
    void displayStats() const;
    
    // Loot management
    // TODO: Implement these in Monster.cpp
    void addLoot(Item* item);
    std::vector<Item*> dropLoot();  // Transfers ownership!
    
    // Getters
    int getExperienceReward() const { return experience_reward; }
    int getGoldReward() const { return gold_reward; }
    
    // AI behavior - different monsters have different attack messages
    // TODO: Implement in Monster.cpp
    virtual std::string getAttackMessage() const;
};

/**
 * Goblin - Weak but common enemy
 */
class Goblin : public Monster {
public:
    // TODO: Implement in Monster.cpp
    // HINT: Call Monster constructor with appropriate stats
    Goblin();
    
    // Override attack message
    // TODO: Implement in Monster.cpp
    std::string getAttackMessage() const;
};

/**
 * Skeleton - Undead warrior
 */
class Skeleton : public Monster {
public:
    // TODO: Implement in Monster.cpp
    Skeleton();
    
    // Override attack message
    // TODO: Implement in Monster.cpp
    std::string getAttackMessage() const;
};

/**
 * Dragon - Boss enemy
 * 
 * Special ability: Deals extra fire damage!
 */
class Dragon : public Monster {
public:
    // TODO: Implement in Monster.cpp
    Dragon();
    
    // Override attack message
    // TODO: Implement in Monster.cpp
    std::string getAttackMessage() const;
    
    // Override calculateDamage to add fire damage bonus
    // TODO: Implement in Monster.cpp
    int calculateDamage() const;
};

#endif // MONSTER_H
