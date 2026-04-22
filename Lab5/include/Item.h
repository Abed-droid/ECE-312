#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
using std::cout;
using std::endl;

/**
 * Item class - Base class for all items in the game
 * 
 * This hierarchy demonstrates inheritance with three derived types:
 * - Weapon (increases attack damage)
 * - Armor (increases defense)
 * - Consumable (single-use healing items)
 * 
 * LEARNING OBJECTIVES:
 * - Practice inheritance
 * - Override virtual functions
 * - Understand polymorphism through items
 */
class Item {
private:
    std::string name;
    std::string description;
    std::string type;  // "Weapon", "Armor", or "Consumable"
    int value;  // Damage bonus, defense bonus, or healing amount
    
public:
    // Constructor
    // TODO: Implement in Item.cpp
    Item(const std::string& name, const std::string& description, 
         const std::string& type, int value);
    
    // Virtual destructor (needed because Item has virtual functions)
    virtual ~Item();
    
    // Getters (inline)
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    std::string getType() const { return type; }
    int getValue() const { return value; }
    
    // Virtual function with default implementation
    // Derived classes should override this
    // TODO: Implement in Item.cpp
    virtual void displayInfo() const;
    
    // Virtual function for using items
    // Default does nothing - Consumable overrides
    virtual void use() { }
    
    // Display brief item info
    // TODO: Implement in Item.cpp
    void displayBrief() const;
};

/**
 * Weapon class - Items that increase attack damage
 */
class Weapon : public Item {
private:
    int damage_bonus;
    
public:
    // Constructor
    // TODO: Implement in Item.cpp
    // HINT: Must call base Item constructor in initializer list
    Weapon(const std::string& name, const std::string& description, int damage);
    
    // Override displayInfo to show weapon-specific format
    // TODO: Implement in Item.cpp
    void displayInfo() const;
    
    // Getter
    int getDamageBonus() const { return damage_bonus; }
};

/**
 * Armor class - Items that increase defense
 */
class Armor : public Item {
private:
    int defense_bonus;
    
public:
    // Constructor
    // TODO: Implement in Item.cpp
    // HINT: Must call base Item constructor in initializer list
    Armor(const std::string& name, const std::string& description, int defense);
    
    // Override displayInfo to show armor-specific format
    // TODO: Implement in Item.cpp
    void displayInfo() const;
    
    // Getter
    int getDefenseBonus() const { return defense_bonus; }
};

/**
 * Consumable class - Single-use items (potions, food)
 */
class Consumable : public Item {
private:
    int healing_amount;
    bool used;
    
public:
    // Constructor
    // TODO: Implement in Item.cpp
    // HINT: Must call base Item constructor in initializer list
    Consumable(const std::string& name, const std::string& description, int healing);
    
    // Override displayInfo to show consumable-specific format
    // TODO: Implement in Item.cpp
    void displayInfo() const;
    
    // Override use function
    // TODO: Implement in Item.cpp
    void use();
    
    // Getters
    int getHealingAmount() const { return healing_amount; }
    bool isUsed() const { return used; }
};

#endif // ITEM_H
