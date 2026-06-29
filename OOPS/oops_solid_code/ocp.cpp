/**
 * @file ocp.cpp
 * @brief Demonstration of the Open/Closed Principle (OCP)
 * 
 * OCP: Software entities should be open for extension, but closed for modification.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// ============================================================================
// ❌ VIOLATION: An AreaCalculator that checks shape types. Adding a new shape
// requires modifying the calculator (violates OCP).
// ============================================================================
enum class ShapeType {
    Rectangle,
    Circle
    // If we want to add Triangle, we must modify the enum and the calculator.
};

struct BadRectangle {
    double width;
    double height;
};

struct BadCircle {
    double radius;
};

class BadAreaCalculator {
public:
    double calculateArea(const vector<pair<ShapeType, void*>>& shapes) const {
        double totalArea = 0.0;
        for (const auto& shapePair : shapes) {
            if (shapePair.first == ShapeType::Rectangle) {
                auto* rect = static_cast<BadRectangle*>(shapePair.second);
                totalArea += rect->width * rect->height;
            } else if (shapePair.first == ShapeType::Circle) {
                auto* circle = static_cast<BadCircle*>(shapePair.second);
                totalArea += M_PI * circle->radius * circle->radius;
            }
            // To add a Triangle here, we must add another 'else if' block!
        }
        return totalArea;
    }
};

// ============================================================================
// ✅ RESOLUTION: Introduce a Shape abstraction. AreaCalculator works with
// the abstraction, so new shapes can be added without modifying existing code.
// ============================================================================

// Interface for shapes
class Shape {
public:
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
};

// Rectangle extension
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double getArea() const override {
        return width * height;
    }
};

// Circle extension
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}
    double getArea() const override {
        return M_PI * radius * radius;
    }
};

// Triangle extension (Added cleanly without changing existing calculator)
class Triangle : public Shape {
private:
    double base;
    double height;

public:
    Triangle(double b, double h) : base(b), height(h) {}
    double getArea() const override {
        return 0.5 * base * height;
    }
};

// The calculator is now closed for modification but open for extension!
class AreaCalculator {
public:
    double calculateArea(const vector<Shape*>& shapes) const {
        double totalArea = 0.0;
        for (const auto& shape : shapes) {
            if (shape != nullptr) {
                totalArea += shape->getArea();
            }
        }
        return totalArea;
    }
};

int main() {
    cout << "=== OCP Demonstration ===" << endl << endl;

    // ❌ Testing Bad Design
    cout << "[Bad Design Example]" << endl;
    BadRectangle rect{10.0, 5.0};
    BadCircle circle{3.0};
    vector<pair<ShapeType, void*>> badShapes = {
        {ShapeType::Rectangle, &rect},
        {ShapeType::Circle, &circle}
    };
    BadAreaCalculator badCalc;
    cout << "Total Area (Bad): " << badCalc.calculateArea(badShapes) << endl;

    cout << endl;

    // ✅ Testing Good Design (OCP Compliant)
    cout << "[Good Design Example]" << endl;
    Rectangle r(10.0, 5.0);
    Circle c(3.0);
    Triangle t(4.0, 5.0); // Extending behavior cleanly with a new class

    vector<Shape*> goodShapes = {&r, &c, &t};
    AreaCalculator goodCalc;
    cout << "Total Area (Good, including Triangle): " << goodCalc.calculateArea(goodShapes) << endl;

    return 0;
}
