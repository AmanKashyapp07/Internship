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
// 1. FIXED-SIZE ARRAY-BASED STACK
// ============================================================================
class ArrayStack {
private:
    int* arr;
    int capacity;
    int topIndex;

public:
    ArrayStack(int cap = 100) {
        capacity = cap;
        arr = new int[capacity];
        topIndex = -1; // Empty stack indicator
    }

    ~ArrayStack() {
        delete[] arr;
    }

    // Push element to the top: O(1)
    bool push(int val) {
        if (isFull()) {
            cout << "[ArrayStack Error] Stack Overflow! Cannot push " << val << "\n";
            return false;
        }
        arr[++topIndex] = val;
        return true;
    }

    // Remove top element: O(1)
    bool pop() {
        if (isEmpty()) {
            cout << "[ArrayStack Error] Stack Underflow! Cannot pop from empty stack\n";
            return false;
        }
        topIndex--;
        return true;
    }

    // Get top element without removing: O(1)
    int top() const {
        if (isEmpty()) {
            throw runtime_error("[ArrayStack Error] Stack is empty! No top element.");
        }
        return arr[topIndex];
    }

    // Check if stack is empty: O(1)
    bool isEmpty() const {
        return topIndex == -1;
    }

    // Check if stack is full: O(1)
    bool isFull() const {
        return topIndex == capacity - 1;
    }

    // Current number of elements: O(1)
    int size() const {
        return topIndex + 1;
    }

    void display() const {
        if (isEmpty()) {
            cout << "[ArrayStack] Empty\n";
            return;
        }
        cout << "[ArrayStack Top -> Bottom]: ";
        for (int i = topIndex; i >= 0; i--) {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }
};

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

// ============================================================================
// 3. LINKED-LIST BASED STACK
// ============================================================================
class LinkedListStack {
private:
    struct Node {
        int val;
        Node* next;
        Node(int v, Node* n = nullptr) : val(v), next(n) {}
    };

    Node* head; // Points to the top of the stack
    int count;

public:
    LinkedListStack() : head(nullptr), count(0) {}

    // Destructor: Clean up all nodes to prevent memory leaks
    ~LinkedListStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Push element to top (Insert at Head): O(1)
    void push(int val) {
        Node* newNode = new Node(val, head);
        head = newNode;
        count++;
    }

    // Pop element from top (Delete Head): O(1)
    bool pop() {
        if (isEmpty()) {
            cout << "[LinkedListStack Error] Stack Underflow! Stack is empty\n";
            return false;
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        return true;
    }

    // Access top element: O(1)
    int top() const {
        if (isEmpty()) {
            throw runtime_error("[LinkedListStack Error] Stack is empty!");
        }
        return head->val;
    }

    // Check if empty: O(1)
    bool isEmpty() const {
        return head == nullptr;
    }

    // Linked-list stack is theoretically never full unless system runs out of heap memory
    bool isFull() const {
        return false;
    }

    int size() const {
        return count;
    }

    void display() const {
        if (isEmpty()) {
            cout << "[LinkedListStack] Empty\n";
            return;
        }
        cout << "[LinkedListStack Top -> Bottom]: ";
        Node* curr = head;
        while (curr != nullptr) {
            cout << curr->val << " -> ";
            curr = curr->next;
        }
        cout << "NULL\n";
    }
};

// ============================================================================
// TEST DRIVER / INTERVIEW DEMONSTRATION
// ============================================================================
int main() {
    cout << "========================================\n";
    cout << "  1. TESTING FIXED-SIZE ARRAY STACK\n";
    cout << "========================================\n";
    ArrayStack arrStk(3);
    arrStk.push(10);
    arrStk.push(20);
    arrStk.push(30);
    arrStk.display();
    cout << "isFull? " << (arrStk.isFull() ? "Yes" : "No") << "\n";
    arrStk.push(40); // Should trigger overflow

    cout << "Top element: " << arrStk.top() << "\n";
    arrStk.pop();
    cout << "After 1 pop, top: " << arrStk.top() << ", size: " << arrStk.size() << "\n";
    arrStk.pop();
    arrStk.pop();
    arrStk.pop(); // Should trigger underflow

    cout << "\n========================================\n";
    cout << "  2. TESTING DYNAMIC RESIZING STACK\n";
    cout << "========================================\n";
    DynamicArrayStack dynStk(2);
    cout << "Initial capacity: " << dynStk.getCapacity() << "\n";
    for (int i = 1; i <= 5; i++) {
        dynStk.push(i * 10);
        cout << "Pushed " << i * 10 << " | Size: " << dynStk.size() << " | Capacity: " << dynStk.getCapacity() << "\n";
    }

    cout << "\n========================================\n";
    cout << "  3. TESTING LINKED-LIST STACK\n";
    cout << "========================================\n";
    LinkedListStack llStk;
    llStk.push(100);
    llStk.push(200);
    llStk.push(300);
    llStk.display();

    cout << "Top element: " << llStk.top() << "\n";
    llStk.pop();
    llStk.display();
    cout << "Size: " << llStk.size() << ", isEmpty: " << (llStk.isEmpty() ? "Yes" : "No") << "\n";

    return 0;
}
