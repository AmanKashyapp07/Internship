/**
 * Monotonic Stack - NGE / NSE / PGE / PSE Variants
 *
 * The four foundational patterns every OA tests:
 *
 * 1. Next Greater Element (NGE)     - LC 496, LC 503
 * 2. Next Smaller Element (NSE)
 * 3. Previous Greater Element (PGE) - Stock Span Problem
 * 4. Previous Smaller Element (PSE)
 * 5. NGE in Circular Array          - LC 503
 *
 * Core Idea:
 * - Maintain a monotonic stack of indices.
 * - For GREATER: stack is non-increasing (pop when current > top).
 * - For SMALLER:  stack is non-decreasing (pop when current < top).
 * - "Next" variants: iterate left to right, pop when answer is found.
 * - "Previous" variants: iterate left to right, push before reading stack.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;
using vi = vector<int>;

#define all(x) (x).begin(), (x).end()

// ─────────────────────────────────────────────────────────────────────────────
// 1. NEXT GREATER ELEMENT (NGE)
// LC 496: Next Greater Element I
//
// For each element, find the first element to its RIGHT that is strictly greater.
// If none exists, return -1.
//
// Idea:
// - Traverse left to right.
// - Maintain a monotonic decreasing stack of indices.
// - When nums[i] > nums[stack.top()], we found the NGE for stack.top().
// - Pop and record the answer, then push i.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi nextGreaterElement(const vi& nums) {
    int n = nums.size();
    vi result(n, -1); // Default: no greater element found
    stack<int> s;   // Monotonic decreasing stack of indices

    for (int i = 0; i < n; i++) {
        // Current element breaks the decreasing order: it's the NGE for all smaller tops
        while (!s.empty() && nums[i] > nums[s.top()]) {
            // Keep equals (do not pop on equal): equal values are not strictly greater than
            // nums[s.top()], so they cannot resolve the Next Greater query for it.
            result[s.top()] = nums[i];
            s.pop();
        }
        s.push(i);
    }
    // Remaining elements in stack have no NGE (already initialized to -1)
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. NEXT SMALLER ELEMENT (NSE)
//
// For each element, find the first element to its RIGHT that is strictly smaller.
//
// Idea: Same as NGE but flip the comparison (pop when current < top).
// Stack is maintained in monotonic increasing order.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi nextSmallerElement(const vi& nums) {
    int n = nums.size();
    vi result(n, -1);
    stack<int> s; // Monotonic increasing stack of indices

    for (int i = 0; i < n; i++) {
        while (!s.empty() && nums[i] < nums[s.top()]) {
            // Keep equals (do not pop on equal): equal values are not strictly smaller than
            // nums[s.top()], so they cannot resolve the Next Smaller query for it.
            result[s.top()] = nums[i];
            s.pop();
        }
        s.push(i);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. PREVIOUS GREATER ELEMENT (PGE)
// Classic: Stock Span Problem
//
// For each element, find the first element to its LEFT that is strictly greater.
//
// Idea:
// - Traverse left to right.
// - Before pushing i, peek at the stack top.
// - The top is the answer for the current element if nums[top] > nums[i].
// - Pop smaller elements that can never be anyone's PGE anymore.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi previousGreaterElement(const vi& nums) {
    int n = nums.size();
    vi result(n, -1);
    stack<int> s; // Monotonic decreasing stack of indices

    for (int i = 0; i < n; i++) {
        // Pop elements that are <= current (they can never be PGE for future elements either)
        while (!s.empty() && nums[s.top()] <= nums[i]) {
            // Pop equals (avoid duplicates in stack): equal values are not strictly greater than
            // nums[i] and keeping them would block nums[i] from seeing a strictly greater element further left.
            s.pop();
        }
        // The remaining top (if any) is the PGE for i
        if (!s.empty()) {
            result[i] = nums[s.top()];
        }
        s.push(i);
    }
    return result;
}

// Stock Span variant: returns the span (count of consecutive previous elements <= current)
vi stockSpan(const vi& prices) {
    int n = prices.size();
    vi span(n, 1);
    stack<int> s; // Stores indices of previous elements with no solution yet

    for (int i = 0; i < n; i++) {
        while (!s.empty() && prices[s.top()] <= prices[i]) {
            s.pop();
        }
        // If stack is empty, all previous elements are <= prices[i]
        span[i] = s.empty() ? (i + 1) : (i - s.top());
        s.push(i);
    }
    return span;
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. PREVIOUS SMALLER ELEMENT (PSE)
//
// For each element, find the first element to its LEFT that is strictly smaller.
// Core building block for: Sum of Subarray Minimums, Largest Rectangle in Histogram.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi previousSmallerElement(const vi& nums) {
    int n = nums.size();
    vi result(n, -1);
    stack<int> s; // Monotonic increasing stack of indices

    for (int i = 0; i < n; i++) {
        while (!s.empty() && nums[s.top()] >= nums[i]) {
            // Pop equals (avoid duplicates in stack): equal values are not strictly smaller than
            // nums[i] and keeping them would block nums[i] from seeing a strictly smaller element further left.
            s.pop();
        }
        if (!s.empty()) {
            result[i] = nums[s.top()];
        }
        s.push(i);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. NGE IN CIRCULAR ARRAY
// LC 503: Next Greater Element II
//
// Same as NGE but the array wraps around circularly.
//
// Trick: Simulate 2 passes by using index modulo (iterate 2*n steps).
// Only record answer for the first n indices.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi nextGreaterCircular(const vi& nums) {
    int n = nums.size();
    vi result(n, -1);
    stack<int> s;

    // Loop twice to simulate circular behavior
    for (int i = 0; i < 2 * n; i++) {
        int cur = nums[i % n];
        while (!s.empty() && cur > nums[s.top()]) {
            // Keep equals (do not pop on equal): equal values are not strictly greater than
            // nums[s.top()], so they cannot resolve the Next Greater query for it.
            result[s.top()] = cur;
            s.pop();
        }
        // Only push actual indices (first pass)
        if (i < n) s.push(i);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        int n; cin >> n;
        vi nums(n);
        for (int& x : nums) cin >> x;

        vi nge = nextGreaterElement(nums);
        vi nse = nextSmallerElement(nums);
        vi pge = previousGreaterElement(nums);
        vi pse = previousSmallerElement(nums);
        vi circ = nextGreaterCircular(nums);
        vi span = stockSpan(nums);

        for (int x : nge) cout << x << ' '; cout << '\n';
    */

    return 0;
}
