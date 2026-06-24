// ============================================================================
// SECTION 4: SHORTEST PATH ALGORITHMS AND PROBLEMS
// ============================================================================

// 1. Shortest Path in Undirected Graph with Unit Weights (Standard BFS)
vi shortestPathUnitWeights(int V, vector<vi>& adj, int src) {
    vi dist(V, INF);
    dist[src] = 0;
    queue<int> q;
    q.push(src);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (auto it : adj[node]) {
            if (dist[node] + 1 < dist[it]) {
                dist[it] = dist[node] + 1;
                q.push(it);
            }
        }
    }
    // Replace INF with -1 for unreachable nodes
    for (int i = 0; i < V; i++) if (dist[i] == INF) dist[i] = -1;
    return dist;
}

// 2. Shortest Path in Directed Acyclic Graph (DAG via Topo Sort)
vi shortestPathDAG(int V, const vector<vector<pii>>& adj, int src) {
    vi indegree(V, 0);
    for (int i = 0; i < V; i++) {
        for (auto it : adj[i]) indegree[it.ff]++;
    }

    queue<int> q;
    for (int i = 0; i < V; i++) if (indegree[i] == 0) q.push(i);

    vi topo;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        topo.pb(node);
        for (auto it : adj[node]) {
            indegree[it.ff]--;
            if (indegree[it.ff] == 0) q.push(it.ff);
        }
    }

    vi dist(V, INF);
    dist[src] = 0;

    for (int i = 0; i < V; i++) {
        int node = topo[i];
        if (dist[node] != INF) {
            for (auto it : adj[node]) {
                int v = it.ff;
                int wt = it.ss;
                if (dist[node] + wt < dist[v]) {
                    dist[v] = dist[node] + wt;
                }
            }
        }
    }
    for (int i = 0; i < V; i++) if (dist[i] == INF) dist[i] = -1;
    return dist;
} // returns vector storing shortest distance from src to all other nodes, -1 if unreachable

// 3 & 4. Dijkstra's Algorithm (Using Min-Priority Queue)
vi dijkstra(int V, const vector<vector<pii>>& adj, int src) {
    // Note: Priority queue is used to greedily pick the minimum distance node first, 
    // optimization that guarantees O((V + E) log V) time complexity.
    priority_queue<pii, vector<pii>, greater<pii>> pq; // {dist, node}
    vi dist(V, INF);

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        int d = pq.top().ff;
        int node = pq.top().ss;
        pq.pop();

        if (d > dist[node]) continue;

        for (auto it : adj[node]) {
            int v = it.ff;
            int wt = it.ss;
            if (d + wt < dist[v]) {
                dist[v] = d + wt;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// 5. Shortest Distance in a Binary Maze
int shortestPathBinaryMaze(vector<vector<int>>& grid, pii source, pii destination) {
    if (source == destination) return 0;
    int n = grid.size(), m = grid[0].size();
    if (grid[source.ff][source.ss] == 0 || grid[destination.ff][destination.ss] == 0) return -1;

    vector<vi> dist(n, vi(m, INF));
    dist[source.ff][source.ss] = 0;
    
    // Standard queue is sufficient because weights are unit steps (1)
    queue<pair<int, pii>> q; // {dist, {r, c}}
    q.push({0, source});

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!q.empty()) {
        int d = q.front().ff;
        int r = q.front().ss.ff;
        int c = q.front().ss.ss;
        q.pop();
        if(r == destination.ff && c == destination.ss) return d;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == 1 && d + 1 < dist[nr][nc]) {
                dist[nr][nc] = d + 1;
                q.push({d + 1, {nr, nc}});
            }
        }
    }
    return -1;
}

// 6. Path With Minimum Effort
int minimumEffortPath(vector<vector<int>>& heights) {
    int n = heights.size(), m = heights[0].size();
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> pq; // {max_diff, {r, c}} , using min heap to always expand the least effort path first
    vector<vi> diff(n, vi(m, INF));
    // modified dijkstra storing (max_diff, {r, c}) in priority queue
    diff[0][0] = 0; // diff[i][j] = minimum effort to reach cell (i, j)
    pq.push({0, {0, 0}});

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!pq.empty()) {
        int effort = pq.top().ff;
        int r = pq.top().ss.ff;
        int c = pq.top().ss.ss;
        pq.pop();

        if (r == n - 1 && c == m - 1) return effort;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                int newEffort = max(effort, abs(heights[r][c] - heights[nr][nc]));
                if (newEffort < diff[nr][nc]) {
                    diff[nr][nc] = newEffort;
                    pq.push({newEffort, {nr, nc}});
                }
            }
        }
    }
    return 0;
}

