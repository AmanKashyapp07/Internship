// Link: https://cses.fi/problemset/task/2417

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
const int MAX_VAL = 1000000;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> count_freq(MAX_VAL + 1, 0); // Frequency array to count occurrences of each number
    int max_x = 0; 
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        count_freq[x]++;
        if (x > max_x) {
            max_x = x;
        }
    }

    // E[d] will store the number of "Exact Pairs" where the GCD is strictly equal to 'd'
    vector<long long> E(max_x + 1, 0);

    // We iterate backwards from the maximum value down to 1.
    // This ensures that when we are calculating the exact pairs for 'd', 
    // the exact pairs for all multiples of 'd' (2d, 3d, etc.) have already been calculated.
    for (int d = max_x; d >= 1; d--) {
        long long multiples_count = 0;
        for (int m = d; m <= max_x; m += d) {
            multiples_count += count_freq[m];
        }

        // Step 2: Calculate "Naive Pairs" (P(d))
        // This is the number of pairs where both numbers are multiples of 'd'.
        // Combination formula: nC2 = n * (n - 1) / 2
        long long naive_pairs = (multiples_count * (multiples_count - 1)) / 2;

        // Initialize exact pairs with naive pairs
        E[d] = naive_pairs;

        // Step 3: Subtract the overcounted exact pairs
        // A pair whose exact GCD is a strict multiple of 'd' (like 2d, 3d) 
        // was included in naive_pairs. We must subtract them to find pairs 
        // where the GCD is *exactly* 'd'.
        for (int m = 2 * d; m <= max_x; m += d) {
            E[d] -= E[m];
        }
    }

    // E[1] now holds the number of pairs where the exact GCD is 1 (i.e., coprime pairs)
    cout << E[1] << "\n";

    return 0;
}