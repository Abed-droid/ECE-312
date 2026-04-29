#include "Character.h"
#include <cstdlib>
#include <vector>

// Constructor — PROVIDED (no changes needed)
Character::Character(const std::string& name, int hp, int attack, int defense)
    : name(name), max_hp(hp), current_hp(hp), attack(attack), 
      defense(defense), alive(true) {
}

// Destructor — PROVIDED (nothing to clean up in base class)
Character::~Character() {
}


// TODO 1: Implement calculateDamage
// Return the character's attack stat plus a small random bonus (0–4).
//
// Example implementation (fill in the one missing expression):
//
//     int Character::calculateDamage() const {
//         return ??? ;   // hint: attack + (rand() % 5)
//     }
//
int Character::calculateDamage() const {
    // TODO: return attack plus a random bonus 0–4


    return attack + (rand() % 5);
}


// TODO 2: Implement takeDamage
// 1. Compute actual_damage = damage – defense (minimum 0)
// 2. Subtract actual_damage from current_hp
// 3. If current_hp <= 0, set it to 0 and set alive = false
// 4. Print a message:  "Name takes X damage! (Y/Z HP)"
//
// Starter code — fill in the blanks marked ???
//
void Character::takeDamage(int damage) {
    int actual_damage = damage - defense;
    if (actual_damage < 0) actual_damage = 0;

    current_hp -= actual_damage;

    if (current_hp <= 0) {current_hp=0; alive=false;}

    std::cout << name << " takes " << actual_damage << " damage! "
              << "(" << current_hp << "/" << max_hp << " HP)" << std::endl;
}


// TODO 3: Implement heal
// 1. Add amount to current_hp
// 2. Cap at max_hp (don't exceed maximum)
// 3. Print: "Name heals X HP! (Y/Z HP)"
//
// Starter code — fill in the blanks:
//
void Character::heal(int amount) {
    current_hp += amount;

    // TODO: cap current_hp at max_hp
    // hint: if (current_hp > max_hp) current_hp = max_hp;
    if (current_hp > max_hp) current_hp = max_hp;
    std::cout << name << " heals " << amount << " HP! "
              << "(" << current_hp << "/" << max_hp << " HP)" << std::endl;
}


// TODO 4: Implement displayStats
// Print the character's name and HP.
// Format: "Name [HP: current/max]"
//
void Character::displayStats() const {
    std::cout << name << " [HP: " << current_hp << "/" << max_hp << "]" << std::endl;
}


// TODO 5: Implement displayStatus (brief one-liner used during combat)
// Same format as displayStats but without a newline at the end.
//
void Character::displayStatus() const {
    std::cout << name << " [HP: " << current_hp << "/" << max_hp << "]";
}
