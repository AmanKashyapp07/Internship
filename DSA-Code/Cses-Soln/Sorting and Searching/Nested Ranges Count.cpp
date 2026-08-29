// Link: https://cses.fi/problemset/task/2169
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, int v) { for (i++; i <= n; i += i & -i) bit[i] += v; }
    int query(int i) { int s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }
};

struct Range { int l, r, idx; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Range> ranges(n); vector<int> vals;
    for (int i = 0; i < n; i++) {
        cin >> ranges[i].l >> ranges[i].r; ranges[i].idx = i;
        vals.push_back(ranges[i].r);
    }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto getRank = [&](int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); };

    for (auto &x : ranges) x.r = getRank(x.r);

    sort(ranges.begin(), ranges.end(), [](const Range &a, const Range &b) {
        if (a.l == b.l) return a.r > b.r;
        return a.l < b.l;
    });

    vector<int> contains(n), contained(n);
    Fenwick bit1(vals.size());
    for (int i = n - 1; i >= 0; i--) {
        contains[ranges[i].idx] = bit1.query(ranges[i].r);
        bit1.add(ranges[i].r, 1);
    }

    Fenwick bit2(vals.size());
    for (int i = 0; i < n; i++) {
        contained[ranges[i].idx] = bit2.query(vals.size() - 1) - (ranges[i].r ? bit2.query(ranges[i].r - 1) : 0);
        bit2.add(ranges[i].r, 1);
    }

    for (int x : contains) cout << x << " ";
    cout << "\n";
    for (int x : contained) cout << x << " ";
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count how many ranges each range contains, and how many ranges contain each range (CSES 2169).
// - Approach: Coordinate Compression + Sweep Line + Fenwick Tree Prefix Sum Queries.
// - Intuition: Sorting ranges by left bound allows querying right bound counts in a Fenwick Tree in $O(\log N)$ time per range.
// - Complexity: Time: O(N \log N), Space: O(N).