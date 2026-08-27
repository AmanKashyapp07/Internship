/**
 * ====================================================================================================
 *                 ULTIMATE STRING ALGORITHMS & HASHING MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Polynomial Rolling Hash (FastHash), KMP Algorithm,
 * Z-Algorithm, Manacher's Algorithm, Booth's Algorithm, Rabin-Karp, Palindromic Decompositions,
 * Distinct Subsequences DP, and Multi-Pattern String Segmentation.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vll = vector<ll>;

const ll MOD = 1e9 + 7;
const ll P = 31;

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
// Interview Explanation:
// - Problem Statement: Answer range queries checking if substring s[l..r] is a palindrome in O(1) per query.
// - Approach: Dual FastHash on original string s and reversed string rev.
// - Intuition: s[l..r] is a palindrome iff its forward hash equals the reverse hash of s[n-1-r..n-1-l].
// - Complexity: Time: O(N) precomputation, O(1) per query, Space: O(N).


// ====================================================================================================
// SECTION 2: CORE STRING MATCHING & PATTERN RECOGNITION
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
// Interview Explanation:
// - Problem Statement: Find all start indices of pattern in text in linear O(N + M) time.
// - Approach: KMP Prefix Function (pi-array failure table).
// - Intuition: pi[i] stores length of longest proper prefix of s[0..i] that is also a suffix; `#` separator prevents pi[i] from exceeding pattern length.
// - Complexity: Time: O(N + M), Space: O(N + M).


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
// Interview Explanation:
// - Problem Statement: Compute Z-array where Z[i] is the length of longest common prefix between s and suffix s[i..n-1].
// - Approach: Z-Algorithm sliding window box [l, r] optimization.
// - Intuition: Reuses previously calculated Z-values inside the active window [l, r] to skip redundant character comparisons.
// - Complexity: Time: O(N), Space: O(N).


// 3. Repeated String Match (LeetCode 686 - Rabin-Karp)
int repeatedStringMatch(string a, string b) {
    string s = "";
    int count = 0;
    while ((int)s.size() < (int)b.size()) {
        s += a;
        count++;
    }
    FastHash H(s + a + a); // Append up to two extra copies for boundary coverage
    ll target = FastHash::hashWord(b);
    int n = s.size(), m = b.size();

    auto checkMatch = [&](int lenStr, int repCount) {
        FastHash H_curr(s);
        for (int i = 0; i <= lenStr - m; i++) {
            if (H_curr.get(i, i + m - 1) == target) return repCount;
        }
        return -1;
    };

    int res = checkMatch(n, count);
    if (res != -1) return res;

    s += a;
    count++;
    res = checkMatch(s.size(), count);
    if (res != -1) return res;

    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of times string a must be repeated so that b is a substring of it.
// - Approach: String repetition bounding + Rabin-Karp hash matching.
// - Intuition: Minimum length needed is b.size(); at most count or count + 1 repetitions are required to contain b.
// - Complexity: Time: O(|A| + |B|), Space: O(|A| + |B|).


// 4. Manacher's Algorithm (O(N) Palindromic Radii)
pair<vi, vi> manacher(const string &s) {
    int n = s.size();
    vi d1(n, 0); // Odd length palindromes radius
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) { l = i - k; r = i + k; }
    }
    vi d2(n, 0); // Even length palindromes radius
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) { l = i - k - 1; r = i + k; }
    }
    return {d1, d2};
}
// Interview Explanation:
// - Problem Statement: Find radii of all odd-length and even-length palindromes centered at every character in O(N).
// - Approach: Manacher's Algorithm using active palindrome window [l, r] mirroring.
// - Intuition: Mirrors palindrome radius across center `l + r - i` to skip redundant expansions, achieving strict O(N) time.
// - Complexity: Time: O(N), Space: O(N).


// 5. Booth's Algorithm (Smallest Circular String Rotation)
string boothsAlgorithm(string s) {
    int n = s.size();
    s += s; // Concatenate string to handle circular shifts
    vi f(2 * n, -1);
    int k = 0; // Index of current smallest rotation
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
// Interview Explanation:
// - Problem Statement: Find the lexicographically smallest circular rotation of a string.
// - Approach: Booth's algorithm modified KMP failure function on duplicated string s + s.
// - Intuition: Maintains candidate minimum rotation index k in linear O(N) time without generating all N rotations.
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// SECTION 3: PALINDROMIC STRING DECOMPOSITIONS & PREPROCESSING
// ====================================================================================================

// 1. Shortest Palindrome (LeetCode 214 - Prepend Minimum Characters)
string shortestPalindrome(string s) {
    if (s.empty()) return s;
    string rev = s;
    reverse(rev.begin(), rev.end());
    FastHash fwd(s), bwd(rev);
    int n = s.size(), bestLen = 0;
    for (int i = 0; i < n; i++) {
        if (fwd.get(0, i) == bwd.get(n - 1 - i, n - 1)) {
            bestLen = i + 1; // Longest palindromic prefix length
        }
    }
    string suffix = s.substr(bestLen);
    reverse(suffix.begin(), suffix.end());
    return suffix + s;
}
// Interview Explanation:
// - Problem Statement: Convert string s into a palindrome by prepending minimum characters at the front.
// - Approach: Find longest palindromic prefix using Rolling Hash comparisons against reversed string.
// - Intuition: Prepending the reversed non-palindromic suffix onto the front creates the shortest palindrome.
// - Complexity: Time: O(N), Space: O(N).


// 2. Longest Chunked Palindrome Decomposition (LeetCode 1147)
int longestDecomposition(string text) {
    int n = text.size();
    FastHash H(text);
    int k = 0, lStart = 0, rEnd = n - 1;
    int i = 0, j = n - 1;
    while (i < j) {
        if (H.get(lStart, i) == H.get(j, rEnd)) {
            k += 2;
            lStart = i + 1;
            rEnd = j - 1;
        }
        i++; j--;
    }
    if (lStart <= rEnd) k += 1;
    return k;
}
// Interview Explanation:
// - Problem Statement: Split string text into maximum number of chunks a_1 + a_2 + ... + a_k such that a_i == a_{k-i+1}.
// - Approach: Two-pointer greedy chunk matching with O(1) rolling hash equality checks.
// - Intuition: Greedily match shortest identical prefix and suffix chunks to maximize total decomposition count.
// - Complexity: Time: O(N), Space: O(N).


// 3. Count Distinct Subsequences (DP with Last Occurrence Tracking)
int countDistinctSubsequences(string s) {
    int n = s.size();
    vll dp(n + 1, 0);
    dp[0] = 1; // Empty subsequence
    vi last(26, 0);
    for (int i = 1; i <= n; i++) {
        int idx = s[i - 1] - 'a';
        dp[i] = (2 * dp[i - 1]) % MOD;
        if (last[idx] != 0) {
            dp[i] = (dp[i] - dp[last[idx] - 1] + MOD) % MOD;
        }
        last[idx] = i;
    }
    return (dp[n] - 1 + MOD) % MOD; // Exclude empty subsequence
}
// Interview Explanation:
// - Problem Statement: Count the number of distinct non-empty subsequences of string s modulo 10^9+7.
// - Approach: DP with last occurrence tracking for duplicate subtraction.
// - Intuition: Appending s[i-1] doubles existing distinct subsequences; subtract dp[last[ch]-1] to eliminate duplicates formed by previous character occurrence.
// - Complexity: Time: O(N), Space: O(N).


// ====================================================================================================
// SECTION 4: MULTI-PATTERN & STRING SEGMENTATION DP
// ====================================================================================================

// 1. CSES Word Combinations / Word Break Counting (LeetCode 139 variant)
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
// Interview Explanation:
// - Problem Statement: Count total ways to construct target string s using dictionary words modulo 10^9+7.
// - Approach: Bottom-up DP + Rolling Hash matching for unique dictionary word lengths.
// - Intuition: `dp[i]` stores ways to form suffix s[i..n-1]; iterate unique lengths and add valid transitions `dp[i + len]`.
// - Complexity: Time: O(N \cdot L) where L = unique word lengths, Space: O(N + M).


// 2. Substring with Concatenation of All Words (LeetCode 30)
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
// Interview Explanation:
// - Problem Statement: Find starting indices of all concatenated substrings containing every word in words exactly once.
// - Approach: Offset Sliding Window + Rolling Hash equality counts.
// - Intuition: Run k independent sliding windows with step size k; maintain hash frequency table to validate exact matches in O(N).
// - Complexity: Time: O(N), Space: O(M \cdot K).


// 3. Isomorphic Strings (LeetCode 205)
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
// Interview Explanation:
// - Problem Statement: Determine if two strings s and t are isomorphic (one-to-one character mapping).
// - Approach: Single-pass position tracking array for both strings.
// - Intuition: Stores last seen position of each character; matching positions guarantee consistent bi-directional mapping.
// - Complexity: Time: O(N), Space: O(1) 256-element direct access arrays.

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: STRING ALGORITHMS & HASHING
 ====================================================================================================

 1. PATTERN MATCHING ALGORITHM SELECTION:
    -------------------------------------------------------------------------------------------------
    Algorithm       | Time        | Best Used For
    -------------------------------------------------------------------------------------------------
    KMP             | O(N + M)    | Exact single pattern matching, Prefix failure function (pi-array)
    Z-Algorithm     | O(N + M)    | Prefix matching, LCP array (Z-array), border finding
    Rabin-Karp      | O(N + M)    | Multi-pattern fixed length matching, Substring hash comparison
    Manacher        | O(N)        | All palindromic radii d1 (odd) and d2 (even)
    Booth's         | O(N)        | Smallest circular string rotation
    -------------------------------------------------------------------------------------------------

 2. ROLLING HASH FORMULAS & COLLISION PREVENTION:
    • Forward Hash: `pref[i+1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD`
    • Substring Query: `get(l, r) = (pref[r+1] - pref[l] * power[r-l+1] % MOD + MOD) % MOD`
    • Choice of Constants: `MOD = 1e9 + 7` (or `1e9 + 9`), `P = 31` (lowercase) or `P = 53` (mixed case).
    • Anti-Hash Defense: Use double hashing (MOD1 = 1e9+7, MOD2 = 1e9+9) to eliminate hash collision attacks.

 3. PALINDROME STRATEGY MATRIX:
    • Check 1 Range [l, r]: Use Dual Rolling Hash (fwd vs bwd) in O(1).
    • Find All Palindromes: Use Manacher's Algorithm in O(N).
    • Palindrome Partitioning: Use 2D DP Table `pal[i][j]` + Backtracking.
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
