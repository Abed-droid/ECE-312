#include "Player.h"
#include <iostream>
#include <algorithm>

// ============================================================================
// Helper: case-insensitive string comparison
// Use this when searching inventory by name so "sword" matches "Sword".
// ============================================================================
static std::string toLower(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}


// Constructor — PROVIDED (no changes needed)
Player::Player(const std::string& name)
    : Character(name, 100, 10, 5),
      level(1), experience(0), gold(0),
      equipped_weapon(NULL), equipped_armor(NULL) {
}


// TODO 1: Implement Player destructor
// CRITICAL: You MUST delete every Item* in the inventory vector,
// then clear it.  Do NOT delete equipped_weapon/equipped_armor
// separately — they just point to items already in the vector.
//
// Hint (loop version):
//     for (int i = 0; i < (int)inventory.size(); i++) {
//         delete inventory[i];
//     }
//     inventory.clear();
//
Player::~Player() {
    // TODO: delete all inventory items
}


// TODO 2: Override displayStats
// Print a nicely formatted stat block.  Example output:
//
//   ===== Alice =====
//   Level: 1
//   HP: 80/100
//   Attack: 10 (+5 from weapon)
//   Defense: 5 (+3 from armor)
//   Gold: 15
//   Experience: 30
//   =====================
//
// Use getName(), getLevel(), getCurrentHP(), getMaxHP(), getAttack(),
// getDefense(), gold, experience, equipped_weapon, equipped_armor.
//
void Player::displayStats() const {
    std::cout << "\n===== " << getName() << " =====" << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "HP: " << getCurrentHP() << "/" << getMaxHP() << std::endl;

    std::cout << "Attack: " << getAttack();
    if (equipped_weapon) {
        std::cout << " (+" << equipped_weapon->getValue() << " from weapon)";
    }
    std::cout << std::endl;

    std::cout << "Defense: " << getDefense();
    if (equipped_armor) {
        std::cout << " (+" << equipped_armor->getValue() << " from armor)";
    }
    std::cout << std::endl;

    std::cout << "Gold: " << gold << std::endl;
    std::cout << "Experience: " << experience << std::endl;
    std::cout << "=====================\n" << std::endl;
}


// TODO 3: Override calculateDamage to include weapon bonus
// Call the base Character::calculateDamage(), then add the weapon's
// damage bonus if a weapon is equipped.
//
// Hint:
//     int dmg = Character::calculateDamage();
//     if (equipped_weapon) dmg += equipped_weapon->getValue();
//     return dmg;
//
int Player::calculateDamage() const {
    // TODO: calculate damage with weapon bonus
    return 0;  // REPLACE THIS
}


// TODO 4: Implement addItem
// Add the item pointer to the inventory vector and print a message.
//
void Player::addItem(Item* item) {
    if (item == NULL) return;
    inventory.push_back(item);
    std::cout << "Picked up: " << item->getName() << std::endl;
}


// TODO 5: Implement removeItem
// Search inventory for an item whose name matches item_name
// (case-insensitive).  If found, delete the Item and erase it
// from the vector.  If not found, print an error.
//
// Starter code:
//
void Player::removeItem(const std::string& item_name) {
    std::string target = toLower(item_name);
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (toLower(inventory[i]->getName()) == target) {
            // TODO: if this item is the equipped weapon or armor,
            //       set the pointer to NULL first!
            delete inventory[i];
            inventory.erase(inventory.begin() + i);
            return;
        }
    }
    std::cout << "Item not found: " << item_name << std::endl;
}


// TODO 6: Implement displayInventory
// Print header, then each item as "- Name (Type)", then footer.
// If inventory is empty, print "Empty".
//
void Player::displayInventory() const {
    std::cout << "\n----- Inventory -----" << std::endl;
    if (inventory.empty()) {
        std::cout << "Empty" << std::endl;
    } else {
        for (int i = 0; i < (int)inventory.size(); i++) {
            std::cout << "- " << inventory[i]->getName()
                      << " (" << inventory[i]->getType() << ")" << std::endl;
        }
    }
    std::cout << "--------------------\n" << std::endl;
}


// TODO 7: Implement hasItem
// Search inventory (case-insensitive). Return true if found.
//
bool Player::hasItem(const std::string& item_name) const {
    std::string target = toLower(item_name);
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (toLower(inventory[i]->getName()) == target) {
            return true;
        }
    }
    return false;
}


