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

