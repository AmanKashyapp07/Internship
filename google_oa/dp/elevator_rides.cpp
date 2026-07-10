#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;


class Solution {
public:
    int getMinElevatorRides(int n, int x, vector<int>& w) {
        int N = 1 << n;  // 2^n states
        
        // dp[mask] = {minimum rides needed, minimum weight in the last ride}
        vector<pair<int, int>> dp(N, {n + 1, 0});  
        dp[0] = {1, 0};  // Starting with 1 empty ride

        for (int mask = 0; mask < N; ++mask) {
            auto [rides, lastWeight] = dp[mask];

            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) continue;  // person already placed

                int newMask = mask | (1 << i);
                pair<int, int> option;

                if (lastWeight + w[i] <= x) {
                    // Add to current ride
                    option = {rides, lastWeight + w[i]};
                } else {
                    // Start a new ride
                    option = {rides + 1, w[i]};
                }

                // Keep the better option
                if (option < dp[newMask]) {
                    dp[newMask] = option;
                }
            }
        }

        return dp[N - 1].first;
    }
};

int main() {
    int n, x;
    cin >> n >> x;
    
    vector<int> w(n);
    for (int &val : w) cin >> val;

    Solution solver;
    cout << solver.getMinElevatorRides(n, x, w) << endl;

    return 0;
}