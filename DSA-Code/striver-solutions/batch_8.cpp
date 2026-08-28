#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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

const ll MOD = 1e9 + 7;

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 8 (Problems 01 to 16)
// ============================================================================

int lengthOfLIS(vi& nums) {
    vi tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return tails.size();
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest strictly increasing subsequence in an array in O(N log N) time.
// - Approach: Binary Search (Patience Sorting / Tails Array).
// - Intuition: `tails[len - 1]` stores the smallest tail of all increasing subsequences of length `len`. For each number `x`, use `lower_bound` to find the first tail >= x and overwrite it with x (making smaller tails to allow future elements to extend the sequence), or append `x` if larger than all tails.
// - Complexity: Time: O(N log N) binary searches, Space: O(N) tails array.

int maxCoins(vi& nums) {
    int n = nums.size();
    vi arr(n + 2, 1);
    for (int i = 0; i < n; i++) arr[i + 1] = nums[i];

    vvi dp(n + 2, vi(n + 2, 0));

    for (int i = n; i >= 1; i--) {
        for (int j = i; j <= n; j++) {
            int max_c = 0;
            for (int k = i; k <= j; k++) {
                int coins = arr[i - 1] * arr[k] * arr[j + 1] + dp[i][k - 1] + dp[k + 1][j];
                max_c = max(max_c, coins);
            }
            dp[i][j] = max_c;
        }
    }

    return dp[1][n];
}
// Interview Explanation:
// - Problem Statement: Find the maximum coins you can collect by bursting balloons, where bursting balloon k gives nums[left] * nums[k] * nums[right].
// - Approach: Matrix Chain Multiplication (MCM) / Partition DP thinking backwards.
// - Intuition: Thinking which balloon to burst *first* introduces cross-boundary dependencies. Thinking which balloon `k` to burst *last* in interval `[i, j]` means boundaries `arr[i-1]` and `arr[j+1]` remain intact to multiply with `arr[k]`, decoupling left subproblem `[i, k-1]` and right subproblem `[k+1, j]`.
// - Complexity: Time: O(N^3), Space: O(N^2) DP table.

class TrieII {
private:
    struct Node {
        Node* links[26];
        int count_ends = 0;
        int count_prefix = 0;
        Node() {
            fill(links, links + 26, nullptr);
        }
    };
    Node* root;

public:
    TrieII() {
        root = new Node();
    }

    void insert(const string& word) {
        Node* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->links[idx]) node->links[idx] = new Node();
            node = node->links[idx];
            node->count_prefix++;
        }
        node->count_ends++;
    }

    int countWordsEqualTo(const string& word) {
        Node* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->links[idx]) return 0;
            node = node->links[idx];
        }
        return node->count_ends;
    }

    int countWordsStartingWith(const string& prefix) {
        Node* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->links[idx]) return 0;
            node = node->links[idx];
        }
        return node->count_prefix;
    }

    void erase(const string& word) {
        Node* node = root;
        for (char c : word) {
            int idx = c - 'a';
            node = node->links[idx];
            node->count_prefix--;
        }
        node->count_ends--;
    }
};
// Interview Explanation:
// - Problem Statement: Implement a Trie (Prefix Tree) supporting word insertion, exact word count, prefix count, and word deletion.
// - Approach: N-ary Trie node with `count_ends` and `count_prefix` frequency counters.
// - Intuition: Augmenting each trie node with `count_prefix` (words passing through) and `count_ends` (words ending here) enables O(L) prefix counting and deletion by decrementing counters along the path.
// - Complexity: Time: O(L) for all operations where L is string length, Space: O(total characters inserted * 26).

class BinaryTrie {
private:
    struct Node {
        Node* links[2] = {nullptr, nullptr};
    };
    Node* root;

public:
    BinaryTrie() { root = new Node(); }

    void insert(int num) {
        Node* node = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!node->links[bit]) node->links[bit] = new Node();
            node = node->links[bit];
        }
    }

    int getMaxXor(int num) {
        Node* node = root;
        int max_xor = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int opposite_bit = 1 - bit;
            if (node->links[opposite_bit]) {
                max_xor |= (1 << i);
                node = node->links[opposite_bit];
            } else {
                node = node->links[bit];
            }
        }
        return max_xor;
    }
};

