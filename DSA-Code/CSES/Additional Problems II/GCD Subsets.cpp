// Link: https://cses.fi/problemset/task/3161
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> freq(n + 1, 0), cnt(n + 1, 0);
    for (int i = 0; i < n; i++) { int x; cin >> x; freq[x]++; }

    vector<long long> pw(n + 1, 1);
    for (int i = 1; i <= n; i++) pw[i] = pw[i - 1] * 2 % MOD;

    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) cnt[i] += freq[j];
    }

    vector<long long> ans(n + 1, 0);
    for (int i = n; i >= 1; i--) {
        ans[i] = (pw[cnt[i]] - 1 + MOD) % MOD;
        for (int j = 2 * i; j <= n; j += i) {
            ans[i] = (ans[i] - ans[j] + MOD) % MOD;
        }
    }

    for (int i = 1; i <= n; i++) cout << ans[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each $g \in [1, n]$, count subsets of array whose greatest common divisor is exactly $g$ (CSES 3161).
// - Approach: Inclusion-Exclusion Principle / Mobius Inversion DP iterating backwards.
// - Intuition: $2^{\text{cnt}[g]} - 1$ counts subsets whose elements are all multiples of $g$; subtracting exact GCD answers for multiples $2g, 3g, \dots$ leaves exact GCD $g$.
// - Complexity: Time: O(N \log N), Space: O(N).