/**
 * File: 14.cpp
 * Demonstrates: Abstract Classes & Pure Virtual Functions
 * Description: Demonstrates defining interfaces and abstract classes that cannot be instantiated directly.
 * Interview Takeaway: A class with at least one pure virtual function is abstract. Derived classes must implement all pure virtuals to be instantiated.
 */

#include <iostream>
#include <string>

// This removes the need to write std:: everywhere in this file
using namespace std;

// 1. ABSTRACT BASE CLASS
class Character { // Class definition
protected: // Accessible only to this class and derived classes
    string name; // No std:: needed
    int health;

public: // Accessible from anywhere in the program
    Character(string name, int health) {
        this->name = name;
        this->health = health;
        cout << "Abstract Character Constructor: Initializing " << name << "\n";
    }

    virtual void attack() = 0;  // Pure virtual function (makes class abstract)

    virtual ~Character() { // Virtual function for runtime override dispatch
        cout << "Abstract Character Destructor called\n";
    }
};

// 2. DERIVED CONCRETE CLASS
class Warrior : public Character { // Class definition
private: // Inaccessible from outside the class
    string weaponType;

public: // Accessible from anywhere in the program
    Warrior(string name, int health, string weapon) 
        : Character(name, health) 
    {
        this->weaponType = weapon;
        cout << "Warrior Constructor: Equipping " << weapon << "\n";
    }

    void attack() override { // Indicates this function overrides a base virtual function
        cout << name << " swings their " << weaponType << " dealing massive damage!\n";
    }

    ~Warrior() override { // Indicates this function overrides a base virtual function
        cout << "Warrior Destructor called\n";
    }
};

int main() { // Entry point of the program
    cout << "--- Creating Warrior ---\n";
    Character* hero = new Warrior("Thorin", 150, "Battleaxe");
    
    cout << "\n--- Performing Action ---\n";
    hero->attack();

    cout << "\n--- Deleting Warrior ---\n";
    delete hero;

    return 0;
}