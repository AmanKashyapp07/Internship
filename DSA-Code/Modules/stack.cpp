#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Next Greater Element (LC 496)               | Monotonic Decreasing Stack        | O(N)     | O(N)     |
 | 2  | Next Greater Element II - Circular (LC 503) | Monotonic Stack on Doubled (2N)   | O(N)     | O(N)     |
 | 3  | Next Smaller Element (NSE)                  | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 4  | Previous Greater & Smaller (PGE / PSE)      | Monotonic Stack Traversal         | O(N)     | O(N)     |
 | 5  | Daily Temperatures (LC 739)                 | Monotonic Decreasing Index Stack  | O(N)     | O(N)     |
 | 6  | 132 Pattern (LC 456)                        | Monotonic Stack + Max S3 Tracker  | O(N)     | O(N)     |
 | 7  | Largest Rectangle in Histogram (LC 84)      | Monotonic Stack Boundary Indices  | O(N)     | O(N)     |
 | 8  | Maximal Rectangle in Binary Grid (LC 85)    | 1D Histogram DP + Monotonic Stack | O(M * N) | O(N)     |
 | 9  | Sum of Subarray Minimums (LC 907)           | Contribution Principle (PSE x NSE)| O(N)     | O(N)     |
 | 10 | Sum of Subarray Ranges (LC 2104)            | Contribution: Subarray(Max - Min) | O(N)     | O(N)     |
 | 11 | Trapping Rain Water (LC 42)                 | Inward Two Pointers / Mono Stack  | O(N)     | O(1)     |
 | 12 | Asteroid Collision (LC 735)                 | Directional Collision Stack       | O(N)     | O(N)     |
 | 13 | Sliding Window Maximum (LC 239)             | Monotonic Decreasing Deque        | O(N)     | O(K)     |
 | 14 | Online Stock Span (LC 901)                  | Monotonic Stack of (Price, Span)  | O(1) avg | O(N)     |
 | 15 | Valid Parentheses & Min Add (LC 20 & 921)   | Character Matching Stack / Counter| O(N)     | O(N)/O(1)|
 | 16 | Remove K Digits (LC 402)                    | Monotonic Increasing String Stack | O(N)     | O(N)     |
 | 17 | Remove Duplicate Letters (LC 316 / 1081)    | Monotonic Stack + Seen Set + Freq | O(N)     | O(1)     |
 | 18 | Reverse Polish Notation (LC 150)            | Operand Stack Evaluation          | O(N)     | O(N)     |
 | 19 | Basic Calculator II (LC 227)                | Precedence Stack + Accumulator    | O(N)     | O(N)     |
 | 20 | Decode String (LC 394)                      | Count Stack + String Context Stack| O(N)     | O(N)     |
 | 21 | Min Stack (LC 155)                          | 2*val - minVal Math / Aux Stack   | O(1) all | O(1)/O(N)|
 | 22 | Array Stack & Queue Implementation          | Fixed Buffer + Pointer Tracking   | O(1) all | O(Cap)   |
 | 23 | Stack via Queue & Queue via Stack (LC 225)  | 2 Stacks / 1 Queue Cost Inversion | O(1)/O(N)| O(N)     |
 | 24 | Recursive Stack Sorting                     | Two-Level Recursive Insertion Sort| O(N^2)   | O(N)     |
 | 25 | LFU Cache (LC 460)                          | Hash Map + Freq-to-List + minFreq | O(1) all | O(Cap)   |
 | 26 | The Celebrity Problem (LC 277)              | Two-Pointer Candidate Elimination | O(N)     | O(1)     |
 | 27 | Max of Mins Every Window Size               | Monotonic Stack (PSE/NSE) + Suffix| O(N)     | O(N)     |
 | 28 | Count Bracket Reversals for Balance (GFG)   | Counter Balance Math              | O(N)     | O(1)     |
 | 29 | LRU Cache (LC 146)                          | Hash Map + Doubly Linked List     | O(1) all | O(Cap)   |
 ====================================================================================================
