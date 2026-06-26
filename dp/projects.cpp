/**
 * CSES 1140 - Projects
 *
 * Description:
 * There are n projects, each with start/end days and reward. Find the maximum reward from non-overlapping projects.
 *
 * Approach:
 * - Sort the projects by start day.
 * - Use Dynamic Programming with Memoization + Binary Search (`upper_bound`).
 * - For each project `idx`:
 *   - Option 1 (Skip): `solve(idx+1)`.
 *   - Option 2 (Take): `reward + solve(next_available_idx)` where `next_available_idx` is found using binary search on start days > current project end day.
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
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