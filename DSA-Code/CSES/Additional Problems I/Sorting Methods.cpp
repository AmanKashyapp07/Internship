// Link: https://cses.fi/problemset/task/1162
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int idx, int val) { for (; idx <= n; idx += idx & -idx) bit[idx] += val; }
    int sum(int idx) { int res = 0; for (; idx > 0; idx -= idx & -idx) res += bit[idx]; return res; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    // 1. Min adjacent swaps = Inversion Count
    Fenwick ft(n); long long inv = 0;
    for (int i = n - 1; i >= 0; i--) { inv += ft.sum(a[i] - 1); ft.add(a[i], 1); }

    // 2. Min arbitrary swaps = N - Permutation Cycles
    vector<bool> vis(n, false); int cycles = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            cycles++; int cur = i;
            while (!vis[cur]) { vis[cur] = true; cur = a[cur] - 1; }
        }
    }

    // 3. Min move-to-any-position = N - LIS
    vector<int> lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }

    // 4. Min move-to-front = N - longest decremental suffix matched
    int need = n;
    for (int i = n - 1; i >= 0; i--) if (a[i] == need) need--;

    cout << inv << " " << (n - cycles) << " " << (n - (int)lis.size()) << " " << need << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum operations to sort a permutation under 4 distinct rules (CSES 1162).
// - Approach: 1) Fenwick inversion count, 2) Cycle decomposition, 3) Patience sorting LIS, 4) Backward suffix scanning.
// - Intuition: Adjacent swap = inversions; arbitrary swap = N - cycles; move anywhere = N - LIS; move to front = N - max matching suffix [need...N].
// - Complexity: Time: O(N \log N), Space: O(N).