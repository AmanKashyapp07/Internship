#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int n_;
    vector<vector<int>> cost_;
    vector<vector<int>> dp_;
    vector<int> houses_;

    int solve(int idx, int k) {
        if (idx == n_) return 0;
        if (k == 0) return 1e9; // If there are no mailboxes left to place, return a large number to indicate it's not possible.

        if (dp_[idx][k] != -1)
            return dp_[idx][k];

        int ans = 1e9;

        // First mailbox serves houses [idx ... j]
        for (int j = idx; j < n_; j++) {
            ans = min(ans, cost_[idx][j] + solve(j + 1, k - 1));
        }

        return dp_[idx][k] = ans;
    } // idx means the index of the house we are currently considering, and k is the number of mailboxes left to place.

    int minDistance(vector<int>& houses, int k) {
        houses_ = houses;
        sort(houses_.begin(), houses_.end());

        n_ = houses_.size();

        // cost_[i][j] = minimum cost to cover houses i..j with one mailbox
        cost_.assign(n_, vector<int>(n_, 0));

        for (int i = 0; i < n_; i++) {
            for (int j = i; j < n_; j++) {
                int mid = (i + j) / 2;
                for (int t = i; t <= j; t++)
                    cost_[i][j] += abs(houses_[t] - houses_[mid]);
            }
        }

        dp_.assign(n_, vector<int>(k + 1, -1));

        return solve(0, k);
    }
};