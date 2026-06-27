/**
 * M-Coloring Problem
 *
 * Description:
 * Given an undirected graph represented as an adjacency list and an integer m, determine if the graph
 * can be colored with at most m colors such that no two adjacent vertices share the same color.
 *
 * Approach:
 * - Backtracking / DFS traversal.
 * - For the current `node`, try coloring it with every color from `1` to `m`.
 * - Validate the coloring choice using `isSafe()` by checking if any neighbor vertex has the same color.
 * - If valid, color the node and recurse to `node + 1`. If all nodes are colored (`node == n`), return true.
 * - If no color works for the current state, backtrack by resetting the color to `0` and return false.
 *
 * Time Complexity: O(M^N) where N is the number of vertices and M is the number of colors.
 * Space Complexity: O(N) for the recursion stack and colors array.
 */

#include <iostream>
#include <vector>
using namespace std;

class Solution {
private:
    bool isSafe(int node, int color, const vector<int>& colors, const vector<vector<int>>& graph) {
        for (int nbr : graph[node]) {
            if (colors[nbr] == color) {
                return false;
            }
        }
        return true;
    } // THIS CHECKS IF THE CURRENT COLOR ASSIGNED TO THE NODE IS SAFE OR NOT, BY CHECKING IF ANY OF ITS NEIGHBORS HAVE THE SAME COLOR. TIME COMPLEXITY IS O(N) WHERE N IS THE NUMBER OF NEIGHBORS OF THE NODE, SPACE COMPLEXITY IS O(1) AS WE ARE NOT USING ANY EXTRA SPACE.

    bool dfs(int node, int m, int n, vector<int>& colors, const vector<vector<int>>& graph) {
        // All vertices colored successfully
        if (node == n) {
            return true;
        }

        // Try every color from 1 to m
        for (int color = 1; color <= m; color++) {
            if (isSafe(node, color, colors, graph)) {
                colors[node] = color; // Choose
                if (dfs(node + 1, m, n, colors, graph)) return true;

                colors[node] = 0; // Backtrack
            }
        }

        return false;
    }

public:
    bool graphColoring(vector<vector<int>>& graph, int m) {
        int n = graph.size();
        vector<int> colors(n, 0); // 0 MEANS UNCOLORED, COLORS ARE 1 TO M
        return dfs(0, m, n, colors, graph);
    }
};
