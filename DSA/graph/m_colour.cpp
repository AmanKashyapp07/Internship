/**
 * GeeksforGeeks - M-Coloring Problem
 *
 * Description:
 * Determine whether an undirected graph can be colored using at most m colors
 * such that no two adjacent vertices have the same color.
 *
 * Approach:
 * - Use backtracking.
 * - For each vertex, try every color from 1 to m.
 * - Assign a color only if none of its neighbors has that color.
 * - Recursively color the remaining vertices.
 *
 * Time Complexity: O(m^V)
 * Space Complexity: O(V)
 */

#include <iostream>
#include <vector>
using namespace std;

class GraphColoring {
    bool isSafe(int u, int color, vector<int>& colors, vector<vector<int>>& graph) {
        for (int v : graph[u])
            if (colors[v] == color)
                return false;
        return true;
    }

    bool dfs(int u, int n, int m, vector<int>& colors, vector<vector<int>>& graph) {
        if (u == n)
            return true;

        for (int color = 1; color <= m; color++) {
            if (!isSafe(u, color, colors, graph))
                continue;

            colors[u] = color;

            if (dfs(u + 1, n, m, colors, graph))
                return true;

            colors[u] = 0;
        }

        return false;
    }

public:
    bool canColor(int n, vector<vector<int>>& graph, int m) {
        vector<int> colors(n, 0);
        return dfs(0, n, m, colors, graph);
    }
};
