/**
 * Problem: Matrix Chain Multiplication (Memoization)
 *
 * Description:
 * Given a sequence of matrices, find the most efficient way to multiply these matrices together.
 * The problem is not actually to perform the multiplications, but merely to decide in which order to perform the multiplications.
 *
 * Approach:
 * - Interval Dynamic Programming with Memoization.
 * - Let `solve(i, j)` return the minimum scalar multiplications to multiply matrices from index `i` to `j`.
 * - Transition: Try splitting the product at position `k` (where `i <= k < j`).
 *   - `solve(i, j) = min(solve(i, k) + solve(k+1, j) + arr[i-1] * arr[k] * arr[j])`.
 *
 * Time Complexity: O(n^3)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;
using ll = long long;

class Solution {
public:
    vector<int> input;
    vector<vector<ll>> memo;

    ll solve(int i, int j) {
        if (i == j) return 0; // base case: one matrix needs 0 multiplications
        if (memo[i][j] != -1) return memo[i][j];

        ll ans = LLONG_MAX;
        for (int k = i; k < j; k++) {
            ll cost = solve(i, k) + solve(k + 1, j) + 1LL * input[i - 1] * input[k] * input[j];
            ans = min(ans, cost);
        }

        return memo[i][j] = ans;
    }

    ll matrixMultiplication(vector<int>& arr) {
        input = arr;
        int n = arr.size();
        memo.assign(n, vector<ll>(n, -1));
        return solve(1, n - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    Solution solver;
    cout << solver.matrixMultiplication(arr) << '\n';

    return 0;
}
