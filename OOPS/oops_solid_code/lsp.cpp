/**
 * @file lsp.cpp
 * @brief Demonstration of the Liskov Substitution Principle (LSP)
 * 
 * LSP: Subtypes must be substitutable for their base types without altering
 * the correctness of the program.
 */

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// ============================================================================
// ❌ VIOLATION: Square inherits from Rectangle. However, modifying width/height
// together in Square breaks the behavioral expectations of a Rectangle.
// ============================================================================
class BadRectangle {
protected:
    int width;
    int height;

public:
    BadRectangle(int w, int h) : width(w), height(h) {}

    virtual void setWidth(int w) {
        width = w;
    }

    virtual void setHeight(int h) {
        height = h;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getArea() const { return width * height; }
};

class BadSquare : public BadRectangle {
public:
    BadSquare(int size) : BadRectangle(size, size) {}

    // Overriding sets both, which violates Rectangle's invariant that width 
    // and height can be varied independently.
    void setWidth(int w) override {
        width = height = w;
    }

    void setHeight(int h) override {
        width = height = h;
    }
};

// Client code that assumes standard Rectangle behavior
void verifyRectangleBehavior(BadRectangle& rect) {
    rect.setWidth(5);
    rect.setHeight(10);
    
    // A caller expects that setting width to 5 and height to 10 yields an area of 50.
    // However, if we pass a BadSquare, setting height to 10 also sets width to 10,
    // resulting in an area of 100! This violates the contract of the base class.
    cout << "Setting Width to 5 and Height to 10..." << endl;
    cout << "Width: " << rect.getWidth() << ", Height: " << rect.getHeight() << endl;
    cout << "Calculated Area: " << rect.getArea() << endl;
    
    if (rect.getArea() == 50) {
        cout << "✅ Rectangle contract satisfied!" << endl;
    } else {
        cout << "❌ LSP Violation! Rectangle contract broken!" << endl;
    }
}

// ============================================================================
// ✅ RESOLUTION: Do not force inheritance if behavioral rules differ.
// Let both inherit from a more generic Shape2D interface, or handle them separately.
// ============================================================================
class Shape2D {
public:
    virtual ~Shape2D() = default;
    virtual int getArea() const = 0;
};

class Rectangle : public Shape2D {
private:
    int width;
    int height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}
    
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    
    int getArea() const override {
        return width * height;
    }
};

class Square : public Shape2D {
private:
    int side;

public:
    Square(int s) : side(s) {}
    
    void setSide(int s) { side = s; }
    
    int getArea() const override {
        return side * side;
    }
};

int main() {
    cout << "=== LSP Demonstration ===" << endl << endl;

    // ❌ Testing Bad Design
    cout << "[Bad Design Example]" << endl;
    BadRectangle regularRect(2, 3);
    cout << "Testing with regular rectangle:" << endl;
    verifyRectangleBehavior(regularRect);

    cout << endl;

    BadSquare square(5);
    cout << "Testing with square subclassing rectangle:" << endl;
    verifyRectangleBehavior(square); // Fails the contract test

    cout << endl;

    // ✅ Testing Good Design (LSP Compliant)
    cout << "[Good Design Example]" << endl;
    Rectangle r(5, 10);
    Square s(5);

    // Both are shapes and can be placed in a collection or passed to generic functions
    vector<Shape2D*> shapes = {&r, &s};
    for (const auto& shape : shapes) {
        cout << "Shape Area: " << shape->getArea() << endl;
    }

    return 0;
}
