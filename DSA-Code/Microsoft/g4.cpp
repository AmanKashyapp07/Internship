#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 61 | Partition Labels                            | Greedy Last Occurrence Window     | O(N)     | O(1)     |
 | 62 | Remove Duplicate Letters                    | Monotonic Stack + Last Seen Index | O(N)     | O(1)     |
 | 63 | Remove K Digits                             | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 64 | Create Maximum Number                       | Monotonic Stack + Vector Merge    | O(K*(N+M))| O(K)    |
 | 65 | Reorganize String                           | Max-Heap Frequency Pairing        | O(N logA)| O(A)     |
 | 66 | Rearrange String k Distance Apart           | Max-Heap + Wait-Queue Cooldown    | O(N logA)| O(A)     |
 | 67 | Minimum Add to Make Parentheses Valid       | Single-Pass Balance Counter       | O(N)     | O(1)     |
 | 68 | Min Remove to Make Valid Parentheses        | Two-Pass Invalidation Filter      | O(N)     | O(N)     |
 | 69 | Valid Parenthesis String                    | Range of Open Parentheses [lo, hi]| O(N)     | O(1)     |
 | 70 | Remove Invalid Parentheses                  | Backtracking DFS Pruning          | O(2^N)   | O(N)     |
 | 71 | Maximum Swap                                | Last Digit Occurrence Table       | O(N)     | O(1)     |
 | 72 | Split a String in Balanced Strings          | Running Balance Counter           | O(N)     | O(1)     |
 | 73 | Partition Min Beautiful Substrings          | Power of 5 DP / Backtracking      | O(N^2)   | O(N)     |
 | 74 | Smallest Subsequence of Distinct Characters | Monotonic Stack Unique Lexicograph| O(N)     | O(1)     |
 | 75 | Largest Number                              | Custom Lexicographical Sort       | O(N logN)| O(N)     |
 | 76 | Connect Ropes With Minimum Cost             | Min-Heap Huffman Optimal Merge    | O(N logN)| O(N)     |
 | 77 | Kth Largest Element in an Array             | Min-Heap Fixed-Size Window        | O(N logK)| O(K)     |
 | 78 | Last Stone Weight                           | Max-Heap Simulation Collision     | O(N logN)| O(N)     |
 | 79 | Last Stone Weight II                        | 0/1 Knapsack Subset Sum Reduction | O(N * S) | O(S)     |
 | 80 | Furthest Building You Can Reach             | Min-Heap Ladders / Bricks Greed   | O(N logL)| O(L)     |
 ====================================================================================================
*/

// =========================================================
// 61. PARTITION LABELS
// =========================================================

