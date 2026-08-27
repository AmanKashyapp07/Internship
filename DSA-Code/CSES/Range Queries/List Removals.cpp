// Link: https://cses.fi/problemset/task/1749
#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n; vector<int> b;
    BIT(int n) : n(n), b(n + 1, 0) {}
    void upd(int i, int v) { for (++i; i <= n; i += i & -i) b[i] += v; }
    int qry(int i) { int s = 0; for (++i; i > 0; i -= i & -i) s += b[i]; return s; }
    // Find first index with prefix sum >= k (binary lift on BIT)
    int find(int k) {
        int pos = 0;
        for (int pw = 1 << __lg(n); pw; pw >>= 1)
            if (pos + pw <= n && b[pos + pw] < k) { pos += pw; k -= b[pos]; }
        return pos; // 0-indexed
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    BIT bit(n);
    for (int i = 0; i < n; i++) { cin >> a[i]; bit.upd(i, 1); }
    for (int i = 0; i < n; i++) {
        int k; cin >> k;
        int idx = bit.find(k);
        cout << a[idx] << " \n"[i == n - 1];
        bit.upd(idx, -1);
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Repeatedly remove the k-th remaining element from a list and print it (CSES 1749).
// - Approach: Fenwick Tree Binary Lifting — BIT tracks active positions; `find(k)` locates k-th active in O(log N).
// - Intuition: BIT stores 1 for active, 0 for removed; binary lifting finds k-th 1 in O(log N) using power-of-2 jumps.
// - Complexity: Time: O(N log N), Space: O(N).
