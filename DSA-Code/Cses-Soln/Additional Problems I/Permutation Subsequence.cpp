// Link: https://cses.fi/problemset/task/3404
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<int> pos(max(n, m) + 1, -1);
    for (int i = 1; i <= n; i++) { int x; cin >> x; pos[x] = i; }

    vector<int> p, val;
    for (int i = 1; i <= m; i++) {
        int x; cin >> x;
        if (x <= max(n, m) && pos[x] != -1) { p.push_back(pos[x]); val.push_back(x); }
    }
    if (p.empty()) { cout << 0 << "\n"; return 0; }

    vector<int> tail, tailIdx, parent(p.size(), -1);
    for (int i = 0; i < (int)p.size(); i++) {
        int j = lower_bound(tail.begin(), tail.end(), p[i]) - tail.begin();
        if (j == (int)tail.size()) { tail.push_back(p[i]); tailIdx.push_back(i); }
        else { tail[j] = p[i]; tailIdx[j] = i; }
        if (j > 0) parent[i] = tailIdx[j - 1];
    }

    vector<int> ans;
    for (int cur = tailIdx.back(); cur != -1; cur = parent[cur]) ans.push_back(val[cur]);
    reverse(ans.begin(), ans.end());

    cout << ans.size() << "\n";
    for (int x : ans) cout << x << " ";
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the Longest Common Subsequence of two permutations (CSES 3404).
// - Approach: Map elements of array B to their 1-based index positions in array A, reducing LCS to LIS.
// - Intuition: Since all elements are unique, a common subsequence corresponds to a strictly increasing sequence of indices in A.
// - Complexity: Time: O(M \log M), Space: O(N + M).