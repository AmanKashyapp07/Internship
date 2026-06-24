// ============================================================================
// SECTION 5: MINIMUM SPANNING TREE & DISJOINT SET PROBLEMS
// ============================================================================

// 1. Disjoint Set Union (DSU) Structure (Union by Rank & Size with Path Compression)
class DisjointSet {
public:
    vi rank, parent, size;
    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 0; i <= n; i++) parent[i] = i;
    }

    int findUPar(int node) {
        if (node == parent[node]) return node;
        return parent[node] = findUPar(parent[node]); // Path compression
    }

    void unionByRank(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v;
        } else if (rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u;
        } else {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }

    void unionBySize(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};

// 2 & 4. Prim's Algorithm (Find MST Weight & Tree)
int primsMST(int V, const vector<vector<pii>>& adj) {
    priority_queue<pii, vector<pii>, greater<pii>> pq; // {weight, node}
    vi vis(V, 0);
    pq.push({0, 0});
    int sum = 0;

    while (!pq.empty()) {
        int wt = pq.top().ff;
        int node = pq.top().ss;
        pq.pop();

        if (vis[node] == 1) continue;
        vis[node] = 1;
        sum += wt;

        for (auto it : adj[node]) {
            int adjNode = it.ff;
            int edW = it.ss;
            if (!vis[adjNode]) {
                pq.push({edW, adjNode});
            }
        }
    }
    return sum;
}

// 5. Number of Operations to Make Network Connected
int makeConnected(int n, vector<vector<int>>& connections) {
    if ((int)connections.size() < n - 1) return -1;
    DisjointSet ds(n);
    int extraEdges = 0;
    for (auto it : connections) {
        if (ds.findUPar(it[0]) == ds.findUPar(it[1])) {
            extraEdges++;
        } else {
            ds.unionBySize(it[0], it[1]);
        }
    }
    int components = 0;
    for (int i = 0; i < n; i++) {
        if (ds.parent[i] == i) components++;
    }
    return components - 1;
} 

int removeStones(vector<vector<int>>& stones) {

        int maxRow = 0, maxCol = 0;

        for (const auto& stone : stones) {
            maxRow = max(maxRow, stone[0]);
            maxCol = max(maxCol, stone[1]);

        }

        DisjointSet ds(maxRow + maxCol + 2);]
        unordered_set<int> usedNodes;

        for (const auto& stone : stones) {
            int rowNode = stone[0];
            int colNode = stone[1] + maxRow + 1;
            ds.unionBySize(rowNode, colNode); // unionBySize(rowNode, colNode) groups together all stones that are directly or indirectly connected through shared rows or columns. Each connected component can be reduced to one stone, so the answer is totalStones - numberOfConnectedComponents.
            usedNodes.insert(rowNode);
            usedNodes.insert(colNode);

        }

        int components = 0;

        for (int node : usedNodes) {
            if (ds.findUPar(node) == node) components++;
        }

        return stones.size() - components;

    }
// answer is total stones - number of connected components. Each connected component can be reduced to 1 stone, so we can remove all other stones in that component.
// a connected component is formed by stones that share the same row or column. We can use DSU to find the number of connected components. Each stone can be represented as a node in the graph, and we can connect nodes that share the same row or column. The number of connected components can be found by counting the number of unique parents in the DSU after processing all stones. The final answer is the total number of stones minus the number of connected components.
// 7. Accounts Merge


vi numIslandsII(int n, int m, vector<vector<int>>& queries) {

    // DSU for all cells in the grid
    DisjointSet ds(n * m);

    // Tracks whether a cell is currently land or water
    vector<vi> vis(n, vi(m, 0));

    // Stores answer after every query
    vi ans;

    // Current number of islands
    int cnt = 0;

    // 4-direction movement
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    for (auto it : queries) {

        int row = it[0];
        int col = it[1];

        // If land already exists here,
        // island count remains unchanged
        if (vis[row][col] == 1) {
            ans.pb(cnt);
            continue;
        }

        // Convert water -> land
        vis[row][col] = 1;

        // Assume this creates a new island
        cnt++;

        // Check all 4 neighbours
        for (int i = 0; i < 4; i++) {

            int adjr = row + dr[i];
            int adjc = col + dc[i];

            // Valid neighbour?
            if (adjr >= 0 && adjr < n &&
                adjc >= 0 && adjc < m) {

                // Neighbour is land
                if (vis[adjr][adjc] == 1) {

                    // Convert 2D cell to DSU node number
                    int nodeNo = row * m + col;
                    int adjNodeNo = adjr * m + adjc;

                    // If both lands belong to different islands
                    if (ds.findUPar(nodeNo) != ds.findUPar(adjNodeNo)) {

                        // Merging two islands into one
                        cnt--;

                        ds.unionBySize(nodeNo, adjNodeNo);
                    }
                }
            }
        }

        // Store island count after this query
        ans.pb(cnt);
    }

    return ans;
}

