/**
 * LeetCode 2050 - Parallel Courses III
 *
 * Description:
 * Find the minimum time required to complete all courses given relationships and course completion durations.
 *
 * Approach:
 * - Topological sort with Kahn's algorithm.
 * - DP on DAG: `dp[u]` tracks the completion time of task `u`.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

class Solution {
public:
    int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
        vector<int> indegree(n + 1, 0);
        vector<vector<int>> graph(n + 1);

        for (const auto& relation : relations) {
            int u = relation[0];
            int v = relation[1];
            graph[u].push_back(v);
            indegree[v]++;
        }

        queue<int> q;
        vector<int> dp(n + 1, 0); // dp[i] will store the minimum time to complete task i

        for (int i = 1; i <= n; ++i) {
            if (indegree[i] == 0) {
                q.push(i);
                dp[i] = time[i - 1];
            }
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbor : graph[node]) {
                indegree[neighbor]--;
                dp[neighbor] = max(dp[neighbor], dp[node] + time[neighbor - 1]);
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
