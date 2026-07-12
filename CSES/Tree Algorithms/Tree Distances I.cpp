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
    int n = adj.size();

    vector<int> dist(n, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    int farthest = start;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);

                if (dist[v] > dist[farthest]) {
                    farthest = v;
                }
            }
        }
    }

    return {farthest, dist[farthest]};
}

void calculateDistances(int start, const vector<vector<int>>& adj, vector<int>& dist) {
    int n = adj.size();
    dist.assign(n, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    auto [u, _] = bfs(0, adj);
    auto [v, diameter] = bfs(u, adj);
    vector<int> dist1(n, -1);
    vector<int> dist2(n, -1);

    calculateDistances(u, adj, dist1);
    calculateDistances(v, adj, dist2);
    for(int i=0; i<n; i++) {
        cout << max(dist1[i], dist2[i]) << ' ';
    }
    cout << '\n';
}
