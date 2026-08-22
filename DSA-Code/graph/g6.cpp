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

class DSU {
public:
    vector<int> p, sz;
    DSU(int n) {
        p.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};


// 1. Number of Operations to Make Network Connected
int makeConnected(int n, vector<vector<int>>& connections) {
    if (connections.size() < n - 1)
        return -1;

    DSU ds(n);

    for (auto &e : connections) {
        ds.unite(e[0], e[1]);
    }

    int components = 0;

    for (int i = 0; i < n; i++)
        if (ds.find(i) == i)
            components++;

    return components - 1;
}

// 2. Most Stones Removed with Same Row or Column
int removeStones(vector<vector<int>>& stones) {
    int maxRow = 0, maxCol = 0;

    for (auto &s : stones) {
        maxRow = max(maxRow, s[0]);
        maxCol = max(maxCol, s[1]);
    }

    DSU ds(maxRow + maxCol + 2);
    unordered_set<int> used;

    for (auto &s : stones) {
        int row = s[0];
        int col = s[1] + maxRow + 1;

        ds.unite(row, col);

        used.insert(row);
        used.insert(col);
    }

    int components = 0;

    for (int node : used)
        if (ds.find(node) == node)
            components++;

    return stones.size() - components;
}

// 3. Number of Islands II
vector<int> numIslandsII(int n, int m, vector<vector<int>>& queries) {
    DSU ds(n * m);

    vector<vector<int>> vis(n, vector<int>(m, 0));
    vector<int> ans;

    int cnt = 0;

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    for (auto &q : queries) {
        int r = q[0];
        int c = q[1];

        if (vis[r][c]) {
            ans.push_back(cnt);
            continue;
        }

        vis[r][c] = 1;
        cnt++;

        int node = r * m + c;

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m)
                continue;

            if (!vis[nr][nc])
                continue;

            int graph = nr * m + nc;

            if (ds.find(node) != ds.find(graph)) {
                cnt--;
                ds.unite(node, graph);
            }
        }

        ans.push_back(cnt);
    }

    return ans;
}

// 4. Making A Large Island
int largestIsland(vector<vector<int>>& grid) {
    int n = grid.size();
    DSU ds(n * n);

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (!grid[r][c])
                continue;

            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc])
                    ds.unite(r * n + c, nr * n + nc);
            }
        }
    }

    int ans = 0;

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {

            if (grid[r][c])
                continue;

            set<int> comps;

            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc])
                    comps.insert(ds.find(nr * n + nc));
            }

            int cur = 1;

            for (int p : comps)
                cur += ds.sz[p];

            ans = max(ans, cur);
        }
    }

    return ans == 0 ? n * n : ans;
}

// 5. Swim in Rising Water
int swimInWater(vector<vector<int>>& grid) {
    int n = grid.size();
    using T = pair<int, pair<int,int>>;
    priority_queue<T, vector<T>, greater<T>> pq; // {time, {r, c}}

    vector<vector<int>> vis(n, vector<int>(n, 0));

    pq.push({grid[0][0], {0, 0}});
    vis[0][0] = 1;

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!pq.empty()) {
        auto [t, cell] = pq.top();
        pq.pop();

        auto [r, c] = cell;

        if (r == n - 1 && c == n - 1)
            return t;

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= n || vis[nr][nc])
                continue;

            vis[nr][nc] = 1;
            pq.push({max(t, grid[nr][nc]), {nr, nc}});
        }
    }

    return 0;
}

// ============================================================================
// SECTION 6: ADVANCED / OTHER GRAPH ALGORITHMS
// ============================================================================

// 1. Bridges in a Graph (Tarjan's Bridge-Finding Algorithm)
void dfsBridge(int node, int parent, int& timer, vi& tin, vi& low, vi& vis, 
               const vector<vi>& graph, vector<vector<int>>& bridges) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    for (auto it : graph[node]) {
        if (it == parent) continue;
        if (!vis[it]) {
            dfsBridge(it, node, timer, tin, low, vis, graph, bridges);
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
    vector<vi> graph(n);
    for (auto it : connections) {
        graph[it[0]].pb(it[1]);
        graph[it[1]].pb(it[0]);
    }
    vi tin(n, -1), low(n, -1), vis(n, 0);
    vector<vector<int>> bridges;
    int timer = 0;
    dfsBridge(0, -1, timer, tin, low, vis, graph, bridges);
    return bridges;
}


// 3. Kosaraju's Algorithm (Strongly Connected Components in Directed Graph)
void dfsKosaraju(int node, vi& vis, const vector<vi>& graph, stack<int>& st) {
    vis[node] = 1;
    for (auto it : graph[node]) {
        if (!vis[it]) dfsKosaraju(it, vis, graph, st);
    }
    st.push(node);
}

void dfsReverse(int node, vi& vis, const vector<vi>& graphRev) {
    vis[node] = 1;
    for (auto it : graphRev[node]) {
        if (!vis[it]) dfsReverse(it, vis, graphRev);
    }
}

int kosarajuSCC(int V, vector<vi>& graph) {
    vi vis(V, 0);
    stack<int> st;
    // Step 1: Sort all nodes based on their finishing times
    for (int i = 0; i < V; i++) {
        if (!vis[i]) dfsKosaraju(i, vis, graph, st);
    }

    // Step 2: Transpose/Reverse graph matrices
    vector<vi> graphRev(V);
    for (int i = 0; i < V; i++) {
        vis[i] = 0; // reset visited array for Step 3
        for (auto it : graph[i]) {
            graphRev[it].pb(i);
        }
    }

    // Step 3: Extract blocks elements using finishing sequence ordering
    int sccCount = 0;
    while (!st.empty()) {
        int node = st.top();
        st.pop();
        if (!vis[node]) {
            sccCount++;
            dfsReverse(node, vis, graphRev);
        }
    }
    return sccCount;
}