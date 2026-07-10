/**
 * LeetCode 947 - Most Stones Removed with Same Row or Column
 *
 * Description:
 * Place stones on a 2D plane. A stone can be removed if it shares a row or column with another stone.
 * Return the maximum number of stones that can be removed.
 *
 * Approach:
 * - Model each stone as a node. Nodes are connected if they share a row or column.
 * - The maximum removed stones is `total_stones - number_of_connected_components`.
 * - Use a Disjoint Set Union (DSU) to connect row indices and column indices.
 *
 * Time Complexity: O(N * alpha(N))
 * Space Complexity: O(N)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class DSU {
public:
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
       return (parent[x] == x) ? x : parent[x] = find(parent[x]);
    }

    void unite(int u, int v) {
        int pu = find(u);
        int pv = find(v);
        if (pu != pv) parent[pu] = pv;
    }
};

class Solution {
public:
    int removeStones(vector<vector<int>>& stones) {
        
        const int OFFSET = 10001;
        DSU dsu(20005);
        
        unordered_set<int> used;
        
        for (auto &s : stones) {
            int row = s[0];
            int col = s[1] + OFFSET;
            
            dsu.unite(row, col);
            
            used.insert(row);
            used.insert(col);
        }
        
        int components = 0;
        
        for (int node : used) if (dsu.find(node) == node) components++;
        
        return stones.size() - components;
    }
};