// Link: https://cses.fi/problemset/task/1138

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Standard Fenwick Tree (BIT) for 0-indexed queries externally
struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, long long val) {
        i++; // Convert 0-indexed parameter to 1-indexed BIT internally
        while (i <= n) {
            bit[i] += val;
            i += i & -i; 
        }
    }

    long long query(int i) {
        long long s = 0;
        i++;
        while (i >= 1) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }
};

// Euler Tour Technique (Variant B) to map path sums (Size 2N)
struct EulerTour {
    int n, timer;
    vector<int> tin, tout;
    // tin vector stores the entry time of each node, tout vector stores the exit time of each node
    // flat_val vector stores the flattened values of the nodes in the Euler tour
    // when using in path queries question , et.tin[node] - 1 is the index in the flattened array for the node's entry, and et.tout[node] - 1 is the index for the node's exit, basically entry and exit means the time when we enter and exit the node during the DFS traversal of the tree, which is used to calculate the path sums efficiently using Fenwick Tree.
    EulerTour(int n) : n(n), timer(0), tin(n + 1, 0), tout(n + 1, 0) {}

    void dfs_path(int u, int p, const vector<vector<int>>& graph, vector<long long>& flat_val, const vector<int>& val) {
        tin[u] = ++timer;
        flat_val[timer - 1] = val[u];
        for (int v : graph[u]) {
            if (v != p) {
                dfs_path(v, u, graph, flat_val, val);
            }
        }
        tout[u] = ++timer;
        flat_val[timer - 1] = -val[u];
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> values(n + 1);
        for (int i = 1; i <= n; i++) cin >> values[i];

        vector<vector<int>> graph(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        // Build path Euler Tour of size 2N
        EulerTour et(n);
        vector<long long> flat_val(2 * n, 0);
        et.dfs_path(1, 0, graph, flat_val, values);

        // Initialize Fenwick tree (sizes correspond to 2N flattened indices)
        Fenwick ft(2 * n);
        for (int i = 0; i < 2 * n; i++) {
            ft.update(i, flat_val[i]);
        }

        // Keep track of current node values
        vector<long long> current_values(n + 1);
        for (int i = 1; i <= n; i++) current_values[i] = values[i];

        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                int s;
                long long x;
                cin >> s >> x;
                long long delta = x - current_values[s];
                
                // In Variant B, update +delta at entry and -delta at exit
                ft.update(et.tin[s] - 1, delta);
                ft.update(et.tout[s] - 1, -delta);
                current_values[s] = x;
            } else {
                int s;
                cin >> s;
                // Path sum is prefix sum up to entry index of s
                cout << ft.query(et.tin[s] - 1) << "\n";
            }
        }
    }
    return 0;
}
