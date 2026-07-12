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

CSES - Tree Matching

Given a tree with n nodes,
find the maximum number of edges in a matching.

A matching is a set of edges such that
no node belongs to more than one chosen edge.
*/

#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 200005;

vector<int> adj[MAXN];
long long dp[MAXN][2];

void dfs(int u, int parent)
{
    long long base = 0;

    for (int v : adj[u])
    {
        if (v == parent)
            continue;

        dfs(v, u);

        base += dp[v][0];
    }

    dp[u][1] = base;
    dp[u][0] = base;

    for (int v : adj[u])
    {
        if (v == parent)
            continue;

        dp[u][0] = max(
            dp[u][0],
            base - dp[v][0] + dp[v][1] + 1
        );
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n - 1; i++)
    {
        int a, b;
        cin >> a >> b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    dfs(1, 0);

    cout << dp[1][0] << '\n';

    return 0;
}
