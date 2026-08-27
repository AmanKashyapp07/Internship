// Link: https://cses.fi/problemset/task/1650
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) { cin >> pref[i]; pref[i] ^= pref[i - 1]; }
    while (q--) {
        int a, b; cin >> a >> b;
        cout << (pref[b] ^ pref[a - 1]) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Answer q range XOR queries [l, r] on a static array (CSES 1650).
// - Approach: 1D Prefix XOR array.
// - Intuition: `xor(l, r) = pref[r] ^ pref[l-1]`; XOR is self-inverse so the same prefix trick applies.
// - Complexity: Time: O(N + Q), Space: O(N).