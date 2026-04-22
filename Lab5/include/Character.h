#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <iostream>

/**
 * Character class - Base class for all characters in the game
 * 
 * This class represents common attributes and behaviors for both
 * Player and Monster characters. It demonstrates:
 * - Encapsulation (private data, public interface)
 * - Virtual functions (for polymorphism)
 * - Member functions vs free functions
 * 
 * LEARNING OBJECTIVES:
 * - Practice writing constructors with initializer lists
 * - Implement member functions
 * - Understand access specifiers
 * - Use const correctness
 */
class Character {
private:
    std::string name;
    int max_hp;
    int current_hp;
    int attack;
    int defense;
    bool alive;

public:
    // Constructor - Initialize all character stats
    // TODO: Implement in Character.cpp
    Character(const std::string& name, int hp, int attack, int defense);
    
    // Destructor
    // TODO: Implement in Character.cpp (add debug output if helpful)
    ~Character();
    
    // Getters (inline functions - defined in header)
    // These are simple one-liners, so we define them here
    std::string getName() const { return name; }
    int getMaxHP() const { return max_hp; }
    int getCurrentHP() const { return current_hp; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    bool isAlive() const { return alive; }
    
    // Setters for derived classes to modify stats
    void setName(const std::string& n) { name = n; }
    void setMaxHP(int hp) { max_hp = hp; }
    void setCurrentHP(int hp) { current_hp = hp; }
    void setAttack(int atk) { attack = atk; }
    void setDefense(int def) { defense = def; }
    void setAlive(bool a) { alive = a; }
    
    // Combat methods
    // Virtual so derived classes can override
    // TODO: Implement in Character.cpp
    virtual int calculateDamage() const;
    
    // TODO: Implement in Character.cpp
    void takeDamage(int damage);
    
    // TODO: Implement in Character.cpp
    void heal(int amount);
    
    // Display methods
    // Virtual with default implementation - can be overridden
    // TODO: Implement in Character.cpp
    virtual void displayStats() const;
    
    // Brief status display (for combat)
    // TODO: Implement in Character.cpp
    void displayStatus() const;
};

#endif // CHARACTER_H
