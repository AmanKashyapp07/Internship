#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// =========================================================
// 1. NEXT GREATER ELEMENT (LEETCODE 496 & GENERAL NGE)
// =========================================================

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

vi nextGreaterElementI(const vi &nums1, const vi &nums2) {
    unordered_map<int, int> ngeMap;
    stack<int> st;
    for (int num : nums2) {
        while (!st.empty() && st.top() < num) {
            ngeMap[st.top()] = num;
            st.pop();
        }
        st.push(num);
    }
    vi res;
    for (int num : nums1) {
        res.push_back(ngeMap.count(num) ? ngeMap[num] : -1);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find the first element strictly greater than nums[i] to its right for every index (LeetCode 496).
// - Approach: Monotonic Decreasing Stack storing unresolved elements/indices.
// - Intuition: Pop smaller elements from stack when an incoming larger element is encountered; popped elements get current value as their NGE.
// - Complexity: Time: O(N), Space: O(N) auxiliary space.

// =========================================================
// 2. NEXT GREATER ELEMENT II - CIRCULAR ARRAY (LEETCODE 503)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Find next greater element in a circular array where end connects back to start (LeetCode 503).
// - Approach: Virtual Double Iteration ($2N - 1$ steps) using Monotonic Decreasing Stack.
// - Intuition: Iterating twice modulo $N$ simulates circular wrap-around while only pushing indices during the first pass ($i < N$).
// - Complexity: Time: O(N) two linear passes, Space: O(N) stack space.

// =========================================================
// 3. NEXT SMALLER ELEMENT (NSE)
// =========================================================

vi nextSmallerElement(const vi &nums) {
    int n = nums.size();
    vi nse(n, n); // default sentinel boundary n
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
// Interview Explanation:
// - Problem Statement: Find the index of the first element strictly smaller than nums[i] to its right.
// - Approach: Monotonic Increasing Stack storing indices.
// - Intuition: Pop larger elements when an incoming smaller element is seen; popped elements get current index as their NSE.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 4. PREVIOUS GREATER (PGE) & PREVIOUS SMALLER (PSE)
// =========================================================

vi previousSmallerElement(const vi &nums) {
    int n = nums.size();
    vi pse(n, -1);
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] >= nums[i]) {
            stk.pop();
        }
        pse[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }
    return pse;
}

vi previousGreaterElement(const vi &nums) {
    int n = nums.size();
    vi pge(n, -1);
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && nums[stk.top()] <= nums[i]) {
            stk.pop();
        }
        pge[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }
    return pge;
}
// Interview Explanation:
// - Problem Statement: Find the closest preceding element strictly smaller (PSE) or strictly greater (PGE).
// - Approach: Left-to-right monotonic stack with immediate query assignment before push.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 5. DAILY TEMPERATURES (LEETCODE 739)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Find number of days until a warmer temperature occurs for each day (LeetCode 739).
// - Approach: Monotonic Decreasing Stack storing day indices.
// - Intuition: When temperature at day `i` is greater than stack top `j`, the waiting span is `i - j`.
// - Complexity: Time: O(N) single pass, Space: O(N) stack buffer.

// =========================================================
// 6. 132 PATTERN (LEETCODE 456)
// =========================================================

bool find132pattern(const vi &nums) {
    int n = nums.size(), s3 = INT_MIN;
    stack<int> stk;
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] < s3) return true; // Found s1 < s3 < s2
        while (!stk.empty() && stk.top() < nums[i]) {
            s3 = stk.top(); // s3 is the maximum valid '2' candidate
            stk.pop();
        }
        stk.push(nums[i]); // candidate for '3' (peak s2)
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if there exists a triplet (i < j < k) such that nums[i] < nums[k] < nums[j] (LeetCode 456).
// - Approach: Right-to-Left Monotonic Stack tracking maximum candidate for $s_3$.
// - Intuition: Iterate backwards; maintaining stack of $s_2$ candidates; when larger incoming $nums[i]$ pops elements, popped value becomes valid $s_3$. Any subsequent $nums[i] < s_3$ immediately confirms 132 pattern.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 7. LARGEST RECTANGLE IN HISTOGRAM (LEETCODE 84)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Find the area of the largest rectangle in a histogram (LeetCode 84).
// - Approach: Monotonic Increasing Stack with sentinel dummy height 0 at index $N$.
// - Intuition: For each popped bar of height $H$, its left bound is the new stack top and right bound is current index $i$; `width = i - top - 1`.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 8. MAXIMAL RECTANGLE IN 2D BINARY GRID (LEETCODE 85)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Find the largest rectangle containing only 1's in a 2D binary matrix (LeetCode 85).
// - Approach: Cumulative row-wise histogram heights + Largest Rectangle in Histogram (LeetCode 84).
// - Intuition: Treat each row as a histogram base; consecutive '1's accumulate height while '0's reset height to 0.
// - Complexity: Time: O(R * C), Space: O(C) for height buffer.

