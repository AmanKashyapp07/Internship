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
// 3. DECODE STRING
// =========================================================================
// Decode strings of the form "k[encoded_string]" (e.g. 3[a2[c]] -> accaccacc).
// Time Complexity: O(N * max_k) | Space Complexity: O(N)
string decodeString(const string& s) {
    stack<int> countStk;
    stack<string> strStk;
    string current = "";
    int k = 0;

    for (char c : s) {
        if (isdigit(c)) {
            k = k * 10 + (c - '0');
        } else if (c == '[') {
            countStk.push(k);
            strStk.push(current);
            current = "";
            k = 0;
        } else if (c == ']') {
            int repeat = countStk.top(); countStk.pop();
            string prev = strStk.top(); strStk.pop();
            string repeated = "";
            for (int i = 0; i < repeat; i++) repeated += current;
            current = prev + repeated;
        } else {
            current += c;
        }
    }
    return current;
}



// =========================================================================
// 6. ASTEROID COLLISION
// =========================================================================
// Simulate collisions of asteroids moving right (+) and left (-).
// Time Complexity: O(N) | Space Complexity: O(N)
vi asteroidCollision(const vi& asteroids) {
    stack<int> stk;

    for (int a : asteroids) {
        bool destroyed = false;
        while (!stk.empty() && a < 0 && stk.top() > 0) {
            if (stk.top() < -a) {
                stk.pop(); // The right-moving asteroid on the stack explodes
            } else if (stk.top() == -a) {
                stk.pop(); // Both asteroids explode
                destroyed = true;
                break;
            } else {
                destroyed = true; // The incoming asteroid explodes
                break;
            }
        }
        if (!destroyed) stk.push(a);
    }

    vi result(stk.size());
    for (int i = result.size() - 1; i >= 0; i--) {
        result[i] = stk.top();
        stk.pop();
    }
    return result;
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
            int v = stk.top(); stk.pop();
            int score = (v == 0) ? 1 : 2 * v;
            stk.top() += score;
        }
    }
    return stk.top();
}
