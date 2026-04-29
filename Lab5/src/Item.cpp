#include "Item.h"
using std::string;
// ============================================================================
// Base Item class implementation
// ============================================================================

// Constructor — PROVIDED (no changes needed)
Item::Item(const std::string& name, const std::string& description,
           const std::string& type, int value)
    :name(name), description(description), type(type), value(value)
{
}

// Destructor — PROVIDED (nothing to clean up)
Item::~Item() {
}



// TODO 1: Implement displayInfo (base version)
// Print item name, description, and value.
// Format:
//   [ITEM] ItemName
//     Description text
//     Value: X
//
// Hint: use getName(), getDescription(), getValue()
//
void Item::displayInfo() const {
    std::cout << "[ITEM] " << getName() << std::endl;
    std::cout << "  " << getDescription() << std::endl;
    std::cout << "  Value: " << getValue() << std::endl;
}


// TODO 2: Implement displayBrief
// One-line format: "Name (Type)"
// Example: "Iron Sword (Weapon)"
//
void Item::displayBrief() const {
    // TODO: print brief info
    cout << getName() << " (" << getType() << ")" << endl;
}


// ============================================================================
// Weapon class implementation
// ============================================================================

// Constructor — PROVIDED (no changes needed)
Weapon::Weapon(const std::string& name, const std::string& description, int damage)
    : Item(name, description, "Weapon", damage), damage_bonus(damage) {
}


// TODO 3: Override displayInfo for Weapon
// Format:
//   [WEAPON] WeaponName
//     Description
//     Damage Bonus: +X
//
void Weapon::displayInfo() const {
    std::cout << "[WEAPON] " << getName() << std::endl;
    std::cout << "  " << getDescription() << std::endl;
    // TODO: print damage bonus line
    std::cout << "  Damage Bonus: +" << getDamageBonus() << std::endl;
}


// ============================================================================
// Armor class implementation
// ============================================================================

// Constructor — PROVIDED (no changes needed)
Armor::Armor(const std::string& name, const std::string& description, int defense)
    : Item(name, description, "Armor", defense), defense_bonus(defense) {
}


// TODO 4: Override displayInfo for Armor
// Format:
//   [ARMOR] ArmorName
//     Description
//     Defense Bonus: +X
//
void Armor::displayInfo() const {
    std::cout << "[ARMOR] " << getName() << std::endl;
    std::cout << "  " << getDescription() << std::endl;
    // TODO: print defense bonus line
    std::cout << "  Defense Bonus: +" << getDefenseBonus() << std::endl;
}


// ============================================================================
// Consumable class implementation
// ============================================================================

// Constructor — PROVIDED (no changes needed)
Consumable::Consumable(const std::string& name, const std::string& description, 
                       int healing)
    : Item(name, description, "Consumable", healing), 
      healing_amount(healing), used(false) {
}


// TODO 5: Override displayInfo for Consumable
// Format:
//   [CONSUMABLE] ConsumableName
//     Description
//     Restores: X HP
//
void Consumable::displayInfo() const {
    std::cout << "[CONSUMABLE] " << getName() << std::endl;
    std::cout << "  " << getDescription() << std::endl;
    // TODO: print healing amount line
    std::cout << "  Restores: " << getHealingAmount() << " HP" << std::endl;
}


// TODO 6: Implement use()
// - If NOT already used: print "Used ItemName! Restored X HP." and set used = true
// - If already used: print "ItemName has already been used!"
//
// Starter code — fill in the blanks:
//
void Consumable::use() {
    if (!used) {
        // TODO: print message and set used = true
        cout << "Used " << getName() <<"! Restored " << getHealingAmount() << " HP." << endl;
        used = true;
    } else {
        std::cout << getName() << " has already been used!" << std::endl;
    }
}
