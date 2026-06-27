/**
 * File: test18.cpp
 * Demonstrates: Static Members (Variables & Functions)
 * Description: Demonstrates static member variables shared across all instances and static member functions called without class instances.
 * Interview Takeaway: Static functions do not have access to the implicit 'this' pointer.
 */

#include <iostream>

class Widget { // Class definition
private: // Inaccessible from outside the class
    int id;                   // Non-static: Each widget has a unique ID
    static int widgetCount;   // Static: Shared counter for ALL widgets

public: // Accessible from anywhere in the program
    Widget() {
        widgetCount++;
        id = widgetCount;
    }

    // 1. Non-Static Function: Can access everything
    void printDetails() {
        std::cout << "Widget ID: " << id << " (Total Widgets: " << widgetCount << ")\n";
    }

    // 2. Static Function: Belongs to the class
    static int getCount() { // Static method: no 'this' pointer, called on class name directly
        // return id; // ERROR! Static function cannot access non-static variable 'id'
        return widgetCount; // Allowed: widgetCount is static
    }
};

// Define the static variable outside the class
int Widget::widgetCount = 0;

int main() { // Entry point of the program
    // Note: We can call the static function BEFORE creating any objects!
    std::cout << "Initial count: " << Widget::getCount() << "\n"; // Outputs 0

    Widget w1;
    Widget w2;

    // Call using the class name (Standard & Recommended way)
    std::cout << "Count after creation: " << Widget::getCount() << "\n"; // Outputs 2

    // Call using an object (Allowed, but can be confusing because it looks non-static)
    //std::cout << "Count via object: " << w1.getCount() << "\n"; // Outputs 2

    return 0;
}