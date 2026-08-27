// Link: https://cses.fi/problemset/task/1731
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7, P = 31;

struct FastHash {
    vector<long long> power, pref;
    FastHash(const string &s) {
        int n = s.size();
        power.assign(n + 1, 1); pref.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    long long get(int l, int r) const {
        return (pref[r + 1] - pref[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }
    static long long hashWord(const string &w) {
        long long val = 0;
        for (char c : w) val = (val * P + (c - 'a' + 1)) % MOD;
        return val;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size(), k; cin >> k;

    unordered_set<long long> wordHashes;
    unordered_set<int> uniqueLens;
    for (int i = 0; i < k; i++) {
        string w; cin >> w;
        wordHashes.insert(FastHash::hashWord(w));
        uniqueLens.insert(w.size());
    }

    FastHash H(s);
    vector<long long> dp(n + 1, 0);
    dp[n] = 1;

    for (int i = n - 1; i >= 0; i--) {
        for (int len : uniqueLens) {
            if (i + len <= n && wordHashes.count(H.get(i, i + len - 1))) {
                dp[i] = (dp[i] + dp[i + len]) % MOD;
            }
        }
    }
    cout << dp[0] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count the number of ways to form string s using dictionary words modulo 10^9+7 (CSES 1731).
// - Approach: Dynamic Programming + Polynomial Rolling Hash lookups for unique dictionary word lengths.
// - Intuition: `dp[i]` sums `dp[i + len]` for all unique dictionary word lengths `len` whose hash matches `s[i..i+len-1]`.
// - Complexity: Time: O(N \cdot L) where L is unique word lengths count, Space: O(N + K).