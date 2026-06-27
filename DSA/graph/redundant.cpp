/**
 * LeetCode 684 - Redundant Connection
 *
 * Description:
 * Given a graph that started as a tree with n nodes but one additional edge added,
 * return the edge that can be removed so that the resulting graph is a tree.
 *
 * Approach:
 * - Use Disjoint Set Union (DSU) / Union-Find.
 * - Process edges one by one.
 * - For each edge `(u, v)`, check if they are already in the same component.
 * - If yes, this edge forms a cycle and is the redundant connection.
 * - If not, union the components of `u` and `v`.
 *
 * Time Complexity: O(N * alpha(N))
 * Space Complexity: O(N)
 */

class DisjointSet {
public:
    vector<int> rank, parent, size;

    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1, 1);

        for (int i = 0; i <= n; i++)
            parent[i] = i;
    }

    int findUPar(int node) {
        if (node == parent[node])
            return node;
        return parent[node] = findUPar(parent[node]);
    }

    void unionByRank(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);

        if (ulp_u == ulp_v)
            return;

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

        if (ulp_u == ulp_v)
            return;

        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};

class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        DisjointSet ds(n);

        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            if (ds.findUPar(u) == ds.findUPar(v))
                return edge;

            ds.unionBySize(u, v); 
        }

        return {};
    }
};