vector<int> partitionLabels(string s) {
    int last[26] = {0};
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        last[s[i] - 'a'] = i;
    }

    vector<int> ans;
    int curMax = 0, start = 0;

    for (int i = 0; i < n; ++i) {
        curMax = max(curMax, last[s[i] - 'a']);
        if (i == curMax) {
            ans.push_back(i - start + 1);
            start = i + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Partition string into maximum number of parts such that each letter appears in at most one part.
// - Approach: Greedy Last Occurrence Window Extension.
// - Intuition:
//   * A character's partition must extend at least to that character's very last occurrence in the string.
//   * Precompute the last index of each character 0..25.
//   * Scan linearly, maintaining curMax = max(curMax, last[s[i]]).
//   * When index i reaches curMax, every character seen so far has completed all its occurrences; record partition size and reset start.
// - Complexity: Time: O(N), Space: O(1) (26 characters).


// =========================================================
// 62. REMOVE DUPLICATE LETTERS
// =========================================================

string removeDuplicateLetters(string s) {
    int last[26] = {0};
    bool inStack[26] = {false};
    int n = s.size();
    for (int i = 0; i < n; ++i) last[s[i] - 'a'] = i;

    string st = "";
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        if (inStack[c]) continue;

        while (!st.empty() && st.back() > s[i] && last[st.back() - 'a'] > i) {
            inStack[st.back() - 'a'] = false;
            st.pop_back();
        }

        st.push_back(s[i]);
        inStack[c] = true;
    }
    return st;
}
// Interview Explanation:
// - Problem Statement: Remove duplicate letters so every letter appears once and result is smallest in lexicographical order.
// - Approach: Monotonic Stack with Last Occurrence Lookup and Visited Array.
// - Intuition:
//   * To minimize lexicographical order, earlier characters should be as small as possible.
//   * When encountering character s[i], if it is already retained in our candidate stack, skip it.
//   * Otherwise, pop characters from stack top that are lexicographically larger than s[i] IF they appear again later (last[c] > i).
//   * Pushing s[i] onto stack maintains monotonic increasing order whenever future duplicates allow.
// - Complexity: Time: O(N), Space: O(1) (alphabet of 26).


// =========================================================
// 63. REMOVE K DIGITS
// =========================================================

string removeKdigits(string num, int k) {
    string st = "";
    for (char c : num) {
        while (!st.empty() && k > 0 && st.back() > c) {
            st.pop_back();
            k--;
        }
        st.push_back(c);
    }

    while (k > 0 && !st.empty()) {
        st.pop_back();
        k--;
    }

    int start = 0;
    while (start < (int)st.size() && st[start] == '0') {
        start++;
    }

    string ans = st.substr(start);
    return ans.empty() ? "0" : ans;
}
// Interview Explanation:
// - Problem Statement: Remove k digits from non-negative integer string num to make the remaining number as small as possible.
// - Approach: Monotonic Increasing Stack.
// - Intuition:
//   * In decimal numbers, higher significant digits (left) dominate the total magnitude.
//   * Whenever a digit is followed by a smaller digit, deleting the larger preceding digit produces a smaller overall value.
//   * Maintain a monotonically increasing character stack; pop when st.back() > current digit while k > 0.
//   * Strip remaining k from the right and trim leading zeros.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 64. CREATE MAXIMUM NUMBER
// =========================================================

vector<int> maxSubsequence(const vector<int>& a, int k) {
    vector<int> st;
    int drop = (int)a.size() - k;
    for (int x : a) {
        while (!st.empty() && drop > 0 && st.back() < x) {
            st.pop_back();
            drop--;
        }
        st.push_back(x);
    }
    st.resize(k);
    return st;
}

vector<int> mergeVectors(vector<int> a, vector<int> b) {
    vector<int> res;
    auto itA = a.begin(), itB = b.begin();
    while (itA != a.end() || itB != b.end()) {
        if (lexicographical_compare(itA, a.end(), itB, b.end())) {
            res.push_back(*itB++);
        } else {
            res.push_back(*itA++);
        }
    }
    return res;
}

vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
    int n = nums1.size(), m = nums2.size();
    vector<int> best;

    for (int i = max(0, k - m); i <= min(k, n); ++i) {
        vector<int> seq1 = maxSubsequence(nums1, i);
        vector<int> seq2 = maxSubsequence(nums2, k - i);
        vector<int> cand = mergeVectors(seq1, seq2);
        if (best.empty() || cand > best) {
            best = cand;
        }
    }
    return best;
}
// Interview Explanation:
// - Problem Statement: Create maximum number of length k from digits of two arrays preserving relative order.
// - Approach: Monotonic Stack Subsequence Extraction + Lexicographical Vector Merge.
// - Intuition:
//   * Decompose into two subproblems:
//     1. Pick largest subsequence of length i from nums1 and length (k - i) from nums2 using a monotonic stack.
//     2. Merge two candidate sequences greedily: compare suffixes lexicographically to decide which vector to pull from.
//   * Iterate over all valid split lengths i in [max(0, k-m), min(k, n)] and retain the globally largest candidate.
// - Complexity: Time: O(K * (N + M)), Space: O(K).


// =========================================================
// 65. REORGANIZE STRING
// =========================================================

