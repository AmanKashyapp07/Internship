// Link: https://cses.fi/problemset/task/1732
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

    for (int k = 1; k < n; k++) {
        if (H.get(0, k - 1) == H.get(n - k, n - 1)) {
            cout << k << " ";
        }
    }
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all border lengths k (1 <= k < n) where prefix of length k equals suffix of length k (CSES 1732).
// - Approach: Polynomial Rolling Hash O(1) Substring Hash Equality Queries.
// - Intuition: Compare prefix hash `H.get(0, k-1)` against suffix hash `H.get(n-k, n-1)` for each length k.
// - Complexity: Time: O(N), Space: O(N).