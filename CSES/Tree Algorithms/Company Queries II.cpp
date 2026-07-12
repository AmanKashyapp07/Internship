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

const int MAX_NODES = 200005;
const int LOG = 20;

vector<int> adj[MAX_NODES];
int up[MAX_NODES][LOG];
int depth[MAX_NODES];

void dfs(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p;

    for (int j = 1; j < LOG; j++) {
        up[u][j] = up[up[u][j - 1]][j - 1];
    }

    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) {
        swap(u, v);
    }

    int diff = depth[u] - depth[v];

    for (int j = LOG - 1; j >= 0; j--) {
        if ((diff >> j) & 1) {
            u = up[u][j];
        }
    }

    if (u == v) {
        return u;
    }

    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }

    return up[u][0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    // Company Queries II input:
    // boss of employees 2..n
    for (int i = 2; i <= n; i++) {
        int boss;
        cin >> boss;

        adj[boss].push_back(i);
        adj[i].push_back(boss);
    }

    dfs(1, 1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;

        cout << get_lca(a, b) << '\n';
    }

    return 0;
}
