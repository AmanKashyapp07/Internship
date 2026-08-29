// Link: https://cses.fi/problemset/task/1646
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) { cin >> pref[i]; pref[i] += pref[i - 1]; }
    while (q--) {
        int a, b; cin >> a >> b;
        cout << pref[b] - pref[a - 1] << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Answer q range sum queries [l, r] on a static array (CSES 1646).
// - Approach: 1D Prefix Sum array.
// - Intuition: `sum(l, r) = pref[r] - pref[l-1]`; precompute in O(N), each query is O(1).
// - Complexity: Time: O(N + Q), Space: O(N).