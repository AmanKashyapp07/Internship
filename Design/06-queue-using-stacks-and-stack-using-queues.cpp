#include <iostream>
#include <stack>
#include <queue>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 06: QUEUE USING STACKS & STACK USING QUEUES
 * ============================================================================
 * 
 * Classic interview "twist" questions testing mastery of LIFO vs FIFO adapters.
 * 
 * ----------------------------------------------------------------------------
 * PART A: QUEUE USING TWO STACKS
 * ----------------------------------------------------------------------------
 * Invariant: Two stacks `inStack` and `outStack`.
 * - push(x) : Push directly to `inStack` -> O(1)
 * - pop()   : If `outStack` is empty, pour all elements from `inStack` to `outStack`.
 *             Pop from `outStack`.
 * - peek()  : Same transfer logic; return `outStack.top()`.
 * 
 * Amortized Complexity Analysis:
 * - Each element is pushed to `inStack` once (1 op), moved to `outStack` once (1 op),
 *   and popped from `outStack` once (1 op).
 * - Total: Exactly 3 operations per element over its entire lifetime.
 * - Amortized Time Complexity: O(1) per operation!
 * 
 * ----------------------------------------------------------------------------
 * PART B: STACK USING QUEUES
 * ----------------------------------------------------------------------------
 * 1. Two Queues (Push Costly):
 *    - Enqueue to q2, pour all of q1 into q2, swap(q1, q2).
 *    - push: O(N), pop: O(1).
 * 2. Single Queue (Queue Rotation Trick):
 *    - Push to queue, then rotate (pop front and push back) (size - 1) times.
 *    - Uses only 1 Queue!
 */

// ============================================================================
// 1. QUEUE USING TWO STACKS (Amortized O(1) Push and Pop)
// ============================================================================
class QueueUsingStacks {
private:
    stack<int> inStack;  // Buffers incoming elements
    stack<int> outStack; // Buffers outgoing elements in reverse (FIFO) order

    // Transfers elements from inStack to outStack only when outStack is empty
    void shiftStacks() {
        if (outStack.empty()) {
            while (!inStack.empty()) {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
    }

public:
    QueueUsingStacks() {}

    // Push element to the back: O(1)
    void push(int x) {
        inStack.push(x);
    }

    // Removes element from front and returns it: Amortized O(1)
    int pop() {
        if (empty()) {
            throw runtime_error("[QueueUsingStacks Error] Queue is empty!");
        }
        shiftStacks();
        int frontVal = outStack.top();
        outStack.pop();
        return frontVal;
    }

    // Get the front element: Amortized O(1)
    int peek() {
        if (empty()) {
            throw runtime_error("[QueueUsingStacks Error] Queue is empty!");
        }
        shiftStacks();
        return outStack.top();
    }

    // Returns whether the queue is empty: O(1)
    bool empty() const {
        return inStack.empty() && outStack.empty();
    }

    int size() const {
        return inStack.size() + outStack.size();
    }
};

// ============================================================================
// 2. STACK USING TWO QUEUES (Push Costly - O(N) push, O(1) pop)
// ============================================================================
class StackUsingTwoQueues {
private:
    queue<int> q1; // Always holds elements in LIFO order
    queue<int> q2; // Helper queue during push

public:
    // Push: O(N)
    void push(int x) {
        // 1. Enqueue new element into empty helper queue q2
        q2.push(x);

        // 2. Pour all existing elements from q1 into q2 behind x
        while (!q1.empty()) {
            q2.push(q1.front());
            q1.pop();
        }

        // 3. Swap q1 and q2 so q1 has x at front
        swap(q1, q2);
    }

    // Pop: O(1)
    int pop() {
        if (q1.empty()) {
            throw runtime_error("[StackUsingTwoQueues Error] Stack is empty!");
        }
        int topVal = q1.front();
        q1.pop();
        return topVal;
    }

    // Top: O(1)
    int top() const {
        if (q1.empty()) {
            throw runtime_error("[StackUsingTwoQueues Error] Stack is empty!");
        }
        return q1.front();
    }

    bool empty() const {
        return q1.empty();
    }

    int size() const {
        return q1.size();
    }
};

// ============================================================================
// 3. STACK USING A SINGLE QUEUE (Queue Rotation Trick)
// ============================================================================
class StackUsingSingleQueue {
private:
    queue<int> q;

public:
    // Push: O(N) - Rotates queue so new element ends up at the front
    void push(int x) {
        q.push(x);
        int sz = q.size();
        // Rotate (sz - 1) elements to the back of the queue
        for (int i = 0; i < sz - 1; i++) {
            q.push(q.front());
            q.pop();
        }
    }

    // Pop: O(1)
    int pop() {
        if (q.empty()) {
            throw runtime_error("[StackUsingSingleQueue Error] Stack is empty!");
        }
        int topVal = q.front();
        q.pop();
        return topVal;
    }

    // Top: O(1)
    int top() const {
        if (q.empty()) {
            throw runtime_error("[StackUsingSingleQueue Error] Stack is empty!");
        }
        return q.front();
    }

    bool empty() const {
        return q.empty();
    }

    int size() const {
        return q.size();
    }
};

// ============================================================================
// TEST DRIVER / INTERVIEW DEMONSTRATION
// ============================================================================
int main() {
    cout << "========================================\n";
    cout << "  1. TESTING QUEUE USING TWO STACKS\n";
    cout << "========================================\n";
    QueueUsingStacks myQueue;
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);
    cout << "Pushed 10, 20, 30\n";
    cout << "Front element (peek): " << myQueue.peek() << " (Expected: 10)\n";
    cout << "Popped: " << myQueue.pop() << " (Expected: 10)\n";
    cout << "New Front: " << myQueue.peek() << " (Expected: 20)\n";

    myQueue.push(40);
    cout << "Pushed 40\n";
    while (!myQueue.empty()) {
        cout << "Popped: " << myQueue.pop() << " ";
    }
    cout << "\n";

    cout << "\n========================================\n";
    cout << "  2. TESTING STACK USING TWO QUEUES\n";
    cout << "========================================\n";
    StackUsingTwoQueues s2q;
    s2q.push(1);
    s2q.push(2);
    s2q.push(3);
    cout << "Pushed 1, 2, 3\n";
    cout << "Top element: " << s2q.top() << " (Expected: 3)\n";
    cout << "Popped: " << s2q.pop() << " (Expected: 3)\n";
    cout << "New Top: " << s2q.top() << " (Expected: 2)\n";

    cout << "\n========================================\n";
    cout << "  3. TESTING STACK USING SINGLE QUEUE\n";
    cout << "========================================\n";
    StackUsingSingleQueue s1q;
    s1q.push(100);
    s1q.push(200);
    s1q.push(300);
    cout << "Pushed 100, 200, 300\n";
    cout << "Top element: " << s1q.top() << " (Expected: 300)\n";
    cout << "Popped: " << s1q.pop() << " (Expected: 300)\n";
    cout << "New Top: " << s1q.top() << " (Expected: 200)\n";
    s1q.push(400);
    cout << "Pushed 400 -> Top: " << s1q.top() << " (Expected: 400)\n";

    return 0;
}