// TODO 8: Implement getItem
// Search inventory (case-insensitive). Return pointer if found, NULL otherwise.
//
Item* Player::getItem(const std::string& item_name) {
    std::string target = toLower(item_name);
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (toLower(inventory[i]->getName()) == target) {
            return inventory[i];
        }
    }
    return NULL;
}


// TODO 9: Implement equipWeapon
// 1. Call getItem() to find the weapon in inventory
// 2. Check it exists and its type is "Weapon"
// 3. Set equipped_weapon to the item pointer
// 4. Print a message
//
// Starter code — fill in the blanks:
//
void Player::equipWeapon(const std::string& weapon_name) {
    Item* item = getItem(weapon_name);
    if (item == NULL) {
        std::cout << "You don't have that item!" << std::endl;
        return;
    }
    if (item->getType() != "Weapon") {
        std::cout << "That's not a weapon!" << std::endl;
        return;
    }
    // TODO: set equipped_weapon and print message
    // hint:
    //   equipped_weapon = item;
    //   std::cout << "Equipped " << weapon_name << "!" << std::endl;
}


// TODO 10: Implement equipArmor (same pattern as equipWeapon)
//
void Player::equipArmor(const std::string& armor_name) {
    Item* item = getItem(armor_name);
    if (item == NULL) {
        std::cout << "You don't have that item!" << std::endl;
        return;
    }
    if (item->getType() != "Armor") {
        std::cout << "That's not armor!" << std::endl;
        return;
    }
    // TODO: set equipped_armor and print message
}


// unequipWeapon — PROVIDED
void Player::unequipWeapon() {
    if (equipped_weapon != NULL) {
        std::cout << "Unequipped " << equipped_weapon->getName() << std::endl;
        equipped_weapon = NULL;
    } else {
        std::cout << "No weapon equipped." << std::endl;
    }
}

// unequipArmor — PROVIDED
void Player::unequipArmor() {
    if (equipped_armor != NULL) {
        std::cout << "Unequipped " << equipped_armor->getName() << std::endl;
        equipped_armor = NULL;
    } else {
        std::cout << "No armor equipped." << std::endl;
    }
}


// TODO 11: Implement useItem
// 1. Find the item by name using getItem()
// 2. Check it exists and its type is "Consumable"
// 3. Cast to Consumable*:  Consumable* c = static_cast<Consumable*>(item);
// 4. Call heal(c->getHealingAmount()) to restore HP
// 5. Call c->use() to mark it as used
// 6. Call removeItem() to delete and remove it from inventory
//
// Starter code:
//
void Player::useItem(const std::string& item_name) {
    Item* item = getItem(item_name);
    if (item == NULL) {
        std::cout << "You don't have that item!" << std::endl;
        return;
    }
    if (item->getType() != "Consumable") {
        std::cout << "You can't use that!" << std::endl;
        return;
    }

    // TODO: cast to Consumable*, heal, mark used, then remove from inventory
    // hint:
    //   Consumable* c = static_cast<Consumable*>(item);
    //   heal(c->getHealingAmount());
    //   c->use();
    //   removeItem(item_name);
}


// TODO 12: Implement gainExperience
// Add exp, print message, check for level up (threshold: level * 100).
//
void Player::gainExperience(int exp) {
    experience += exp;
    std::cout << "Gained " << exp << " experience!" << std::endl;

    // TODO: if experience >= level * 100, call levelUp()
}


// TODO 13: Implement levelUp
// Increment level, reset experience to 0, increase stats, print message.
//
// Stat increases per level:
//   max_hp  += 10    (use setMaxHP(getMaxHP() + 10))
//   current_hp = max  (use setCurrentHP(getMaxHP())  — full heal!)
//   attack  += 2     (use setAttack(getAttack() + 2))
//   defense += 1     (use setDefense(getDefense() + 1))
//
void Player::levelUp() {
    // TODO: level up the player and print a message
    // hint:
    //   level++;
    //   experience = 0;
    //   setMaxHP(getMaxHP() + 10);
    //   setCurrentHP(getMaxHP());
    //   setAttack(getAttack() + 2);
    //   setDefense(getDefense() + 1);
    //   std::cout << "\n*** LEVEL UP! ***" << std::endl;
    //   std::cout << "You are now level " << level << "!" << std::endl;
    //   displayStats();
}
