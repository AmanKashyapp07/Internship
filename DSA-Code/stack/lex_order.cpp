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
    stack<char> stk;
    for(char c : num) {
        while (!stk.empty() && k > 0 && stk.top() > c) {
            // Keep equals (do not pop on equal): replacing a digit with an identical copy
            // does not lexicographically improve the number.
            stk.pop();
            k--;
        }
        stk.push(c);
    }
    while (k > 0 && !stk.empty()) {
        stk.pop();
        k--;
    }

    string res;
    while (!stk.empty()) {
        res += stk.top();
        stk.pop();
    }
    reverse(res.begin(), res.end());
    int i = 0;
    while (i < (int)res.size() && res[i] == '0') i++; // Skip leading zeros
    if (i == (int)res.size()) return "0"; // All zeros
    return res.substr(i);
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
    stack<char> stk;

    // Record the last occurrence index of each character
    for (int i = 0; i < (int)s.size(); i++) {
        lastIndex[s[i] - 'a'] = i;
    }

    for (int i = 0; i < (int)s.size(); i++) {
        int c = s[i] - 'a';

        // Skip if this character is already in the result at an earlier position
        if (inStack[c]) continue;

        // Pop characters that are larger AND will appear again later
        while (!stk.empty() && stk.top() > s[i] && lastIndex[stk.top() - 'a'] > i) {
            // Keep equals (do not pop on equal): replacing a character with an identical copy
            // does not lexicographically improve the prefix.
            inStack[stk.top() - 'a'] = false;
            stk.pop();
        }

        stk.push(s[i]);
        inStack[c] = true;
    }

    string result;
    while (!stk.empty()) {
        result += stk.top();
        stk.pop();
    }
    return string(result.rbegin(), result.rend()); // Reverse to get correct order
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
    stack<int> stk; // Monotonic decreasing stack of digits
    int drop = n - k; // How many elements we're allowed to skip/drop
    // we will keep monotonic decreasing stack of size k, so we can drop n-k elements

    for (int i = 0; i < n; i++) {
        // Pop smaller elements from stack if we still have drops left
        while (drop > 0 && !stk.empty() && stk.top() < nums[i]) {
            // Keep equals (do not pop on equal): replacing a digit with an identical copy
            // does not lexicographically improve the number.
            stk.pop();
            drop--;
        }
        stk.push(nums[i]);
    }

    vi result;
    while (!stk.empty()) {
        result.push_back(stk.top());
        stk.pop();
    }
    reverse(result.begin(), result.end());
    // If we have extra elements (more than k), trim from right (they are the smallest due to monotonic stack)
    if (result.size() > k) {
        result.erase(result.begin(), result.begin() + (result.size() - k));
    }
    return result;
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
        while (!stk.empty() && temps[stk.top()] < temps[i]) {
            // Keep equals (do not pop on equal): an equal temperature is not strictly warmer
            // than the stack top, so it cannot resolve the Next Greater query for it.
            int j = stk.top(); // for jth index, i is the next warmer day
            stk.pop();
            result[j] = i - j; // Days to wait for a warmer temperature, as i is the index of the next warmer day, so i-1 - j + 1 = i - j
        }
        stk.push(i);
    }
    return result;
}
