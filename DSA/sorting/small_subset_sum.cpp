/**
 * CSES Problem Set
 * 
 * Problem: Missing Coin Sum
 * Link: https://cses.fi/problemset/task/2183
 * Category: Sorting and Searching
 * 
 * Description:
 * Find the smallest sum that cannot be formed by a subset of coin values.
 * 
 * Logic/Approach:
 * Sort coin values. Maintain reachable sum target. If current coin > target, then target is the smallest impossible sum.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    // Step 1: Sort the coins in ascending order
    sort(coins.begin(), coins.end());

    // Step 2: Initialize the smallest impossible sum to 1
    // We use long long because the maximum possible sum could exceed the 32-bit integer limit
    long long target = 1;

    // Step 3: Iterate through the sorted coins
    for (int i = 0; i < n; i++) {
        // If the current coin is greater than our target, we've found the gap
        if (coins[i] > target) {
            break; 
        }
        // Otherwise, add the coin's value to extend our reachable range
        target += coins[i];
    }

    // Print the answer
    cout << target << "\n";

    return 0;
}