/**
 * LeetCode 851 - Loud and Rich
 *
 * Description:
 * There are n people, where richer[i] = [a, b] means person 'a' is richer
 * than person 'b'. Each person also has a quietness value.
 *
 * For every person, return the index of the quietest person among everyone
 * who is richer than or equal to them.
 *
 * Approach:
 * 1. Build a graph:
 *      richer person -> poorer person.
 * 2. Compute the indegree of every person.
 * 3. Initialize answer[i] = i since everyone is initially their own
 *    quietest candidate.
 * 4. Perform Kahn's Topological Sort starting from people with indegree 0
 *    (no one is richer than them).
 * 5. While traversing an edge (rich -> poor):
 *      - If the quietest person reachable from 'rich' is quieter than the
 *        current best for 'poor', update answer[poor].
 *      - Reduce indegree of 'poor'.
 *      - Push 'poor' when its indegree becomes 0.
 *
 * Time Complexity: O(n + e)
 *     n = number of people
 *     e = number of richer relationships
 *
 * Space Complexity: O(n + e)
 */


#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;


class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();

        vector<vector<int>> graph(n);
        vector<int> indegree(n), ans(n);
        // ans[i] = index of quietest person reachable from i (including i)
        // quiet[i] = quietness of person i
        for (auto &e : richer) {
            graph[e[0]].push_back(e[1]); // making graph from richer to poorer
            indegree[e[1]]++;
        }

        queue<int> q;

        for (int i = 0; i < n; i++) {
            ans[i] = i;
            if (indegree[i] == 0)
                q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph[u]) {
                if (quiet[ans[u]] < quiet[ans[v]]) ans[v] = ans[u]; // update quietest person for v if u's quietest is quieter
                if (--indegree[v] == 0)
                    q.push(v);
            }
        }

        return ans;
    }
};