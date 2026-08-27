// Link: https://cses.fi/problemset/task/1144
#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n; vector<int> b;
    BIT(int n) : n(n), b(n + 1, 0) {}
    void upd(int i, int v) { for (++i; i <= n; i += i & -i) b[i] += v; }
    int qry(int l, int r) {
        int s = 0;
        for (int i = r + 1; i > 0; i -= i & -i) s += b[i];
        if (l) for (int i = l; i > 0; i -= i & -i) s -= b[i];
        return s;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> sal(n + 1);

    // Coordinate compress all values
    vector<int> vals;
    for (int i = 1; i <= n; i++) { cin >> sal[i]; vals.push_back(sal[i]); }

    struct Ev { char t; int a, b; };
    vector<Ev> ev(q);
    for (auto& [t, a, b] : ev) {
        cin >> t >> a >> b;
        vals.push_back(b);
        if (t == '?') vals.push_back(a);
    }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto compress = [&](int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); };

    BIT bit(vals.size());
    for (int i = 1; i <= n; i++) bit.upd(compress(sal[i]), 1);

    for (auto& [t, a, b] : ev) {
        if (t == '!') {
            bit.upd(compress(sal[a]), -1);
            sal[a] = b;
            bit.upd(compress(b), 1);
        } else {
            cout << bit.qry(compress(a), compress(b)) << '\n';
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Support salary updates and range-count queries (how many salaries in [a, b]?) (CSES 1144).
// - Approach: Fenwick Tree on Coordinate-Compressed salary values.
// - Intuition: Compress all possible salary values; BIT tracks frequency per compressed index; range count = prefix difference; updates remove old value and add new one.
// - Complexity: Time: O((N + Q) log(N + Q)), Space: O(N + Q).
