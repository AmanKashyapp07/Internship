/**
 * LeetCode 847 - Shortest Path Visiting All Nodes
 *
 * Description:
 * You have an undirected, connected graph of n nodes. Return the length of the shortest path
 * that visits every node. You may start and stop at any node, and repeat nodes and edges.
 *
 * Approach:
 * - Perform Multi-Source BFS on the state space.
 * - State is represented as a pair: (current_node, visited_nodes_mask).
 * - Start the BFS from every node with its initial mask (1 << node).
 * - Process neighbors by updating the bitmask.
 * - The first state popped from the queue with a full mask (all bits set to 1) represents the shortest path.
 *
 * Time Complexity: O(n * 2^n)
 * Space Complexity: O(n * 2^n)
 */

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();

        if (n == 1) return 0;

        int allVisited = (1 << n) - 1;

        queue<pair<int, int>> q;                  // {node, mask}
        vector<vector<bool>> vis(n, vector<bool>(1 << n, false));

        // Multi-source BFS
        for (int i = 0; i < n; i++) {
            int mask = 1 << i;
            q.push({i, mask});
            vis[i][mask] = true;
        }

        int steps = 0;
        vector<int> dr = {-1, 0, 1, 0};
        vector<int> dc = {0, 1, 0, -1};
        while (!q.empty()) {
            int sz = q.size();

            while (sz--) {
                auto [u, mask] = q.front();
                q.pop();

                if (mask == allVisited)
                    return steps;

                for (int v : graph[u]) {
                    int newMask = mask | (1 << v);

                    if (vis[v][newMask]) // If we have already visited this state (node v with newMask), we skip it to avoid redundant processing and ensure we only explore unique states in our BFS. This prevents cycles and ensures that we find the shortest path efficiently.
                        continue;

                    vis[v][newMask] = true;
                    q.push({v, newMask});
                }
            }

            steps++;
        }

        return -1;
    }
};