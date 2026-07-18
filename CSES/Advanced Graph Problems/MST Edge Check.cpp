// Link: https://cses.fi/problemset/task/3407

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

/*
  =========================================
  THEORY & LOGIC FOR "EDGE IN MST" PROBLEM
  =========================================
  
  The goal is to determine if a given edge can belong to *at least one* 
  Minimum Spanning Tree (MST) of an undirected weighted graph. 
  We solve this by modifying Kruskal's algorithm:

  1. STRICT SMALLER WEIGHTS CONSTRAINT:
     An edge with weight W can NEVER be part of any MST if its endpoints 
     are already connected by a path where every edge weighs strictly 
     less than W. Adding it would create a cycle where it is the heaviest 
     edge, violating the fundamental cycle property of MSTs.

  2. EQUAL WEIGHT ADVANTAGE (BATCH PROCESSING):
     Standard Kruskal's breaks ties between edges of the exact same weight 
     arbitrarily. If we have a batch of edges with identical weight W, 
     an edge CAN be in an MST if its endpoints belong to different 
     components *before* any edges of weight W are applied. 
     This is because we could theoretically choose to process this specific 
     edge first within its weight batch.

  3. TWO-PHASE ALGORITHM PER BATCH:
     - Sort all edges ascending by weight.
     - Group edges into batches of identical weights.
     - Phase 1 (Evaluate): For each edge in the current batch, check if its 
       endpoints are in different components using `dsu.find(u) != dsu.find(v)`. 
       If they are, this edge can be in an MST. We must do this check for ALL 
       edges in the batch BEFORE updating the DSU structure.
     - Phase 2 (Union): Iterate through the current batch again and actually 
       merge the components using `dsu.unite(u, v)`. This prepares the DSU 
       state for the next, strictly heavier batch of edges.
*/

// Structure to hold edge data
struct Edge {
    int u, v, w, id;
    
    // Sort edges ascending by weight
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

// User-provided DSU template
struct DSU {
    vector<int> p, sz;
    int comp;
    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    bool unite(int a, int b) {
        if ((a = find(a)) == (b = find(b))) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    
    int size(int x) { return sz[find(x)]; }
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].id = i; // Save original index to output answers correctly
    }

    // Sort edges by weight
    sort(edges.begin(), edges.end());

    DSU dsu(n);
    vector<bool> can_be_in_mst(m, false);

    // Process edges in batches of identical weights
    for (int i = 0; i < m; ) {
        int j = i;
        
        // Find the range [i, j) of edges with the same weight
        while (j < m && edges[j].w == edges[i].w) {
            j++;
        }

        // Phase 1: Check if the edge connects different components
        for (int k = i; k < j; k++) {
            if (dsu.find(edges[k].u) != dsu.find(edges[k].v)) {
                can_be_in_mst[edges[k].id] = true;
            }
        }

        // Phase 2: Unite the components for this weight batch
        for (int k = i; k < j; k++) {
            dsu.unite(edges[k].u, edges[k].v);
        }

        // Move to the next batch
        i = j;
    }

    // Output results in the original edge order
    for (int i = 0; i < m; i++) {
        if (can_be_in_mst[i]) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}