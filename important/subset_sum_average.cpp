/**
 * CSES Problem Set
 * 
 * Problem: Subset Sum Average
 * Link: https://cses.fi/
 * Category: Dynamic Programming
 * 
 * Description:
 * Count subsets of an array whose average equals a target value a.
 * 
 * Logic/Approach:
 * Subtract a from all numbers. The problem reduces to finding subsets that sum to 0. Run subset sum DP using offset array.
 */

#include <iostream>
#include <vector>

using namespace std;

const int MOD = 1e9 + 7;
const int OFFSET = 250000;
const int MAX_SUM = 500000; // The maximum possible sum after balancing is 500,000 (250,000 + 250,000).

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, a;
    if (!(cin >> n >> a)) return 0;

    vector<int> x(n);
    for (int i = 0; i < n; i++) {
        int val;
        cin >> val;
        x[i] = val - a; // The balancing trick
    }

    // We only need two rows to track our states
    vector<int> prev_row(MAX_SUM + 1, 0); // This will hold the number of ways to achieve each sum after considering the first i numbers
    vector<int> curr_row(MAX_SUM + 1, 0); // This will hold the number of ways to achieve each sum after considering the first i+1 numbers
    
    // Base case: The empty set sum is 0 (which sits at OFFSET)
    prev_row[OFFSET] = 1;

    // Go through each number in our array one by one
    for (int i = 0; i < n; i++) {
        int y = x[i];

        // Check every possible sum from 0 to 500,000
        for (int sum = 0; sum <= MAX_SUM; sum++) {
            
            // OPTION 1: Leave it
            // The number of ways to make 'sum' is at least the number of ways 
            // we could make it before looking at this number.
            curr_row[sum] = prev_row[sum];
            
            // OPTION 2: Take it
            // If we take this number, we need to have been able to make 'sum - y' before.
            if(sum - y >= 0 && sum - y <= MAX_SUM) {
                curr_row[sum] = (curr_row[sum] + prev_row[sum - y]) % MOD;
            }
        }
        
        // Our current row is finished. It becomes the previous row for the next number!
        prev_row = curr_row;
    }

    // Our answer is the number of ways to make 0 (OFFSET), minus 1 for the empty set
    long long ans = (prev_row[OFFSET] - 1 + MOD) % MOD;
    
    cout << ans << "\n";

    return 0;
}