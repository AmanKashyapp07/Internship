/**
 * File: 2.cpp
 * Demonstrates: Liskov Substitution Principle (LSP)
 * Description: Demonstrates how inheriting Square from Rectangle violates LSP, and shows the correct way using a common Shape base.
 * Interview Takeaway: Derived classes must be substitutable for their base classes without breaking the code's expected behavior.
 */

#include <iostream>
using namespace std;

class Rectangle { // Class definition
protected: // Accessible only to this class and derived classes
    int width, height;
public: // Accessible from anywhere in the program
    virtual void setWidth(int w) { width = w; } // Virtual function for runtime override dispatch
    virtual void setHeight(int h) { height = h; } // Virtual function for runtime override dispatch
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getArea() const { return width * height; }
};

// Square inherits from Rectangle
class Square : public Rectangle { // Class definition
public: // Accessible from anywhere in the program
    // A square must keep width and height equal, so it forces both to change!
    void setWidth(int w) override { width = w; height = w; } // Indicates this function overrides a base virtual function
    void setHeight(int h) override { width = h; height = h; } // Indicates this function overrides a base virtual function
};

// An innocent global function testing the system
void verifyRectangleSize(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(10);
    
    // For any true Rectangle, Area = 5 * 10 = 50
    if (r.getArea() == 50) {
        cout << "Success: Rectangle behaves perfectly!\n";
    } else {
        cout << "CRITICAL ERROR: Area is " << r.getArea() << ". LSP broken!\n";
    }
}

int main() { // Entry point of the program
    Rectangle realRect;
    verifyRectangleSize(realRect); // Works perfectly!

    Square skywalkerSquare;
    verifyRectangleSize(skywalkerSquare); // FAILS! Outputs 100 instead of 50.
    // Square object is passing because it is a derived class of Rectangle, but it does not behave like a true Rectangle due to the way it overrides the setWidth and setHeight functions, which violates the Liskov Substitution Principle (LSP), we can pass derived class object to the base class reference, but it does not behave like a true Rectangle, which is why the area calculation fails and demonstrates that the Square class cannot be substituted for a Rectangle without breaking the program's correctness.
    
    return 0;
}

// LSP Principle states that objects of a superclass should be replaceable with objects of a subclass without affecting the correctness of the program. In this example, the Square class violates the LSP because it changes the expected behavior of the Rectangle class. When we call verifyRectangleSize with a Square object, it does not behave like a true Rectangle, and the area calculation is incorrect, demonstrating that the Square class cannot be substituted for a Rectangle without breaking the program's correctness.