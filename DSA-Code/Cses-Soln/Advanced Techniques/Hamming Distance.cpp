// Link: https://cses.fi/problemset/task/2136
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        int x = 0;
        for (char c : s) x = (x << 1) | (c - '0');
        a[i] = x;
    }

    int ans = k;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ans = min(ans, __builtin_popcount(a[i] ^ a[j]));
            if (ans == 0) { cout << 0 << '\n'; return 0; }
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum Hamming distance (bit-differing positions) across all pairs of n binary strings of length k (CSES 2136).
// - Approach: Bitmask XOR + Popcount Pairwise Scan.
// - Intuition: `__builtin_popcount(a[i] ^ a[j])` counts differing bits between strings $i$ and $j$ in $O(1)$ hardware instruction.
// - Complexity: Time: O(N^2), Space: O(N).