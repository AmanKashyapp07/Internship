// Link: https://cses.fi/problemset/task/2217
#include <bits/stdc++.h>
using namespace std;

bool bad(int x, const vector<int>& pos, int n) {
    if (x < 1 || x >= n) return false;
    return pos[x] > pos[x + 1];
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<int> a(n + 1), pos(n + 1);
    for (int i = 1; i <= n; i++) { cin >> a[i]; pos[a[i]] = i; }

    int rounds = 1;
    for (int x = 1; x < n; x++) if (pos[x] > pos[x + 1]) rounds++;

    while (m--) {
        int p, q; cin >> p >> q;
        int u = a[p], v = a[q];
        set<int> affected = {u - 1, u, v - 1, v};
        for (int x : affected) rounds -= bad(x, pos, n);
        swap(a[p], a[q]); swap(pos[u], pos[v]);
        for (int x : affected) rounds += bad(x, pos, n);
        cout << rounds << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process element swaps and count the number of left-to-right passes to collect numbers 1 to n (CSES 2217).
// - Approach: Local Position Inversion Tracking via `set<int> affected`.
// - Intuition: Swapping elements at indices p and q only affects neighbor pairs $(u-1, u), (u, u+1), (v-1, v), (v, v+1)$; updating changes locally yields $O(1)$ query time.
// - Complexity: Time: O(N + M), Space: O(N).