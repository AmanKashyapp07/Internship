/**
 * Problem: Projects (https://cses.fi/problemset/task/1140)
 * Find the maximum reward from non-overlapping projects.
 * Time: O(N log N) time, O(N) space.
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Project {
    int l, r, val;
    bool operator<(const Project& o) const { return l < o.l; }
};

int n;
vector<Project> a;
vector<long long> dp;

long long solve(int idx) {
    if (idx == n) return 0;
    if (dp[idx] != -1) return dp[idx];
    long long exclude = solve(idx + 1);
    // Binary search for the first project starting after the current project ends (a[idx].r)
    Project target = {a[idx].r + 1, 0, 0};
    int next_idx = lower_bound(a.begin() + idx + 1, a.end(), target) - a.begin();
    long long include = a[idx].val + solve(next_idx);
    return dp[idx] = max(exclude, include);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    if (cin >> n) {
        a.resize(n);
        for (int i = 0; i < n; i++) cin >> a[i].l >> a[i].r >> a[i].val;
        sort(a.begin(), a.end());
        dp.assign(n, -1);
        cout << solve(0) << '\n';
    }
}