*/


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

vi lexicographicalMinSubsequence(vector<int> nums, int k) {
    stack<int> st;
    int remove = nums.size() - k;

    for (int x : nums) {
        while (!st.empty() && st.top() > x && remove > 0) {
            st.pop();
            remove--;
        }

        st.push(x);
    }

    // Remove remaining elements from the end if necessary
    while (remove > 0) {
        st.pop();
        remove--;
    }

    vector<int> res;
    while (!st.empty()) {
        res.push_back(st.top());
        st.pop();
    }

    reverse(res.begin(), res.end());
    return res;
}

vi lexicographicalMaxSubsequence(vector<int> nums, int k) {
    stack<int> st;
    int remove = nums.size() - k;

    for (int x : nums) {
        while (!st.empty() && st.top() < x && remove > 0) {
            st.pop();
            remove--;
        }

        st.push(x);
    }

    // Remove remaining elements from the end if necessary
    while (remove > 0) {
        st.pop();
        remove--;
    }

    vector<int> res;
    while (!st.empty()) {
        res.push_back(st.top());
        st.pop();
    }

    reverse(res.begin(), res.end());
    return res;
}

vi maxMerge(vi &nums1, vi &nums2) {
    int n1 = nums1.size(), n2 = nums2.size();
    vi merged;
    int i = 0, j = 0;
    while (i < n1 || j < n2) {
        if (lexicographical_compare(nums1.begin() + i, nums1.end(), nums2.begin() + j, nums2.end())) {
            merged.push_back(nums2[j++]);
        } else {
            merged.push_back(nums1[i++]);
        }
    }
    return merged;
}

vi maxNumber(vi& nums1, vi& nums2, int k) {
    int n1 = nums1.size(), n2 = nums2.size();
    vi best;
    for (int i = max(0, k - n2); i <= min(k, n1); i++) {
        vi subseq1 = lexicographicalMaxSubsequence(nums1, i);
        vi subseq2 = lexicographicalMaxSubsequence(nums2, k - i);
        vi candidate = maxMerge(subseq1, subseq2);
        if (candidate > best) best = candidate;
    }
    reverse(best.begin(), best.end());
    return best;
}
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


// =========================================================
// 22. ARRAY IMPLEMENTATION OF STACK & QUEUE
// =========================================================

class ArrayStack {
    int topIndex;
    int capacity;
    int *arr;
public:
    ArrayStack(int cap = 1000) : topIndex(-1), capacity(cap) {
        arr = new int[capacity];
    }
    void push(int x) {
        if (topIndex < capacity - 1) arr[++topIndex] = x;
    }
    int pop() {
        if (topIndex >= 0) return arr[topIndex--];
        return -1;
    }
    int top() {
        if (topIndex >= 0) return arr[topIndex];
        return -1;
    }
    bool empty() { return topIndex == -1; }
};

class ArrayQueue {
    int frontIndex, rearIndex, count, capacity;
    int *arr;
public:
    ArrayQueue(int cap = 1000) : frontIndex(0), rearIndex(0), count(0), capacity(cap) {
        arr = new int[capacity];
    }
    void push(int x) {
        if (count < capacity) {
            arr[rearIndex % capacity] = x;
            rearIndex++;
            count++;
        }
    }
    int pop() {
        if (count == 0) return -1;
        int val = arr[frontIndex % capacity];
        frontIndex++;
        count--;
        return val;
    }
    int front() {
        if (count == 0) return -1;
        return arr[frontIndex % capacity];
    }
    bool empty() { return count == 0; }
};
// Interview Explanation:
// - Problem Statement: Implement Stack and Queue data structures using fixed-size arrays.
// - Approach: Pointer index tracking (`topIndex` for stack, circular modular arithmetic for queue).
// - Complexity: Time: O(1) all operations, Space: O(capacity).


