#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <utility>
#include <tuple>
#include <string>
#include <functional>

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

vector<int> bfs(int n, vector<vector<int>>& adj, int src) {
    vector<int> vis(n, 0), order;
    queue<int> q;
    q.push(src);
    vis[src] = 1;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        order.push_back(node);
        for (int nbr : adj[node]) {
            if (!vis[nbr]) {
                vis[nbr] = 1;
                q.push(nbr);
            }
        }
    }
    return order;
}

void dfs(int node, vector<vector<int>>& adj, vector<int>& vis) {
    vis[node] = 1;
    for (int nbr : adj[node]) {
        if (!vis[nbr]) dfs(nbr, adj, vis);
    }
}

int countComponents(int n, vector<vector<int>>& adj) {
    vector<int> vis(n, 0);
    int components = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            components++;
            dfs(i, adj, vis);
        }
    }
    return components;
}

vector<int> kahnTopo(int n, vector<vector<int>>& adj) {
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) indegree[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    vector<int> topo;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        topo.push_back(node);
        for (int nbr : adj[node]) {
            indegree[nbr]--;
            if (indegree[nbr] == 0) q.push(nbr);
        }
    }
    return topo;
}

vector<int> shortestPathUnweighted(int n, vector<vector<int>>& adj, int src) {
    vector<int> dist(n, INF);
    queue<int> q;
    q.push(src);
    dist[src] = 0;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int nbr : adj[node]) {
            if (dist[node] + 1 < dist[nbr]) {
                dist[nbr] = dist[node] + 1;
                q.push(nbr);
            }
        }
    }
    return dist;
}

vector<ll> dijkstra(int n, vector<vector<pair<int, int>>>& adj, int src) {
    vector<ll> dist(n, LINF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, node] = pq.top(); pq.pop();
        if (d > dist[node]) continue;
        for (auto [nbr, wt] : adj[node]) {
            if (dist[node] + wt < dist[nbr]) {
                dist[nbr] = dist[node] + wt;
                pq.push({dist[nbr], nbr});
            }
        }
    }
    return dist;
}

vector<ll> shortestPathDAG(int n, vector<vector<pair<int, int>>>& adj, int src) {
    vector<vector<int>> dag(n);
    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u]) dag[u].push_back(v);
    }
    vector<int> topo = kahnTopo(n, dag);
    vector<ll> dist(n, LINF);
    dist[src] = 0;
    for (int node : topo) {
        if (dist[node] == LINF) continue;
        for (auto [nbr, wt] : adj[node]) {
            dist[nbr] = min(dist[nbr], dist[node] + wt);
        }
    }
    return dist;
}

vector<ll> bellmanFord(int n, vector<vector<int>>& edges, int src) {
    vector<ll> dist(n, LINF);
    dist[src] = 0;
    for (int i = 1; i <= n - 1; i++) {
        for (auto& e : edges) {
            int u = e[0], v = e[1], wt = e[2];
            if (dist[u] != LINF && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }
    return dist;
}   

bool hasNegativeCycle(int n, vector<vector<int>>& edges, vector<ll>& dist) {
    for (auto& e : edges) {
        int u = e[0], v = e[1], wt = e[2];
        if (dist[u] != LINF && dist[u] + wt < dist[v]) return true;
    }
    return false;
}

void floydWarshall(vector<vector<ll>>& dist) {
    int n = dist.size();
    for(int i = 0; i < n; i++) dist[i][i] = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                if(dist[j][i] != LINF && dist[i][k] != LINF) {
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);
                }
            }
        }
    }
}
