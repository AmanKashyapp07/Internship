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
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int MOD = 1e9 + 7;
const int INF = INT_MAX;



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
// when min -> use increasing stack (arr[stack.top()] >= arr[i])
// when max -> use decreasing stack (arr[stack.top()] <= arr[i])
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
            if(left < k && k < right) { // if k is in the range of left and right, then we can take this subarray
                maxi = max(maxi, (ll)nums[j] * (right - left - 1)); // (ll) cast to avoid overflow, use this always in OAs
            }
        }
        if(i < n) stk.push(i); // Push current index onto stack
    }
    return maxi;
} // problem statement is to find the maximum score of a subarray that contains the index k, where the score is defined as the minimum value in the subarray multiplied by the length of the subarray. The approach is to use a monotonic increasing stack to find the previous smaller element (PSE) and next smaller element (NSE) for each index, and then calculate the maximum score based on these boundaries.
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}
