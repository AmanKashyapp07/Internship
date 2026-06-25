/**
 * Hamiltonian Path (Bitmask DP)
 *
 * State:
 * dp[mask][u] = number of ways to reach node u
 *               after visiting exactly the nodes in mask.
 *
 * Transition:
 * For every edge u -> v,
 * if v is not visited,
 *     dp[mask | (1 << v)][v] += dp[mask][u]
 *
 * Base:
 * dp[1 << start][start] = 1
 *
 * Answer:
 * dp[(1 << n) - 1][end]
 *
 * Time:  O((V + E) * 2^V)
 * Space: O(V * 2^V)
 */
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
    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        graph[u].push_back(v);
    }

    vector<vector<long long>> dp(1 << n, vector<long long>(n, 0)); // dp stores (mask, u) = number of ways to reach node u after visiting exactly the nodes in mask

    dp[1][0] = 1;   // start from node 0

    for (int mask = 1; mask < (1 << n); mask++) {

        for (int u = 0; u < n; u++) {

            if (!(mask & (1 << u))) // if node u is not visited in the current mask, skip it
                continue;

            if (dp[mask][u] == 0) // if there are no ways to reach node u with the current mask, skip it
                continue;

            for (int v : graph[u]) {

                if (mask & (1 << v)) // if node v is already visited in the current mask, skip it
                    continue;

                int newMask = mask | (1 << v); // create a new mask that includes node v as visited

                dp[newMask][v] += dp[mask][u]; // update the number of ways to reach node v with the new mask by adding the number of ways to reach node u with the current mask
                dp[newMask][v] %= MOD;
            }
        }
    }

    cout << dp[(1 << n) - 1][n - 1] << '\n';
}