/**
 * LeetCode 797 - All Paths From Source to Target
 *
 * Description:
 * Given a directed acyclic graph (DAG) of n nodes labeled from 0 to n - 1,
 * find all possible paths from node 0 to node n - 1 and return them in any order.
 *
 * Approach:
 * - Standard Depth First Search (DFS) starting from node 0.
 * - Keep track of the current path.
 * - When node n - 1 is reached, add a copy of the path to the results.
 * - Backtrack by removing the node from the path when retreating.
 *
 * Time Complexity: O(2^n * n) since there can be at most 2^(n-1) paths and each path takes O(n) to copy.
 * Space Complexity: O(n) recursion stack (excluding the output memory).
 */

class Solution {
public:
    vector<vector<int>> result;

    void dfs(int node, vector<vector<int>>& graph, vector<int>& path) {
        path.push_back(node);

        if (node == graph.size() - 1) {
            result.push_back(path);
        } else {
            for (int nei : graph[node]) {
                dfs(nei, graph, path);
            }
        }

        path.pop_back();
    }

    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<int> path;
        dfs(0, graph, path);
        return result;
    }
};