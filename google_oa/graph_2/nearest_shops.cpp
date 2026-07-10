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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;
using namespace std;

const int INF = 1e9;

vector<int> d1, d2, r1, r2;
queue<int> q;

void relax(int v, int d, int r) {
    if (r == r1[v] || r == r2[v]) return;

    if (d < d1[v]) {
        d2[v] = d1[v];
        r2[v] = r1[v];
        d1[v] = d;
        r1[v] = r;
        q.push(v);
    }
    else if (d < d2[v]) {
        d2[v] = d;
        r2[v] = r;
        q.push(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<int>> adj(n + 1);
    vector<bool> shop(n + 1);

    d1.assign(n + 1, INF);
    d2.assign(n + 1, INF);
    r1.assign(n + 1, 0);
    r2.assign(n + 1, 0);

    while (k--) {
        int x;
        cin >> x;
        shop[x] = true;
        d1[x] = 0;
        r1[x] = x;
        q.push(x);
    }

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            relax(v, d1[u] + 1, r1[u]);
            if (r2[u]) relax(v, d2[u] + 1, r2[u]);
        }
    }

    for (int i = 1; i <= n; i++)
        cout << (shop[i] ? (d2[i] == INF ? -1 : d2[i])
                         : (d1[i] == INF ? -1 : d1[i]))
             << ' ';
}