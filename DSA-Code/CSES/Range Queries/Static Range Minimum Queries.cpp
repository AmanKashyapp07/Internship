// Link: https://cses.fi/problemset/task/1647
#include <bits/stdc++.h>
using namespace std;

struct SparseTable {
    int n; vector<vector<int>> st; vector<int> lg;
    SparseTable(const vector<int>& a) : n(a.size()), st(__lg(a.size()) + 1, vector<int>(a.size())), lg(a.size() + 1) {
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
        st[0] = a;
        for (int j = 1; j < (int)st.size(); j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = min(st[j-1][i], st[j-1][i + (1 << (j-1))]);
    }
    int query(int l, int r) {
        int j = lg[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SparseTable sp(a);
    while (q--) {
        int l, r; cin >> l >> r;
        cout << sp.query(l - 1, r - 1) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Answer q static range minimum queries (CSES 1647).
// - Approach: Sparse Table with O(1) overlapping-interval RMQ.
// - Intuition: Precompute `st[j][i]` = min over `[i, i+2^j-1]`; query uses two overlapping windows of size $2^{\lfloor\log(r-l+1)\rfloor}$.
// - Complexity: Time: O(N log N) build, O(1) query; Space: O(N log N).
