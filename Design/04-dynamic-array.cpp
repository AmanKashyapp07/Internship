#include <iostream>
#include <stdexcept>
#include <utility>
using namespace std;

/**
 * ============================================================================
 * DESIGN 04: DYNAMIC ARRAY (Vector Clone)
 * ============================================================================
 * 
 * A Dynamic Array is a resizable array that provides random access O(1)
 * and amortized O(1) insertion at the end.
 * 
 * Frequently Asked in Interviews:
 * 1. Resizing Strategy: Why double capacity (2x) instead of adding constant (+K)?
 *    - Geometric progression (doubling) guarantees AMORTIZED O(1) insertion.
 *    - Adding fixed K causes O(N^2) total work for N insertions (average O(N) per push).
 * 2. Rule of Five / Memory Management:
 *    - Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment.
 * 3. Core Operations:
 *    - push_back, pop_back, insert, erase, operator[], at, reserve, shrink_to_fit.
 * 
 * Time Complexities:
 * - Access operator[] : O(1)
 * - push_back()       : Amortized O(1) [Worst-case O(N) when resizing]
 * - pop_back()        : O(1)
 * - insert(idx, val)  : O(N) [Shifting elements]
 * - erase(idx)        : O(N) [Shifting elements]
 * Space Complexity: O(N)
 */

template <typename T>
class Vector {
private:
    T* data;
    size_t sz;       // Number of active elements
    size_t cap;      // Allocated buffer capacity

    // Reallocate internal buffer to new capacity
    void reallocate(size_t newCap) {
        if (newCap < sz) newCap = sz; // Ensure capacity can hold existing elements
        if (newCap == 0) newCap = 1;

        T* newData = new T[newCap];
        for (size_t i = 0; i < sz; i++) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    // ========================================================================
    // 1. CONSTRUCTORS & RULE OF FIVE
    // ========================================================================

    // Default & Capacity Constructor
    Vector(size_t initialCap = 2) : sz(0), cap(initialCap) {
        if (cap == 0) cap = 1;
        data = new T[cap];
    }

    // Fill Constructor (size n with default value)
    Vector(size_t n, const T& val) : sz(n), cap(n * 2) {
        data = new T[cap];
        for (size_t i = 0; i < sz; i++) {
            data[i] = val;
        }
    }

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Copy Constructor (Deep Copy)
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (size_t i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
    }

    // Copy Assignment Operator (Copy-and-Swap idiom / Deep Copy)
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (size_t i = 0; i < sz; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move Constructor (Transfer Ownership)
    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Move Assignment Operator
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // ========================================================================
    // 2. CAPACITY & SIZING
    // ========================================================================

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cap;
    }

    bool empty() const {
        return sz == 0;
    }

    // Pre-allocate memory to avoid multiple resizes
    void reserve(size_t newCap) {
        if (newCap > cap) {
            reallocate(newCap);
        }
    }

    // Shrink unused buffer capacity to fit exact size
    void shrink_to_fit() {
        if (cap > sz) {
            reallocate(sz == 0 ? 1 : sz);
        }
    }

    void clear() {
        sz = 0; // Logically clear elements without deallocating buffer
    }

    // ========================================================================
    // 3. ELEMENT ACCESS
    // ========================================================================

    // Direct access (unchecked for performance): O(1)
    T& operator[](size_t idx) {
        return data[idx];
    }

    const T& operator[](size_t idx) const {
        return data[idx];
    }

    // Bounds-checked access: O(1)
    T& at(size_t idx) {
        if (idx >= sz) {
            throw out_of_range("[Vector Error] Index out of range: " + to_string(idx));
        }
        return data[idx];
    }

    const T& at(size_t idx) const {
        if (idx >= sz) {
            throw out_of_range("[Vector Error] Index out of range: " + to_string(idx));
        }
        return data[idx];
    }

    T& front() {
        if (empty()) throw runtime_error("[Vector Error] Vector is empty!");
        return data[0];
    }

    T& back() {
        if (empty()) throw runtime_error("[Vector Error] Vector is empty!");
        return data[sz - 1];
    }

    // ========================================================================
    // 4. MODIFIERS (PUSH, POP, INSERT, ERASE)
    // ========================================================================

    // Append element: Amortized O(1)
    void push_back(const T& val) {
        if (sz == cap) {
            reallocate(cap * 2); // Double capacity when full
        }
        data[sz++] = val;
    }

    // Append element with move semantics
    void push_back(T&& val) {
        if (sz == cap) {
            reallocate(cap * 2);
        }
        data[sz++] = std::move(val);
    }

    // Remove last element: O(1)
    void pop_back() {
        if (empty()) {
            throw runtime_error("[Vector Error] Cannot pop from empty vector!");
        }
        sz--;
    }

    // Insert element at index: O(N) [Shifts subsequent elements right]
    void insert(size_t idx, const T& val) {
        if (idx > sz) {
            throw out_of_range("[Vector Error] Insert index out of range: " + to_string(idx));
        }
        if (sz == cap) {
            reallocate(cap * 2);
        }
        // Shift elements right
        for (size_t i = sz; i > idx; i--) {
            data[i] = std::move(data[i - 1]);
        }
        data[idx] = val;
        sz++;
    }

    // Erase element at index: O(N) [Shifts subsequent elements left]
    void erase(size_t idx) {
        if (idx >= sz) {
            throw out_of_range("[Vector Error] Erase index out of range: " + to_string(idx));
        }
        // Shift elements left
        for (size_t i = idx; i < sz - 1; i++) {
            data[i] = std::move(data[i + 1]);
        }
        sz--;
    }

    // ========================================================================
    // 5. ITERATOR SUPPORT (for range-based for loops)
    // ========================================================================
    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + sz; }
    const T* end() const { return data + sz; }

