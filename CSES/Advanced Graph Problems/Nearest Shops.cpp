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

vector<int> d1, d2;
vector<int> r1, r2;
queue<int> q;

void relax(int node, int dist, int root) {
    if (root == r1[node] || root == r2[node])
        return;

    if (dist < d1[node]) {
        d2[node] = d1[node];
        r2[node] = r1[node];

        d1[node] = dist;
        r1[node] = root;

        q.push(node);
    }
    else if (dist < d2[node]) {
        d2[node] = dist;
        r2[node] = root;

        q.push(node);
    }
} // this function relax is responsible for updating the distances and root shops for a given node, it checks if the new distance is better than the current nearest or second nearest distance, and if so it updates the distances and root shops accordingly, and also pushes the node into the queue to continue the BFS from that node, this way we are able to find the nearest and second nearest shops for all nodes in the graph efficiently using a multi-source BFS approach.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> shops(k);
    vector<bool> isShop(n + 1, false);

    for (int i = 0; i < k; i++) {
        cin >> shops[i];
        isShop[shops[i]] = true;
    }

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    d1.assign(n + 1, INF); // Distance to the nearest shop
    d2.assign(n + 1, INF); // Distance to the second nearest shop
    r1.assign(n + 1, 0); // Root shop for the nearest shop
    r2.assign(n + 1, 0); // Root shop for the second nearest shop

    for (int shop : shops) {
        d1[shop] = 0;
        r1[shop] = shop;
        q.push(shop);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            relax(v, d1[u] + 1, r1[u]);

            if (r2[u] != 0) // If there is a second nearest shop for u, try to relax with that as well
                relax(v, d2[u] + 1, r2[u]);
        }
    }

    for (int i = 1; i <= n; i++) {
        if (isShop[i]) {
            cout << (d2[i] == INF ? -1 : d2[i]) << ' ';
        } else {
            cout << (d1[i] == INF ? -1 : d1[i]) << ' ';
        }
    }

    cout << '\n';
    return 0;
}
