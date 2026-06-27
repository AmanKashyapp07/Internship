/**
 * @file l6_smart_pointers_memory.cpp
 * @brief Lecture 6: Smart Pointers & Memory Management (unique_ptr, shared_ptr, weak_ptr, struct padding, EBO, RAII, move semantics, std::optional, std::variant)
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <optional>
#include <variant>
#include <utility>

using namespace std;

// === 1. Smart Pointers Setup ===
class Resource {
public:
    Resource()  { cout << "Resource acquired\n"; }
    ~Resource() { cout << "Resource released\n"; }
    void use()  { cout << "Using Resource\n"; }
};

// Custom deleter example
void customDeleter(Resource* r) {
    cout << "Custom deleter active: cleaning up Resource...\n";
    delete r;
}

// === 2. Circular Reference & weak_ptr solution ===
struct Node {
    int id;
    shared_ptr<Node> next;
    
    // Violation: shared_ptr<Node> prev; // causes circular dependency leak!
    // Resolution: non-owning weak_ptr back-pointer
    weak_ptr<Node> prev;

    Node(int val) : id(val) {}
    ~Node() { cout << "Node " << id << " destroyed\n"; }
};

// Enable shared_ptr from this (get shared_ptr of 'this' safely)
class SharedSelfDemo : public std::enable_shared_from_this<SharedSelfDemo> {
public:
    shared_ptr<SharedSelfDemo> getShared() {
        return shared_from_this(); // safe, does not create secondary control block
    }
    void show() { cout << "SharedSelfDemo active\n"; }
};

// === 3. Struct Padding & Sizing ===

// Alignment padding: total size is 24 bytes on 64-bit architecture
struct BadAlign {
    char c;    // 1 byte  + 3 bytes padding
    int i;     // 4 bytes
    char c2;   // 1 byte  + 7 bytes padding
    double d;  // 8 bytes
};

// Reordered members: size reduced to 16 bytes
struct GoodAlign {
    double d;  // 8 bytes
    int i;     // 4 bytes
    char c;    // 1 byte
    char c2;   // 1 byte
               // 2 bytes padding at end
};

// Standalone empty class takes 1 byte to ensure unique address
class EmptyClass {};

// Empty Base Optimization (EBO): Base class consumes 0 bytes when inherited
class DerivedFromEmpty : public EmptyClass {
    [[maybe_unused]] int val; // 4 bytes
};

// === 4. Custom RAII Scoped Lock ===
class CustomScopedLock {
private:
    mutex& mtx;

public:
    explicit CustomScopedLock(mutex& m) : mtx(m) {
        mtx.lock();
        cout << "Mutex locked (Resource acquired in constructor)\n";
    }

    ~CustomScopedLock() {
        mtx.unlock();
        cout << "Mutex unlocked (Resource released in destructor)\n";
    }

    // Disable copy constructor and assignment operator to prevent duplicate locks
    CustomScopedLock(const CustomScopedLock&) = delete;
    CustomScopedLock& operator=(const CustomScopedLock&) = delete;
};

// === 5. Move Semantics & Perfect Forwarding ===

// Perfect forwarding target function
void processValue(int& lval) {
    cout << "processValue: Lvalue reference received (" << lval << ")\n";
}

void processValue(int&& rval) {
    cout << "processValue: Rvalue reference received (" << rval << ")\n";
}

// Universal reference wrapper template using perfect forwarding
template<typename T>
void wrapper(T&& arg) {
    // std::forward preserves the lvalue/rvalue category of the original argument passed
    processValue(std::forward<T>(arg));
}

// === 6. std::optional & std::variant ===
optional<int> safeDivide(int num, int den) {
    if (den == 0) return nullopt; // empty state
    return num / den;
}

int main() {
    cout << "=== 1. unique_ptr (Exclusive Ownership) ===\n";
    {
        unique_ptr<Resource> u1 = make_unique<Resource>();
        u1->use();

        // unique_ptr<Resource> u2 = u1; // COMPILE ERROR: Copy constructor is deleted
        unique_ptr<Resource> u2 = std::move(u1); // OK: Ownership transferred via move
        if (!u1) {
            cout << "u1 is now empty / nullptr\n";
        }
        u2->use();
    } // Resource automatically released here

    // custom deleter
    {
        unique_ptr<Resource, decltype(&customDeleter)> uCustom(new Resource(), customDeleter);
    }

    // array form
    {
        unique_ptr<int[]> arr = make_unique<int[]>(3);
        arr[0] = 100;
        cout << "unique_ptr array element 0: " << arr[0] << "\n";
    } // delete[] called automatically

    cout << "\n=== 2. shared_ptr & weak_ptr ===\n";
    shared_ptr<Resource> sp1 = make_shared<Resource>();
    cout << "Initial sp1 use count: " << sp1.use_count() << "\n";
    {
        shared_ptr<Resource> sp2 = sp1;
        cout << "sp2 copies sp1, use count: " << sp1.use_count() << "\n";
    }
    cout << "sp2 out of scope, use count: " << sp1.use_count() << "\n";

    // Testing weak_ptr expired check and locking
    weak_ptr<Resource> wp1 = sp1;
    if (auto locked = wp1.lock()) {
        cout << "Lock acquired on weak_ptr\n";
        locked->use();
    }
    sp1.reset(); // Resource destroyed since use_count drops to 0
    if (wp1.expired()) {
        cout << "weak_ptr is now expired (Resource was destroyed)\n";
    }

    // Breaking Circular References Example
    {
        auto n1 = make_shared<Node>(1);
        auto n2 = make_shared<Node>(2);
        n1->next = n2;
        n2->prev = n1; // weak_ptr backlink breaks the cycle
    } // Both nodes destroyed correctly! No memory leaks.

    // enable_shared_from_this
    {
        shared_ptr<SharedSelfDemo> demo = make_shared<SharedSelfDemo>();
        shared_ptr<SharedSelfDemo> demoRef = demo->getShared();
        demoRef->show();
    }

    cout << "\n=== 3. Sizing & Alignment padding ===\n";
    cout << "Size of BadAlign struct: " << sizeof(BadAlign) << " bytes\n";
    cout << "Size of GoodAlign struct (optimized): " << sizeof(GoodAlign) << " bytes\n";
    cout << "Size of EmptyClass: " << sizeof(EmptyClass) << " byte\n";
    cout << "Size of DerivedFromEmpty (EBO): " << sizeof(DerivedFromEmpty) << " bytes\n";

    cout << "\n=== 4. Custom RAII implementation ===\n";
    mutex dummyMutex;
    {
        CustomScopedLock lock(dummyMutex);
        cout << "Performing thread-safe operations in scope...\n";
    } // Lock automatically goes out of scope and releases the mutex

    cout << "\n=== 5. Move Semantics & Perfect Forwarding ===\n";
    int x = 5;
    wrapper(x);           // Passes lvalue -> calls processValue(int&)
    wrapper(10);          // Passes rvalue -> calls processValue(int&&)
    wrapper(std::move(x)); // Passes rvalue reference via move -> calls processValue(int&&)

    cout << "\n=== 6. std::optional & std::variant (C++17) ===\n";
    // optional demo
    auto divResult = safeDivide(10, 2);
    if (divResult.has_value()) {
        cout << "Division result: " << divResult.value() << "\n";
    }
    auto divResultFail = safeDivide(10, 0);
    cout << "Division by zero result or default: " << divResultFail.value_or(-1) << "\n";

    // variant demo (type-safe union)
    variant<int, string, double> myVariant;
    myVariant = 42;
    cout << "Variant holds int: " << get<int>(myVariant) << "\n";
    myVariant = "Hello Modern C++";
    cout << "Variant holds string: " << get<string>(myVariant) << "\n";

    // visiting variant (pattern matching)
    std::visit([](const auto& val) {
        cout << "Visiting variant: " << val << "\n";
    }, myVariant);

    return 0;
}
