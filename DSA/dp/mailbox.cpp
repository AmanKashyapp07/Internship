class Solution {
public:
    int n;
    vector<vector<int>> cost;
    vector<vector<int>> dp;

    int solve(int idx, int k, vector<int>& houses) {
        if (idx == n) return 0;
        if (k == 0) return 1e9; // If there are no mailboxes left to place, return a large number to indicate it's not possible.

        if (dp[idx][k] != -1)
            return dp[idx][k];

        int ans = 1e9;

        // First mailbox serves houses [idx ... j]
        for (int j = idx; j < n; j++) {
            ans = min(ans, cost[idx][j] + solve(j + 1, k - 1, houses));
        }

        return dp[idx][k] = ans;
    } // idx means the index of the house we are currently considering, and k is the number of mailboxes left to place.

    int minDistance(vector<int>& houses, int k) {
        sort(houses.begin(), houses.end());

        n = houses.size();

        // cost[i][j] = minimum cost to cover houses i..j with one mailbox
        cost.assign(n, vector<int>(n, 0));

        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                int mid = (i + j) / 2;
                for (int t = i; t <= j; t++)
                    cost[i][j] += abs(houses[t] - houses[mid]);
            }
        }

        dp.assign(n, vector<int>(k + 1, -1));

        return solve(0, k, houses);
    }
};