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

// ============================================================================
// SECTION 1: GRAPH REPRESENTATIONS & BASICS
// ============================================================================


// 3. Traversal Techniques (BFS Helper Example)
void bfs(int startNode, const vector<vi>& adj, vi& visited) {
    queue<int> q;
    visited[startNode] = 1;
    q.push(startNode);
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        for (auto it : adj[node]) {
            if (!visited[it]) {
                visited[it] = 1;
                q.push(it);
            }
        }
    }
}

// 4. DFS Traversal Implementation
void dfs(int node, const vector<vi>& adj, vi& visited) {
    visited[node] = 1;
    for (auto it : adj[node]) {
        if (!visited[it]) {
            dfs(it, adj, visited);
        }
    }
}



// 2. Connected Components Problem in Matrix / Number of Islands
int numIslands(vector<vector<char>>& grid) {
    int n = grid.size();
    if (n == 0) return 0;
    int m = grid[0].size();
    int count = 0;
    
    // Direction vectors for moving 4-directionally (or 8-directionally)
    int delRow[] = {-1, 0, 1, 0};
    int delCol[] = {0, 1, 0, -1};
    
    auto bfsHelper = [&](int r, int c) {
        grid[r][c] = '0'; // Mark as visited
        queue<pii> q;
        q.push({r, c});
        while(!q.empty()){
            int row = q.front().ff;
            int col = q.front().ss;
            q.pop();
            
            for(int i = 0; i < 4; ++i){
                int nrow = row + delRow[i];
                int ncol = col + delCol[i];
                if(nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && grid[nrow][ncol] == '1'){
                    grid[nrow][ncol] = '0';
                    q.push({nrow, ncol});
                }
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '1') {
                count++;
                bfsHelper(i, j);
            }
        }
    }
    return count;
}

// 3. Rotten Oranges (Multi-source BFS)
int orangesRotting(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    queue<pair<pii, int>> q; // {{r, c}, time}
    int cntFresh = 0;
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(grid[i][j] == 2) q.push({{i, j}, 0});
            else if(grid[i][j] == 1) cntFresh++;
        }
    }
    
    int tm = 0, cnt = 0;
    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};
    
    while(!q.empty()){
        int r = q.front().ff.ff;
        int c = q.front().ff.ss;
        int t = q.front().ss;
        tm = max(tm, t);
        q.pop();
        
        for(int i = 0; i < 4; ++i){
            int nrow = r + dRow[i], ncol = c + dCol[i];
            if(nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && grid[nrow][ncol] == 1){
                grid[nrow][ncol] = 2;
                q.push({{nrow, ncol}, t + 1});
                cnt++;
            }
        }
    }
    return (cnt == cntFresh) ? tm : -1;
}

// 4. Flood Fill Algorithm
vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int iniColor = image[sr][sc];
    if (iniColor == newColor) return image;
    
    int n = image.size(), m = image[0].size();
    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};
    
    auto dfsHelper = [&](auto& self, int r, int c) -> void {
        image[r][c] = newColor;
        for(int i = 0; i < 4; ++i){
            int nrow = r + dRow[i], ncol = c + dCol[i];
            if(nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && image[nrow][ncol] == iniColor){
                self(self, nrow, ncol);
            }
        }
    };
    
    dfsHelper(dfsHelper, sr, sc);
    return image;
}



// 7. Distance of nearest cell having 1 (0/1 BFS Matrix)
vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
    int n = mat.size(), m = mat[0].size();
    vector<vi> dist(n, vi(m, INF));
    queue<pii> q;
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(mat[i][j] == 1){ // assuming 1 is target cell
                dist[i][j] = 0;
                q.push({i, j});
            }
        }
    }
    
    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};
    
    while(!q.empty()){
        int r = q.front().ff, c = q.front().ss;
        q.pop();
        for(int i = 0; i < 4; ++i){
            int nr = r + dRow[i], nc = c + dCol[i];
            if(nr >= 0 && nr < n && nc >= 0 && nc < m){
                if(dist[nr][nc] > dist[r][c] + 1){
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
    }
    return dist;
}

// 8 & 9. Surrounded Regions & Number of Enclaves (Boundary BFS/DFS)
void solveSurroundedRegions(vector<vector<char>>& board) {
    int n = board.size(); if(n == 0) return;
    int m = board[0].size();
    
    auto dfsBoundary = [&](auto& self, int r, int c) -> void {
        if(r < 0 || r >= n || c < 0 || c >= m || board[r][c] != 'O') return;
        board[r][c] = '#'; // Temporary flag
        self(self, r+1, c); self(self, r-1, c);
        self(self, r, c+1); self(self, r, c-1);
    };
    
    for(int i = 0; i < n; ++i) {
        if(board[i][0] == 'O') dfsBoundary(dfsBoundary, i, 0);
        if(board[i][m-1] == 'O') dfsBoundary(dfsBoundary, i, m-1);
    }
    for(int j = 0; j < m; ++j) {
        if(board[0][j] == 'O') dfsBoundary(dfsBoundary, 0, j);
        if(board[n-1][j] == 'O') dfsBoundary(dfsBoundary, n-1, j);
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(board[i][j] == 'O') board[i][j] = 'X';
            else if(board[i][j] == '#') board[i][j] = 'O';
        }
    }
}



// 12. Bipartite Graph Verification (DFS/BFS)
bool checkBipartiteDFS(int node, int col, vi& color, const vector<vi>& adj) {
    color[node] = col;
    for (auto it : adj[node]) {
        if (color[it] == -1) {
            if (!checkBipartiteDFS(it, !col, color, adj)) return false;
        } else if (color[it] == col) {
            return false;
        }
    }
    return true;
}

// 13. Cycle Detection in Directed Graph (DFS)
bool isCyclicDirectedDFS(int node, vector<vi>& adj, vi& vis, vi& pathVis) {
    vis[node] = 1;
    pathVis[node] = 1;
    
    for(auto it : adj[node]) {
        if(!vis[it]) {
            if(isCyclicDirectedDFS(it, adj, vis, pathVis)) return true;
        } else if(pathVis[it]) {
            return true;
        }
    }
    pathVis[node] = 0;
    return false;
}

// ============================================================================
// SECTION 3: TOPOLOGICAL SORT AND PROBLEMS
// ============================================================================

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

// 7. Alien Dictionary
string findOrderAlienDictionary(vector<string>& dict, int N, int K) {
    vector<vi> adj(K);
    for(int i = 0; i < N - 1; ++i){
        string s1 = dict[i], s2 = dict[i+1];
        int len = min(s1.size(), s2.size());
        for(int ptr = 0; ptr < len; ++ptr){
            if(s1[ptr] != s2[ptr]){
                adj[s1[ptr] - 'a'].pb(s2[ptr] - 'a');
                break;
            }
        }
    } // add edges based on first differing character
    vi topo = topoSortKahn(K, adj);
    string order = "";
    for(auto it : topo) order += (char)(it + 'a');
    return order;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Call your required function executions here

    return 0;
}