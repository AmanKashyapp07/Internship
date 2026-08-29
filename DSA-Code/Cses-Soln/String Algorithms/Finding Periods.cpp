// Link: https://cses.fi/problemset/task/1733
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
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; if (!(cin >> s)) return 0;
    int n = s.size();
    FastHash H(s);

    for (int k = 1; k <= n; k++) {
        if (k == n) { cout << n << " "; continue; }
        int overlap = n - k;
        if (H.get(0, overlap - 1) == H.get(k, n - 1)) {
            cout << k << " ";
        }
    }
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all valid period lengths k (1 <= k <= n) of string s (CSES 1733).
// - Approach: Overlapping Prefix-Suffix Substring Hash Matching.
// - Intuition: Length k is a period iff prefix of length `(n - k)` equals suffix starting at index k `s[k..n-1]`.
// - Complexity: Time: O(N), Space: O(N).