// 9. Making a Large Island
int largestIsland(vector<vector<int>>& grid) {
    int n = grid.size();
    DisjointSet ds(n * n);
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    // Step 1: Connect existing 1s
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 0) continue;
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i], nc = c + dc[i];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    ds.unionBySize(r * n + c, nr * n + nc);
                }
            }
        }
    }

    // Step 2: Try converting each 0 to 1
    int mx = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 1) continue;
            set<int> components;
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i], nc = c + dc[i];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    components.insert(ds.findUPar(nr * n + nc));
                }
            }
            // components holds distinct connected components of 1s adjacent to the current 0 cell. We can calculate the total size of the new island formed by converting this 0 to 1 by summing the sizes of these distinct components and adding 1 for the current cell.
            int sizeTotal = 1;
            for (auto it : components) sizeTotal += ds.size[it];
            mx = max(mx, sizeTotal);
        }
    }
    // Handle the fallback where grid has no 0s
    return mx == 0 ? n * n : mx;
}

// 10. Swim in Rising Water (DSU or Dijkstra variant)
int swimInWater(vector<vector<int>>& grid) {
    int n = grid.size();
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> pq; // {max_height, {r, c}}
    vector<vi> vis(n, vi(n, 0));

    pq.push({grid[0][0], {0, 0}});
    vis[0][0] = 1;
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!pq.empty()) {
        int t = pq.top().ff;
        int r = pq.top().ss.ff;
        int c = pq.top().ss.ss;
        pq.pop();

        if (r == n - 1 && c == n - 1) return t;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !vis[nr][nc]) {
                vis[nr][nc] = 1;
                pq.push({max(t, grid[nr][nc]), {nr, nc}}); // max(t, grid[nr][nc]) ensures that we always consider the maximum height encountered along the path to reach (nr, nc). This is crucial because we can only move to a cell if the water level has risen to at least the height of that cell.
            }
        }
    }
    return 0;
}

// ============================================================================
// SECTION 6: ADVANCED / OTHER GRAPH ALGORITHMS
// ============================================================================

// 1. Bridges in a Graph (Tarjan's Bridge-Finding Algorithm)
void dfsBridge(int node, int parent, int& timer, vi& tin, vi& low, vi& vis, 
               const vector<vi>& adj, vector<vector<int>>& bridges) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    for (auto it : adj[node]) {
        if (it == parent) continue;
        if (!vis[it]) {
            dfsBridge(it, node, timer, tin, low, vis, adj, bridges);
            low[node] = min(low[node], low[it]);
            if (low[it] > tin[node]) {
                bridges.pb({node, it});
            }
        } else {
            low[node] = min(low[node], tin[it]);
        }
    }
}

vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
    vector<vi> adj(n);
    for (auto it : connections) {
        adj[it[0]].pb(it[1]);
        adj[it[1]].pb(it[0]);
    }
    vi tin(n, -1), low(n, -1), vis(n, 0);
    vector<vector<int>> bridges;
    int timer = 0;
    dfsBridge(0, -1, timer, tin, low, vis, adj, bridges);
    return bridges;
}

// 2. Articulation Points in a Graph
void dfsArticulation(int node, int parent, int& timer, vi& tin, vi& low, vi& vis,
                      vi& mark, const vector<vi>& adj) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    int child = 0;
    for (auto it : adj[node]) {
        if (it == parent) continue;
        if (!vis[it]) {
            dfsArticulation(it, node, timer, tin, low, vis, mark, adj);
            low[node] = min(low[node], low[it]);
            if (low[it] >= tin[node] && parent != -1) {
                mark[node] = 1;
            }
            child++;
        } else {
            low[node] = min(low[node], tin[it]);
        }
    }
    if (child > 1 && parent == -1) mark[node] = 1;
}

vi articulationPoints(int V, vector<vi>& adj) {
    vi tin(V), low(V), vis(V, 0), mark(V, 0);
    int timer = 0;
    for (int i = 0; i < V; i++) {
        if (!vis[i]) dfsArticulation(i, -1, timer, tin, low, vis, mark, adj);
    }
    vi ans;
    for (int i = 0; i < V; i++) if (mark[i] == 1) ans.pb(i);
    if (ans.size() == 0) return {-1};
    return ans;
}

// 3. Kosaraju's Algorithm (Strongly Connected Components in Directed Graph)
void dfsKosaraju(int node, vi& vis, const vector<vi>& adj, stack<int>& st) {
    vis[node] = 1;
    for (auto it : adj[node]) {
        if (!vis[it]) dfsKosaraju(it, vis, adj, st);
    }
    st.push(node);
}

void dfsReverse(int node, vi& vis, const vector<vi>& adjRev) {
    vis[node] = 1;
    for (auto it : adjRev[node]) {
        if (!vis[it]) dfsReverse(it, vis, adjRev);
    }
}

int kosarajuSCC(int V, vector<vi>& adj) {
    vi vis(V, 0);
    stack<int> st;
    // Step 1: Sort all nodes based on their finishing times
    for (int i = 0; i < V; i++) {
        if (!vis[i]) dfsKosaraju(i, vis, adj, st);
    }

    // Step 2: Transpose/Reverse graph matrices
    vector<vi> adjRev(V);
    for (int i = 0; i < V; i++) {
        vis[i] = 0; // reset visited array for Step 3
        for (auto it : adj[i]) {
            adjRev[it].pb(i);
        }
    }

    // Step 3: Extract blocks elements using finishing sequence ordering
    int sccCount = 0;
    while (!st.empty()) {
        int node = st.top();
        st.pop();
        if (!vis[node]) {
            sccCount++;
            dfsReverse(node, vis, adjRev);
        }
    }
    return sccCount;
}