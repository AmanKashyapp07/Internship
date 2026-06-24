#include <iostream>
#include <vector>
using namespace std;

class Solution {
private:
    bool isSafe(int node,
                int color,
                vector<int>& colors,
                vector<vector<int>>& graph) {

        for (int nbr : graph[node]) {
            if (colors[nbr] == color)
                return false;
        }

        return true;
    }

    bool dfs(int node,
             int m,
             int n,
             vector<int>& colors,
             vector<vector<int>>& graph) {

        // All vertices colored
        if (node == n)
            return true;

        // Try every color
        for (int color = 1; color <= m; color++) {

            if (!isSafe(node, color, colors, graph))
                continue;

            colors[node] = color;     // Choose

            if (dfs(node + 1, m, n, colors, graph))
                return true;

            colors[node] = 0;         // Backtrack
        }

        return false;
    }

public:
    bool graphColoring(vector<vector<int>>& graph, int m) {

        int n = graph.size();

        vector<int> colors(n, 0);

        return dfs(0, m, n, colors, graph);
    }
};