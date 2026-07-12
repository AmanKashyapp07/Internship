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

pair<int, int> bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size() - 1;

    vector<int> dist(n + 1, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    int farthestNode = start;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] != -1) continue;

            dist[v] = dist[u] + 1;
            q.push(v);

            if (dist[v] > dist[farthestNode]) {
                farthestNode = v;
            }
        }
    }

    return {farthestNode, dist[farthestNode]};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    auto [A, _] = bfs(1, adj);
    auto [B, diameter] = bfs(A, adj);

    cout << diameter << '\n';

    return 0;
}
