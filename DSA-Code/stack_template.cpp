/**
 * ====================================================================================================
 *                 ULTIMATE MONOTONIC STACK & QUEUE MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Next/Previous Greater/Smaller Elements, Circular Arrays,
 * Histogram & Matrix Rectangles, Trapping Rain Water, Subarray Extrema Contributions,
 * Lexicographical Optimizations (Remove K Digits, Remove Duplicate Letters), 132 Pattern,
 * Sliding Window Maximum Deque, Expression Parsing (RPN & Calculator), Asteroid Collision,
 * and Dynamic Stock Spanner Class.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;

const ll MOD = 1e9 + 7;

// ====================================================================================================
// SECTION 1: MONOTONIC STACK & QUEUE CORE PRIMITIVES
// ====================================================================================================

// 1. Next Greater Element (LeetCode 496 / General NGE)
vi nextGreaterElement(const vi &nums) {
    int n = nums.size();
    vi nge(n, -1);
    stack<int> stk; // Monotonic decreasing stack storing indices
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            nge[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    return nge;
}
// Interview Explanation:
// - Problem Statement: Find the first element strictly greater than nums[i] to its right for every index.
// - Approach: Monotonic Decreasing Stack storing indices.
// - Intuition: Pop smaller elements from stack when a larger element incoming is encountered; popped elements get current value as NGE.
// - Complexity: Time: O(N), Space: O(N).


// 2. Next Smaller Element (NSE)
vi nextSmallerElement(const vi &nums) {
    int n = nums.size();
    vi nse(n, n);
    stack<int> stk; // Monotonic increasing stack storing indices
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] > nums[i]) {
            nse[stk.top()] = i;
            stk.pop();
        }
        stk.push(i);
    }
    return nse;
}

// 3. Sliding Window Maximum (LeetCode 239 - Monotonic Deque)
vi maxSlidingWindow(const vi &nums, int k) {
    int n = nums.size();
    deque<int> dq; // Monotonic Decreasing Deque storing indices
    vi ans;
    for (int i = 0; i < n; i++) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front(); // Remove elements out of window k
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back(); // Maintain decreasing order
        dq.push_back(i);
        if (i >= k - 1) ans.push_back(nums[dq.front()]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum element in each sliding window of size k moving left to right (LeetCode 239).
// - Approach: Monotonic Decreasing Deque storing indices.
// - Intuition: Deque front always stores the maximum element index for the active window; smaller elements behind are popped as they can never be the max.
// - Complexity: Time: O(N) amortized (each element pushed/popped at most once), Space: O(K).


// 4. Online Stock Span Dynamic Class (LeetCode 901)
class StockSpanner {
    stack<pii> stk; // {price, span}
public:
    StockSpanner() {}
    int next(int price) {
        int span = 1;
        while (!stk.empty() && stk.top().first <= price) {
            span += stk.top().second;
            stk.pop();
        }
        stk.push({price, span});
        return span;
    }
};
// Interview Explanation:
// - Problem Statement: Compute daily stock price span dynamically on stream queries (LeetCode 901).
// - Approach: Monotonic Decreasing Stack accumulating spans of popped prices.
// - Intuition: Popping equal or smaller prices aggregates their combined spans into the current price state in O(1) amortized time.
// - Complexity: Time: O(1) amortized per `next()` call, Space: O(N).


// 5. Next Greater Element II - Circular Array (LeetCode 503)
vi nextGreaterElementsCircular(const vi &nums) {
    int n = nums.size();
    vi nge(n, -1);
    stack<int> stk;
    for (int i = 0; i < 2 * n; i++) {
        int idx = i % n;
        while (!stk.empty() && nums[stk.top()] < nums[idx]) {
            nge[stk.top()] = nums[idx];
            stk.pop();
        }
        if (i < n) stk.push(idx);
    }
    return nge;
}


// ====================================================================================================
// SECTION 2: AREA & RANGE EXTREMA (HISTOGRAM & MATRIX)
// ====================================================================================================

// 1. Largest Rectangle in Histogram (LeetCode 84)
int largestRectangleArea(const vi &heights) {
    int n = heights.size(), maxArea = 0;
    stack<int> stk;
    for (int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        while (!stk.empty() && heights[stk.top()] >= h) {
            int height = heights[stk.top()];
            stk.pop();
            int width = stk.empty() ? i : (i - stk.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        stk.push(i);
    }
    return maxArea;
}

// 2. Maximal Rectangle in 2D Binary Grid (LeetCode 85)
int maximalRectangle(const vector<vector<char>> &matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    int n = matrix.size(), m = matrix[0].size(), maxArea = 0;
    vi heights(m, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0;
        }
        maxArea = max(maxArea, largestRectangleArea(heights));
    }
    return maxArea;
}

// 3. Sum of Subarray Minimums (LeetCode 907)
int sumSubarrayMins(const vi &arr) {
    int n = arr.size();
    vi pse(n), nse(n);
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && arr[stk.top()] > arr[i]) stk.pop();
        pse[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }
    while (!stk.empty()) stk.pop();
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && arr[stk.top()] >= arr[i]) stk.pop();
        nse[i] = stk.empty() ? n : stk.top();
        stk.push(i);
    }
    ll totalSum = 0;
    for (int i = 0; i < n; i++) {
        ll leftSpan = i - pse[i];
        ll rightSpan = nse[i] - i;
        totalSum = (totalSum + arr[i] * leftSpan % MOD * rightSpan % MOD) % MOD;
    }
    return totalSum;
}

// 4. Sum of Subarray Ranges (LeetCode 2104)
ll subArrayRanges(const vi &nums) {
    int n = nums.size();
    ll totalRanges = 0;
    vi pse(n), nse(n), pge(n), nge(n);
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] > nums[i]) stk.pop();
        pse[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }
    while (!stk.empty()) stk.pop();
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && nums[stk.top()] >= nums[i]) stk.pop();
        nse[i] = stk.empty() ? n : stk.top();
        stk.push(i);
    }
    while (!stk.empty()) stk.pop();
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] < nums[i]) stk.pop();
        pge[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }
    while (!stk.empty()) stk.pop();
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && nums[stk.top()] <= nums[i]) stk.pop();
        nge[i] = stk.empty() ? n : stk.top();
        stk.push(i);
    }
    for (int i = 0; i < n; i++) {
        ll maxCount = 1LL * (i - pge[i]) * (nge[i] - i);
        ll minCount = 1LL * (i - pse[i]) * (nse[i] - i);
        totalRanges += nums[i] * (maxCount - minCount);
    }
    return totalRanges;
}


// ====================================================================================================
// SECTION 3: WATER TRAPPING, SIMULATION & EXPRESSION PARSING
// ====================================================================================================

// 1. Trapping Rain Water (LeetCode 42)
int trapRainWaterTwoPointer(const vi &height) {
    int left = 0, right = (int)height.size() - 1;
    int maxLeft = 0, maxRight = 0, water = 0;
    while (left < right) {
        if (height[left] <= height[right]) {
            if (height[left] >= maxLeft) maxLeft = height[left];
            else water += maxLeft - height[left];
            left++;
        } else {
            if (height[right] >= maxRight) maxRight = height[right];
            else water += maxRight - height[right];
            right--;
        }
    }
    return water;
}

// 2. Asteroid Collision (LeetCode 735)
vi asteroidCollision(const vi &asteroids) {
    vi stk;
    for (int ast : asteroids) {
        bool destroyed = false;
        while (!stk.empty() && stk.back() > 0 && ast < 0) {
            if (stk.back() < -ast) {
                stk.pop_back(); // Right-moving asteroid explodes
                continue;
            } else if (stk.back() == -ast) {
                stk.pop_back(); // Both explode
            }
            destroyed = true;
            break;
        }
        if (!destroyed) stk.push_back(ast);
    }
    return stk;
}
// Interview Explanation:
// - Problem Statement: Simulate collisions of asteroids moving right (+) and left (-) with speed equal to size (LeetCode 735).
// - Approach: Monotonic Stack Collision Simulation.
// - Intuition: Collisions only occur when stack top is moving right (`> 0`) and incoming asteroid is moving left (`< 0`).
// - Complexity: Time: O(N), Space: O(N).

// 3. Evaluate Reverse Polish Notation (LeetCode 150)
int evalRPN(vector<string> &tokens) {
    stack<int> stk;
    for (const string &t : tokens) {
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            int b = stk.top(); stk.pop();
            int a = stk.top(); stk.pop();
            if (t == "+") stk.push(a + b);
            else if (t == "-") stk.push(a - b);
            else if (t == "*") stk.push(a * b);
            else if (t == "/") stk.push(a / b);
        } else {
            stk.push(stoi(t));
        }
    }
    return stk.top();
}
// Interview Explanation:
// - Problem Statement: Evaluate arithmetic expression in Reverse Polish Notation (Postfix) (LeetCode 150).
// - Approach: Stack Evaluation.
// - Intuition: Operands are pushed onto stack; operators pop top two operands `b` and `a`, evaluating `a op b` and pushing result back.
// - Complexity: Time: O(N), Space: O(N).

// 4. Basic Calculator II (LeetCode 227 - Operator Precedence Stack)
int calculate(string s) {
    int n = s.size(), currentNumber = 0;
    char operation = '+';
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        char currentChar = s[i];
        if (isdigit(currentChar)) {
            currentNumber = (currentNumber * 10) + (currentChar - '0');
        }
        if ((!isdigit(currentChar) && !isspace(currentChar)) || i == n - 1) {
            if (operation == '-') stk.push(-currentNumber);
            else if (operation == '+') stk.push(currentNumber);
            else if (operation == '*') {
                int top = stk.top(); stk.pop();
                stk.push(top * currentNumber);
            } else if (operation == '/') {
                int top = stk.top(); stk.pop();
                stk.push(top / currentNumber);
            }
            operation = currentChar;
            currentNumber = 0;
        }
    }
    int result = 0;
    while (!stk.empty()) { result += stk.top(); stk.pop(); }
    return result;
}
// Interview Explanation:
// - Problem Statement: Evaluate basic string expression containing '+', '-', '*', '/' and non-negative integers (LeetCode 227).
// - Approach: Operator Precedence Stack handling high precedence ('*', '/') immediately.
// - Intuition: Low precedence terms ('+', '-') are stored on stack (as positive/negative values) to be summed at the end.
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// SECTION 4: LEXICOGRAPHICAL STACK & MATCHING OPTIMIZATIONS
// ====================================================================================================

// 1. Valid Parentheses (LeetCode 20)
bool isValidParentheses(string s) {
    stack<char> stk;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') stk.push(c);
        else {
            if (stk.empty()) return false;
            char top = stk.top(); stk.pop();
            if ((c == ')' && top != '(') || (c == '}' && top != '{') || (c == ']' && top != '['))
                return false;
        }
    }
    return stk.empty();
}

// 2. Minimum Add to Make Parentheses Valid (LeetCode 921)
int minAddToMakeValid(string s) {
    int openNeeded = 0, closeNeeded = 0;
    for (char c : s) {
        if (c == '(') {
            closeNeeded++;
        } else {
            if (closeNeeded > 0) closeNeeded--;
            else openNeeded++;
        }
    }
    return openNeeded + closeNeeded;
}

// 3. Remove K Digits (LeetCode 402)
string removeKdigits(string num, int k) {
    string stk;
    for (char c : num) {
        while (!stk.empty() && k > 0 && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    while (k > 0 && !stk.empty()) {
        stk.pop_back();
        k--;
    }
    int start = 0;
    while (start < (int)stk.size() && stk[start] == '0') start++;
    string ans = stk.substr(start);
    return ans.empty() ? "0" : ans;
}

// 4. Remove Duplicate Letters (LeetCode 316)
string removeDuplicateLetters(string s) {
    vector<int> lastIndex(26, -1);
    vector<bool> inStack(26, false);
    for (int i = 0; i < (int)s.size(); i++) lastIndex[s[i] - 'a'] = i;

    string stk;
    for (int i = 0; i < (int)s.size(); i++) {
        int c = s[i] - 'a';
        if (inStack[c]) continue;
        while (!stk.empty() && stk.back() > s[i] && lastIndex[stk.back() - 'a'] > i) {
            inStack[stk.back() - 'a'] = false;
            stk.pop_back();
        }
        stk.push_back(s[i]);
        inStack[c] = true;
    }
    return stk;
}

// 5. Daily Temperatures (LeetCode 739)
vi dailyTemperatures(const vi &temperatures) {
    int n = temperatures.size();
    vi ans(n, 0);
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && temperatures[stk.top()] < temperatures[i]) {
            int j = stk.top(); stk.pop();
            ans[j] = i - j;
        }
        stk.push(i);
    }
    return ans;
}

// 6. 132 Pattern (LeetCode 456)
bool find132pattern(const vi &nums) {
    int n = nums.size(), s3 = INT_MIN;
    stack<int> stk;
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] < s3) return true;
        while (!stk.empty() && stk.top() < nums[i]) {
            s3 = stk.top();
            stk.pop();
        }
        stk.push(nums[i]);
    }
    return false;
}

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: MONOTONIC STACK & QUEUE
 ====================================================================================================

 1. MONOTONIC STACK DECISION MATRIX:
    -------------------------------------------------------------------------------------------------
    Query Type                      | Stack Order   | Traversal Direction  | Pop Condition
    -------------------------------------------------------------------------------------------------
    Next Greater Element (NGE)      | Decreasing    | Left → Right         | incoming > stk.top()
    Next Smaller Element (NSE)      | Increasing    | Left → Right         | incoming < stk.top()
    Previous Greater Element (PGE)  | Decreasing    | Left → Right         | incoming >= stk.top()
    Previous Smaller Element (PSE)  | Increasing    | Left → Right         | incoming <= stk.top()
    Sliding Window Max (LeetCode 239)| Decreasing Deque| Left → Right       | pop_back if incoming >= back
    Histogram / Matrix Area         | Increasing    | Left → Right         | incoming <= stk.top() + sentinel
    -------------------------------------------------------------------------------------------------

 2. EXPRESSION & SIMULATION STACK RULES:
    • Postfix (RPN): Push numbers; operator pops `b` then `a`, computes `a op b`, pushes result.
    • Precedence Parsing (Basic Calculator): High precedence operators ('*', '/') evaluated immediately; low precedence ('+', '-') stored as numbers on stack to sum at end.
    • Collisions (Asteroids): Only collide when stack top moves RIGHT (`> 0`) and incoming moves LEFT (`< 0`).
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
