// Link: https://cses.fi/problemset/task/2185
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; int k; cin >> n >> k;
    vector<long long> p(k);
    for (int i = 0; i < k; i++) cin >> p[i];

    long long ans = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        long long prod = 1; int cnt = 0; bool ok = true;
        for (int i = 0; i < k; i++) {
            if (mask & (1 << i)) {
                cnt++;
                if (prod > n / p[i]) { ok = false; break; }
                prod *= p[i];
            }
        }
        if (ok) {
            if (cnt & 1) ans += n / prod;
            else ans -= n / prod;
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count integers in 1..n divisible by at least one of k given prime numbers (CSES 2185).
// - Approach: Inclusion-Exclusion Principle over Bitmask Subsets.
// - Intuition: Total multiples = Σ (n / prod(S)) * (-1)^(|S|+1) for all non-empty subsets of primes; overflow is guarded by division check.
// - Complexity: Time: O(K \cdot 2^K), Space: O(K).