/**
 * Monotonic Stack - Histogram & Matrix Problems
 *
 * 1. Largest Rectangle in Histogram      - LC 84 (Hard) — classic interview killer
 * 2. Maximal Rectangle in Binary Matrix  - LC 85 (Hard) — histogram per row
 * 3. Maximum Width Ramp                  - LC 962
 *
 * Core Idea for Histogram:
 * - Use PSE (Previous Smaller) + NSE (Next Smaller) to find the span
 *   over which each bar is the minimum height.
 * - Area = height[i] * (NSE[i] - PSE[i] - 1)
 * - Take the maximum over all bars.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;

// ─────────────────────────────────────────────────────────────────────────────
// 1. LARGEST RECTANGLE IN HISTOGRAM
// LC 84
//
// Given bar heights, find the largest rectangular area that fits within the histogram.
//
// Idea:
// - For each bar i, the rectangle using bar i as the shortest bar extends:
//     left  -> index of Previous Smaller Element (exclusive)
//     right -> index of Next Smaller Element (exclusive)
//     width = right - left - 1
//     area  = height[i] * width
//
// One-pass approach with stack:
// - Traverse left to right. Push indices onto a monotonic increasing stack.
// - When we see a bar shorter than the top, pop and compute area.
//   The width extends from the new top (new left boundary) to current i (right boundary).
// - After iteration, flush remaining bars using n as right boundary.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

int largestRectangle(const vi& heights) {
    int n = heights.size();
    stack<int> stk; // Monotonic increasing stack of indices
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        // Use 0 as sentinel height at position n to flush remaining stack
        int curHeight = (i == n) ? 0 : heights[i];

        while (!stk.empty() && curHeight < heights[stk.top()]) {
            int h = heights[stk.top()];
            stk.pop();

            // Left boundary: index of new top (element still in stack)
            // Right boundary: i (current element that caused the pop)
            int width = stk.empty() ? i : (i - stk.top() - 1);
            maxArea = max(maxArea, h * width);
        }
        stk.push(i);
    }
    return maxArea;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. MAXIMAL RECTANGLE IN BINARY MATRIX
// LC 85
//
// Given a binary matrix, find the area of the largest rectangle containing only 1s.
//
// Idea:
// - Process the matrix row by row.
// - For each row, build a histogram where heights[j] = consecutive 1s ending at this row.
//   If matrix[row][j] == '0', reset heights[j] = 0.
//   If matrix[row][j] == '1', increment heights[j].
// - Run Largest Rectangle in Histogram on the current heights.
// - Answer is the max over all rows.
//
// Time: O(M * N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

int maximalRectangle(vector<vector<char>>& matrix) {
    if (matrix.empty()) return 0;
    int m = matrix.size(), n = matrix[0].size();
    vi heights(n, 0); // Running histogram heights
    int maxArea = 0;

    for (int row = 0; row < m; row++) {
        // Update histogram: extend if 1, reset if 0
        for (int col = 0; col < n; col++) {
            heights[col] = (matrix[row][col] == '1') ? heights[col] + 1 : 0;
        }
        // Find the largest rectangle in this row's histogram
        maxArea = max(maxArea, largestRectangle(heights));
    }
    return maxArea;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. MAXIMUM WIDTH RAMP
// LC 962
//
// A "ramp" is a pair (i, j) with i < j and nums[i] <= nums[j].
// Find the maximum width j - i of any ramp.
//
// Idea:
// - Build a "decreasing candidates stack" of indices from left to right.
//   Only push index i if nums[i] < nums[stack.top()] — these are potential left endpoints.
//   (If nums[i] >= nums[stack.top()], i can never be a better left endpoint than top.)
// - Then scan from RIGHT to LEFT.
//   For each j, pop from the stack while nums[stack.top()] <= nums[j].
//   Each popped index i gives a valid ramp (i, j). Width = j - i.
//   Since we scan j right-to-left and pop, the first valid (i, j) for a given i
//   has the maximum possible j.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

int maxWidthRamp(const vi& nums) {
    int n = nums.size();
    stack<int> stk;

    // Build decreasing stack of potential left endpoints
    for (int i = 0; i < n; i++) {
        if (stk.empty() || nums[i] < nums[stk.top()]) {
            stk.push(i);
        }
    }

    int maxWidth = 0;

    // Scan right to left, greedily match with the largest left index possible
    for (int j = n - 1; j >= 0; j--) {
        while (!stk.empty() && nums[stk.top()] <= nums[j]) {
            maxWidth = max(maxWidth, j - stk.top());
            stk.pop(); // This left endpoint is fully used (j is the farthest right for it)
        }
    }
    return maxWidth;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        // Largest Rectangle in Histogram
        int n; cin >> n;
        vi h(n);
        for (int& x : h) cin >> x;
        cout << largestRectangle(h) << '\n';
    */

    /*
        // Maximal Rectangle in Binary Matrix
        int m, n; cin >> m >> n;
        vector<vector<char>> mat(m, vector<char>(n));
        for (auto& row : mat)
            for (char& c : row) cin >> c;
        cout << maximalRectangle(mat) << '\n';
    */

    /*
        // Maximum Width Ramp
        int n; cin >> n;
        vi nums(n);
        for (int& x : nums) cin >> x;
        cout << maxWidthRamp(nums) << '\n';
    */

    return 0;
}
