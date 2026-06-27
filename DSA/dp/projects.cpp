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

class Solution {
public:
    int n_;
    vector<Project> a_;
    vector<long long> dp_;

    long long solve(int idx) {
        if (idx == n_) return 0;
        if (dp_[idx] != -1) return dp_[idx];
        long long exclude = solve(idx + 1);
        // Binary search for the first project starting after the current project ends (a_[idx].r)
        Project target = {a_[idx].r + 1, 0, 0};
        int next_idx = lower_bound(a_.begin() + idx + 1, a_.end(), target) - a_.begin();
        long long include = a_[idx].val + solve(next_idx);
        return dp_[idx] = max(exclude, include);
    }

    long long getMaxReward(int n, vector<Project>& a) {
        n_ = n;
        a_ = a;
        sort(a_.begin(), a_.end());
        dp_.assign(n_, -1);
        return solve(0);
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (cin >> n) {
        vector<Project> a(n);
        for (int i = 0; i < n; i++) cin >> a[i].l >> a[i].r >> a[i].val;
        
        Solution solver;
        cout << solver.getMaxReward(n, a) << '\n';
    }
    return 0;
}