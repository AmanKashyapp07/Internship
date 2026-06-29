/**
 * Monotonic Stack - Subarray Sum via Contribution Technique
 *
 * 1. Sum of Subarray Minimums  - LC 907 (Amazon OA classic)
 * 2. Sum of Subarray Maximums  - symmetric variant
 * 3. Sum of (Max - Min) Over All Subarrays - combined
 *
 * Core Idea: Contribution Technique
 * - Instead of iterating over all O(N^2) subarrays, ask:
 *   "For how many subarrays is nums[i] the MINIMUM?"
 * - The answer = number of subarrays where nums[i] is the minimum
 *              = (i - PSE[i]) * (NSE[i] - i)
 *   where PSE = Previous Smaller Element index, NSE = Next Smaller Element index.
 * - Total sum = sum of nums[i] * count[i] over all i.
 *
 * Tie-breaking rule (to avoid double counting):
 * - For PSE: use strictly smaller  (<)
 * - For NSE: use smaller or equal  (<=)
 * (Or vice versa, just be consistent.)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;
using ll = long long;
using vi = vector<int>;

const ll MOD = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────
// 1. SUM OF SUBARRAY MINIMUMS
// LC 907
//
// Return the sum of min(subarray) for all contiguous subarrays.
//
// Idea (Contribution Technique):
// - For each index i, find:
//     left  = i - PSE_index[i]  (distance to previous strictly smaller element)
//     right = NSE_index[i] - i  (distance to next smaller-or-equal element)
// - nums[i] contributes nums[i] * left * right to the answer.
//   (It is the minimum in exactly left * right subarrays.)
//
// One-pass combined approach using a single stack:
// - Use a monotonic increasing stack of indices.
// - When we pop index j (because nums[i] <= nums[j]):
//     left boundary  = stack.empty() ? -1 : stack.top()
//     right boundary = i
//     contribution   = nums[j] * (j - left) * (right - j)
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

ll sumSubarrayMins(const vi& arr) {
    int n = arr.size();
    stack<int> stk; // Monotonic increasing stack of indices
    ll total = 0;

    // Sentinel value: process remaining stack at the end with i = n
    for (int i = 0; i <= n; i++) {

        while (!stk.empty() && (i==n || arr[stk.top()] >= arr[i])) { 
            // Pop equals (avoid duplicates in stack) to ensure PSE is strict (<) and NSE is non-strict (<=).
            // This prevents duplicate values in subarrays from being counted twice.
            int j = stk.top();
            stk.pop();

            // PSE index (strictly smaller on the left), that's why left is stk.top(), not stk.top() + 1
            int left = stk.empty() ? -1 : stk.top();
            // NSE index (smaller or equal on the right) = i, that's why right is i, not i - 1
            int right = i;
            // keep in mind, both boundaries are exclusive, so the number of subarrays where arr[j] is the minimum is (j - left) * (right - j)

            ll leftCount = j - left;   // Subarrays starting between (left, j]
            ll rightCount = right - j; // Subarrays ending between [j, right)

            total = (total + (ll)arr[j] % MOD * leftCount % MOD * rightCount) % MOD; // (ll) cast to avoid overflow, use this always in OAs 
        }
        if(i < n) stk.push(i); // Push current index onto stack
    }
    return total;
}

int maximumScore(vector<int>& nums, int k) {
    int n = nums.size();
    stack<int> stk; // Monotonic increasing stack of indices
    ll maxi = -1;

    // Sentinel value: process remaining stack at the end with i = n
    for (int i = 0; i <= n; i++) {

        while (!stk.empty() && (i==n || nums[stk.top()] >= nums[i])) { 
            // Pop equals (avoid duplicates in stack) to ensure PSE is strict (<) and NSE is non-strict (<=).
            // This prevents duplicate values in subarrays from being counted twice.
            int j = stk.top();
            stk.pop();

            // PSE index (strictly smaller on the left), that's why left is stk.top(), not stk.top() + 1
            int left = stk.empty() ? -1 : stk.top();
            // NSE index (smaller or equal on the right) = i, that's why right is i, not i - 1
            int right = i;
            // keep in mind, both boundaries are exclusive, so the number of subarrays where arr[j] is the minimum is (j - left) * (right - j)
            left=max(left, k); 
            right=min(right, k); // right boundary should be at most k+1

            maxi=max(maxi, nums[j] * (right-left+1)); 
        }
        if(i < n) stk.push(i); // Push current index onto stack
    }
    return maxi;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. SUM OF SUBARRAY MAXIMUMS
// Symmetric to Sum of Subarray Minimums — use a decreasing stack instead.
//
// For each index i, nums[i] is the maximum in subarrays bounded by:
//   PGE = Previous Strictly Greater (left boundary)
//   NGE = Next Greater or Equal (right boundary)
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

ll sumSubarrayMaxs(const vi& arr) {
    int n = arr.size();
    stack<int> stk; // Monotonic decreasing stack of indices
    ll total = 0;

    for (int i = 0; i <= n; i++) {

        while (!stk.empty() && (i==n || arr[stk.top()] <= arr[i])) { 
            // Pop equals (avoid duplicates in stack) to ensure PGE is strict (>) and NGE is non-strict (>=).
            // This prevents duplicate values in subarrays from being counted twice.
            int j = stk.top();
            stk.pop();

            // PGE index (strictly greater on the left)
            int left = stk.empty() ? -1 : stk.top();
            int right = i;
            // keep in mind, both boundaries are exclusive, so the number of subarrays where arr[j] is the maximum is (j - left) * (right - j)
            ll leftCount = j - left;
            ll rightCount = right - j;

            total = (total + (ll)arr[j] % MOD * leftCount % MOD * rightCount) % MOD;
        }
        if(i < n) stk.push(i);
    }
    return total;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. SUM OF (MAX - MIN) OVER ALL SUBARRAYS
//
// = Sum of Subarray Maximums - Sum of Subarray Minimums
//
// Naive O(N^2) approach for reference:
// ─────────────────────────────────────────────────────────────────────────────

ll sumMaxMinusMins(const vi& arr) {
    return (sumSubarrayMaxs(arr) - sumSubarrayMins(arr) + MOD) % MOD;
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. BONUS: COUNT OF SUBARRAYS WITH SUM = TOTAL MIN CONTRIBUTION
// (Amazon OA Variant — "Total Cost" problems)
//
// Sometimes OAs ask: "sum of minimums of all subarrays of length >= 2"
// Same approach but start with subarrays of length >= 2:
// contribution for pair (l, r) where r - l >= 1
// ─────────────────────────────────────────────────────────────────────────────

ll sumSubarrayMinsLengthAtLeast2(const vi& arr) {
    int n = arr.size();
    stack<int> stk;
    ll total = 0;

    for (int i = 0; i <= n; i++) {

        while (!stk.empty() && (i==n || arr[stk.top()] >= arr[i])) {
            // Pop equals (avoid duplicates in stack) to prevent double-counting of duplicate elements.
            int j = stk.top();
            stk.pop();

            int left = stk.empty() ? -1 : stk.top();
            int right = i;

            ll leftCount = j - left;
            ll rightCount = right - j;

            // Total subarrays where j is minimum
            ll totalSubarrays = leftCount * rightCount;
            // Minus the single-element subarray [j, j] (length 1)
            ll singleElem = 1;
            ll validCount = totalSubarrays - singleElem;

            total = (total + (ll)arr[j] % MOD * validCount) % MOD;
        }
        if(i < n) stk.push(i);
    }
    return (total + MOD) % MOD;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        int n; cin >> n;
        vi arr(n);
        for (int& x : arr) cin >> x;

        cout << sumSubarrayMins(arr) << '\n';     // LC 907
        cout << sumSubarrayMaxs(arr) << '\n';     // Symmetric variant
        cout << sumMaxMinusMins(arr) << '\n';     // Combined
    */

    return 0;
}