// =========================================================
// 9. SUM OF SUBARRAY MINIMUMS (LEETCODE 907)
// =========================================================

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
        while (!stk.empty() && arr[stk.top()] >= arr[i]) stk.pop(); // Strict vs non-strict prevents double counting
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
// Interview Explanation:
// - Problem Statement: Calculate the sum of minimums of all subarrays modulo 1e9+7 (LeetCode 907).
// - Approach: Contribution Technique using Previous Smaller Element (PSE) and Next Smaller Element (NSE).
// - Intuition: Element `arr[i]` is minimum in exactly `(i - pse[i]) * (nse[i] - i)` subarrays. Using strict inequality on one side avoids duplicate counting on identical values.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 10. SUM OF SUBARRAY RANGES (LEETCODE 2104)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Find sum of `(max - min)` across all subarrays (LeetCode 2104).
// - Approach: Linearity of Expectation / Contribution Method: $\sum (	ext{max} - 	ext{min}) = \sum 	ext{max} - \sum 	ext{min}$.
// - Intuition: Compute total contribution of each element as maximum minus total contribution as minimum via 4 monotonic stack passes.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 11. TRAPPING RAIN WATER (LEETCODE 42)
// =========================================================

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

int trapRainWaterStack(const vi &height) {
    int n = height.size(), water = 0;
    stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && height[i] > height[stk.top()]) {
            int top = stk.top(); stk.pop();
            if (stk.empty()) break;
            int dist = i - stk.top() - 1;
            int boundedHeight = min(height[i], height[stk.top()]) - height[top];
            water += dist * boundedHeight;
        }
        stk.push(i);
    }
    return water;
}
// Interview Explanation:
// - Problem Statement: Compute trapped water after raining given elevation map (LeetCode 42).
// - Approach: Two-Pointer optimal sweep ($O(1)$ space) & Monotonic Decreasing Stack bounded horizontal filling.
// - Intuition: Two-pointer tracks `min(maxLeft, maxRight)`; monotonic stack calculates horizontal bounded water slabs `(min(h[i], h[left]) - h[bottom]) * width`.
// - Complexity: Time: O(N), Space: O(1) two-pointer / O(N) stack.

// =========================================================
// 12. ASTEROID COLLISION (LEETCODE 735)
// =========================================================

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
// - Intuition: Collisions only occur when stack top moves RIGHT (`> 0`) and incoming asteroid moves LEFT (`< 0`).
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 13. SLIDING WINDOW MAXIMUM (LEETCODE 239)
// =========================================================

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

// =========================================================
// 14. ONLINE STOCK SPAN (LEETCODE 901)
// =========================================================

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

// =========================================================
// 15. VALID PARENTHESES & MIN ADD (LEETCODE 20 & 921)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Validate balanced bracket strings (LC 20) and find minimum parenthesis additions to balance (LC 921).
// - Approach: Stack matching and Counter balance tracking.
// - Complexity: Time: O(N), Space: O(N) stack / O(1) space for min add.

// =========================================================
// 16. REMOVE K DIGITS (LEETCODE 402)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Remove k digits from non-negative integer string to produce the smallest possible number (LeetCode 402).
// - Approach: Greedy Monotonic Increasing Stack on string digits.
// - Intuition: A smaller leading digit always minimizes number magnitude; pop previous larger digits while `k > 0`; trim leading zeroes.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 17. REMOVE DUPLICATE LETTERS (LEETCODE 316 / 1081)
// =========================================================

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
// Interview Explanation:
// - Problem Statement: Remove duplicate letters such that every letter appears once and result is smallest in lexicographical order (LeetCode 316).
// - Approach: Monotonic Increasing Stack with Last Occurrence Tracking & Visited Set.
// - Intuition: Pop larger character only if it appears again later in string (`lastIndex > i`); skip characters already in stack.
// - Complexity: Time: O(N), Space: O(1) (26 characters).

// =========================================================
// 18. EVALUATE REVERSE POLISH NOTATION (LEETCODE 150)
// =========================================================

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

// =========================================================
// 19. BASIC CALCULATOR II (LEETCODE 227)
// =========================================================

