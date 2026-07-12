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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

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
