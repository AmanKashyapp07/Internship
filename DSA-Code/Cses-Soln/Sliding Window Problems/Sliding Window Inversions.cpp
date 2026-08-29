// Link: https://cses.fi/problemset/task/3223
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, int val) { for (i++; i <= n; i += i & -i) bit[i] += val; }
    int query(int i) { int s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }
    int queryRange(int l, int r) { return (l > r) ? 0 : query(r) - (l > 0 ? query(l - 1) : 0); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n), vals;
    for (int i = 0; i < n; i++) { cin >> a[i]; vals.push_back(a[i]); }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto getRank = [&](int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); };

    vector<int> id(n);
    for (int i = 0; i < n; i++) id[i] = getRank(a[i]);

    Fenwick bit(vals.size());
    long long inv = 0;

    for (int i = 0; i < k; i++) {
        inv += bit.queryRange(id[i] + 1, vals.size() - 1);
        bit.add(id[i], 1);
    }
    cout << inv;

    for (int i = k; i < n; i++) {
        int out = i - k;
        bit.add(id[out], -1);
        inv -= bit.queryRange(0, id[out] - 1);

        inv += bit.queryRange(id[i] + 1, vals.size() - 1);
        bit.add(id[i], 1);
        cout << " " << inv;
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the number of inversion pairs in every sliding window of size k (CSES 3223).
// - Approach: Coordinate Compression + Fenwick Tree tracking sliding window frequencies.
// - Intuition: Removing outgoing element subtracts smaller elements before it; adding incoming element adds larger elements before it in $O(\log N)$ time.
// - Complexity: Time: O(N \log N), Space: O(N).