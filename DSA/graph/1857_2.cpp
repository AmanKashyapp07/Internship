/**
 * LeetCode 1857 - Largest Color Value in a Directed Graph
 *
 * Description:
 * Given a directed graph where each node has a color, return the maximum
 * frequency of any single color along any valid path.
 *
 * Return -1 if the graph contains a cycle.
 *
 * Approach:
 * Phase 1:
 * - Use Kahn's Algorithm to obtain a topological ordering.
 * - If all nodes are not processed, a cycle exists.
 *
 * Phase 2:
 * - dp[u][c] = maximum occurrences of color 'c'
 *   on any path ending at node u.
 * - Traverse nodes in topological order and propagate DP values
 *   to all outgoing neighbors.
 *
 * Time Complexity: O((n + m) * 26)
 * Space Complexity: O(n * 26)
 */

class Solution {
public:
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        int n = colors.size();

        // Build graph
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);

        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];

            graph[u].push_back(v);
            indegree[v]++;
        }

        // ---------------- Phase 1 : Topological Sort ----------------

        vector<int> topo;
        queue<int> q;

        // Push all source nodes
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0)
                q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            topo.push_back(u);

            for (int v : graph[u]) {
                if (--indegree[v] == 0)
                    q.push(v);
            }
        }

        // Cycle detected
        if (topo.size() != n)
            return -1;

        // ---------------- Phase 2 : DP on DAG ----------------

        // dp[node][color] =
        // maximum frequency of 'color'
        // on any path ending at 'node'
        vector<vector<int>> dp(n, vector<int>(26, 0));

        // Every node alone contributes one occurrence of its own color
        for (int u : topo)
            dp[u][colors[u] - 'a'] = 1;

        int ans = 1;

        // Process nodes in topological order
        for (int u : topo) {

            // Update global answer
            for (int c = 0; c < 26; c++)
                ans = max(ans, dp[u][c]);

            // Propagate DP values to neighbors
            for (int v : graph[u]) {

                for (int c = 0; c < 26; c++) {

                    // Carry current color frequency
                    int value = dp[u][c];

                    // If neighbor has this color,
                    // increase its frequency by one
                    if (c == colors[v] - 'a')
                        value++;

                    dp[v][c] = max(dp[v][c], value);
                }
            }
        }

        return ans;
    }
};