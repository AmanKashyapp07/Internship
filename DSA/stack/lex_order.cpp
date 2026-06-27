/**
 * Monotonic Stack - Lexicographic Optimization
 *
 * Using a monotonic stack to build the lexicographically smallest (or largest)
 * result under constraints. Core OA pattern for string/array manipulation.
 *
 * 1. Remove K Digits              - LC 402 (lexicographically smallest number)
 * 2. Remove Duplicate Letters     - LC 316 (lexicographically smallest with all unique chars)
 * 3. Create Maximum Number        - LC 321 (merge two arrays for max number)
 * 4. 132 Pattern                  - LC 456 (existence check with monotonic stack + min)
 *
 * Core Idea:
 * - Maintain a monotonic increasing stack for smallest result.
 * - Pop elements from the top if the incoming element is smaller AND we still
 *   have "removals" left (or the character can appear again later).
 * - This greedy ensures the prefix is always as small as possible.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;
using vi = vector<int>;

// ─────────────────────────────────────────────────────────────────────────────
// 1. REMOVE K DIGITS
// LC 402
//
// Remove k digits from a number string to make it lexicographically smallest.
//
// Idea:
// - Use a monotonic increasing stack (build the smallest possible prefix).
// - For each digit, pop larger digits from the stack while k > 0.
//   (A larger digit earlier makes the number larger — remove it.)
// - If k > 0 after processing all digits, remove the last k digits (they're
//   already in sorted order; the tail is the largest part to cut).
// - Strip leading zeros.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

string removeKDigits(string num, int k) {
    string stk; // Acts as our monotonic stack (string = efficient char stack)

    for (char c : num) {
        // Pop digits larger than current while we still have removals left
        while (k > 0 && !stk.empty() && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }

    // If removals remain, cut from the end (number is now non-decreasing)
    stk.resize(stk.size() - k);

    // Remove leading zeros
    int start = 0;
    while (start < (int)stk.size() - 1 && stk[start] == '0') {
        start++;
    }

    return stk.substr(start);
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. REMOVE DUPLICATE LETTERS
// LC 316 (same as LC 1081: Smallest Subsequence of Distinct Characters)
//
// Remove duplicates so each letter appears once.
// Result must be the lexicographically smallest possible subsequence.
//
// Idea:
// - Precompute lastIndex[c] = last occurrence of character c.
// - Use a monotonic increasing stack.
// - For each character c:
//     - Skip if already in the stack (it's already placed at an optimal position).
//     - Pop characters larger than c from the stack, BUT ONLY IF they appear
//       again later (lastIndex[top] > i). If they don't appear later, we must keep them.
//
// Time: O(N) | Space: O(1) (at most 26 chars on stack)
// ─────────────────────────────────────────────────────────────────────────────

string removeDuplicateLetters(const string& s) {
    vector<int> lastIndex(26, -1);
    vector<bool> inStack(26, false);
    string stk;

    // Record the last occurrence index of each character
    for (int i = 0; i < (int)s.size(); i++) {
        lastIndex[s[i] - 'a'] = i;
    }

    for (int i = 0; i < (int)s.size(); i++) {
        int c = s[i] - 'a';

        // Skip if this character is already in the result at an earlier position
        if (inStack[c]) continue;

        // Pop characters that are larger AND will appear again later
        while (!stk.empty() && stk.back() > s[i] && lastIndex[stk.back() - 'a'] > i) {
            inStack[stk.back() - 'a'] = false;
            stk.pop_back();
        }

        stk.push_back(s[i]);
        inStack[c] = true;
    }

    return stk;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. MAXIMUM NUMBER FROM ARRAY (Single array version)
//    Given array of digits, pick k of them in order to form the largest number.
//
// Idea:
// - Greedily build a monotonic decreasing stack of size k.
// - Pop smaller digits while we can (n - i elements remaining can fill the rest).
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi maxNumberFromArray(const vi& nums, int k) {
    int n = nums.size();
    vi stk;
    int drop = n - k; // How many elements we're allowed to skip/drop

    for (int i = 0; i < n; i++) {
        // Pop smaller elements from stack if we still have drops left
        while (drop > 0 && !stk.empty() && stk.back() < nums[i]) {
            stk.pop_back();
            drop--;
        }
        stk.push_back(nums[i]);
    }

    // Take only the first k elements
    return vi(stk.begin(), stk.begin() + k);
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. 132 PATTERN
// LC 456
//
// Return true if there exist indices i < j < k such that nums[i] < nums[k] < nums[j].
// (i.e., the "132" pattern: a small number, then a large number, then a medium number.)
//
// Idea:
// - Process from RIGHT to LEFT.
// - Maintain a monotonic decreasing stack.
// - Track `third` = the largest value popped so far (this is the "2" in 132 pattern —
//   it was previously a valid "peak" that something smaller came after).
// - For each element from the right:
//     - Pop all stack elements smaller than current (they become candidates for "2").
//     - Update third = max popped.
//     - If current element < third, we found: current = "1", stack_top = "3", third = "2".
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

bool find132Pattern(const vi& nums) {
    stack<int> stk;        // Monotonic decreasing stack (candidates for "3" in 132)
    int third = INT_MIN;   // The "2" in 132 — largest element that was once a peak

    // Traverse right to left
    for (int i = nums.size() - 1; i >= 0; i--) {
        // If current < third, we found the "1" — pattern exists
        if (nums[i] < third) return true;

        // Pop all elements smaller than current into `third`
        // They were previously "peaks" (the "3"), current now surpasses them
        while (!stk.empty() && nums[i] > stk.top()) {
            third = stk.top(); // Best candidate for "2" seen so far from the right
            stk.pop();
        }

        stk.push(nums[i]);
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. DAILY TEMPERATURES
// LC 739
//
// For each day, find how many days until a warmer temperature.
// This is a direct application of Next Greater Element with index distances.
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

vi dailyTemperatures(const vi& temps) {
    int n = temps.size();
    vi result(n, 0); // 0 means no warmer day found
    stack<int> stk;  // Monotonic decreasing stack of indices

    for (int i = 0; i < n; i++) {
        while (!stk.empty() && temps[i] > temps[stk.top()]) {
            int j = stk.top();
            stk.pop();
            result[j] = i - j; // Days to wait for a warmer temperature
        }
        stk.push(i);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        // Remove K Digits
        string num; int k;
        cin >> num >> k;
        cout << removeKDigits(num, k) << '\n';
    */

    /*
        // Remove Duplicate Letters
        string s; cin >> s;
        cout << removeDuplicateLetters(s) << '\n';
    */

    /*
        // Max Number From Array (pick k elements in order)
        int n, k; cin >> n >> k;
        vi nums(n);
        for (int& x : nums) cin >> x;
        vi res = maxNumberFromArray(nums, k);
        for (int x : res) cout << x << ' '; cout << '\n';
    */

    /*
        // 132 Pattern
        int n; cin >> n;
        vi nums(n);
        for (int& x : nums) cin >> x;
        cout << (find132Pattern(nums) ? "true" : "false") << '\n';
    */

    /*
        // Daily Temperatures
        int n; cin >> n;
        vi temps(n);
        for (int& x : temps) cin >> x;
        vi ans = dailyTemperatures(temps);
        for (int x : ans) cout << x << ' '; cout << '\n';
    */

    return 0;
}
