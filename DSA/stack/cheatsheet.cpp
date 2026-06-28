/**
 * Monotonic Stack - Master Cheat Sheet & Quick Reference
 *
 * Use this file to:
 * - Quickly recall which pattern to apply
 * - Understand the invariant maintained by the stack
 * - Identify the time/space complexity at a glance
 *
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │                    MONOTONIC STACK QUICK DECISION GUIDE                 │
 * ├────────────────────────────────┬────────────────┬────────────────────────┤
 * │ Problem Type                   │ Stack Order    │ Direction              │
 * ├────────────────────────────────┼────────────────┼────────────────────────┤
 * │ Next Greater Element           │ Decreasing     │ Left → Right           │
 * │ Next Smaller Element           │ Increasing     │ Left → Right           │
 * │ Previous Greater Element       │ Decreasing     │ Left → Right (pre-pop) │
 * │ Previous Smaller Element       │ Increasing     │ Left → Right (pre-pop) │
 * │ Largest Rectangle Histogram    │ Increasing     │ L→R + flush at end     │
 * │ Trapping Rain Water            │ Decreasing     │ Left → Right           │
 * │ Sum of Subarray Minimums       │ Increasing     │ L→R + flush at end     │
 * │ Sum of Subarray Maximums       │ Decreasing     │ L→R + flush at end     │
 * │ Remove K Digits (min number)   │ Increasing     │ Left → Right           │
 * │ Remove Duplicate Letters       │ Increasing     │ Left → Right           │
 * │ 132 Pattern                    │ Decreasing     │ Right → Left           │
 * │ Daily Temperatures             │ Decreasing     │ Left → Right (NGE)     │
 * │ Stock Span                     │ Decreasing     │ Left → Right (PGE)     │
 * └────────────────────────────────┴────────────────┴────────────────────────┘
 *
 * Files in this folder:
 * ├── nge_variants.cpp    — NGE / NSE / PGE / PSE + circular array
 * ├── histogram.cpp       — Largest Rectangle, Maximal Rectangle, Width Ramp
 * ├── rain_water.cpp      — Trapping Rain Water (3 approaches) + Container With Most Water
 * ├── subarray_sum.cpp    — Sum of Subarray Minimums/Maximums (contribution technique)
 * ├── lex_order.cpp       — Remove K Digits, Remove Duplicates, 132 Pattern, Daily Temps
 * └── cheatsheet.cpp      — This file
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;
using ll = long long;
using vi = vector<int>;

// ─────────────────────────────────────────────────────────────────────────────
// TEMPLATE: MONOTONIC INCREASING STACK (for NSE, PSE, Histogram, Sum of Mins)
//
// Invariant: stk[0] < stk[1] < ... < stk[top] (indices, values are increasing)
// When to pop: incoming value < stk.top() value
// ─────────────────────────────────────────────────────────────────────────────

void templateIncreasing(const vi& nums) {
    stack<int> stk; // Stores indices; values are monotonically increasing

    for (int i = 0; i <= (int)nums.size(); i++) {
        int curVal = (i == (int)nums.size()) ? INT_MIN : nums[i]; // Sentinel to flush

        while (!stk.empty() && curVal <= nums[stk.top()]) {
            int j = stk.top();
            stk.pop();

            // --- PROCESS j here ---
            // j is the index of the element being popped.
            // Its NSE (Next Smaller Element) is at index i.
            // Its PSE (Previous Smaller Element) is at index stk.top() (or -1 if empty).
            int pse = stk.empty() ? -1 : stk.top();
            int nse = i;
            // leftSpan  = j - pse   (number of subarrays where j is the right boundary of min)
            // rightSpan = nse - j   (number of subarrays where j is the left boundary of min)
            // contribution = nums[j] * leftSpan * rightSpan
        }
        stk.push(i);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TEMPLATE: MONOTONIC DECREASING STACK (for NGE, PGE, Rain Water, Sum of Maxs)
//
// Invariant: stk[0] > stk[1] > ... > stk[top] (indices, values are decreasing)
// When to pop: incoming value > stk.top() value
// ─────────────────────────────────────────────────────────────────────────────

void templateDecreasing(const vi& nums) {
    stack<int> stk; // Stores indices; values are monotonically decreasing

    for (int i = 0; i <= (int)nums.size(); i++) {
        int curVal = (i == (int)nums.size()) ? INT_MAX : nums[i]; // Sentinel to flush

        while (!stk.empty() && curVal >= nums[stk.top()]) {
            int j = stk.top();
            stk.pop();

            // --- PROCESS j here ---
            // j is the index of the element being popped.
            // Its NGE (Next Greater Element) is at index i.
            // Its PGE (Previous Greater Element) is at index stk.top() (or -1 if empty).
            int pge = stk.empty() ? -1 : stk.top();
            int nge = i;
        }
        stk.push(i);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MENTAL MODEL SUMMARY
// ─────────────────────────────────────────────────────────────────────────────

/*
    INCREASING STACK (for MINIMUM-related problems):
    ─────────────────────────────────────────────────
    - Stack stays increasing from bottom to top.
    - Pop happens when current < top.
    - When you pop index j, you know:
        - NSE of j = current index i
        - PSE of j = new top (after pop)
    - Use for: NSE, PSE, histogram, sum of minimums.

    DECREASING STACK (for MAXIMUM-related problems):
    ─────────────────────────────────────────────────
    - Stack stays decreasing from bottom to top.
    - Pop happens when current > top.
    - When you pop index j, you know:
        - NGE of j = current index i
        - PGE of j = new top (after pop)
    - Use for: NGE, PGE, rain water, sum of maximums.

    LEX ORDER STACK (building optimal string/sequence):
    ────────────────────────────────────────────────────
    - Same as increasing stack but with a constraint:
      only pop if the element will appear later (or if removals remain).
    - Use for: Remove K Digits, Remove Duplicate Letters.

    RIGHT-TO-LEFT SCAN:
    ───────────────────
    - 132 Pattern: process right to left with decreasing stack.
      Track the "second largest from right" as the middle element.
    - Maximum Width Ramp: build candidate stack left-to-right,
      then scan right-to-left to greedily match.
*/

