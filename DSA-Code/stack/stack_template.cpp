/**
 * ====================================================================================================
 *                 ULTIMATE MONOTONIC STACK & QUEUE MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Next/Previous Greater/Smaller Elements, Circular Arrays,
 * Histogram & Matrix Rectangles, Trapping Rain Water, Subarray Extrema Contributions,
 * Lexicographical Optimizations (Remove K Digits, Remove Duplicate Letters), and 132 Pattern.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;

const ll MOD = 1e9 + 7;

// ====================================================================================================
// SECTION 1: MONOTONIC STACK CORE PRIMITIVES (NGE / NSE / PGE / PSE)
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
// - Complexity: Time: O(N) each element pushed and popped at most once, Space: O(N).


// 2. Next Smaller Element (NSE)
vi nextSmallerElement(const vi &nums) {
    int n = nums.size();
    vi nse(n, n); // Default to n (boundary) if no smaller element exists
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
// - Intuition: Incoming smaller value invalidates the increasing order; elements popped from the stack have their NSE set to current index.
// - Complexity: Time: O(N), Space: O(N).


// 3. Previous Greater Element / Stock Span (LeetCode 901)
vi stockSpan(const vi &prices) {
    int n = prices.size();
    vi span(n);
    stack<int> stk; // Monotonic decreasing stack of indices
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && prices[stk.top()] <= prices[i]) {
            stk.pop();
        }
        span[i] = stk.empty() ? (i + 1) : (i - stk.top());
        stk.push(i);
    }
    return span;
}
// Interview Explanation:
// - Problem Statement: Compute the span of stock prices (number of consecutive days before today with price <= today's price).
// - Approach: Previous Greater Element via Monotonic Decreasing Stack.
// - Intuition: The span is the distance from current index i to the previous strictly greater element index `stk.top()`.
// - Complexity: Time: O(N), Space: O(N).


// 4. Next Greater Element II - Circular Array (LeetCode 503)
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
// - Problem Statement: Find the Next Greater Element in a circular array.
// - Approach: Virtual 2N loop with Monotonic Decreasing Stack.
// - Intuition: Simulates searching across array boundary by iterating up to 2N - 1, only pushing indices on the first pass (i < N).
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// SECTION 2: AREA & RANGE EXTREMA (HISTOGRAM & MATRIX)
// ====================================================================================================

// 1. Largest Rectangle in Histogram (LeetCode 84)
int largestRectangleArea(const vi &heights) {
    int n = heights.size(), maxArea = 0;
    stack<int> stk;
    for (int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i]; // Flush sentinel at index n
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
// - Problem Statement: Find the area of the largest rectangle in a histogram bar heights array.
// - Approach: Monotonic Increasing Stack with a 0-height sentinel flush at index N.
// - Intuition: When a shorter bar is encountered, pop the stack; the popped bar is the limiting height, with width bounded by its PSE (`stk.top()`) and NSE (`i`).
// - Complexity: Time: O(N), Space: O(N).


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
// Interview Explanation:
// - Problem Statement: Find the largest rectangle containing only 1s in a 2D binary grid.
// - Approach: Dynamic Histogram construction per row + Largest Rectangle in Histogram solver.
// - Intuition: Each row forms a histogram of contiguous '1' column heights; running `largestRectangleArea` on every row finds the global maximum rectangle.
// - Complexity: Time: O(N \cdot M), Space: O(M).


// 3. Sum of Subarray Minimums (LeetCode 907)
int sumSubarrayMins(const vi &arr) {
    int n = arr.size();
    vi pse(n), nse(n);
    stack<int> stk;

    // Previous Smaller or Equal Element (PSE - non-strict inequality to avoid double counting)
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && arr[stk.top()] > arr[i]) stk.pop();
        pse[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    while (!stk.empty()) stk.pop();

    // Next Smaller Element (NSE - strict inequality)
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
// Interview Explanation:
// - Problem Statement: Compute the sum of min(b) for all contiguous subarrays b of arr modulo 10^9+7.
// - Approach: Contribution Technique via Monotonic Stack calculating PSE and NSE spans.
// - Intuition: Element arr[i] is the minimum for exactly `(i - pse[i]) * (nse[i] - i)` subarrays; non-strict PSE prevents duplicate subarray double counting.
// - Complexity: Time: O(N), Space: O(N).


// 4. Sum of Subarray Ranges (LeetCode 2104 - Max Sum minus Min Sum)
ll subArrayRanges(const vi &nums) {
    int n = nums.size();
    ll totalRanges = 0;
    // Calculate Sum of Maxs - Sum of Mins
    // Sum of Mins
    vi pse(n), nse(n);
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

    // Sum of Maxs
    vi pge(n), nge(n);
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
// - Problem Statement: Find the sum of (max(b) - min(b)) for all contiguous subarrays b of nums.
// - Approach: Contribution Technique using Monotonic Stacks for both Maxima and Minima.
// - Intuition: `Range Sum = Sum(Subarray Maximums) - Sum(Subarray Minimums)`; compute element contributions independently in linear time.
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// SECTION 3: WATER TRAPPING & CONTAINER BOUNDARIES
// ====================================================================================================

// 1. Trapping Rain Water (LeetCode 42 - Monotonic Stack & Two Pointers)
int trapRainWaterStack(const vi &height) {
    int n = height.size(), water = 0;
    stack<int> stk; // Monotonic Decreasing Stack storing indices
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && height[stk.top()] < height[i]) {
            int bottom = stk.top();
            stk.pop();
            if (stk.empty()) break;
            int leftWall = stk.top();
            int width = i - leftWall - 1;
            int boundedHeight = min(height[leftWall], height[i]) - height[bottom];
            water += width * boundedHeight;
        }
        stk.push(i);
    }
    return water;
}

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
// Interview Explanation:
// - Problem Statement: Compute how much water elevation map can trap after raining.
// - Approach: (1) Monotonic Decreasing Stack horizontal water layer filling. (2) Two-Pointer O(1) space inward walk.
// - Intuition: Stack approach fills water in horizontal slices bounded by leftWall (`stk.top()`) and rightWall (`i`); Two-Pointer relies on `min(maxLeft, maxRight)` bottleneck.
// - Complexity: Time: O(N), Space: Stack O(N), Two-Pointer O(1).


// 2. Container With Most Water (LeetCode 11 - Two Pointers)
int maxAreaContainer(const vi &height) {
    int i = 0, j = (int)height.size() - 1, maxArea = 0;
    while (i < j) {
        int area = (j - i) * min(height[i], height[j]);
        maxArea = max(maxArea, area);
        if (height[i] < height[j]) i++;
        else j--;
    }
    return maxArea;
}
// Interview Explanation:
// - Problem Statement: Find two lines that together with x-axis form a container holding the maximum water.
// - Approach: Two-Pointer greedy inward contraction.
// - Intuition: Container area is limited by the shorter line; moving the taller line inward reduces width without possibility of increasing height, so always move shorter line.
// - Complexity: Time: O(N), Space: O(1).


// ====================================================================================================
// SECTION 4: LEXICOGRAPHICAL STACK & GREEDY OPTIMIZATIONS
// ====================================================================================================

// 1. Remove K Digits (LeetCode 402)
string removeKdigits(string num, int k) {
    string stk; // Use string as monotonic increasing stack
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
    while (start < (int)stk.size() && stk[start] == '0') start++; // Strip leading zeros
    string ans = stk.substr(start);
    return ans.empty() ? "0" : ans;
}
// Interview Explanation:
// - Problem Statement: Remove k digits from num string to make the resulting number lexicographically smallest.
// - Approach: Monotonic Increasing Stack popping larger leading digits while k > 0.
// - Intuition: A smaller digit earlier produces a smaller number overall; pop digits that are strictly larger than the incoming digit.
// - Complexity: Time: O(N), Space: O(N).


// 2. Remove Duplicate Letters / Smallest Subsequence of Distinct Characters (LeetCode 316 / 1081)
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
// - Problem Statement: Remove duplicate letters so that every letter appears once and result is lexicographically smallest.
// - Approach: Monotonic Increasing Stack with last-occurrence tracking (`lastIndex`) and visited state array.
// - Intuition: Only pop a larger character from stack top if `lastIndex[stk.top()] > i` (guaranteeing it will appear again later in the string).
// - Complexity: Time: O(N), Space: O(1) stack size <= 26.


// 3. Daily Temperatures (LeetCode 739)
vi dailyTemperatures(const vi &temperatures) {
    int n = temperatures.size();
    vi ans(n, 0);
    stack<int> stk; // Monotonic Decreasing Stack storing indices
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && temperatures[stk.top()] < temperatures[i]) {
            int j = stk.top();
            stk.pop();
            ans[j] = i - j;
        }
        stk.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find how many days you have to wait until a warmer temperature occurs.
// - Approach: Monotonic Decreasing Stack storing day indices (Next Greater Element variant).
// - Intuition: Pop colder days when a warmer day `temperatures[i]` arrives; the wait time for popped day `j` is `i - j`.
// - Complexity: Time: O(N), Space: O(N).


// 4. 132 Pattern (LeetCode 456)
bool find132pattern(const vi &nums) {
    int n = nums.size(), s3 = INT_MIN;
    stack<int> stk; // Monotonic Decreasing Stack scanned right-to-left
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] < s3) return true; // Found s1 < s3 < s2
        while (!stk.empty() && stk.top() < nums[i]) {
            s3 = stk.top(); // Update s3 to highest possible value smaller than s2 (nums[i])
            stk.pop();
        }
        stk.push(nums[i]);
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if there exists a 132 pattern (i < j < k with nums[i] < nums[k] < nums[j]).
// - Approach: Right-to-Left scan with Monotonic Decreasing Stack tracking the optimal $s_3$ candidate.
// - Intuition: Stack represents $s_2$ candidates; popping elements smaller than current $nums[i]$ maximizes $s_3$. If $nums[i] < s_3$, valid 132 pattern is found.
// - Complexity: Time: O(N), Space: O(N).

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: MONOTONIC STACK
 ====================================================================================================

 1. MONOTONIC STACK DECISION MATRIX:
    -------------------------------------------------------------------------------------------------
    Query Type                      | Stack Order   | Traversal Direction  | Pop Condition
    -------------------------------------------------------------------------------------------------
    Next Greater Element (NGE)      | Decreasing    | Left → Right         | incoming > stk.top()
    Next Smaller Element (NSE)      | Increasing    | Left → Right         | incoming < stk.top()
    Previous Greater Element (PGE)  | Decreasing    | Left → Right         | incoming >= stk.top()
    Previous Smaller Element (PSE)  | Increasing    | Left → Right         | incoming <= stk.top()
    Largest Rectangle Histogram     | Increasing    | Left → Right         | incoming <= stk.top() + flush 0
    Trapping Rain Water (1D)        | Decreasing    | Left → Right         | incoming > stk.top()
    Remove K Digits                 | Increasing    | Left → Right         | incoming < stk.top() && k > 0
    Remove Duplicate Letters        | Increasing    | Left → Right         | incoming < top && top in future
    132 Pattern                     | Decreasing    | Right → Left         | incoming > stk.top()
    -------------------------------------------------------------------------------------------------

 2. SUBARRAY CONTRIBUTION FORMULA:
    • Total Subarrays where `arr[i]` is MINIMUM:
      $\text{Count} = (i - \text{PSE}[i]) \times (\text{NSE}[i] - i)$
    • Total Subarrays where `arr[i]` is MAXIMUM:
      $\text{Count} = (i - \text{PGE}[i]) \times (\text{NGE}[i] - i)$
    • CRITICAL GOTCHA (TIE-BREAKING DUPLICATES):
      To prevent double-counting equal elements (e.g. [2, 2, 2]), use STRICT inequality (< or >) for one
      boundary (e.g., PSE) and NON-STRICT inequality (<= or >=) for the other boundary (e.g., NSE)!

 3. HISTOGRAM & MATRIX RECTANGLE SENTINEL TRICK:
    • Always append a sentinel value of `0` at index `N` when running `largestRectangleArea` so that
      all remaining elements on the stack are automatically popped and evaluated before termination.
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
