/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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



// shortest path in DAG (directed acyclic graph) using topological sort

vector<int> shortestPathDAG(int n, vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> indeg(n);

    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u]) {
            indeg[v]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    vector<int> topo;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        topo.push_back(u);

        for (auto [v, wt] : adj[u]) {
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }
    }

    vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    for (int u : topo) {
        if (dist[u] == INT_MAX) continue;

        for (auto [v, wt] : adj[u]) {
            dist[v] = min(dist[v], dist[u] + wt);
        }
    }

    return dist;
}