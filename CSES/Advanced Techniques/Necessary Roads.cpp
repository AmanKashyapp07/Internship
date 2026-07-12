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

int n, m;
vector<vector<int>> adj;

vector<int> tin, low;
vector<pair<int, int>> bridges;
int timer = 0;

void dfs(int u, int parent) {
    tin[u] = low[u] = ++timer;

    for (int v : adj[u]) {

        if (v == parent) continue;

        if (tin[v]) {
            // back edge
            low[u] = min(low[u], tin[v]);
        }
        else {
            // tree edge
            dfs(v, u);

            low[u] = min(low[u], low[v]);

            if (low[v] > tin[u]) {
                bridges.push_back({u, v});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    adj.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    tin.assign(n + 1, 0);
    low.assign(n + 1, 0);

    dfs(1, -1);

    cout << bridges.size() << '\n';

    for (auto [u, v] : bridges) {
        cout << u << ' ' << v << '\n';
    }

    return 0;
}