// =========================================================
// 23. STACK USING QUEUE & QUEUE USING STACK (LEETCODE 225 & 232)
// =========================================================

class MyStack {
    queue<int> q;
public:
    MyStack() {}
    void push(int x) {
        q.push(x);
        for (int i = 0; i < (int)q.size() - 1; i++) {
            q.push(q.front());
            q.pop();
        }
    }
    int pop() {
        int val = q.front(); q.pop();
        return val;
    }
    int top() { return q.front(); }
    bool empty() { return q.empty(); }
};

class MyQueue {
    stack<int> input, output;
    void transfer() {
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
    }
public:
    MyQueue() {}
    void push(int x) { input.push(x); }
    int pop() {
        transfer();
        int val = output.top(); output.pop();
        return val;
    }
    int peek() {
        transfer();
        return output.top();
    }
    bool empty() { return input.empty() && output.empty(); }
};
// Interview Explanation:
// - Problem Statement: Implement Stack using a single Queue and Queue using two Stacks.
// - Approach: Circular rotation upon push for Stack; Amortized O(1) lazy transfer between input/output stacks for Queue.
// - Complexity: Stack: Push O(N), Pop O(1); Queue: Push O(1), Pop Amortized O(1), Space: O(N).


// =========================================================
// 24. RECURSIVE STACK SORTING (GFG / STRIVER SDE)
// =========================================================

void insertSorted(stack<int>& st, int x) {
    if (st.empty() || st.top() <= x) {
        st.push(x);
        return;
    }
    int topVal = st.top(); st.pop();
    insertSorted(st, x);
    st.push(topVal);
}

void sortStack(stack<int>& st) {
    if (st.empty()) return;
    int topVal = st.top(); st.pop();
    sortStack(st);
    insertSorted(st, topVal);
}
// Interview Explanation:
// - Problem Statement: Sort a stack in ascending order using recursion (no extra data structures).
// - Approach: Two-level recursive backtracking.
// - Intuition: Recursively pop elements until stack is empty, then insert each element back into its correct sorted position using helper `insertSorted`.
// - Complexity: Time: O(N^2), Space: O(N) recursion call stack.


// =========================================================
// 25. LFU CACHE (LEETCODE 460)
// =========================================================

class LFUCache {
    struct Node {
        int key, val, freq;
        Node(int k, int v) : key(k), val(v), freq(1) {}
    };

    int cap, minFreq;
    unordered_map<int, list<Node>::iterator> keyMap;
    unordered_map<int, list<Node>> freqMap;

public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        if (!keyMap.count(key)) return -1;
        auto it = keyMap[key];
        int val = it->val, f = it->freq;
        freqMap[f].erase(it);
        if (freqMap[f].empty() && minFreq == f) minFreq++;

        freqMap[f + 1].push_front(Node(key, val));
        freqMap[f + 1].front().freq = f + 1;
        keyMap[key] = freqMap[f + 1].begin();
        return val;
    }

    void put(int key, int value) {
        if (cap <= 0) return;
        if (get(key) != -1) {
            keyMap[key]->val = value;
            return;
        }

        if ((int)keyMap.size() >= cap) {
            auto evictNode = freqMap[minFreq].back();
            keyMap.erase(evictNode.key);
            freqMap[minFreq].pop_back();
        }

        minFreq = 1;
        freqMap[1].push_front(Node(key, value));
        keyMap[key] = freqMap[1].begin();
    }
};
// Interview Explanation:
// - Problem Statement: Design a Least Frequently Used (LFU) cache supporting get and put in O(1) time.
// - Approach: Hash Map + Doubly Linked Lists per frequency (`freqMap`) + `minFreq` tracker.
// - Intuition: When item is accessed, increment its frequency and move to `freqMap[f+1]`. If capacity is exceeded, evict the least recently used node in `freqMap[minFreq]`.
// - Complexity: Time: O(1) for both get and put, Space: O(capacity).


