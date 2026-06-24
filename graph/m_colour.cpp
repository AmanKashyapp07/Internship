/**
 * Problem: M-Coloring Problem
 * Check if a graph can be colored with at most m colors such that no adjacent vertices share a color.
 * Time: O(M^V) worst-case time, O(V) space (recursion stack).
 */
#include <iostream>
#include <vector>
using namespace std;

class GraphColoring {
    bool isSafe(int node, int col, vector<int>& colors, vector<vector<int>>& graph) {
        // isSafe: verify no neighbor of the current node shares the target color
        for (int v : graph[node]) if (colors[v] == col) return false;
        return true;
    }
    bool solve(int node, int n, int m, vector<int>& colors, vector<vector<int>>& graph) {
        if (node == n) return true;
        for (int col = 1; col <= m; col++) {
            if (isSafe(node, col, colors, graph)) {
                colors[node] = col; // Tentatively assign color
                if (solve(node + 1, n, m, colors, graph)) return true;
                colors[node] = 0; // Backtrack
            }
        }
        return false;
    }
public:
    bool canColor(int n, vector<vector<int>>& graph, int m) {
        vector<int> colors(n, 0); // colous[i] = 0 means uncolored, otherwise holds color index
        return solve(0, n, m, colors, graph);
    }
};

int main() {
    int n = 4, m = 3;
    vector<vector<int>> graph(n);
    graph[0] = {1, 2, 3}; graph[1] = {0, 2}; graph[2] = {0, 1, 3}; graph[3] = {0, 2};
    GraphColoring obj;
    cout << (obj.canColor(n, graph, m) ? "Coloring Possible\n" : "Coloring Not Possible\n");
}
