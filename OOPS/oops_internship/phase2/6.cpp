/**
 * File: 6.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

// ==========================================
// 1. SINGLE INHERITANCE
// ==========================================
// A single derived class inherits from a single base class.
// Structure: Base -> Derived

class Animal { // Class definition
public: // Accessible from anywhere in the program
    void eat() {
        cout << "Animal is eating..." << endl;
    }
};

// Dog inherits only from Animal
class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void bark() {
        cout << "Dog is barking..." << endl;
    }
};


// ==========================================
// 2. MULTILEVEL INHERITANCE
// ==========================================
// A class is derived from another derived class, creating a chain.
// Structure: Base -> Intermediate -> Derived

class Puppy : public Dog { // Class definition
public: // Accessible from anywhere in the program
    void weep() {
        cout << "Puppy is weeping..." << endl;
    }
};


// ==========================================
// 3. MULTIPLE INHERITANCE
// ==========================================
// A single derived class inherits from MORE THAN ONE base class.
// Structure: Base1 + Base2 -> Derived

class Transformer { // Class definition
public: // Accessible from anywhere in the program
    void transform() {
        cout << "Transforming into a vehicle!" << endl;
    }
};

// RobotDog inherits features from BOTH Dog and Transformer
class RobotDog : public Dog, public Transformer { // Class definition
public: // Accessible from anywhere in the program
    void charge() {
        cout << "RobotDog is charging its battery..." << endl;
    }
};


// ==========================================
// 4. HIERARCHICAL INHERITANCE
// ==========================================
// Multiple derived classes inherit from a single base class.
// Structure: Base -> Derived1  AND  Base -> Derived2

class Cat : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void meow() {
        cout << "Cat is meowing..." << endl;
    }
};


// ==========================================
// 5. HYBRID (DIAMOND) INHERITANCE
// ==========================================
// A combination of two or more types of inheritance. 
// Classic "Diamond Problem" structure:
//      Organism
//       /    \
//    Mammal  Bird
//       \    /
//       Platypus
// Note: We use 'virtual' public inheritance to prevent the grand-child 
// from getting duplicate copies of the grand-parent class properties.

class Organism { // Class definition
public: // Accessible from anywhere in the program
    void breathe() {
        cout << "Organism is breathing..." << endl;
    }
};

// Virtual keyword solves the diamond problem duplicate issue
class Mammal : virtual public Organism { // Class definition
public: // Accessible from anywhere in the program
    void feedMilk() {
        cout << "Mammal feeding milk..." << endl;
    }
};

class Bird : virtual public Organism { // Class definition
public: // Accessible from anywhere in the program
    void layEggs() {
        cout << "Bird laying eggs..." << endl;
    }
};

// Platypus combines Mammal and Bird, and safely shares one Organism base
class Platypus : public Mammal, public Bird { // Class definition
public: // Accessible from anywhere in the program
    void swim() {
        cout << "Platypus is swimming..." << endl;
    }
};


// ==========================================
// MAIN FUNCTION (Testing all types)
// ==========================================
int main() { // Entry point of the program
    
    cout << "--- 1. Testing Single Inheritance ---" << endl;
    Dog myDog;
    myDog.eat();  // Inherited from Animal
    myDog.bark(); // Own method

    cout << "\n--- 2. Testing Multilevel Inheritance ---" << endl;
    Puppy myPuppy;
    myPuppy.eat();  // Inherited from Animal (Grandparent)
    myPuppy.bark(); // Inherited from Dog (Parent)
    myPuppy.weep(); // Own method

    cout << "\n--- 3. Testing Multiple Inheritance ---" << endl;
    RobotDog myRobot;
    myRobot.bark();      // From Dog Base
    myRobot.transform(); // From Transformer Base
    myRobot.charge();    // Own method

    cout << "\n--- 4. Testing Hierarchical Inheritance ---" << endl;
    Cat myCat;
    myCat.eat();  // Inherited from Animal (Shared with Dog class)
    myCat.meow(); // Own method

    cout << "\n--- 5. Testing Hybrid Inheritance (Diamond) ---" << endl;
    Platypus perry;
    perry.breathe();  // Inherited safely via Virtual Base Class (Organism)
    perry.feedMilk(); // From Mammal
    perry.layEggs();  // From Bird
    perry.swim();     // Own method

    return 0;
}