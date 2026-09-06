/**
 * ====================================================================================================
 *                 ULTIMATE STRING ALGORITHMS & HASHING MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Polynomial Rolling Hash (FastHash), KMP Algorithm,
 * Z-Algorithm, Manacher's Algorithm, Booth's Algorithm, Rabin-Karp, Minimum Window Substring,
 * Longest Substring Without Repeating Characters, Group Anagrams, Trie Blueprint,
 * Palindromic Decompositions, Distinct Subsequences DP, and Multi-Pattern String Segmentation.
 */

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
using vll = vector<ll>;

const ll MOD = 1e9 + 7;
const ll P = 31;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | FastHash Infrastructure                     | Polynomial Prefix Rolling Hash    | O(N)+O(1)| O(N)     |
 | 2  | Rabin-Karp String Matching                  | Rolling Hash Substring Match      | O(N + M) | O(N)     |
 | 3  | Finding Borders (Prefix == Suffix)          | Hash Verification of Pref/Suff    | O(N)     | O(N)     |
 | 4  | Finding String Periods                      | Rolling Hash Substring Periodicity| O(N)     | O(N)     |
 | 5  | O(1) Palindrome Range Queries               | Forward & Backward Hash Arrays    | O(N)+O(1)| O(N)     |
 | 6  | Minimum Window Substring (LC 76)            | Sliding Window + Match Counter    | O(S + T) | O(1)     |
 | 7  | Longest Substring Without Repeating (LC 3)  | Sliding Window + Direct Array Map | O(N)     | O(1)     |
 | 8  | Group Anagrams (LC 49)                      | Sorted String / Count Hash Bucket | O(N KlogK| O(N * K) |
 | 9  | Trie Prefix Tree (LC 208)                   | Array-Based Trie Node Traversal   | O(L) per | O(Sigma*N|
 | 10 | KMP Algorithm (Prefix Function Pi)          | Failure Table State Transitions   | O(N)     | O(N)     |
 | 11 | Z-Algorithm (Longest Common Prefix)         | Sliding Box [L, R] Segment Match  | O(N)     | O(N)     |
 | 12 | Manacher's Algorithm (Palindromic Radii)    | Symmetry Center & Boundary Jump   | O(N)     | O(N)     |
 | 13 | Longest Palindromic Substring (LC 5)        | Expand Around Centers (2N - 1)    | O(N^2)   | O(1)     |
 | 14 | Booth's Algorithm (Min String Rotation)     | Modified KMP on Doubled (S + S)   | O(N)     | O(N)     |
 | 15 | Shortest Palindrome (LC 214)                | KMP Pi Table on (S + '#' + revS)  | O(N)     | O(N)     |
 | 16 | Word Break Counting (CSES / LC 139)         | 1D DP + Trie Multi-Pattern Match  | O(N * L) | O(TotalL)|
 | 17 | Substring Concatenation All Words (LC 30)   | Multi-Offset Fixed Window HashMap | O(N * K) | O(Words) |
 | 18 | Isomorphic Strings (LC 205)                 | Dual 256-ASCII Mapping Arrays     | O(N)     | O(1)     |
 | 19 | Reverse Words in a String (LC 151)          | Word Tokenization & Suffix Build  | O(N)     | O(N)     |
 | 20 | Roman to Integer (LC 13)                    | Right-to-Left Value Subtraction   | O(N)     | O(1)     |
 | 21 | String to Integer / ATOI (LC 8)             | Whitespace Strip + Overflow Clamp | O(N)     | O(1)     |
 | 22 | Longest Common Prefix (LC 14)               | Horizontal / Vertical Scan Match  | O(N * L) | O(1)     |
 | 23 | Min Insertions for Palindrome (LC 1312)     | |S| - LCS(S, reverse(S)) DP       | O(N^2)   | O(N^2)   |
 | 24 | Count and Say (LC 38)                       | Run-Length Encoding Simulation    | O(2^N)   | O(2^N)   |
 | 25 | Compare Version Numbers (LC 165)            | Dot-Separated Numerical Parsing   | O(N + M) | O(1)     |
 | 26 | Encode and Decode Strings (LC 271)          | Length-Prefixed Framing (<len>#<str>)| O(N)   | O(1)     |
 | 27 | Repeating Substring (CSES 2106 / LC 1062)   | Binary Search + Double Rolling Hash| O(N log^2N)| O(N)  |
 | 28 | All Palindromic Endpoints (CSES 3138)       | Manacher Radii + Endpoint Jump     | O(N)     | O(N)     |
 | 29 | Distinct Subsequences II (CSES 1149 / LC 940)| 1D DP + Last Occurrence Deduplication| O(N)   | O(N + S) |
 | 30 | Dynamic Palindrome Queries (CSES 2420)      | Dual Fenwick Trees + Rolling Hash  | O((N+Q)lgN| O(N)    |
 | 31 | String Functions: KMP Pi & Z-Array (CSES 2107)| Dual Prefix-Function & LCP Arrays | O(N)     | O(N)     |
 | 32 | Valid Palindrome II (LC 680)                | 2-Pointer Greedy Mismatch Check    | O(N)     | O(1)     |
 ====================================================================================================
*/


// ====================================================================================================
// SECTION 1: POLYNOMIAL ROLLING HASH INFRASTRUCTURE (FASTHASH)
// ====================================================================================================

struct FastHash {
    vll power, pref;
    FastHash(const string &s) {
        int n = s.size();
        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    // Returns polynomial rolling hash of substring s[l..r] in O(1) time
    ll get(int l, int r) const {
        return (pref[r + 1] - pref[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }

    // Hash of a standalone string in O(length)
    static ll hashWord(const string &s) {
        ll val = 0;
        for (char c : s) val = (val * P + (c - 'a' + 1)) % MOD;
        return val;
    }
};
// Interview Explanation:
// - Problem Statement: Compute rolling polynomial hashes allowing O(1) substring hash queries for string comparison.
// - Approach: Precomputed prefix hash array and base power powers of P modulo 10^9+7.
// - Intuition: Substring hash s[l..r] = (pref[r+1] - pref[l] * P^{r-l+1}) % MOD.
// - Complexity: Time: O(N) precomputation, O(1) range query, Space: O(N).

bool isPalindromeRange(const string &s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) return false;
        l++; r--;
    }
    return true;
}

bool isPalindromeAfterOneDeletion(const string &s) {
    int n = s.size();
    int l = 0, r = n - 1;
    while (l < r) {
        if (s[l] != s[r]) {
            return isPalindromeRange(s, l + 1, r) || isPalindromeRange(s, l, r - 1);
        }
        l++; r--;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Check if string can be a palindrome after deleting at most one character (LeetCode 680).
// - Approach: Two Pointers inward convergence with single mismatch branch.
// - Intuition: On first mismatched characters s[l] != s[r], the string is valid iff either s[l+1..r] or s[l..r-1] is a palindrome.
// - Complexity: Time: O(N), Space: O(1).


// 1. String Pattern Matching Occurrences (Rabin-Karp)
int countPatternOccurrences(const string &text, const string &pattern) {
    int n = text.size(), m = pattern.size();
    if (m > n) return 0;
    FastHash H(text);
    ll target = FastHash::hashWord(pattern);
    int count = 0;
    for (int i = 0; i <= n - m; i++) {
        if (H.get(i, i + m - 1) == target) count++;
    }
    return count;
}

// 2. Finding Borders (Prefix == Suffix)
vi findingBorders(const string &s) {
    int n = s.size();
    FastHash H(s);
    vi borders;
    for (int k = 1; k < n; k++) {
        if (H.get(0, k - 1) == H.get(n - k, n - 1)) {
            borders.push_back(k);
        }
    }
    return borders;
}

// 3. Finding String Periods
vi findingPeriods(const string &s) {
    int n = s.size();
    FastHash H(s);
    vi periods;
    for (int k = 1; k <= n; k++) {
        if (k == n) { periods.push_back(n); continue; }
        int overlap = n - k;
        if (H.get(0, overlap - 1) == H.get(k, n - 1)) {
            periods.push_back(k);
        }
    }
    return periods;
}

// 4. O(1) Palindrome Queries
vector<bool> palindromeQueries(const string &s, const vector<pii> &queries) {
    int n = s.size();
    string rev = s;
    reverse(rev.begin(), rev.end());
    FastHash H1(s), H2(rev);
    vector<bool> ans;
    for (auto &[l, r] : queries) {
        ll fwd = H1.get(l, r);
        ll bwd = H2.get(n - 1 - r, n - 1 - l);
        ans.push_back(fwd == bwd);
    }
    return ans;
}


// ====================================================================================================
// SECTION 2: SLIDING WINDOW & ANAGRAM HASHING PRIMITIVES
// ====================================================================================================

// 1. Minimum Window Substring (LeetCode 76)
string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";
    vi target(128, 0);
    for (char c : t) target[c]++;
    int required = 0;
    for (int count : target) if (count > 0) required++;

    vi window(128, 0);
    int formed = 0, l = 0, minLen = INT_MAX, startIdx = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        char c = s[r];
        window[c]++;
        if (target[c] > 0 && window[c] == target[c]) formed++;

        while (l <= r && formed == required) {
            if (r - l + 1 < minLen) {
                minLen = r - l + 1;
                startIdx = l;
            }
            char leftChar = s[l];
            window[leftChar]--;
            if (target[leftChar] > 0 && window[leftChar] < target[leftChar]) formed--;
            l++;
        }
    }
    return minLen == INT_MAX ? "" : s.substr(startIdx, minLen);
}
// Interview Explanation:
// - Problem Statement: Find the minimum window substring of s containing all characters of t in O(N) time (LeetCode 76).
// - Approach: Sliding Window with frequency table matching.
// - Intuition: Expand right pointer until window contains all required characters; contract left pointer to minimize window size while maintaining validity.
// - Complexity: Time: O(N), Space: O(1) 128-element ASCII table.


// 2. Longest Substring Without Repeating Characters (LeetCode 3)
int lengthOfLongestSubstring(string s) {
    vi lastPos(256, -1);
    int maxLen = 0, l = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        if (lastPos[(unsigned char)s[r]] >= l) {
            l = lastPos[(unsigned char)s[r]] + 1; // Jump left pointer past previous duplicate
        }
        lastPos[(unsigned char)s[r]] = r;
        maxLen = max(maxLen, r - l + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring without repeating characters (LeetCode 3).
// - Approach: Sliding Window with last-seen character position map.
// - Intuition: Encountering a duplicate at `lastPos[c] >= l` lets us immediately jump `l = lastPos[c] + 1` in O(1).
// - Complexity: Time: O(N), Space: O(1).


// 3. Group Anagrams (LeetCode 49)
vector<vector<string>> groupAnagrams(vector<string> &strs) {
    unordered_map<string, vector<string>> groups;
    for (const string &s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto &[key, group] : groups) result.push_back(group);
    return result;
}
// Interview Explanation:
// - Problem Statement: Group string array elements into subsets of anagrams (LeetCode 49).
// - Approach: Sorted string key hashing.
// - Intuition: Anagrams yield identical sorted string keys; group matching keys in a hash map.
// - Complexity: Time: O(N \cdot L \log L), Space: O(N \cdot L).


// 4. TRIE CLASS BLUEPRINT (Prefix Tree - LeetCode 208)
class Trie {
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() {
            isEnd = false;
            for (int i = 0; i < 26; i++) children[i] = nullptr;
        }
    };
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }

    void insert(const string &word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->isEnd = true;
    }

    bool search(const string &word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr->isEnd;
    }

    bool startsWith(const string &prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Implement a Prefix Tree (Trie) supporting insert, exact search, and prefix matching (LeetCode 208).
// - Approach: 26-ary tree node structure.
// - Intuition: Shared prefix paths allow checking prefix existence and word insertion in O(L) time where L is word length.
// - Complexity: Time: O(L) per operation, Space: O(\sum L \cdot 26).


// ====================================================================================================
// SECTION 3: CORE STRING MATCHING & PATTERN RECOGNITION
// ====================================================================================================

// 1. KMP Algorithm (Prefix Function / Failure Table)
vi buildPiArray(const string &s) {
    int n = s.size();
    vi pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

vi kmpSearch(const string &text, const string &pattern) {
    string concat = pattern + "#" + text;
    vi pi = buildPiArray(concat);
    vi matches;
    int m = pattern.size();
    for (size_t i = m + 1; i < concat.size(); i++) {
        if (pi[i] == m) {
            matches.push_back(i - 2 * m);
        }
    }
    return matches;
}

// 2. Z-Algorithm (Longest Common Prefix Array)
vi zArray(const string &s) {
    int n = s.size();
    vi z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// 3. Manacher's Algorithm (O(N) Palindromic Radii)
pair<vi, vi> manacher(const string &s) {
    int n = s.size();
    vi d1(n, 0), d2(n, 0);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) { l = i - k; r = i + k; }
    }
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) { l = i - k - 1; r = i + k; }
    }
    return {d1, d2};
}

// 4. Longest Palindromic Substring (LeetCode 5 - Expand Around Center)
void expandAroundCenter(const string& s, int l, int r, int& start, int& maxLen) {
    while (l >= 0 && r < (int)s.size() && s[l] == s[r]) { l--; r++; }
    if (r - l - 1 > maxLen) {
        start = l + 1;
        maxLen = r - l - 1;
    }
}

string longestPalindromeSubstring(string s) {
    if (s.empty()) return "";
    int start = 0, maxLen = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        expandAroundCenter(s, i, i, start, maxLen);     // Odd length center
        expandAroundCenter(s, i, i + 1, start, maxLen); // Even length center
    }
    return s.substr(start, maxLen);
}

// 5. Booth's Algorithm (Smallest Circular String Rotation)
string boothsAlgorithm(string s) {
    int n = s.size();
    s += s;
    vi f(2 * n, -1);
    int k = 0;
    for (int j = 1; j < 2 * n; j++) {
        char sj = s[j];
        int i = f[j - k - 1];
        while (i != -1 && sj != s[k + i + 1]) {
            if (sj < s[k + i + 1]) k = j - i - 1;
            i = f[i];
        }
        if (sj != s[k + i + 1]) {
            if (sj < s[k]) k = j;
            f[j - k] = -1;
        } else {
            f[j - k] = i + 1;
        }
    }
    return s.substr(k, n);
}


// ====================================================================================================
// SECTION 4: PALINDROMIC DECOMPOSITIONS & SEGMENTATION DP
// ====================================================================================================

// 1. Shortest Palindrome (LeetCode 214)
string shortestPalindrome(string s) {
    if (s.empty()) return s;
    string rev = s;
    reverse(rev.begin(), rev.end());
    FastHash fwd(s), bwd(rev);
    int n = s.size(), bestLen = 0;
    for (int i = 0; i < n; i++) {
        if (fwd.get(0, i) == bwd.get(n - 1 - i, n - 1)) {
            bestLen = i + 1;
        }
    }
    string suffix = s.substr(bestLen);
    reverse(suffix.begin(), suffix.end());
    return suffix + s;
}

// 2. CSES Word Combinations / Word Break Counting (LeetCode 139 variant)
ll wordCombinations(const string &s, const vector<string> &wordDict) {
    int n = s.size();
    FastHash H(s);
    unordered_set<ll> dictHashes;
    unordered_set<int> uniqueLens;
    for (const auto &w : wordDict) {
        dictHashes.insert(FastHash::hashWord(w));
        uniqueLens.insert(w.size());
    }
    vll dp(n + 1, 0);
    dp[n] = 1;
    for (int i = n - 1; i >= 0; i--) {
        for (int len : uniqueLens) {
            if (i + len <= n && dictHashes.count(H.get(i, i + len - 1))) {
                dp[i] = (dp[i] + dp[i + len]) % MOD;
            }
        }
    }
    return dp[0];
}

// 3. Substring with Concatenation of All Words (LeetCode 30)
vi findSubstringConcatenation(string s, vector<string> &words) {
    if (s.empty() || words.empty()) return {};
    int n = s.size(), k = words[0].size(), m = words.size(), totalLen = k * m;
    if (n < totalLen) return {};

    FastHash H(s);
    unordered_map<ll, int> wordCounts;
    for (const auto &w : words) wordCounts[FastHash::hashWord(w)]++;

    vi ans;
    for (int offset = 0; offset < k; offset++) {
        unordered_map<ll, int> windowCounts;
        int left = offset;
        for (int right = offset; right + k <= n; right += k) {
            ll hashVal = H.get(right, right + k - 1);
            windowCounts[hashVal]++;
            while (windowCounts[hashVal] > wordCounts[hashVal]) {
                windowCounts[H.get(left, left + k - 1)]--;
                left += k;
            }
            if (right - left + k == totalLen) ans.push_back(left);
        }
    }
    return ans;
}

// 4. Isomorphic Strings (LeetCode 205)
bool isIsomorphic(string s, string t) {
    if (s.size() != t.size()) return false;
    vi m1(256, -1), m2(256, -1);
    for (size_t i = 0; i < s.size(); i++) {
        if (m1[(unsigned char)s[i]] != m2[(unsigned char)t[i]]) return false;
        m1[(unsigned char)s[i]] = i;
        m2[(unsigned char)t[i]] = i;
    }
    return true;
}


// ====================================================================================================
// SECTION 5: STRIVER SDE SHEET CLASSIC STRING ALGORITHMS
// ====================================================================================================

// 1. Reverse Words in a String (LeetCode 151)
string reverseWords(string s) {
    reverse(s.begin(), s.end());
    int n = s.size(), idx = 0;

    for (int start = 0; start < n; start++) {
        if (s[start] != ' ') {
            if (idx != 0) s[idx++] = ' ';
            int end = start;
            while (end < n && s[end] != ' ') end++;
            reverse(s.begin() + idx, s.begin() + idx + (end - start));
            for (int i = start; i < end; i++) s[idx++] = s[i];
            start = end;
        }
    }
    s.resize(idx);
    return s;
}
// Interview Explanation:
// - Problem Statement: Reverse the order of words in string s with normalized single whitespace (LeetCode 151).
// - Approach: Reverse full string, then reverse individual words in-place.
// - Intuition: Reversing entire string puts words in correct reversed position but characters reversed; reversing characters in each word restores word spelling.
// - Complexity: Time: O(N) two passes, Space: O(1) in-place.

// 2. Roman to Integer (LeetCode 13)
int romanToInt(string s) {
    unordered_map<char, int> val = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500}, {'M', 1000}
    };
    int ans = 0, n = s.size();
    for (int i = 0; i < n; i++) {
        if (i + 1 < n && val[s[i]] < val[s[i + 1]]) {
            ans -= val[s[i]]; // subtractive notation e.g. IV = 4, IX = 9
        } else {
            ans += val[s[i]];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Convert Roman numeral string to an integer (LeetCode 13).
// - Approach: Greedy symbol mapping with subtractive lookahead.
// - Intuition: If current numeral is smaller than the next (`val[s[i]] < val[s[i+1]]`), subtract current value; otherwise add it.
// - Complexity: Time: O(N), Space: O(1).

// 3. String to Integer / ATOI (LeetCode 8)
int myAtoi(string s) {
    int i = 0, n = s.size();
    while (i < n && s[i] == ' ') i++; // skip leading whitespace
    if (i == n) return 0;

    int sign = 1;
    if (s[i] == '+' || s[i] == '-') {
        sign = (s[i] == '-') ? -1 : 1;
        i++;
    }

    long long ans = 0;
    while (i < n && isdigit(s[i])) {
        int digit = s[i] - '0';
        ans = ans * 10 + digit;
        if (sign == 1 && ans > INT_MAX) return INT_MAX;
        if (sign == -1 && -ans < INT_MIN) return INT_MIN;
        i++;
    }
    return (int)(sign * ans);
}
// Interview Explanation:
// - Problem Statement: Convert string to 32-bit signed integer with whitespace, sign, and clamping rules (LeetCode 8).
// - Approach: Linear state parser with 32-bit boundary clamp checks.
// - Intuition: Parse leading spaces, extract sign, process digit sequence clamping overflow immediately against `INT_MAX` and `INT_MIN`.
// - Complexity: Time: O(N) single pass, Space: O(1).

// 4. Longest Common Prefix (LeetCode 14)
string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";
    sort(strs.begin(), strs.end());
    string first = strs.front(), last = strs.back();
    int len = 0;
    while (len < (int)first.size() && len < (int)last.size() && first[len] == last[len]) {
        len++;
    }
    return first.substr(0, len);
}
// Interview Explanation:
// - Problem Statement: Find longest common prefix among an array of strings (LeetCode 14).
// - Approach: Sort strings and compare only the first and last lexicographical strings.
// - Intuition: Sorting orders strings lexicographically; the common prefix shared across all strings is precisely the prefix shared between the first and last strings.
// - Complexity: Time: O(N \log N \cdot L), Space: O(1) auxiliary space.

// 5. Minimum Insertion Steps to Make a String Palindrome (LeetCode 1312)
int minInsertionsToPalindrome(string s) {
    int n = s.size();
    string rev = s;
    reverse(rev.begin(), rev.end());

    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        vector<int> curr(n + 1, 0);
        for (int j = 1; j <= n; j++) {
            if (s[i - 1] == rev[j - 1]) curr[j] = 1 + dp[j - 1];
            else curr[j] = max(dp[j], curr[j - 1]);
        }
        dp = curr;
    }
    int lps = dp[n];
    return n - lps;
}
// Interview Explanation:
// - Problem Statement: Find minimum insertions to make string a palindrome (LeetCode 1312).
// - Approach: Reduction to Longest Palindromic Subsequence (LPS).
// - Intuition: Keep the longest palindromic subsequence intact (length `lps`), and insert matching characters for all remaining `n - lps` characters.
// - Complexity: Time: O(N^2), Space: O(N) 1D DP table.

// 6. Count and Say (LeetCode 38)
string countAndSay(int n) {
    if (n == 1) return "1";
    string s = "1";
    for (int i = 2; i <= n; i++) {
        string next = "";
        int len = s.size();
        for (int j = 0; j < len; ) {
            int count = 1;
            while (j + 1 < len && s[j] == s[j + 1]) {
                count++;
                j++;
            }
            next += to_string(count) + s[j];
            j++;
        }
        s = next;
    }
    return s;
}
// Interview Explanation:
// - Problem Statement: Generate the n-th term of the Count and Say sequence (LeetCode 38).
// - Approach: Iterative Run-Length Encoding (RLE) simulation.
// - Intuition: Count consecutive identical character runs and serialize as `<count><digit>`. Repeat n-1 times.
// - Complexity: Time: O(N \cdot L), Space: O(L) for string buffer.

// 7. Compare Version Numbers (LeetCode 165)
int compareVersion(string version1, string version2) {
    int i = 0, j = 0, n1 = version1.size(), n2 = version2.size();
    while (i < n1 || j < n2) {
        long long num1 = 0, num2 = 0;
        while (i < n1 && version1[i] != '.') num1 = num1 * 10 + (version1[i++] - '0');
        while (j < n2 && version2[j] != '.') num2 = num2 * 10 + (version2[j++] - '0');

        if (num1 < num2) return -1;
        if (num1 > num2) return 1;

        i++; j++; // skip '.'
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Compare two version numbers version1 and version2 (LeetCode 165).
// - Approach: Two-Pointer parsing by '.' delimiters.
// - Intuition: Parse integer revision between dots; missing revisions default to 0. Compare revision by revision from left to right.
// - Complexity: Time: O(N + M) single pass, Space: O(1).

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: STRING ALGORITHMS & HASHING
 ====================================================================================================

 1. PATTERN MATCHING ALGORITHM SELECTION:
    -------------------------------------------------------------------------------------------------
    Algorithm       | Time        | Best Used For
    -------------------------------------------------------------------------------------------------
    KMP             | O(N + M)    | Single pattern matching, Prefix failure function (pi-array)
    Z-Algorithm     | O(N + M)    | Prefix matching, LCP array (Z-array), border finding
    Rabin-Karp      | O(N + M)    | Multi-pattern fixed length matching, Substring hash comparison
    Sliding Window  | O(N)        | Minimum Window Substring, Longest Substring without repeats
    Trie            | O(L)        | Prefix matching, Auto-complete, Word Dictionary search
    Manacher        | O(N)        | All palindromic radii d1 (odd) and d2 (even)
    -------------------------------------------------------------------------------------------------

 2. ROLLING HASH FORMULAS:
    • Forward Hash: `pref[i+1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD`
    • Substring Query: `get(l, r) = (pref[r+1] - pref[l] * power[r-l+1] % MOD + MOD) % MOD`
    • Choice of Constants: `MOD = 1e9 + 7`, `P = 31` (lowercase) or `P = 53` (mixed case).
 ====================================================================================================
*/

// ====================================================================================================
// 26. ENCODE AND DECODE STRINGS (LEETCODE 271)
// ====================================================================================================

string encodeStrings(const vector<string>& strs) {
    string encoded = "";
    for (const string& s : strs) {
        encoded += to_string(s.size()) + "#" + s;
    }
    return encoded;
}

vector<string> decodeStrings(const string& s) {
    vector<string> result;
    int i = 0, n = s.size();
    while (i < n) {
        int hash_pos = s.find('#', i);
        int len = stoi(s.substr(i, hash_pos - i));
        result.push_back(s.substr(hash_pos + 1, len));
        i = hash_pos + 1 + len;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Encode a list of strings to a single string and decode back to original list.
// - Approach: Length-prefixed string framing (<length>#<string>).
// - Complexity: Time: O(Total Characters), Space: O(1) auxiliary space.



// ====================================================================================================
// 27. REPEATING SUBSTRING (CSES 2106 / LEETCODE 1062)
// ====================================================================================================

struct DoubleStringHash {
    int n;
    const ll MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, BASE = 31;
    vll p1, p2, h1, h2;

    DoubleStringHash(const string& s) : n(s.size()), p1(n + 1, 1), p2(n + 1, 1), h1(n + 1, 0), h2(n + 1, 0) {
        for (int i = 0; i < n; i++) {
            p1[i + 1] = p1[i] * BASE % MOD1;
            p2[i + 1] = p2[i] * BASE % MOD2;
            h1[i + 1] = (h1[i] * BASE + (s[i] - 'a' + 1)) % MOD1;
            h2[i + 1] = (h2[i] * BASE + (s[i] - 'a' + 1)) % MOD2;
        }
    }

    pair<ll, ll> getHash(int l, int r) const {
        ll x1 = (h1[r + 1] - h1[l] * p1[r - l + 1] % MOD1 + MOD1) % MOD1;
        ll x2 = (h2[r + 1] - h2[l] * p2[r - l + 1] % MOD2 + MOD2) % MOD2;
        return {x1, x2};
    }

    int checkDuplicate(int len) const {
        map<pair<ll, ll>, int> seen;
        for (int i = 0; i + len <= n; i++) {
            auto h = getHash(i, i + len - 1);
            if (seen.count(h)) return i;
            seen[h] = i;
        }
        return -1;
    }
};

string repeatingSubstring(const string& s) {
    int n = s.size();
    if (n <= 1) return "";
    DoubleStringHash dsh(s);

    int lo = 1, hi = n - 1, bestLen = 0, bestPos = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int pos = dsh.checkDuplicate(mid);
        if (pos != -1) {
            bestLen = mid;
            bestPos = pos;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return bestPos == -1 ? "" : s.substr(bestPos, bestLen);
}
// Interview Explanation:
// - Problem Statement: Find the longest substring of s that occurs at least twice (CSES 2106 / LC 1062).
// - Approach: Binary Search on Substring Length + Double Polynomial Rolling Hash Table.
// - Intuition: If a repeating substring of length L exists, smaller repeating substrings must also exist; monotonic predicate allows binary search.
// - Complexity: Time: O(N log^2 N), Space: O(N).


// ====================================================================================================
// 28. ALL PALINDROMIC ENDPOINTS (CSES 3138 / 2110)
// ====================================================================================================

vi maxPalindromeEndingAtEachPosition(const string& s) {
    int n = s.size();
    if (n == 0) return {};
    vi d1(n, 0), d2(n, 0);
    for (int l = 0, r = -1, i = 0; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) { l = i - k; r = i + k; }
    }
    for (int l = 0, r = -1, i = 0; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) { l = i - k - 1; r = i + k; }
    }

    vi ans(n, 0);
    for (int c = 0; c < n; c++) {
        int r = d1[c], R = c + r - 1;
        ans[R] = max(ans[R], 2 * r - 1);
    }
    for (int c = 0; c < n; c++) {
        int r = d2[c];
        if (r == 0) continue;
        int R = c + r - 1;
        ans[R] = max(ans[R], 2 * r);
    }
    for (int i = n - 1; i > 0; i--) {
        if (ans[i] >= 2) ans[i - 1] = max(ans[i - 1], ans[i] - 2);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: For each index i, find maximum length of a palindromic substring ending at position i (CSES 3138).
// - Approach: Manacher's Algorithm + Backward Maximum Radius Propagation.
// - Intuition: Compute exact odd (d1) and even (d2) radii; map right endpoints and propagate decremented lengths backwards (ans[i-1] = max(ans[i-1], ans[i]-2)).
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// 29. DISTINCT SUBSEQUENCES II (CSES 1149 / LEETCODE 940)
// ====================================================================================================

int distinctSubsequencesII(const string& s) {
    int n = s.size();
    vll dp(n + 1, 0);
    dp[0] = 1;
    vi last(26, 0);

    for (int i = 1; i <= n; i++) {
        int c = s[i - 1] - 'a';
        dp[i] = (2 * dp[i - 1]) % MOD;
        if (last[c] != 0) {
            dp[i] = (dp[i] - dp[last[c] - 1] + MOD) % MOD;
        }
        last[c] = i;
    }
    return (int)((dp[n] - 1 + MOD) % MOD);
}
// Interview Explanation:
// - Problem Statement: Count number of distinct non-empty subsequences of string s modulo 10^9+7 (CSES 1149 / LC 940).
// - Approach: 1D Dynamic Programming with Last Sighting Deduplication (`dp[i] = 2 * dp[i-1] - dp[last[c]-1]`).
// - Intuition: Appending character s[i-1] doubles the previous valid choices; subtracting choices from before s[i-1]'s previous appearance removes duplicates.
// - Complexity: Time: O(N), Space: O(N + Sigma).


// ====================================================================================================
// 30. DYNAMIC PALINDROME QUERIES (CSES 2420)
// ====================================================================================================

class DynamicPalindromeQueries {
    int n;
    vll p, fwdBIT, bwdBIT;

    void bitUpdate(vll& bit, int idx, ll val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] = (bit[idx] + val % MOD + MOD) % MOD;
    }

    ll bitQuery(const vll& bit, int idx) const {
        ll sum = 0;
        for (; idx > 0; idx -= idx & -idx) sum = (sum + bit[idx]) % MOD;
        return sum;
    }

    ll queryRange(const vll& bit, int l, int r) const {
        return (bitQuery(bit, r) - bitQuery(bit, l - 1) + MOD) % MOD;
    }

public:
    DynamicPalindromeQueries(const string& s) {
        n = s.size();
        p.assign(n + 1, 1);
        fwdBIT.assign(n + 1, 0);
        bwdBIT.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) p[i] = (p[i - 1] * P) % MOD;
        for (int i = 1; i <= n; i++) {
            ll val = s[i - 1] - 'a' + 1;
            bitUpdate(fwdBIT, i, val * p[i - 1] % MOD);
            bitUpdate(bwdBIT, i, val * p[n - i] % MOD);
        }
    }

    void update(int idx, char oldChar, char newChar) {
        ll diff = ((newChar - 'a' + 1) - (oldChar - 'a' + 1) + MOD) % MOD;
        bitUpdate(fwdBIT, idx, diff * p[idx - 1] % MOD);
        bitUpdate(bwdBIT, idx, diff * p[n - idx] % MOD);
    }

    bool isPalindrome(int l, int r) const {
        ll fwd = queryRange(fwdBIT, l, r);
        ll bwd = queryRange(bwdBIT, l, r);
        ll fwdNormalized = fwd * p[n - r] % MOD;
        ll bwdNormalized = bwd * p[l - 1] % MOD;
        return fwdNormalized == bwdNormalized;
    }
};
// Interview Explanation:
// - Problem Statement: Process point character updates and substring palindrome queries in O(log N) (CSES 2420).
// - Approach: Dual Fenwick Trees tracking Forward and Backward Polynomial Rolling Hashes.
// - Intuition: A substring s[l..r] is a palindrome iff its forward rolling hash equals its backward rolling hash; Fenwick Trees maintain dynamic prefix hash sums.
// - Complexity: Time: O(log N) per query/update, Space: O(N).


// ====================================================================================================
// 31. STRING FUNCTIONS: KMP PI & Z-ARRAY (CSES 2107)
// ====================================================================================================

pair<vi, vi> computeStringFunctions(const string& s) {
    int n = s.size();
    vi pi(n, 0), z(n, 0);

    // Compute KMP Pi
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    // Compute Z-Array
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }

    return {pi, z};
}
// Interview Explanation:
// - Problem Statement: Compute both KMP prefix function (pi) and Z-array for string s in O(N) (CSES 2107).
// - Approach: Standard linear-time algorithms for failure function and sliding LCP box.
// - Complexity: Time: O(N), Space: O(N).

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
