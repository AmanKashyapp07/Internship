#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 01: STACK IMPLEMENTATIONS
 * ============================================================================
 * 
 * A Stack is a LIFO (Last-In, First-Out) linear data structure.
 * 
 * Frequently Asked in Interviews:
 * 1. Fixed-size Array-based Stack (handling overflow/underflow)
 * 2. Dynamic Resizing Array Stack (doubling capacity - vector-like amortized O(1))
 * 3. Singly Linked List-based Stack (dynamic sizing, no fixed capacity limits)
 * 
 * Time Complexities:
 * - push()   : O(1) [Amortized O(1) for dynamic array]
 * - pop()    : O(1)
 * - top()    : O(1)
 * - isEmpty(): O(1)
 * - isFull() : O(1)
 * Space Complexity: O(N) where N is number of elements
 */


// ============================================================================
// 2. DYNAMIC RESIZING ARRAY STACK (Auto-doubles capacity)
// ============================================================================
class DynamicArrayStack {
private:
    int* arr;
    int capacity;
    int topIndex;

    void resize(int newCap) {
        int* newArr = new int[newCap];
        for (int i = 0; i <= topIndex; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCap;
    }

public:
    DynamicArrayStack(int initialCap = 2) {
        capacity = initialCap;
        arr = new int[capacity];
        topIndex = -1;
    }

    ~DynamicArrayStack() {
        delete[] arr;
    }

    // Amortized O(1) push with capacity doubling
    void push(int val) {
        if (topIndex == capacity - 1) {
            resize(capacity * 2); // Double capacity when full
        }
        arr[++topIndex] = val;
    }

    // O(1) pop with optional shrink when 1/4 full to save memory
    bool pop() {
        if (isEmpty()) {
            cout << "[DynamicStack Error] Stack Underflow!\n";
            return false;
        }
        topIndex--;
        // Shrink capacity by half if elements occupy 1/4 of array (minimum cap = 2)
        if (topIndex >= 0 && topIndex + 1 == capacity / 4 && capacity / 2 >= 2) {
            resize(capacity / 2);
        }
        return true;
    }

    int top() const {
        if (isEmpty()) {
            throw runtime_error("[DynamicStack Error] Stack is empty!");
        }
        return arr[topIndex];
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }

    int getCapacity() const {
        return capacity;
    }
};