vi maximizeXor(vi& nums, vvi& queries) {
    int q_size = queries.size();
    vector<vi> offline_q(q_size);
    for (int i = 0; i < q_size; i++) {
        offline_q[i] = {queries[i][1], queries[i][0], i};
    }

    sort(nums.begin(), nums.end());
    sort(offline_q.begin(), offline_q.end());

    BinaryTrie trie;
    vi ans(q_size, -1);
    int idx = 0, n = nums.size();

    for (auto& q : offline_q) {
        int m = q[0], x = q[1], original_idx = q[2];
        while (idx < n && nums[idx] <= m) {
            trie.insert(nums[idx++]);
        }
        if (idx > 0) {
            ans[original_idx] = trie.getMaxXor(x);
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: For each query [x, m], find maximum XOR value of x with any element in nums <= m (return -1 if no element <= m).
// - Approach: Offline Query Sorting + Binary (Bitwise 0/1) Trie.
// - Intuition: Sort both `nums` and queries ascendingly by threshold `m`. Greedily insert only numbers `<= m` into the Bitwise Trie before answering query `(x, m)`. In the Trie, greedily choose the opposite bit at each power of 2 from bit 31 down to 0 to maximize XOR.
// - Complexity: Time: O(N log N + Q log Q + 32 * (N + Q)), Space: O(32 * N) for Bitwise Trie.

int countDistinctSubstrings(string s) {
    struct Node {
        Node* links[26] = {nullptr};
    };

    Node* root = new Node();
    int count = 0;
    int n = s.size();

    for (int i = 0; i < n; i++) {
        Node* node = root;
        for (int j = i; j < n; j++) {
            int idx = s[j] - 'a';
            if (!node->links[idx]) {
                node->links[idx] = new Node();
                count++;
            }
            node = node->links[idx];
        }
    }

    return count + 1;
}
// Interview Explanation:
// - Problem Statement: Count the number of distinct non-empty substrings in a string.
// - Approach: Trie Node Counting (or Suffix Automaton / Suffix Array).
// - Intuition: Every distinct substring in `s` is a prefix of some suffix. By inserting all suffixes of `s` into a Trie, each newly allocated Trie node represents a newly discovered unique substring.
// - Complexity: Time: O(N^2), Space: O(N^2 * 26) Trie nodes.

int countBracketReversals(string s) {
    int n = s.size();
    if (n % 2 != 0) return -1;

    int open_needed = 0, close_needed = 0;
    for (char c : s) {
        if (c == '{') {
            open_needed++;
        } else {
            if (open_needed > 0) open_needed--;
            else close_needed++;
        }
    }

    return (open_needed + 1) / 2 + (close_needed + 1) / 2;
}
// Interview Explanation:
// - Problem Statement: Find minimum reversals of '{' and '}' to make an expression balanced (return -1 if impossible).
// - Approach: Stack / Counter balance simulation.
// - Intuition: Remove all valid balanced pairs `{}`. The remaining string has form `}}...}{{...{` (m closing brackets followed by n opening brackets). To balance, reverse `ceil(m / 2) + ceil(n / 2)` brackets, which equals `(m + 1)/2 + (n + 1)/2`.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int strStrRabinKarp(string text, string pattern) {
    int n = text.size(), m = pattern.size();
    if (m == 0) return 0;
    if (n < m) return -1;

    const ll BASE = 256;
    const ll PRIME_MOD = 1e9 + 7;

    ll pattern_hash = 0, window_hash = 0, power = 1;

    for (int i = 0; i < m - 1; i++) {
        power = (power * BASE) % PRIME_MOD;
    }

    for (int i = 0; i < m; i++) {
        pattern_hash = (pattern_hash * BASE + pattern[i]) % PRIME_MOD;
        window_hash = (window_hash * BASE + text[i]) % PRIME_MOD;
    }

    for (int i = 0; i <= n - m; i++) {
        if (pattern_hash == window_hash) {
            if (text.substr(i, m) == pattern) return i;
        }
        if (i < n - m) {
            window_hash = (BASE * (window_hash - text[i] * power) + text[i + m]) % PRIME_MOD;
            if (window_hash < 0) window_hash += PRIME_MOD;
        }
    }

    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the first occurrence index of pattern string within text string using rolling hashes.
// - Approach: Rabin-Karp Algorithm using polynomial rolling hash with modular arithmetic.
// - Intuition: Compute hash of pattern and first window of text. Slide window in O(1) time by subtracting leading character contribution `(char * BASE^(m-1))` and adding trailing character. On hash match, verify exact characters to safeguard against collisions.
// - Complexity: Time: O(N + M) average, O(N * M) worst case with many collisions, Space: O(1) auxiliary space.

vi computeZFunction(const string& s) {
    int n = s.size();
    vi z(n, 0);
    int l = 0, r = 0;

    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int strStrZAlgorithm(string text, string pattern) {
    if (pattern.empty()) return 0;
    string combined = pattern + "$" + text;
    vi z = computeZFunction(combined);
    int m = pattern.size();

    for (int i = 0; i < (int)z.size(); i++) {
        if (z[i] == m) {
            return i - m - 1;
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Pattern searching in text using linear time Z-algorithm.
// - Approach: Construct combined string `pattern + "$" + text` and compute Z-array using `[L, R]` prefix match segment.
// - Intuition: Maintain current rightmost matched interval `[L, R]`. Inside interval `i <= R`, reuse precomputed values `z[i - L]`; expand beyond `R` only when needed, advancing `R` monotonically to achieve strict linear time.
// - Complexity: Time: O(N + M) strict linear time, Space: O(N + M) for Z-array.

vi computeLPS(const string& pattern) {
    int m = pattern.size();
    vi lps(m, 0);
    int len = 0, i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
    }
    return lps;
}

int strStrKMP(string text, string pattern) {
    if (pattern.empty()) return 0;
    int n = text.size(), m = pattern.size();
    vi lps = computeLPS(pattern);

    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++; j++;
        }
        if (j == m) {
            return i - j;
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1;
}
// Interview Explanation:
// - Problem Statement: Exact pattern matching in string text using the Knuth-Morris-Pratt (KMP) Algorithm in O(N + M) time.
// - Approach: Longest Prefix Suffix (LPS) array preprocessing + 2-pointer text scanning without backtracking pointer `i`.
// - Intuition: When a character mismatch occurs at `pattern[j]`, rather than restarting matching from scratch (`i = i - j + 1`), leverage the LPS array to reset `j = lps[j - 1]`, preserving already matched prefix characters and never backtracking text index `i`.
// - Complexity: Time: O(N + M) single pass over text and pattern, Space: O(M) for LPS array.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (10 to 16)
// ----------------------------------------------------------------------------

int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1, prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Count distinct ways to climb a staircase of n steps taking 1 or 2 steps at a time.
// - Approach: Dynamic Programming / Fibonacci sequence with O(1) space.
// - Intuition: To reach step `n`, you can come from step `n-1` (1 step) or step `n-2` (2 steps). `ways(n) = ways(n-1) + ways(n-2)`.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int coinChange(vi& coins, int amount) {
    vi dp(amount + 1, amount + 1);
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (i - coin >= 0) {
                dp[i] = min(dp[i], 1 + dp[i - coin]);
            }
        }
    }

    return dp[amount] > amount ? -1 : dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find the fewest number of coins needed to make up a given amount (return -1 if impossible).
// - Approach: Bottom-up 1D Dynamic Programming (Unbounded Knapsack).
// - Intuition: `dp[i]` represents min coins for amount `i`. For each amount, test all coins: `dp[i] = min(dp[i], 1 + dp[i - coin])`.
// - Complexity: Time: O(amount * coins.size()), Space: O(amount) DP table.

bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }

    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Determine if string s can be segmented into a space-separated sequence of dictionary words.
// - Approach: 1D Dynamic Programming with Hash Set word lookup.
// - Intuition: `dp[i]` is true if prefix `s[0...i-1]` can be segmented. If `dp[j]` is true and substring `s[j...i-1]` exists in dictionary, set `dp[i] = true`.
// - Complexity: Time: O(N^2 * L) where L is max word length, Space: O(N + total dictionary characters).

int robLinear(const vi& nums, int start, int end) {
    int prev1 = 0, prev2 = 0;
    for (int i = start; i <= end; i++) {
        int curr = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int robII(vi& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
}
// Interview Explanation:
// - Problem Statement: Maximize money robbed from houses arranged in a circle without robbing adjacent houses.
// - Approach: Two passes of House Robber I DP over linear sub-arrays `[0, n-2]` and `[1, n-1]`.
// - Intuition: First and last houses are adjacent and cannot both be robbed. Split the circular problem into two linear problems: rob from `0` to `n-2` (excluding last) and from `1` to `n-1` (excluding first), taking the maximum.
// - Complexity: Time: O(N) two linear passes, Space: O(1) auxiliary space.

int numDecodings(string s) {
    int n = s.size();
    if (n == 0 || s[0] == '0') return 0;

    int prev2 = 1, prev1 = 1;

    for (int i = 1; i < n; i++) {
        int curr = 0;
        if (s[i] != '0') curr += prev1;

        int two_digit = stoi(s.substr(i - 1, 2));
        if (two_digit >= 10 && two_digit <= 26) {
            curr += prev2;
        }

        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
// Interview Explanation:
// - Problem Statement: Find the number of ways to decode an encoded message containing digits mapped 'A'->1 ... 'Z'->26.
// - Approach: 1D Dynamic Programming with state compression (like Fibonacci with validation constraints).
// - Intuition: Single digit `s[i] != '0'` contributes `dp[i-1]`. Valid two digits `10 <= s[i-1..i] <= 26` contributes `dp[i-2]`.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int uniquePaths(int m, int n) {
    vi dp(n, 1);
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[j] += dp[j - 1];
        }
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Find the number of possible unique paths from top-left corner to bottom-right corner of an m x n grid (only moving right or down).
// - Approach: 2D Dynamic Programming with 1D row compression (or Combinatorics C(m+n-2, m-1)).
// - Intuition: To reach cell `(i, j)`, one can only come from top or left. `dp[j] = dp[j] + dp[j-1]`.
// - Complexity: Time: O(M * N), Space: O(N) single row array.

bool canJump(vi& nums) {
    int max_reach = 0, n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i > max_reach) return false;
        max_reach = max(max_reach, i + nums[i]);
        if (max_reach >= n - 1) return true;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if you can reach the last index starting at index 0 given maximum jump lengths at each position.
// - Approach: Greedy Max Reach tracking.
// - Intuition: At each index `i`, if `i > max_reach`, the current position is unreachable; return false. Otherwise, greedily update `max_reach = max(max_reach, i + nums[i])`.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.
