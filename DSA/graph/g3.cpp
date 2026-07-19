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
/**
 * Problem: Multi-Source Breadth-First Search (BFS)
 * Find the shortest distances to all vertices starting from multiple source vertices simultaneously.
 * Places all sources into the queue initially with distance zero, then performs standard level BFS exploration.
 * Complexity: O(V + E) time and O(V) space.
 */
vector<int> multiSourceBFS(int n, vector<vector<int>>& graph, vector<int>& sources) {
    vector<int> dist(n, INF);
    queue<int> q;
    for (int src : sources) {
        dist[src] = 0;
        q.push(src);
    }
    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int nbr : graph[node]) {
            if (dist[nbr] > dist[node] + 1) {
                dist[nbr] = dist[node] + 1;
                q.push(nbr);
            }
        }
    }
    return dist;
}

/**
 * Problem: 0-1 BFS (Shortest Path on Binary Weights)
 * Find the shortest path from a source vertex where edge weights can only be 0 or 1.
 * Uses a double-ended queue (deque) to push 0-weight edges to the front and 1-weight edges to the back.
 * Complexity: O(V + E) time, which is more efficient than Dijkstra's algorithm for binary weights.
 */
vector<int> zeroOneBFS(int n, vector<vector<pair<int, int>>>& graph, int src) {
    vector<int> dist(n, INF);
    deque<int> dq;
    dist[src] = 0;
    dq.push_front(src);
    while (!dq.empty()) {
        int node = dq.front(); dq.pop_front();
        for (auto [nbr, wt] : graph[node]) {
            if (dist[node] + wt < dist[nbr]) {
                dist[nbr] = dist[node] + wt;
                if (wt == 0) dq.push_front(nbr);
                else dq.push_back(nbr);
            }
        }
    }
    return dist;
}

/**
 * Problem: K Shortest Paths (k-th optimal path search)
 * Determine the top k shortest paths from a source node to a destination node in a weighted graph.
 * Uses a modified Dijkstra's algorithm that keeps track of the visit count (cnt) for each vertex up to k times.
 * Complexity: O(K * E log V) time complexity.
 */
vector<ll> kShortestPaths(int n, vector<vector<pair<int, int>>>& graph, int src, int dest, int k) {
    vector<int> cnt(n, 0);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({0, src});
    vector<ll> answer;
    while (!pq.empty()) {
        auto [dist, node] = pq.top(); pq.pop();
        cnt[node]++;
        if (node == dest) {
            answer.push_back(dist);
            if (answer.size() == k) return answer;
        }
        if (cnt[node] > k) continue;
        for (auto [nbr, wt] : graph[node]) {
            pq.push({dist + wt, nbr});
        }
    }
    return answer;
}

vector<int> tin, tout, euler;
int timer = 0;

/**
 * Problem: Euler Tour (Tree Flattening Technique)
 * Flatten a tree structure into a linear array to support subtree queries and range updates.
 * Performs a DFS traversal to record entry times (tin), exit times (tout), and the traversal order.
 * Complexity: O(V) time and space, where subtrees correspond to contiguous subsegments [tin[node], tout[node]].
 */
void eulerTour(int node, int parent, vector<vector<int>>& tree) {
    tin[node] = timer++;
    euler.push_back(node);
    for (int child : tree[node]) {
        if (child == parent) continue;
        eulerTour(child, node, tree);
    }
    tout[node] = timer - 1;
}
