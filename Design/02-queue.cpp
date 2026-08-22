#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 02: QUEUE IMPLEMENTATIONS
 * ============================================================================
 * 
 * A Queue is a FIFO (First-In, First-Out) linear data structure.
 * 
 * Frequently Asked in Interviews:
 * 1. Linear Array-based Queue (and why it suffers from "False Overflow")
 * 2. Circular Array-based Queue (modulo arithmetic to reuse freed space)
 * 3. Singly Linked List-based Queue (with front & rear pointers for O(1) ops)
 * 
 * Time Complexities:
 * - enqueue() : O(1)
 * - dequeue() : O(1)
 * - getFront(): O(1)
 * - getRear() : O(1)
 * - isEmpty() : O(1)
 * - isFull()  : O(1)
 * Space Complexity: O(N)
 */

// ============================================================================
// 1. CIRCULAR ARRAY-BASED QUEUE (Optimal Array Queue)
// ============================================================================
class CircularQueue {
private:
    int* arr;
    int capacity;
    int front;
    int rear;
    int count; // Tracks current number of elements

public:
    CircularQueue(int cap = 5) {
        capacity = cap;
        arr = new int[capacity];
        front = 0;
        rear = -1;
        count = 0;
    }

    ~CircularQueue() {
        delete[] arr;
    }

    // Insert element at the rear: O(1)
    bool enqueue(int val) {
        if (isFull()) {
            cout << "[CircularQueue Error] Queue Overflow! Cannot enqueue " << val << "\n";
            return false;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = val;
        count++;
        return true;
    }

    // Remove element from the front: O(1)
    bool dequeue() {
        if (isEmpty()) {
            cout << "[CircularQueue Error] Queue Underflow! Cannot dequeue from empty queue\n";
            return false;
        }
        front = (front + 1) % capacity;
        count--;
        return true;
    }

    // Access front element: O(1)
    int getFront() const {
        if (isEmpty()) {
            throw runtime_error("[CircularQueue Error] Queue is empty! No front element.");
        }
        return arr[front];
    }

    // Access rear element: O(1)
    int getRear() const {
        if (isEmpty()) {
            throw runtime_error("[CircularQueue Error] Queue is empty! No rear element.");
        }
        return arr[rear];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    int size() const {
        return count;
    }

    void display() const {
        if (isEmpty()) {
            cout << "[CircularQueue] Empty\n";
            return;
        }
        cout << "[CircularQueue Front -> Rear]: ";
        for (int i = 0; i < count; i++) {
            int idx = (front + i) % capacity;
            cout << arr[idx] << " ";
        }
        cout << "\n";
    }
};

// ============================================================================
// 2. LINKED-LIST BASED QUEUE
// ============================================================================
class LinkedListQueue {
private:
    struct Node {
        int val;
        Node* next;
        Node(int v, Node* n = nullptr) : val(v), next(n) {}
    };

    Node* frontNode; // Deletion happens here: O(1)
    Node* rearNode;  // Insertion happens here: O(1)
    int count;

public:
    LinkedListQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~LinkedListQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue at rear: O(1)
    void enqueue(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    // Dequeue from front: O(1)
    bool dequeue() {
        if (isEmpty()) {
            cout << "[LinkedListQueue Error] Queue Underflow! Queue is empty\n";
            return false;
        }
        Node* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        count--;
        if (frontNode == nullptr) {
            rearNode = nullptr; // Reset rear when queue becomes empty
        }
        return true;
    }

    int getFront() const {
        if (isEmpty()) {
            throw runtime_error("[LinkedListQueue Error] Queue is empty!");
        }
        return frontNode->val;
    }

    int getRear() const {
        if (isEmpty()) {
            throw runtime_error("[LinkedListQueue Error] Queue is empty!");
        }
        return rearNode->val;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }

    bool isFull() const {
        return false; // Dynamic memory: only bounded by system RAM
    }

    int size() const {
        return count;
    }

    void display() const {
        if (isEmpty()) {
            cout << "[LinkedListQueue] Empty\n";
            return;
        }
        cout << "[LinkedListQueue Front -> Rear]: ";
        Node* curr = frontNode;
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
    cout << "  1. TESTING CIRCULAR ARRAY QUEUE\n";
    cout << "========================================\n";
    CircularQueue cq(4);
    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);
    cq.enqueue(40);
    cq.display();
    cout << "isFull? " << (cq.isFull() ? "Yes" : "No") << "\n";
    cq.enqueue(50); // Overflow attempt

    cout << "Front: " << cq.getFront() << " | Rear: " << cq.getRear() << "\n";

    // Dequeue two elements
    cq.dequeue();
    cq.dequeue();
    cout << "After 2 dequeues:\n";
    cq.display();

    // Enqueue new elements to test circular wrap-around
    cout << "Enqueuing 50 and 60 (testing circular wrap-around):\n";
    cq.enqueue(50);
    cq.enqueue(60);
    cq.display();
    cout << "Front: " << cq.getFront() << " | Rear: " << cq.getRear() << "\n";

    cout << "\n========================================\n";
    cout << "  2. TESTING LINKED-LIST QUEUE\n";
    cout << "========================================\n";
    LinkedListQueue lq;
    lq.enqueue(100);
    lq.enqueue(200);
    lq.enqueue(300);
    lq.display();

    cout << "Front: " << lq.getFront() << " | Rear: " << lq.getRear() << "\n";
    lq.dequeue();
    lq.display();
    cout << "Size: " << lq.size() << " | isEmpty: " << (lq.isEmpty() ? "Yes" : "No") << "\n";

    lq.dequeue();
    lq.dequeue();
    lq.dequeue(); // Underflow attempt

    return 0;
}