    void print() const {
        cout << "[Vector Size: " << sz << " | Cap: " << cap << "]: [ ";
        for (size_t i = 0; i < sz; i++) {
            cout << data[i] << (i + 1 < sz ? ", " : " ");
        }
        cout << "]\n";
    }
};

// ============================================================================
// TEST DRIVER / INTERVIEW DEMONSTRATION
// ============================================================================
int main() {
    cout << "========================================\n";
    cout << "  1. PUSH_BACK & GEOMETRIC EXPANSION\n";
    cout << "========================================\n";
    Vector<int> vec;
    cout << "Initial size: " << vec.size() << " | Capacity: " << vec.capacity() << "\n";

    for (int i = 1; i <= 10; i++) {
        vec.push_back(i * 10);
        cout << "Pushed " << i * 10 << " -> Size: " << vec.size() << " | Cap: " << vec.capacity() << "\n";
    }
    vec.print();

    cout << "\n========================================\n";
    cout << "  2. ELEMENT ACCESS & RANGE-BASED LOOP\n";
    cout << "========================================\n";
    cout << "Front: " << vec.front() << " | Back: " << vec.back() << " | vec[3]: " << vec[3] << "\n";
    cout << "Iterating via Range-Based For Loop: ";
    for (const auto& item : vec) {
        cout << item << " ";
    }
    cout << "\n";

    cout << "\n========================================\n";
    cout << "  3. INSERTION & DELETION (SHIFTING)\n";
    cout << "========================================\n";
    cout << "Inserting 999 at index 3...\n";
    vec.insert(3, 999);
    vec.print();

    cout << "Erasing element at index 3...\n";
    vec.erase(3);
    vec.print();

    cout << "Popping 2 elements from back...\n";
    vec.pop_back();
    vec.pop_back();
    vec.print();

    cout << "\n========================================\n";
    cout << "  4. RESERVE & SHRINK TO FIT\n";
    cout << "========================================\n";
    cout << "Current Capacity before shrink: " << vec.capacity() << "\n";
    vec.shrink_to_fit();
    cout << "Capacity after shrink_to_fit: " << vec.capacity() << "\n";

    vec.reserve(50);
    cout << "Capacity after reserve(50): " << vec.capacity() << "\n";

    cout << "\n========================================\n";
    cout << "  5. COPY & MOVE SEMANTICS\n";
    cout << "========================================\n";
    Vector<int> copyVec = vec; // Copy constructor
    cout << "Copied vector size: " << copyVec.size() << "\n";

    Vector<int> movedVec = std::move(copyVec); // Move constructor
    cout << "Moved vector size: " << movedVec.size() << " | Source size after move: " << copyVec.size() << "\n";

    return 0;
}
