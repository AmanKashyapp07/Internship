#include <bits/stdc++.h>
using namespace std;

class DSU {
public:
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
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
        
        for (int node : used) {
            if (dsu.find(node) == node)
                components++;
        }
        
        return stones.size() - components;
    }
};