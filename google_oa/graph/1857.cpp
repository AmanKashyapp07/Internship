/**
 * LeetCode 1857 - Largest Color Value in a Directed Graph
 *
 * Description:
 * Given a directed graph where each node has a color, return the maximum
 * number of occurrences of any single color along any valid path.
 *
 * Return -1 if the graph contains a cycle.
 *
 * Approach:
 * - Use Kahn's Algorithm to obtain a topological ordering.
 * - dp[u][c] stores the maximum count of color 'c' on any path ending at node u.
 * - For every edge u -> v, propagate all 26 color counts to v.
 * - If v itself has color c, increment that count by 1.
 * - If all nodes are not processed, the graph contains a cycle.
 *
 * Time Complexity: O((n + m) * 26)
 * Space Complexity: O(n * 26)
 */

class Solution {
public:
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        int n = colors.size();

        // Build graph and indegree array
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);

        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];

            graph[u].push_back(v);
            indegree[v]++;
        }

        // dp[node][color] = maximum frequency of 'color'
        // on any path ending at 'node'
        vector<vector<int>> dp(n, vector<int>(26, 0));

        queue<int> q;

        // Initialize all source nodes
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push(i);

                // Path containing only itself
                dp[i][colors[i] - 'a'] = 1;
            }
        }

        int processed = 0;
        int ans = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            processed++;

            // Update global answer
            for (int c = 0; c < 26; c++)
                ans = max(ans, dp[u][c]);

            // Relax every outgoing edge
            for (int v : graph[u]) {

                // Transfer all color frequencies
                for (int c = 0; c < 26; c++) {

                    // Carry forward the current frequency
                    int value = dp[u][c];

                    // Increment if v has this color
                    if (c == colors[v] - 'a')
                        value++;

                    // Keep the best path reaching v
                    dp[v][c] = max(dp[v][c], value);
                }

                // Standard Kahn's Algorithm
                indegree[v]--;

                if (indegree[v] == 0)
                    q.push(v);
            }
        }

        // Cycle exists if not all nodes were processed
        return processed == n ? ans : -1;
    }
};