// =========================================================
// 26. THE CELEBRITY PROBLEM (LEETCODE 277 / GFG)
// =========================================================

template <typename KnowsFunc>
int findCelebrity(int n, const KnowsFunc& knows) {
    int candidate = 0;
    for (int i = 1; i < n; i++) {
        if (knows(candidate, i)) {
            candidate = i;
        }
    }

    for (int i = 0; i < n; i++) {
        if (i != candidate) {
            if (knows(candidate, i) || !knows(i, candidate)) return -1;
        }
    }
    return candidate;
}
// Interview Explanation:
// - Problem Statement: Find celebrity at a party who is known by everyone but knows no one (return -1 if none).
// - Approach: Elimination pass + Verification pass.
// - Intuition: If `knows(a, b)` is true, `a` cannot be celebrity. If false, `b` cannot be celebrity. Candidate remains after N-1 steps; verify against all other N-1 people.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 27. MAXIMUM OF MINIMUMS FOR EVERY WINDOW SIZE (GFG / STRIVER SDE)
// =========================================================

vi maxOfMinWindow(const vi& arr) {
    int n = arr.size();
    vi left(n, -1), right(n, n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && arr[st.top()] >= arr[i]) st.pop();
        if (!st.empty()) left[i] = st.top();
        st.push(i);
    }

    while (!st.empty()) st.pop();

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && arr[st.top()] >= arr[i]) st.pop();
        if (!st.empty()) right[i] = st.top();
        st.push(i);
    }

    vi ans(n + 1, 0);
    for (int i = 0; i < n; i++) {
        int windowLen = right[i] - left[i] - 1;
        ans[windowLen] = max(ans[windowLen], arr[i]);
    }

    for (int i = n - 1; i >= 1; i--) {
        ans[i] = max(ans[i], ans[i + 1]);
    }

    ans.erase(ans.begin());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum of minimums for every window size from 1 to n (GFG / Striver SDE #90).
// - Approach: Monotonic Stack (PSE & NSE) + Window aggregation.
// - Intuition: Element `arr[i]` is minimum in a window of size `len = right[i] - left[i] - 1`. Populate `ans[len] = max(ans[len], arr[i])` and propagate backwards `ans[i] = max(ans[i], ans[i+1])`.
// =========================================================
// 28. COUNT BRACKET REVERSALS FOR BALANCE (GFG)
// =========================================================

int countBracketReversals(string s) {
    int n = s.size();
    if (n % 2 != 0) return -1;

    int open_needed = 0, close_needed = 0;
    for (char c : s) {
        if (c == '{') {
            open_needed++;
        } else {
            if (open_needed > 0) open_needed--;
            else close_needed++;
        }
    }

    return (open_needed + 1) / 2 + (close_needed + 1) / 2;
}
// Interview Explanation:
// - Problem Statement: Find minimum reversals of '{' and '}' to make expression balanced.
// - Approach: Balance simulation counting unmatched open and close brackets.
// - Intuition: Balance remaining open and close brackets: ceil(open/2) + ceil(close/2).
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 29. LRU CACHE (LEETCODE 146)
// =========================================================

class LRUCache {
private:
    struct Node {
        int key, val;
        Node *prev, *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;
    unordered_map<int, Node*> cache;
    Node *head, *tail;

    void addNode(Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void moveToHead(Node *node) {
        removeNode(node);
        addNode(node);
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        Node *node = cache[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            Node *node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            if ((int)cache.size() == cap) {
                Node *lru = tail->prev;
                cache.erase(lru->key);
                removeNode(lru);
                delete lru;
            }
            Node *node = new Node(key, value);
            cache[key] = node;
            addNode(node);
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design a Least Recently Used (LRU) cache with O(1) get and put operations.
// - Approach: Hash Map + Doubly Linked List with dummy head and tail sentinel nodes.
// - Complexity: Time: O(1) get and put, Space: O(capacity).



