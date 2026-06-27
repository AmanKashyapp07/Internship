/**
 * LeetCode 90 - Subsets II
 *
 * Description:
 * Given an integer array nums that may contain duplicates, return all possible subsets (the power set).
 * The solution set must not contain duplicate subsets.
 *
 * Approach:
 * - Sort the array first to group duplicate elements.
 * - DFS / Backtracking.
 * - At the start of each recursive call, add the current subset `ds` to the results because every
 *   intermediate state is a valid subset.
 * - Iterate through elements starting from `idx`. Avoid duplicate subsets by skipping identical elements
 *   at the same recursion level using: `if (i > idx && nums[i] == nums[i - 1]) continue;`.
 *
 * Time Complexity: O(2^N * N) where N is the number of elements.
 * Space Complexity: O(N) for the recursion stack and current path.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

void generateSubsets(int idx, vi& nums, vi& ds, vector<vi>& ans) {
    ans.pb(ds);

    for (int i = idx; i < nums.size(); i++) {
        // Skip duplicates at the same level
        if (i > idx && nums[i] == nums[i - 1])
            continue;

        ds.pb(nums[i]);

        generateSubsets(i + 1, nums, ds, ans);

        ds.pop_back(); // backtrack
    } // here there is for loop because we are iterating over the array and generating subsets, so we need to backtrack after each recursive call to explore other possibilities, it;s not simple pick
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vi nums(n);
    for (int &x : nums)
        cin >> x;

    sort(all(nums));

    vector<vi> ans;
    vi ds;

    generateSubsets(0, nums, ds, ans);

    for (auto &subset : ans) {
        cout << "{ ";
        for (int x : subset)
            cout << x << " ";
        cout << "}\n";
    }

    return 0;
}
