// Link: https://cses.fi/problemset/task/3398
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long pw(long long a, long long b) {
    long long r = 1; a %= MOD;
    for (; b; b >>= 1) { if (b & 1) r = r * a % MOD; a = a * a % MOD; }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<bool> vis(n + 1, false);
    unordered_map<int, int> maxPower;

    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        int cur = i, len = 0;
        while (!vis[cur]) { vis[cur] = true; cur = a[cur]; len++; }

        int temp = len;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) { temp /= p; cnt++; }
                maxPower[p] = max(maxPower[p], cnt);
            }
        }
        if (temp > 1) maxPower[temp] = max(maxPower[temp], 1);
    }

    long long lcm = 1;
    for (auto [prime, power] : maxPower) lcm = lcm * pw(prime, power) % MOD;
    cout << lcm << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find number of rounds for a permutation to return to original state modulo 10^9+7 (CSES 3398).
// - Approach: Cycle Decomposition + LCM of Cycle Lengths over Prime Factorization.
// - Intuition: Decomposition into disjoint cycles; order of permutation is LCM of cycle lengths calculated via max prime power per prime modulo 10^9+7.
// - Complexity: Time: O(N + Σ √L_i), Space: O(N).