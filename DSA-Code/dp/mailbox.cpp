#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int size;
    vector<vector<int>> costs;
    vector<vector<int>> memo;
    vector<int> input;

    int solve(int idx, int k) {
        if (idx == size) return 0;
        if (k == 0) return 1e9; // If there are no mailboxes left to place, return a large number to indicate it's not possible.

        if (memo[idx][k] != -1)
            return memo[idx][k];

        int ans = 1e9;

        // First mailbox serves houses [idx ... j]
        for (int j = idx; j < size; j++) {
            ans = min(ans, costs[idx][j] + solve(j + 1, k - 1));
        }

        return memo[idx][k] = ans;
    } // idx means the index of the house we are currently considering, and k is the number of mailboxes left to place.

    int minDistance(vector<int>& houses, int k) {
        input = houses;
        sort(input.begin(), input.end());

        size = input.size();

        // costs[i][j] = minimum cost to cover houses i..j with one mailbox
        costs.assign(size, vector<int>(size, 0));

        for (int i = 0; i < size; i++) {
            for (int j = i; j < size; j++) {
                int mid = (i + j) / 2;
                for (int t = i; t <= j; t++)
                    costs[i][j] += abs(input[t] - input[mid]);
            }
        }

        memo.assign(size, vector<int>(k + 1, -1));

        return solve(0, k);
    }
};