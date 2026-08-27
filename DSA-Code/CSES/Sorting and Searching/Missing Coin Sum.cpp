// Link: https://cses.fi/problemset/task/2183
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<long long> coins(n);
    for (int i = 0; i < n; i++) cin >> coins[i];
    sort(coins.begin(), coins.end());

    long long target = 1;
    for (int i = 0; i < n; i++) {
        if (coins[i] > target) break;
        target += coins[i];
    }
    cout << target << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find smallest positive sum that cannot be formed by a subset of coin values (CSES 2183).
// - Approach: Greedy Range Extension on Sorted Coins.
// - Intuition: If sorted coins form all sums $[1..target-1]$ and next coin is $x \le target$, new reachable range extends to $[1..target-1+x]$.
// - Complexity: Time: O(N \log N), Space: O(N).