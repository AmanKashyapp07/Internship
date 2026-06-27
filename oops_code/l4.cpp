/**
 * @file l4_constructors_destructors.cpp
 * @brief Lecture 4: Constructors, Destructors & Special Features (Types of Ctors, Member Initializer List, explicit, Rule of 3/5/0, Shallow/Deep Copy, Friend, RVO)
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// === 1. Types of Constructors & Initializer List ===
class Point {
public:
    int x, y;

    // Default constructor
    Point() : x(0), y(0) {
        cout << "Default constructor called\n";
    }

    // Parameterized constructor
    Point(int x, int y) : x(x), y(y) {
        cout << "Parameterized constructor called\n";
    }

    // Delegating constructor: delegates constructor duties to another constructor
    Point(int val) : Point(val, val) {
        cout << "Delegating constructor called\n";
    }

    // Copy Constructor (takes const ref to prevent infinite recursion)
    Point(const Point& other) : x(other.x), y(other.y) {
        cout << "Copy constructor called\n";
    }
};

// Initializer List order trap: members are initialized in DECLARATION order, not list order!
class Trap {
private:
    [[maybe_unused]] int b; // declared first
    [[maybe_unused]] int a; // declared second

public:
    // Trap(int x) : a(x), b(a) {} // DANGEROUS: b is initialized first using unitialized 'a'.
    Trap(int x) : b(x), a(x) {} // SAFE (list order matches declaration order)
};

// === 2. explicit keyword ===
class Wrapper {
private:
    int val;

public:
    // Mark single-argument constructor explicit to prevent implicit type conversions
    explicit Wrapper(int v) : val(v) {}
    int getVal() const { return val; }
};

void printWrapper(const Wrapper& w) {
    cout << "Wrapper value: " << w.getVal() << "\n";
}

// === 3. Rule of Three / Five / Zero & Shallow/Deep Copy ===

// Deep Copy Class (Rule of Five implementation)
class DynamicArray {
private:
    int* data;
    size_t size;

public:
    // Constructor
    DynamicArray(size_t n) : data(new int[n]()), size(n) {
        cout << "Acquired data memory of size " << size << "\n";
    }

    // 1. Destructor
    virtual ~DynamicArray() {
        if (data) {
            delete[] data;
            cout << "Released data memory of size " << size << "\n";
        }
    }

    // 2. Copy Constructor (Deep copy)
    DynamicArray(const DynamicArray& other) : data(new int[other.size]), size(other.size) {
        cout << "Deep copy constructor called\n";
        std::copy(other.data, other.data + size, data);
    }

    // 3. Copy Assignment Operator (Deep copy)
    DynamicArray& operator=(const DynamicArray& other) {
        cout << "Deep copy assignment operator called\n";
        if (this == &other) return *this; // Self-assignment check

        delete[] data; // Free existing resource
        size = other.size;
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        return *this;
    }

    // 4. Move Constructor (C++11: resource stealing, marked noexcept for STL container compatibility)
    DynamicArray(DynamicArray&& other) noexcept : data(other.data), size(other.size) {
        cout << "Move constructor called (stole resource)\n";
        other.data = nullptr; // Reset other object to prevent double-free
        other.size = 0;
    }

    // 5. Move Assignment Operator (C++11: resource stealing)
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        cout << "Move assignment operator called\n";
        if (this == &other) return *this;

        delete[] data; // Free current resource
        data = other.data; // Steal resource
        size = other.size;
        other.data = nullptr; // Reset other object
        other.size = 0;
        return *this;
    }

    void set(size_t idx, int val) { if (idx < size) data[idx] = val; }
    int get(size_t idx) const { return (idx < size) ? data[idx] : -1; }
};

// Rule of Zero: Let the compiler generate all destructor/copy/move constructors
// by using RAII-wrapped standard members (like std::vector / std::string).
class ModernClass {
private:
    vector<int> data;
    string name;

public:
    ModernClass(string n, size_t size) : data(size, 0), name(n) {}
    // Zero custom dtor, copy/move ctor, assignment operators needed!
};

// === 4. Friend Declarations ===
class Matrix;

class Vector3D {
private:
    double coords[3];

public:
    Vector3D(double x, double y, double z) : coords{x, y, z} {}

    // Friend function declaration: not a member, but has access to private members
    friend double dotProduct(const Vector3D& a, const Vector3D& b);

    // Friend class declaration
    friend class Matrix;
};

double dotProduct(const Vector3D& a, const Vector3D& b) {
    return a.coords[0]*b.coords[0] + a.coords[1]*b.coords[1] + a.coords[2]*b.coords[2];
}

class Matrix {
public:
    void printVector(const Vector3D& v) {
        // Can access private member 'coords' due to friend status
        cout << "Vector coordinates: " << v.coords[0] << ", " 
             << v.coords[1] << ", " << v.coords[2] << "\n";
    }
};

// === 5. Return Value Optimization (RVO) / Copy Elision ===
class Heavy {
public:
    Heavy() { cout << "Heavy constructed\n"; }
    Heavy(const Heavy&) { cout << "Heavy copy-constructed\n"; }
    Heavy(Heavy&&) noexcept { cout << "Heavy move-constructed\n"; }
};

Heavy createHeavy() {
    return Heavy(); // RVO: compiler constructs the object in-place directly in caller's memory
}

int main() {
    cout << "=== 1. Constructor Types ===\n";
    Point p1;        // Default
    Point p2(3, 4);  // Parameterized
    Point p3(5);     // Delegating
    Point p4 = p2;   // Copy

    cout << "\n=== 2. explicit Keyword ===\n";
    Wrapper w(42);
    printWrapper(w);
    // printWrapper(42); // COMPILE ERROR: explicit constructor blocks implicit conversion from int to Wrapper
    printWrapper(Wrapper(42)); // OK: explicit construction

    cout << "\n=== 3. Rule of Five & Shallow/Deep Copy ===\n";
    {
        DynamicArray arr1(5);
        arr1.set(0, 99);

        // Deep copy: allocates new memory
        DynamicArray arr2 = arr1; 
        arr2.set(0, 100);
        cout << "arr1[0] = " << arr1.get(0) << " | arr2[0] = " << arr2.get(0) << " (Distinct memory!)\n";

        // Move semantics: transfer ownership
        DynamicArray arr3 = std::move(arr1); // Calls Move constructor
        cout << "arr3[0] = " << arr3.get(0) << "\n";
        // Accessing arr1 now is undefined / dangerous as its pointer is nullptr
    }

    cout << "\n=== 4. Friend Functions and Classes ===\n";
    Vector3D v1(1, 2, 3), v2(4, 5, 6);
    cout << "Dot product: " << dotProduct(v1, v2) << "\n";
    
    Matrix m;
    m.printVector(v1);

    cout << "\n=== 5. Copy Elision / RVO ===\n";
    [[maybe_unused]] Heavy h = createHeavy(); // In modern C++, copy/move is completely elided here.

    return 0;
}
