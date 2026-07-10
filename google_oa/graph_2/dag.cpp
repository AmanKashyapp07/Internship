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
// shortest path from source to all other nodes in a weighted directed acyclic graph (DAG) using topological sorting and dynamic programming. The algorithm first computes the topological order of the nodes, then relaxes the edges in that order to find the shortest paths.

vector<int> shortestPathDAG(int n, vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> indeg(n);

    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u])
            indeg[v]++;
    }

    vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indeg[i] == 0)
            q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto [v, wt] : adj[u]) {
            if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
            indeg[v]--;
            if (indeg[v] == 0)
                q.push(v);
        }
    }

    return dist;
}