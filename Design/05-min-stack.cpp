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
        if (minStack.empty() || val <= minStack.top()) minStack.push(val);
    }

    // Pop: O(1)
    void pop() {
        if (mainStack.empty()) throw runtime_error("[MinStack Error] Stack is empty!");
        if (mainStack.top() == minStack.top()) minStack.pop();
        mainStack.pop();
    }

    // Top: O(1)
    int top() const {
        if (mainStack.empty()) throw runtime_error("[MinStack Error] Stack is empty!");
        return mainStack.top();
    }

    // GetMin: O(1)
    int getMin() const {
        if (minStack.empty()) throw runtime_error("[MinStack Error] Stack is empty!");
        return minStack.top();
    }

    bool empty() const { return mainStack.empty(); }
    int size() const { return mainStack.size(); }
};

// ============================================================================
// BONUS: MIN-MAX STACK (Simultaneous O(1) Min and Max)
// ============================================================================
class MinMaxStack {
private:
    struct Element { int val, minSoFar, maxSoFar; };
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
