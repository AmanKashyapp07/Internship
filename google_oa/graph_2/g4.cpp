/**
 * CSES Problem Set + Graph Algorithms
 *
 * Problem: Comprehensive Graph Problems Template
 * Category: Graphs (BFS, DFS, Topo Sort)
 *
 * Description:
 * Includes structures and boilerplate code for solving common graph problems 
 * from the standard roadmap.
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

// 1 & 2. Topological Sort / Kahn's Algorithm (BFS-based)
vi topoSortKahn(int V, vector<vi>& adj) {
    vi indegree(V, 0);
    for(int i = 0; i < V; ++i) {
        for(auto it : adj[i]) indegree[it]++;
    }
    
    queue<int> q;
    for(int i = 0; i < V; ++i) {
        if(indegree[i] == 0) q.push(i);
    }
    
    vi topo;
    while(!q.empty()){
        int node = q.front();
        q.pop();
        topo.pb(node);
        
        for(auto it : adj[node]){
            indegree[it]--;
            if(indegree[it] == 0) q.push(it);
        }
    }
    return topo;
}

// 3. Cycle Detection in Directed Graph using Kahn's Algorithm
bool isCyclicKahn(int V, vector<vi>& adj) {
    vi topo = topoSortKahn(V, adj);
    return (topo.size() < (size_t)V); // If size is less than V, cycle exists
}

// 4 & 5. Course Schedule I & II
vi findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vi> adj(numCourses);
    for(auto& pre : prerequisites){
        adj[pre[1]].pb(pre[0]);
    }
    vi topo = topoSortKahn(numCourses, adj);
    if(topo.size() == (size_t)numCourses) return topo;
    return {};
}

// 6. Find Eventual Safe States
vi eventualSafeNodes(int V, vector<vi>& adj) {
    // A node is safe if all paths leading from it go to a terminal node.
    // Equivalent to reversing edges and running Kahn's algorithm
    vector<vi> reverse(V); // build reverse graph and push nodes with indegree 0 into queue
    vi indegree(V, 0);
    for(int i = 0; i < V; ++i){
        for(auto it : adj[i]){
            reverse[it].pb(i);
            indegree[i]++;
        }
    }
    queue<int> q;
    for(int i = 0; i < V; ++i) if(indegree[i] == 0) q.push(i);
    
    vi safeNodes;
    while(!q.empty()){
        int node = q.front(); q.pop();
        safeNodes.pb(node);
        for(auto it : reverse[node]){
            indegree[it]--;
            if(indegree[it] == 0) q.push(it);
        }
    }
    sort(all(safeNodes));
    return safeNodes;
}