string reorganizeString(string s) {
    int cnt[26] = {0};
    int n = s.size();
    for (char c : s) cnt[c - 'a']++;

    priority_queue<pair<int, char>> pq;
    for (int i = 0; i < 26; ++i) {
        if (cnt[i] > (n + 1) / 2) return "";
        if (cnt[i] > 0) pq.push({cnt[i], (char)('a' + i)});
    }

    string ans = "";
    while (pq.size() >= 2) {
        auto [c1, ch1] = pq.top(); pq.pop();
        auto [c2, ch2] = pq.top(); pq.pop();

        ans.push_back(ch1);
        ans.push_back(ch2);

        if (--c1 > 0) pq.push({c1, ch1});
        if (--c2 > 0) pq.push({c2, ch2});
    }

    if (!pq.empty()) {
        ans.push_back(pq.top().second);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Rearrange characters such that no two adjacent characters are identical.
// - Approach: Max-Heap Frequency Pairing.
// - Intuition:
//   * Pigeonhole principle: if any character has count > (n + 1) / 2, valid arrangement is impossible; return "".
//   * Maintain a max-heap of character frequencies.
//   * Greedily pull the two most frequent characters simultaneously and append them in sequence.
//   * Decrement their counts and push back to the heap if remaining count > 0.
//   * Guarantees adjacent elements are distinct.
// - Complexity: Time: O(N log A) where A = 26, Space: O(A) = O(1).


// =========================================================
// 66. REARRANGE STRING K DISTANCE APART
// =========================================================

string rearrangeString(string s, int k) {
    if (k <= 1) return s;

    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    priority_queue<pair<int, char>> pq;
    for (auto& [c, count] : freq) pq.push({count, c});

    queue<pair<int, char>> waitQ;
    string ans = "";

    while (!pq.empty()) {
        auto [count, c] = pq.top();
        pq.pop();

        ans.push_back(c);
        waitQ.push({count - 1, c});

        if ((int)waitQ.size() >= k) {
            auto front = waitQ.front();
            waitQ.pop();
            if (front.first > 0) pq.push(front);
        }
    }

    return ans.size() == s.size() ? ans : "";
}
// Interview Explanation:
// - Problem Statement: Rearrange string such that identical characters are at least distance k apart.
// - Approach: Max-Heap + Cooldown Wait-Queue of Size k.
// - Intuition:
//   * Always prioritize scheduling the most frequent characters first.
//   * When a character is placed, it cannot be reused for the next (k - 1) slots.
//   * Buffer used characters in a cooldown queue of size k.
//   * When the cooldown queue reaches size k, release its front element back into the priority queue if occurrences remain.
//   * If the priority queue empties before all characters are placed, valid placement is impossible.
// - Complexity: Time: O(N log A), Space: O(A).


// =========================================================
// 67. MINIMUM ADD TO MAKE PARENTHESES VALID
// =========================================================

int minAddToMakeValid(string s) {
    int open = 0, add = 0;
    for (char c : s) {
        if (c == '(') {
            open++;
        } else {
            if (open > 0) {
                open--;
            } else {
                add++;
            }
        }
    }
    return add + open;
}
// Interview Explanation:
// - Problem Statement: Find minimum parentheses added to make string valid.
// - Approach: Single-Pass Balance Tracking Counter.
// - Intuition:
//   * On '(': increment available open bracket balance open++.
//   * On ')': if open > 0, match with an available '(' (open--).
//   * If open == 0, the ')' is unmatched and requires adding an opening parenthesis (add++).
//   * At the end, any remaining unclosed '(' count open also requires matching closing brackets.
//   * Total insertions needed is add + open.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 68. MINIMUM REMOVE TO MAKE VALID PARENTHESES
// =========================================================

string minRemoveToMakeValid(string s) {
    stack<int> st;
    vector<bool> remove(s.size(), false);

    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else if (s[i] == ')') {
            if (!st.empty()) {
                st.pop();
            } else {
                remove[i] = true; // Unmatched closing
            }
        }
    }

    while (!st.empty()) {
        remove[st.top()] = true; // Unmatched opening
        st.pop();
    }

    string ans = "";
    for (int i = 0; i < (int)s.size(); ++i) {
        if (!remove[i]) ans.push_back(s[i]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Remove minimum parentheses to make resulting parentheses valid.
// - Approach: Two-Pass Invalidation Filter with Index Stack.
// - Intuition:
//   * A ')' is invalid if encountered when no unmatched '(' exists; mark its index for removal.
//   * Push indices of '(' onto stack; pop on matching ')'.
//   * Any '(' remaining on stack after full traversal has no matching ')' and must be removed.
//   * Filter out marked indices in a second pass.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 69. VALID PARENTHESIS STRING
// =========================================================

bool checkValidString(string s) {
    int lo = 0, hi = 0; // Range of possible open bracket counts

    for (char c : s) {
        if (c == '(') {
            lo++;
            hi++;
        } else if (c == ')') {
            lo--;
            hi--;
        } else {
            // '*' can be ')', empty, or '('
            lo--;
            hi++;
        }

        if (hi < 0) return false;
        lo = max(lo, 0);
    }
    return lo == 0;
}
// Interview Explanation:
// - Problem Statement: Validate parenthesis string containing '(', ')', and '*' (which can act as '(', ')', or empty).
// - Approach: Dynamic Open-Parenthesis Range Tracking [lo, hi].
// - Intuition:
//   * Maintain bounds [lo, hi] representing the minimum and maximum possible count of open '(' brackets.
//   * On '(': both lo and hi increment.
//   * On ')': both lo and hi decrement.
//   * On '*': treating '*' as ')' decreases lo, treating as '(' increases hi.
//   * If hi < 0, even converting every '*' to '(' cannot match excess ')'; return false.
//   * Clamping lo = max(lo, 0) handles cases where '*' acts as empty rather than invalid negative open brackets.
//   * Valid iff 0 falls within the final range (lo == 0).
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 70. REMOVE INVALID PARENTHESES
// =========================================================

class SolutionRemoveInvalidParentheses {
    unordered_set<string> validStrings;

    void dfs(const string& s, int idx, int leftRem, int rightRem, int open, string cur) {
        if (idx == (int)s.size()) {
            if (leftRem == 0 && rightRem == 0 && open == 0) {
                validStrings.insert(cur);
            }
            return;
        }

        char c = s[idx];
        if (c == '(') {
            // Discard '('
            if (leftRem > 0) dfs(s, idx + 1, leftRem - 1, rightRem, open, cur);
            // Keep '('
            dfs(s, idx + 1, leftRem, rightRem, open + 1, cur + c);
        } else if (c == ')') {
            // Discard ')'
            if (rightRem > 0) dfs(s, idx + 1, leftRem, rightRem - 1, open, cur);
            // Keep ')' if valid open bracket available
            if (open > 0) dfs(s, idx + 1, leftRem, rightRem, open - 1, cur + c);
        } else {
            dfs(s, idx + 1, leftRem, rightRem, open, cur + c);
        }
    }

public:
    vector<string> removeInvalidParentheses(string s) {
        int leftRem = 0, rightRem = 0;
        for (char c : s) {
            if (c == '(') leftRem++;
            else if (c == ')') {
                if (leftRem > 0) leftRem--;
                else rightRem++;
            }
        }

        validStrings.clear();
        dfs(s, 0, leftRem, rightRem, 0, "");
        return vector<string>(validStrings.begin(), validStrings.end());
    }
};
// Interview Explanation:
// - Problem Statement: Remove minimum invalid parentheses and return all unique valid results.
// - Approach: Backtracking DFS with Exact Discard Count Pruning.
// - Intuition:
//   * Pass 1: compute the exact count of misplaced '(' (leftRem) and ')' (rightRem).
//   * Launch backtracking DFS tracking running balance open.
//   * Only branch to discard '(' if leftRem > 0; only discard ')' if rightRem > 0.
//   * Only keep ')' if open > 0 to maintain prefix validity.
//   * Deduplicate leaf results using a hash set.
// - Complexity: Time: O(2^N) bounded by exact removals, Space: O(N).


// =========================================================
// 71. MAXIMUM SWAP
// =========================================================

int maximumSwap(int num) {
    string s = to_string(num);
    int last[10] = {0};
    int n = s.size();

    for (int i = 0; i < n; ++i) {
        last[s[i] - '0'] = i;
    }

    for (int i = 0; i < n; ++i) {
        int d = s[i] - '0';
        for (int larger = 9; larger > d; --larger) {
            if (last[larger] > i) {
                swap(s[i], s[last[larger]]);
                return stoi(s);
            }
        }
    }
    return num;
}
// Interview Explanation:
// - Problem Statement: Swap two digits at most once to get maximum possible value.
// - Approach: Last Digit Occurrence Table + Greedy Left-to-Right Scan.
// - Intuition:
//   * Modifying more significant (leftmost) digits provides the greatest value increase.
//   * Precompute the last index of each digit 0..9.
//   * Scan digits from left to right.
//   * For each digit d, check if any larger digit (9 down to d+1) occurs at an index greater than current index i.
//   * Perform the swap with the largest available digit's last occurrence and return immediately.
// - Complexity: Time: O(N) where N <= 9 digits, Space: O(1).


// =========================================================
// 72. SPLIT A STRING IN BALANCED STRINGS
// =========================================================

int balancedStringSplit(string s) {
    int bal = 0, ans = 0;
    for (char c : s) {
        if (c == 'L') bal++;
        else bal--;
        if (bal == 0) ans++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Split string into maximum number of balanced strings ('L' and 'R' counts equal).
// - Approach: Greedy Running Balance Counter.
// - Intuition:
//   * Increment balance for 'L', decrement for 'R'.
//   * Whenever balance resets to 0, a minimal valid balanced substring has completed.
//   * Greedily slicing immediately upon reaching 0 maximizes the total number of valid partitions.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 73. PARTITION STRING INTO MINIMUM BEAUTIFUL SUBSTRINGS
// =========================================================

bool isPowerOfFive(const string& s, int l, int r) {
    if (s[l] == '0') return false; // No leading zeros
    int val = 0;
    for (int i = l; i <= r; ++i) {
        val = (val << 1) + (s[i] - '0');
    }
    while (val > 1 && val % 5 == 0) val /= 5;
    return val == 1;
}

int minimumBeautifulSubstrings(string s) {
    int n = s.size();
    vector<int> dp(n + 1, 1e9);
    dp[0] = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (dp[j] != 1e9 && isPowerOfFive(s, j, i - 1)) {
                dp[i] = min(dp[i], dp[j] + 1);
            }
        }
    }
    return dp[n] >= 1e9 ? -1 : dp[n];
}
// Interview Explanation:
// - Problem Statement: Partition binary string into minimum substrings representing powers of 5 without leading zeros.
// - Approach: 1D Dynamic Programming over Prefix Partitions.
// - Intuition:
//   * Powers of 5 in binary are: 1 (1), 5 (101), 25 (11001), 125 (1111101), etc.
//   * Let dp[i] be the minimum partitions for prefix s[0..i-1].
//   * For each split point j < i, check if substring s[j..i-1] has no leading zero and is a power of 5.
//   * Update dp[i] = min(dp[i], dp[j] + 1).
// - Complexity: Time: O(N^2), Space: O(N).


// =========================================================
// 74. SMALLEST SUBSEQUENCE OF DISTINCT CHARACTERS
// =========================================================

string smallestSubsequence(string s) {
    int last[26] = {0};
    bool inStack[26] = {false};
    int n = s.size();
    for (int i = 0; i < n; ++i) last[s[i] - 'a'] = i;

    string st = "";
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        if (inStack[c]) continue;

        while (!st.empty() && st.back() > s[i] && last[st.back() - 'a'] > i) {
            inStack[st.back() - 'a'] = false;
            st.pop_back();
        }

        st.push_back(s[i]);
        inStack[c] = true;
    }
    return st;
}
// Interview Explanation:
// - Problem Statement: Return smallest subsequence containing each distinct character of s exactly once.
// - Approach: Monotonic Stack with Rightmost Character Occurrences.
// - Intuition:
//   * Build monotonic increasing result stack.
//   * When encountering character c not yet in stack:
//   * Pop characters from stack top that are strictly larger than c IF they appear again later in the string.
//   * Appending c guarantees the smallest lexicographical choice while retaining all unique characters.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 75. LARGEST NUMBER
// =========================================================

bool compareLargestNum(const string& a, const string& b) {
    return (a + b) > (b + a);
}

string largestNumber(vector<int>& a) {
    vector<string> s;
    for (int x : a) s.push_back(to_string(x));
    sort(s.begin(), s.end(), compareLargestNum);

    if (s[0] == "0") return "0";

    string ans = "";
    for (const string& str : s) ans += str;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Arrange list of non-negative integers such that they form the largest number.
// - Approach: Custom Lexicographical String Sorting.
// - Intuition:
//   * Standard integer or single-string comparison fails (e.g., "3" vs "30": "330" > "303").
//   * Define custom comparator: string a precedes b iff (a + b) > (b + a).
//   * This relation is transitive and defines a strict weak ordering.
//   * Sort strings under this comparator and concatenate.
//   * Handle edge case where highest element is "0" (return "0").
// - Complexity: Time: O(N log N * L), Space: O(N * L).


// =========================================================
// 76. CONNECT ROPES WITH MINIMUM COST
// =========================================================

long long minCostToConnectRopes(vector<long long>& a) {
    priority_queue<long long, vector<long long>, greater<long long>> pq(a.begin(), a.end());
    long long totalCost = 0;

    while (pq.size() > 1) {
        long long first = pq.top(); pq.pop();
        long long second = pq.top(); pq.pop();
        long long combined = first + second;
        totalCost += combined;
        pq.push(combined);
    }
    return totalCost;
}
// Interview Explanation:
// - Problem Statement: Connect n ropes with minimum total cost where cost to connect two ropes is sum of their lengths.
// - Approach: Min-Heap Greedy Huffman Optimal Merge Pattern.
// - Intuition:
//   * Ropes combined earlier contribute to multiple subsequent additions in the merge tree.
//   * To minimize total cost, shorter ropes should participate in more merges, while longer ropes should participate in fewer.
//   * Push all lengths into a min-heap.
//   * Repeatedly extract the two shortest ropes, combine them, accumulate cost, and push the combined rope back.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 77. KTH LARGEST ELEMENT IN AN ARRAY
// =========================================================

int findKthLargest(vector<int>& a, int k) {
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of size k
    for (int x : a) {
        pq.push(x);
        if ((int)pq.size() > k) {
            pq.pop();
        }
    }
    return pq.top();
}
// Interview Explanation:
// - Problem Statement: Find k-th largest element in an unsorted array.
// - Approach: Min-Heap Fixed-Size Window of Size k.
// - Intuition:
//   * Maintain the k largest elements seen so far in a min-heap.
//   * The top of the min-heap always holds the smallest among the k largest elements.
//   * When heap size exceeds k, evict pq.top().
//   * At the end of traversal, pq.top() is precisely the k-th largest element in the entire array.
// - Complexity: Time: O(N log K), Space: O(K).


// =========================================================
// 78. LAST STONE WEIGHT
// =========================================================

int lastStoneWeight(vector<int>& stones) {
    priority_queue<int> pq(stones.begin(), stones.end());

    while (pq.size() > 1) {
        int y = pq.top(); pq.pop();
        int x = pq.top(); pq.pop();
        if (y != x) {
            pq.push(y - x);
        }
    }
    return pq.empty() ? 0 : pq.top();
}
// Interview Explanation:
// - Problem Statement: Repeatedly smash the two heaviest stones (y >= x) leaving (y - x) if y != x; return remaining stone weight.
// - Approach: Max-Heap Collision Simulation.
// - Intuition:
//   * The problem dictates greedily choosing the two heaviest stones in every round.
//   * A max-heap retrieves the two largest elements in O(log N) per smash.
//   * If y > x, reinsert y - x; if equal, both are destroyed.
//   * Repeat until at most 1 stone remains.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 79. LAST STONE WEIGHT II
// =========================================================

int lastStoneWeightII(vector<int>& stones) {
    int total = accumulate(stones.begin(), stones.end(), 0);
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int s : stones) {
        for (int i = target; i >= s; --i) {
            dp[i] = dp[i] || dp[i - s];
        }
    }

    for (int i = target; i >= 0; --i) {
        if (dp[i]) {
            return total - 2 * i;
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Minimize remaining stone weight after arbitrary smash orders.
// - Approach: 0/1 Knapsack Subset Sum Mathematical Reduction.
// - Intuition:
//   * Any sequence of stone smashes assigns '+' or '-' signs to each stone weight: sum(S1) - sum(S2).
//   * Minimizing the difference is equivalent to finding a subset S1 whose sum is as close to total / 2 as possible.
//   * Use 0/1 Knapsack boolean DP with target = total / 2.
//   * Best achievable subset sum i <= total / 2 yields minimal final weight (total - 2*i).
// - Complexity: Time: O(N * (Total / 2)), Space: O(Total / 2).


// =========================================================
// 80. FURTHEST BUILDING YOU CAN REACH
// =========================================================

int furthestBuilding(vector<int>& h, int bricks, int ladders) {
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of ladder climbs

    for (int i = 0; i < (int)h.size() - 1; ++i) {
        int diff = h[i + 1] - h[i];
        if (diff > 0) {
            pq.push(diff);
            if ((int)pq.size() > ladders) {
                bricks -= pq.top();
                pq.pop();
            }
            if (bricks < 0) {
                return i;
            }
        }
    }
    return (int)h.size() - 1;
}
// Interview Explanation:
// - Problem Statement: Find furthest building reachable using limited bricks and ladders.
// - Approach: Min-Heap Greedy Allocation (Ladders for Largest Climbs, Bricks for Smallest).
// - Intuition:
//   * Ladders can cover any height difference regardless of magnitude; bricks scale linearly with height.
//   * Therefore, ladders should greedily be reserved for the largest climbs encountered.
//   * Tentatively assign ladders to all climbs by pushing climb heights into a min-heap.
//   * If ladder count is exceeded, convert the smallest climb seen so far into a brick expenditure (pq.top()).
//   * If required bricks exceed available supply, no further progress is possible; return building index i.
// - Complexity: Time: O(N log(Ladders)), Space: O(Ladders).