int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<vector<pair<int, int>>> adj(n);
    for (auto& f : flights) {
        adj[f[0]].push_back({f[1], f[2]});
    }

    int maxFlights = k + 1;
    vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX));

    // Min-heap storing: {cost, node, flightsTaken}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    dist[src][0] = 0;
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [cost, node, flightsTaken] = pq.top();
        pq.pop();

        if (cost > dist[node][flightsTaken]) {
            continue;
        }

        if (node == dst) {
            return cost;
        }

        for (auto& [next, price] : adj[node]) {
            if (flightsTaken + 1 > maxFlights) {
                continue;
            }

            int newCost = cost + price;
            if (newCost < dist[next][flightsTaken + 1]) {
                dist[next][flightsTaken + 1] = newCost;
                pq.push({newCost, next, flightsTaken + 1});
            }
        }
    }

    return -1;
}

// 8. Network Delay Time (Dijkstra Variant)
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pii>> adj(n + 1);
    for (auto& t : times) adj[t[0]].pb({t[1], t[2]});

    vi dist(n + 1, INF);
    priority_queue<pii, vector<pii>, greater<pii>> pq; // storing {time, node}
    // dist stores the shortest time to reach each node from the source k
    dist[k] = 0;
    pq.push({0, k});

    while (!pq.empty()) {
        int d = pq.top().ff;
        int node = pq.top().ss;
        pq.pop();

        if (d > dist[node]) continue;

        for (auto& it : adj[node]) {
            if (d + it.ss < dist[it.ff]) {
                dist[it.ff] = d + it.ss;
                pq.push({dist[it.ff], it.ff});
            }
        }
    }

    int mxTime = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) return -1;
        mxTime = max(mxTime, dist[i]);
    }
    return mxTime;
} // it is just standard dijkstra's algorithm with a priority queue, but we are keeping track of the maximum time taken to reach any node from the source k. If any node is unreachable (dist[i] == INF), we return -1. Otherwise, we return the maximum time taken to reach all nodes.

// 9. Number of Ways to Arrive at Destination
int countPaths(int n, vector<vector<int>>& roads) {
    vector<vector<pll>> adj(n);
    for (auto& r : roads) {
        adj[r[0]].pb({r[1], r[2]});
        adj[r[1]].pb({r[0], r[2]});
    }

    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    vll dist(n, LINF), ways(n, 0);

    dist[0] = 0;
    ways[0] = 1;
    pq.push({0, 0});

    while (!pq.empty()) {
        ll d = pq.top().ff;
        int node = pq.top().ss;
        pq.pop();

        if (d > dist[node]) continue;

        for (auto& it : adj[node]) {
            int v = it.ff;
            ll wt = it.ss;

            if (d + wt < dist[v]) {
                dist[v] = d + wt;
                ways[v] = ways[node];
                pq.push({dist[v], v});
            } else if (d + wt == dist[v]) {
                ways[v] = (ways[v] + ways[node]) % MOD;
            }
        }
    }
    return ways[n - 1] % MOD;
}

// 10. Minimum Multiplications to Reach End (State BFS)
int minimumMultiplications(vi& arr, int start, int end) {
    if (start == end) return 0;
    vi dist(100000, INF);
    dist[start] = 0;
    queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (auto it : arr) {
            int num = (1LL * node * it) % 100000;
            if (dist[node] + 1 < dist[num]) {
                dist[num] = dist[node] + 1;
                if (num == end) return dist[num];
                q.push(num);
            }
        }
    }
    return -1;
}

// 11. Bellman Ford Algorithm (Handles negative weights & detects negative cycles)
vi bellmanFord(int V, vector<vector<int>>& edges, int src, bool& hasNegativeCycle) {
    vi dist(V, 1e8); // Using 1e8 as per standard competitive problem bounds to avoid overflow
    dist[src] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; i++) {
        for (auto it : edges) {
            int u = it[0], v = it[1], wt = it[2];
            if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }

    // Nth relaxation check for negative cycles
    hasNegativeCycle = false;
    for (auto it : edges) {
        int u = it[0], v = it[1], wt = it[2];
        if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
            hasNegativeCycle = true;
            return {-1};
        }
    }
    return dist;
}

// 12 & 13. Floyd Warshall Algorithm & Find the City with Smallest Number of Neighbors
int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
    vector<vi> matrix(n, vi(n, INF));
    for (int i = 0; i < n; i++) matrix[i][i] = 0;
    for (auto& it : edges) {
        matrix[it[0]][it[1]] = it[2];
        matrix[it[1]][it[0]] = it[2];
    }

    // Classic Floyd Warshall All-Pairs Shortest Path DP execution
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][k] != INF && matrix[k][j] != INF) {
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                }
            }
        }
    }

    int cntCity = n;
    int cityNo = -1;

    for (int i = 0; i < n; i++) {
        int cnt = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] <= distanceThreshold) cnt++;
        }
        if (cnt <= cntCity) {
            cntCity = cnt;
            cityNo = i;
        }
    }
    return cityNo;
}