int calculateBasicCalculatorII(string s) {
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

// =========================================================
// 20. DECODE STRING (LEETCODE 394)
// =========================================================

string decodeString(const string &s) {
    stack<int> countStack;
    stack<string> stringStack;
    string currentString = "";
    int k = 0;

    for (char c : s) {
        if (isdigit(c)) {
            k = k * 10 + (c - '0');
        } else if (c == '[') {
            countStack.push(k);
            stringStack.push(currentString);
            currentString = "";
            k = 0;
        } else if (c == ']') {
            string decodedString = stringStack.top(); stringStack.pop();
            int repeatCount = countStack.top(); countStack.pop();
            while (repeatCount--) decodedString += currentString;
            currentString = decodedString;
        } else {
            currentString += c;
        }
    }
    return currentString;
}
// Interview Explanation:
// - Problem Statement: Decode an encoded string formatted as `k[encoded_string]` (LeetCode 394).
// - Approach: Dual Stack (Count Stack & String Context Stack) for nested bracket parsing.
// - Intuition: '[' pushes current multiplier `k` and parent string prefix; ']' pops multiplier and appends repeated inner string to parent context.
// - Complexity: Time: O(	ext{maxK} \cdot N), Space: O(N) stack space.

// =========================================================
// 21. MIN STACK (LEETCODE 155 - AUXILIARY & O(1) SPACE DESIGNS)
// =========================================================

class MinStackTwoStacks {
    stack<int> valStack;
    stack<int> minStack;
public:
    MinStackTwoStacks() {}
    void push(int val) {
        valStack.push(val);
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }
    void pop() {
        if (valStack.top() == minStack.top()) {
            minStack.pop();
        }
        valStack.pop();
    }
    int top() { return valStack.top(); }
    int getMin() { return minStack.top(); }
};

class MinStackO1ExtraSpace {
    stack<ll> st;
    ll minVal;
public:
    MinStackO1ExtraSpace() : minVal(LLONG_MAX) {}

    void push(int val) {
        if (st.empty()) {
            minVal = val;
            st.push(val);
        } else if (val >= minVal) {
            st.push(val);
        } else {
            st.push(2LL * val - minVal); // Mathematical encoding
            minVal = val;
        }
    }
    void pop() {
        if (st.empty()) return;
        ll topVal = st.top(); st.pop();
        if (topVal < minVal) {
            minVal = 2 * minVal - topVal; // Decode previous minimum
        }
    }
    int top() {
        ll topVal = st.top();
        if (topVal < minVal) return (int)minVal;
        return (int)topVal;
    }
    int getMin() { return (int)minVal; }
};
// Interview Explanation:
// - Problem Statement: Design a stack supporting push, pop, top, and retrieving the minimum element in O(1) time.
// - Approach: Auxiliary tracking stack or mathematical encoding $2v - 	ext{minVal}$ for true $O(1)$ extra space.
// - Complexity: Time: O(1) all operations, Space: O(N) / O(1) extra space.


/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: MONOTONIC STACK & QUEUE
 ====================================================================================================

 1. MONOTONIC STACK DECISION MATRIX:
    -------------------------------------------------------------------------------------------------
    Query Type                      | Stack Order   | Traversal Direction  | Pop Condition
    -------------------------------------------------------------------------------------------------
    Next Greater Element (NGE)      | Decreasing    | Left -> Right        | incoming > stk.top()
    Next Smaller Element (NSE)      | Increasing    | Left -> Right        | incoming < stk.top()
    Previous Greater Element (PGE)  | Decreasing    | Left -> Right        | incoming >= stk.top()
    Previous Smaller Element (PSE)  | Increasing    | Left -> Right        | incoming <= stk.top()
    Sliding Window Max (LC 239)     | Decreasing Deque| Left -> Right      | pop_back if incoming >= back
    Histogram / Matrix Area (LC 84) | Increasing    | Left -> Right        | incoming <= stk.top() + sentinel 0
    -------------------------------------------------------------------------------------------------

 2. EXPRESSION & SIMULATION STACK RULES:
    * Postfix (RPN): Push numbers; operator pops `b` then `a`, computes `a op b`, pushes result.
    * Precedence Parsing (Basic Calculator): High precedence operators ('*', '/') evaluated immediately; low precedence ('+', '-') stored as numbers on stack to sum at end.
    * Collisions (Asteroids): Only collide when stack top moves RIGHT (`> 0`) and incoming moves LEFT (`< 0`).
    * Nested Brackets (Decode String): Stack stores multiplier `k` and outer string context upon '[', merges on ']'.
 ====================================================================================================
*/
