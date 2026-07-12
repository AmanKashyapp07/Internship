#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// ------------------- FAST HASH TEMPLATE (CSES OPTIMIZED) -------------------
using ll = long long;

const ll MOD = 1e9 + 7;
const ll P   = 31;

class FastHash {
private:
    vector<ll> power, pref;

public:
    FastHash(const string& s) {
        int n = s.size();
        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    // Returns the polynomial hash of substring s[l..r] in O(1) time
    ll get(int l, int r) const {
        return (pref[r + 1] - pref[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }

    // Computes the hash of an entire string in O(m) time
    static ll hashWord(const string& s) {
        ll h = 0;
        for (char c : s) {
            h = (h * P + (c - 'a' + 1)) % MOD;
        }
        return h;
    }
};

// ---------------------------------------------------------------------------

class Solution {
public:
    // 1. String Matching: Count occurrences of pattern `p` in text `s`
    int countOccurrences(string s, string p) {
        int n = s.length(), m = p.length();
        if (m > n) return 0;

        FastHash string_hash(s);
        FastHash pattern_hash(p);
        ll target = pattern_hash.get(0, m - 1);
        int match_count = 0;

        for (int i = 0; i <= n - m; i++) {
            if (string_hash.get(i, i + m - 1) == target) {
                match_count++;
            }
        }
        return match_count;
    }

    // 2. Finding Borders: Find all border lengths of string `s` (prefix == suffix)
    vector<int> findingBorders(string s) {
        int n = s.length();
        FastHash string_hash(s);
        vector<int> borders;

        for (int k = 1; k < n; k++) {
            ll prefix_hash = string_hash.get(0, k - 1);
            ll suffix_hash = string_hash.get(n - k, n - 1);
            if (prefix_hash == suffix_hash) {
                borders.push_back(k);
            }
        }
        return borders;
    }

    // 3. Finding Multiple Patterns: Check if each pattern exists in text `s`
    vector<bool> findPatterns(string s, vector<string>& patterns) {
        int n = s.length();
        int k = patterns.size();
        
        // Group patterns by length to avoid multiple text hash passes
        vector<vector<pair<ll, int>>> groups(n + 1);
        for (int i = 0; i < k; i++) {
            string p = patterns[i];
            if (p.length() <= n) {
                groups[p.length()].push_back({FastHash::hashWord(p), i});
            }
        }

        FastHash text_hash(s);
        vector<bool> ans(k, false);
        vector<ll> text_subs;

        for (int len = 1; len <= n; len++) {
            if (groups[len].empty()) continue;
            
            text_subs.clear();
            for (int i = 0; i <= n - len; i++) {
                text_subs.push_back(text_hash.get(i, i + len - 1));
            } // push all substring hashes of length `len` into text_subs
            sort(text_subs.begin(), text_subs.end()); // sort for binary search

            for (const auto& pat : groups[len]) {
                if (binary_search(text_subs.begin(), text_subs.end(), pat.first)) {
                    ans[pat.second] = true;
                }
            }
        }
        return ans;
    }

    // 4. Finding Periods: Find all period lengths of string `s`
    vector<int> findingPeriods(string s) {
        int n = s.length();
        FastHash string_hash(s);
        vector<int> periods;

        for (int k = 1; k <= n; k++) {
            if (k == n) {
                periods.push_back(n);
                continue;
            }
            int overlap_len = n - k;
            ll prefix_hash = string_hash.get(0, overlap_len - 1);
            ll suffix_hash = string_hash.get(k, n - 1);
            if (prefix_hash == suffix_hash) {
                periods.push_back(k);
            }
        }
        return periods;
    }

    // 5. Palindrome Queries: Answer range palindromic queries [l, r]
    vector<bool> palindromeQueries(string s, vector<pair<int, int>>& queries) {
        int n = s.size();
        string rev = s;
        reverse(rev.begin(), rev.end());

        FastHash H1(s);
        FastHash H2(rev);
        vector<bool> ans;

        for (const auto& q : queries) {
            int l = q.first;
            int r = q.second;
            ll hash1 = H1.get(l, r);
            ll hash2 = H2.get(n - 1 - r, n - 1 - l);
            ans.push_back(hash1 == hash2);
        }
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Solution solver;

    // --- TEST 1: String Matching ---
    cout << "Testing countOccurrences...\n";
    assert(solver.countOccurrences("abacaba", "aba") == 2);
    assert(solver.countOccurrences("aaaaa", "aa") == 4);
    cout << "Passed!\n\n";

    // --- TEST 2: Finding Borders ---
    cout << "Testing findingBorders...\n";
    vector<int> expected_borders = {1, 3}; // for "abacaba", borders are "a" (1) and "aba" (3)
    assert(solver.findingBorders("abacaba") == expected_borders);
    cout << "Passed!\n\n";

    // --- TEST 3: Finding Multiple Patterns ---
    cout << "Testing findPatterns...\n";
    vector<string> patterns = {"aba", "cab", "xyz", "abacaba"};
    vector<bool> expected_find = {true, true, false, true};
    assert(solver.findPatterns("abacaba", patterns) == expected_find);
    cout << "Passed!\n\n";

    // --- TEST 4: Finding Periods ---
    cout << "Testing findingPeriods...\n";
    vector<int> expected_periods = {3, 6, 7}; // for "abaabaa", periods are 3, 6, 7
    assert(solver.findingPeriods("abaabaa") == expected_periods);
    cout << "Passed!\n\n";

    // --- TEST 5: Palindrome Queries ---
    cout << "Testing palindromeQueries...\n";
    vector<pair<int, int>> queries = {{0, 2}, {1, 3}, {2, 4}, {0, 6}}; // for "abacaba"
    vector<bool> expected_pal = {true, false, true, true}; // "aba" (yes), "bac" (no), "aca" (yes), "abacaba" (yes)
    assert(solver.palindromeQueries("abacaba", queries) == expected_pal);
    cout << "Passed!\n\n";

    cout << "All tests passed successfully!" << endl;
    return 0;
}
