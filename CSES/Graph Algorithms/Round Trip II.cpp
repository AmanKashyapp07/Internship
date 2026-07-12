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

vector<vector<int>> graph;
vector<int> color, parent;
vector<int> cycle;

bool dfs(int u) {
    color[u] = 1; // currently in recursion stack

    for (int v : graph[u]) {
        if (color[v] == 0) {
            parent[v] = u;

            if (dfs(v))
                return true;
        }
        else if (color[v] == 1) {
            // Found a back edge u -> v
            cycle.push_back(v);

            int cur = u;
            while (cur != v) {
                cycle.push_back(cur);
                cur = parent[cur];
            }

            cycle.push_back(v);
            reverse(cycle.begin(), cycle.end());

            return true;
        }
    }

    color[u] = 2; // fully processed
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    graph.resize(n + 1);
    color.assign(n + 1, 0);
    parent.assign(n + 1, -1);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
    }

    for (int i = 1; i <= n; i++) {
        if (color[i] == 0) {
            if (dfs(i)) {
                cout << cycle.size() << '\n';
                for (int city : cycle) {
                    cout << city << ' ';
                }
                cout << '\n';
                return 0;
            }
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}
