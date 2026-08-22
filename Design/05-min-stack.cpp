#include <iostream>
#include <stack>
#include <stdexcept>
#include <utility>
using namespace std;

/**
 * ============================================================================
 * DESIGN 05: MIN STACK (O(1) Minimum Retrieval)
 * ============================================================================
 * 
 * Design a stack that supports push, pop, top, and retrieving the minimum
 * element in constant O(1) time.
 * 
 * Frequently Asked in Interviews:
 * 1. Approach 1: Auxiliary Min-Stack / Pair Stack:
 *    - Store {value, current_min} or maintain a separate minStack.
 *    - Time: O(1) all operations, Space: O(N) extra space.
 * 2. Approach 2: Mathematical Encoding Trick (O(1) Extra Space):
 *    - Push (2 * val - minVal) whenever a new minimum is encountered.
 *    - Pop restores old minimum: minVal = 2 * minVal - top.
 *    - CAUTION: Must use 64-bit int (long long) to prevent integer overflow!
 * 3. Follow-up: Max Stack & Min-Max Stack (O(1) for both Min and Max).
 * 
 * Time Complexities:
 * - push(val): O(1)
 * - pop()    : O(1)
 * - top()    : O(1)
 * - getMin() : O(1)
 */

// ============================================================================
// APPROACH 1: TWO STACKS / PAIR STACK (Standard & Safe)
// ============================================================================
class MinStackTwoStacks {
private:
    stack<int> mainStack;
    stack<int> minStack; // Tracks the minimum element at each stack height

public:
    MinStackTwoStacks() {}

    // Push: O(1)
    void push(int val) {
        mainStack.push(val);
        // Push to minStack if empty or if val <= current minimum
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }

    // Pop: O(1)
    void pop() {
        if (mainStack.empty()) {
            throw runtime_error("[MinStack Error] Stack is empty!");
        }
        // If popped element was the minimum, pop from minStack as well
        if (mainStack.top() == minStack.top()) {
            minStack.pop();
        }
        mainStack.pop();
    }

    // Top: O(1)
    int top() const {
        if (mainStack.empty()) {
            throw runtime_error("[MinStack Error] Stack is empty!");
        }
        return mainStack.top();
    }

    // GetMin: O(1)
    int getMin() const {
        if (minStack.empty()) {
            throw runtime_error("[MinStack Error] Stack is empty!");
        }
        return minStack.top();
    }

    bool empty() const {
        return mainStack.empty();
    }

    int size() const {
        return mainStack.size();
    }
};

// ============================================================================
// APPROACH 2: MATHEMATICAL ENCODING (O(1) Auxiliary Space)
// ============================================================================
class MinStackOptimalSpace {
private:
    stack<long long> st;
    long long minVal;

public:
    MinStackOptimalSpace() : minVal(0) {}

    // Push: O(1)
    void push(int value) {
        long long val = value;
        if (st.empty()) {
            minVal = val;
            st.push(val);
        } else if (val >= minVal) {
            // Normal push when val is not a new minimum
            st.push(val);
        } else {
            // New minimum encountered! Encode modified value: 2 * val - minVal
            // Since val < minVal, (2 * val - minVal) < val.
            st.push(2 * val - minVal);
            minVal = val; // Update current minimum
        }
    }

    // Pop: O(1)
    void pop() {
        if (st.empty()) {
            throw runtime_error("[MinStack Error] Stack Underflow!");
        }
        long long topVal = st.top();
        st.pop();

        // If topVal < minVal, it was an encoded value! Restore previous minimum:
        if (topVal < minVal) {
            minVal = 2 * minVal - topVal;
        }
    }

    // Top: O(1)
    int top() const {
        if (st.empty()) {
            throw runtime_error("[MinStack Error] Stack is empty!");
        }
        long long topVal = st.top();
        if (topVal < minVal) {
            // Encoded value: the actual pushed value was minVal
            return (int)minVal;
        }
        return (int)topVal;
    }

    // GetMin: O(1)
    int getMin() const {
        if (st.empty()) {
            throw runtime_error("[MinStack Error] Stack is empty!");
        }
        return (int)minVal;
    }

    bool empty() const {
        return st.empty();
    }
};

// ============================================================================
// BONUS: MIN-MAX STACK (Simultaneous O(1) Min and Max)
// ============================================================================
class MinMaxStack {
private:
    struct Element {
        int val;
        int minSoFar;
        int maxSoFar;
    };
    stack<Element> st;

public:
    void push(int val) {
        int curMin = st.empty() ? val : min(val, st.top().minSoFar);
        int curMax = st.empty() ? val : max(val, st.top().maxSoFar);
        st.push({val, curMin, curMax});
    }

    void pop() {
        if (st.empty()) throw runtime_error("[MinMaxStack Error] Stack is empty!");
        st.pop();
    }

    int top() const {
        if (st.empty()) throw runtime_error("[MinMaxStack Error] Stack is empty!");
        return st.top().val;
    }

    int getMin() const {
        if (st.empty()) throw runtime_error("[MinMaxStack Error] Stack is empty!");
        return st.top().minSoFar;
    }

    int getMax() const {
        if (st.empty()) throw runtime_error("[MinMaxStack Error] Stack is empty!");
        return st.top().maxSoFar;
    }
};

// ============================================================================
// TEST DRIVER / INTERVIEW DEMONSTRATION
// ============================================================================
int main() {
    cout << "========================================\n";
    cout << "  1. TESTING MIN STACK (TWO STACKS)\n";
    cout << "========================================\n";
    MinStackTwoStacks s1;
    s1.push(-2);
    s1.push(0);
    s1.push(-3);
    cout << "Pushed [-2, 0, -3]\n";
    cout << "Current Min: " << s1.getMin() << " (Expected: -3)\n";
    cout << "Top element: " << s1.top() << " (Expected: -3)\n";

    s1.pop();
    cout << "Popped -3\n";
    cout << "Current Top: " << s1.top() << " (Expected: 0)\n";
    cout << "Current Min: " << s1.getMin() << " (Expected: -2)\n";

    cout << "\n========================================\n";
    cout << "  2. TESTING OPTIMAL O(1) SPACE MIN STACK\n";
    cout << "========================================\n";
    MinStackOptimalSpace s2;
    // Sequence: 5, 3, 7, 2, 2, 8
    int values[] = {5, 3, 7, 2, 2, 8};
    for (int v : values) {
        s2.push(v);
        cout << "Pushed " << v << " -> Current Top: " << s2.top() << " | Current Min: " << s2.getMin() << "\n";
    }

    cout << "\nPopping elements one by one:\n";
    while (!s2.empty()) {
        cout << "Popping Top: " << s2.top() << " | Min before pop: " << s2.getMin();
        s2.pop();
        if (!s2.empty()) {
            cout << " -> New Min: " << s2.getMin() << "\n";
        } else {
            cout << " -> Stack is now empty\n";
        }
    }

    cout << "\n========================================\n";
    cout << "  3. TESTING MIN-MAX STACK\n";
    cout << "========================================\n";
    MinMaxStack s3;
    s3.push(10);
    s3.push(5);
    s3.push(30);
    s3.push(1);
    cout << "Pushed [10, 5, 30, 1]\n";
    cout << "Top: " << s3.top() << " | Min: " << s3.getMin() << " | Max: " << s3.getMax() << "\n";
    s3.pop();
    cout << "After popping 1 -> Top: " << s3.top() << " | Min: " << s3.getMin() << " | Max: " << s3.getMax() << "\n";

    return 0;
}
