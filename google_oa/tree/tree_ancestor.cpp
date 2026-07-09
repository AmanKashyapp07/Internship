/**
 * @file tree_ancestor.cpp
 * @brief LeetCode 1483: Kth Ancestor of a Tree Node
 * Method: Binary Lifting (Doubling pointers up[i][j] = parent at 2^j levels above node i)
 */

#include <vector>
#include <iostream>

using namespace std;

class TreeAncestor {
private:
    vector<vector<int>> up;
    int LOG;

public:
    TreeAncestor(int n, vector<int>& parent) {
        LOG = 20; // 2^20 > 5 * 10^4 (sufficient for LeetCode constraints)
        up.assign(n, vector<int>(LOG, -1));

        // Initialize 2^0 ancestors (direct parents)
        for (int i = 0; i < n; i++) {
            up[i][0] = parent[i];
        }

        // Iteratively compute up[i][j] = parent at 2^j levels above node i
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                if (up[i][j - 1] == -1) {
                    up[i][j] = -1; // Bound overshoot
                } else {
                    up[i][j] = up[up[i][j - 1]][j - 1];
                }
            }
        }
    }
    
    int getKthAncestor(int node, int k) {
        // Examine bit flag representations to lift the node up by k levels
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) {
                node = up[node][j];
                if (node == -1) {
                    return -1;
                }
            }
        }
        return node;
    }
};

/**
 * Your TreeAncestor object will be instantiated and called as such:
 * TreeAncestor* obj = new TreeAncestor(n, parent);
 * int param_1 = obj->getKthAncestor(node,k);
 */
