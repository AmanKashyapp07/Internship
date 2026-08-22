/**
 * LeetCode 1306 - Jump Game III
 *
 * Description:
 * You are given an array of non-negative integers and a starting index.
 *
 * From index i, you can jump either:
 *      i + arr[i]
 *      i - arr[i]
 *
 * Return true if it is possible to reach any index having value 0.
 *
 * Approach:
 * 1. DFS + Memoization.
 * 2. Memo stores whether an index can eventually reach a zero.
 * 3. A visited array is used to detect cycles during the current DFS path.
 * 4. From each index, recursively try both forward and backward jumps.
 *
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 */

class Solution {
public:
    vector<int> memo;

    bool dfs(int i, vector<int>& arr, vector<bool>& vis) {
        if (i < 0 || i >= arr.size())
            return false;

        if (arr[i] == 0)
            return true;

        if (vis[i])
            return false;

        if (memo[i] != -1)
            return memo[i];

        vis[i] = true;

        bool ans = dfs(i + arr[i], arr, vis) ||
                   dfs(i - arr[i], arr, vis);

        vis[i] = false;

        return memo[i] = ans;
    }

    bool canReach(vector<int>& arr, int start) {
        int n = arr.size();

        memo.assign(n, -1);
        vector<bool> vis(n, false);

        return dfs(start, arr, vis);
    }
};