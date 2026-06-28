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
       

        while (!stk.empty() && (i==n || heights[i] <= heights[stk.top()])) {
            // Pop equals (avoid duplicates in stack) to cleanly partition duplicate bar boundaries
            // and prevent calculating duplicate rect spans during contribution processing.
            int h = heights[stk.top()];
            stk.pop();
            int leftBoundary = stk.empty() ? -1 : stk.top(); // New left boundary after pop (strictly smaller)
            int rightBoundary = i; // Current index is the right boundary (smaller or equal)
            int width = rightBoundary - leftBoundary - 1; // (both boundaries are exclusive)
            // Left boundary: index of new top (element still in stack)
            // Right boundary: i (current element that caused the pop)
            
            maxArea = max(maxArea, h * width);
        }
        if(i < n) stk.push(i); // Push current index onto stack
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
    } // holds indices of decreasing values from left to right

    int maxWidth = 0;

    // Scan right to left, greedily match with the largest left index possible, when we start from the right, we are guaranteed to find the largest j for each i, because we are scanning from the rightmost end of the array. This ensures that for each left index i, we find the farthest right index j that satisfies the ramp condition.
    for (int j = n - 1; j >= 0; j--) {
        while (!stk.empty() && nums[stk.top()] <= nums[j]) {
            maxWidth = max(maxWidth, j - stk.top());
            stk.pop(); // This left endpoint is fully used (j is the farthest right for it)
        }
    }
    return maxWidth;
}

