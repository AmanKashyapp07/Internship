// Link: https://cses.fi/problemset/task/3421
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    sort(a.begin(), a.end());

    long long ans = 1, curFreq = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] == a[i - 1]) curFreq++;
        else {
            ans = (ans * (curFreq + 1)) % MOD;
            curFreq = 1;
        }
    }
    if (n > 0) ans = (ans * (curFreq + 1)) % MOD;
    ans = (ans - 1 + MOD) % MOD;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count non-empty subsequences containing distinct values (CSES 3421).
// - Approach: Combinatorial Product over Element Frequencies $\prod (freq(x) + 1) - 1$.
// - Intuition: For each distinct element $x$ appearing $c$ times, we can pick 0 or 1 instance from $c$ choices ($(c + 1)$ choices); product gives total distinct subsequences minus empty set.
// - Complexity: Time: O(N \log N), Space: O(N).