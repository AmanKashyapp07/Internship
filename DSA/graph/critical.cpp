class Solution {

    // -------------------------------------------------------------------------
    // Theory:
    //
    // Let W = weight of the original Minimum Spanning Tree (MST).
    //
    // We classify every edge using two Kruskal runs.
    //
    // 1) Critical Edge
    // ----------------
    // Skip this edge and run Kruskal.
    //
    // If
    //   - graph becomes disconnected, OR
    //   - new MST weight > W,
    //
    // then every MST must contain this edge.
    //
    // 2) Pseudo-Critical Edge
    // -----------------------
    // Force this edge into the MST first.
    //
    // - Add its weight.
    // - Union its endpoints.
    // - Continue Kruskal normally.
    //
    // If final MST weight == W,
    // then there exists an MST containing this edge.
    //
    // 3) Neither
    // ----------
    // If skipping doesn't increase the MST weight,
    // but forcing it increases the MST weight,
    // then the edge never belongs to any MST.
    //
    // Algorithm:
    // ----------
    // 1. Store original index of every edge.
    // 2. Sort edges by weight.
    // 3. Compute original MST weight W.
    // 4. For every edge:
    //      a) Skip it -> if weight > W => Critical.
    //      b) Else force it -> if weight == W => Pseudo-Critical.
    //
    // Complexity:
    // O(m² · α(n))
    // -------------------------------------------------------------------------

    struct DSU {
        vector<int> parent, sz;

        DSU(int n) {
            parent.resize(n);
            sz.assign(n, 1);
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int x) {
            if (parent[x] == x) return x;
            return parent[x] = find(parent[x]);
        }

        bool unite(int a, int b) {
            a = find(a);
            b = find(b);

            if (a == b) return false;

            if (sz[a] < sz[b]) swap(a, b);

            parent[b] = a;
            sz[a] += sz[b];
            return true;
        }
    };

public:
    int kruskal(int n, vector<vector<int>>& edges, int skip, int force) {
        DSU dsu(n);

        int mstWeight = 0;
        int edgesUsed = 0;

        // Force an edge into the MST first.
        if (force != -1) {
            dsu.unite(edges[force][0], edges[force][1]);
            mstWeight += edges[force][2];
            edgesUsed++;
        }

        // Standard Kruskal.
        for (int i = 0; i < edges.size(); i++) {

            if (i == skip || i == force)
                continue;

            int u = edges[i][0];
            int v = edges[i][1];
            int w = edges[i][2];

            if (dsu.unite(u, v)) {
                mstWeight += w;
                edgesUsed++;
            }
        }

        // Graph disconnected.
        if (edgesUsed != n - 1)
            return INT_MAX;

        return mstWeight;
    }

    vector<vector<int>> findCriticalAndPseudoCriticalEdges(
        int n, vector<vector<int>>& edges) {

        int m = edges.size();

        // Save original index.
        for (int i = 0; i < m; i++)
            edges[i].push_back(i);

        sort(edges.begin(), edges.end(),
             [](auto &a, auto &b) {
                 return a[2] < b[2];
             });

        int originalMST = kruskal(n, edges, -1, -1);

        vector<int> critical;
        vector<int> pseudo;

        for (int i = 0; i < m; i++) {

            // Removing this edge increases MST weight.
            if (kruskal(n, edges, i, -1) > originalMST) {
                critical.push_back(edges[i][3]);
            }

            // Edge can still belong to some MST.
            else if (kruskal(n, edges, -1, i) == originalMST) {
                pseudo.push_back(edges[i][3]);
            }
        }

        return {critical, pseudo};
    }
};