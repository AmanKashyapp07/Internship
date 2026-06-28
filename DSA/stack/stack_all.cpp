/**
 * Stack - Non-Monotonic Applications & General Variants
 *
 * A single comprehensive reference covering standard stack patterns
 * (excluding monotonic stack variants) asked in top internship OAs and interviews.
 *
 * Sections:
 * 1. Valid Parentheses / Bracket Matching      - LC 20
 * 2. Min Stack                                 - LC 155
 * 3. Decode String                             - LC 394
 * 4. Evaluate Reverse Polish Notation (RPN)    - LC 150
 * 5. Basic Calculator II (infix evaluation)    - LC 227
 * 6. Asteroid Collision                        - LC 735
 * 7. Simplify Path                             - LC 71
 * 8. Score of Parentheses                      - LC 856
 * 9. Iterative Binary Tree Traversals          - Inorder, Preorder, Postorder
 */

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;
using vi = vector<int>;


// =========================================================================
// 2. MIN STACK
// =========================================================================
// A stack that supports push, pop, top, and retrieving the minimum element in O(1) time.
// Time Complexity: O(1) for all operations | Space Complexity: O(N)
class MinStack {
private:
    stack<int> stk;
    stack<int> minStk; // Parallel stack to keep track of the minimums

public:
    void push(int val) {
        stk.push(val);
        int curMin = minStk.empty() ? val : min(val, minStk.top());
        minStk.push(curMin);
    }

    void pop() {
        stk.pop();
        minStk.pop();
    }

    int top() {
        return stk.top();
    }

    int getMin() {
        return minStk.top();
    }
};




// =========================================================================
// 6. ASTEROID COLLISION
// =========================================================================
// Simulate collisions of asteroids moving right (+) and left (-).
// Time Complexity: O(N) | Space Complexity: O(N)
vi asteroidCollision(const vi& asteroids) {
    vi st;

    for (int a : asteroids) {

        // Keep destroying smaller right-moving asteroids.
        // Collision is possible only when:
        // - stack top is moving right (st.back() > 0)
        // - current asteroid is moving left (a < 0)
        // - current asteroid is larger (st.back() < -a)
        while (!st.empty() && st.back() > 0 && a < 0 && st.back() < -a)
            st.pop_back();

        // No collision is possible if:
        // 1. stack is empty
        // 2. top is moving left
        // 3. current asteroid is moving right
        if (st.empty() || st.back() < 0 || a > 0)
            st.push_back(a);

        // Same size -> both explode.
        else if (st.back() == -a)
            st.pop_back();

        // Otherwise:
        // st.back() > -a, so the current asteroid explodes.
        // Do nothing.
    }

    return st;
}


// =========================================================================
// 8. SCORE OF PARENTHESES
// =========================================================================
// Calculate score: () is 1, (A) is 2 * A, and AB is A + B.
// Time Complexity: O(N) | Space Complexity: O(N)
int scoreOfParentheses(const string& s) {
    stack<int> stk;
    stk.push(0); // Base frame score

    for (char c : s) {
        if (c == '(') {
            stk.push(0); // Open nested context
        } else {
            int v = stk.top(); stk.pop(); // popping out the inner score
            int score = (v == 0) ? 1 : 2 * v; // if inner score is 0, it's a simple '()', else double the inner score
            stk.top() += score; // Add to the score of the outer context
        }
    }
    return stk.top();
}