// ─────────────────────────────────────────────────────────────────────────────
// CSES / LEETCODE PROBLEM MAP
// ─────────────────────────────────────────────────────────────────────────────

/*
    LC 496  — Next Greater Element I        → nge_variants.cpp: nextGreaterElement
    LC 503  — Next Greater Element II       → nge_variants.cpp: nextGreaterCircular
    LC 739  — Daily Temperatures            → lex_order.cpp: dailyTemperatures
    LC 901  — Online Stock Span             → nge_variants.cpp: stockSpan
    LC 907  — Sum of Subarray Minimums      → subarray_sum.cpp: sumSubarrayMins
    LC 84   — Largest Rectangle Histogram   → histogram.cpp: largestRectangle
    LC 85   — Maximal Rectangle             → histogram.cpp: maximalRectangle
    LC 42   — Trapping Rain Water           → rain_water.cpp: trapTwoPointer / trapStack
    LC 407  — Trapping Rain Water II        → rain_water.cpp: trapRainWater2D
    LC 11   — Container With Most Water     → rain_water.cpp: maxWater
    LC 402  — Remove K Digits              → lex_order.cpp: removeKDigits
    LC 316  — Remove Duplicate Letters      → lex_order.cpp: removeDuplicateLetters
    LC 456  — 132 Pattern                   → lex_order.cpp: find132Pattern
    LC 962  — Maximum Width Ramp            → histogram.cpp: maxWidthRamp
    LC 321  — Create Maximum Number         → lex_order.cpp: maxNumberFromArray

    CSES:
    CSES 1143 — Maximum Subarray Sum        → Classic Kadane, not stack
    CSES 1621 — Distinct Values Queries     → Offline + sorting, related pattern
*/

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}
