/**
 * CSES 1140 - Projects
 *
 * Description:
 * There are n projects, each with start/end days and reward. Find the maximum reward from non-overlapping projects.
 *
 * Approach:
 * - Sort the projects by start day.
 * - Use Dynamic Programming with Memoization + Binary Search (`lower_bound`).
 * - For each project `idx`:
 *   - Option 1 (Skip): `solve(idx+1)`.
 *   - Option 2 (Take): `reward + solve(nextIdx)` where `nextIdx` is found using binary search on start days > current project end day.
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
    int size;
    vector<Project> input;
    vector<long long> memo;

    long long solve(int idx) {
        if (idx == size) return 0;
        if (memo[idx] != -1) return memo[idx];
        long long exclude = solve(idx + 1);
        // Binary search for the first project starting after the current project ends (input[idx].r)
        Project target = {input[idx].r + 1, 0, 0};
        int nextIdx = lower_bound(input.begin() + idx + 1, input.end(), target) - input.begin();
        long long include = input[idx].val + solve(nextIdx);
        return memo[idx] = max(exclude, include);
    }

    long long getMaxReward(int n, vector<Project>& a) {
        size = n;
        input = a;
        sort(input.begin(), input.end());
        memo.assign(size, -1);
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