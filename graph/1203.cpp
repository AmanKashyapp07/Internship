/**
 * LeetCode 1203 - Sort Items by Groups Respecting Dependencies
 *
 * Description:
 * There are n items belonging to m groups.
 * Some items may not belong to any group (-1).
 *
 * Return any ordering of items such that:
 * 1. Every dependency is satisfied.
 * 2. Items belonging to the same group appear together.
 *
 * Approach:
 * 1. Assign every ungrouped item a unique group.
 * 2. Build:
 *      - Item dependency graph.
 *      - Group dependency graph.
 * 3. Topologically sort both graphs.
 * 4. Collect items according to item topo order.
 * 5. Output groups according to group topo order.
 *
 * Time Complexity: O(n + e)
 * Space Complexity: O(n + e)
 */

class Solution {
public:

    // Standard Kahn's Algorithm
    vector<int> topologicalSort(vector<vector<int>>& graph,
                                vector<int> indegree) {

        queue<int> q;
        vector<int> order;

        for (int i = 0; i < graph.size(); i++) {
            if (indegree[i] == 0)
                q.push(i);
        }

        while (!q.empty()) {

            int u = q.front();
            q.pop();

            order.push_back(u);

            for (int v : graph[u]) {
                if (--indegree[v] == 0)
                    q.push(v);
            }
        }

        if (order.size() != graph.size())
            return {};

        return order;
    }

    vector<int> sortItems(int n, int m,
                          vector<int>& group,
                          vector<vector<int>>& beforeItems) {

        // -------------------------------------------------------
        // Step 1 : Give every ungrouped item its own new group
        // -------------------------------------------------------

        int groupId = m;

        for (int i = 0; i < n; i++) {
            if (group[i] == -1)
                group[i] = groupId++;
        }

        // -------------------------------------------------------
        // Step 2 : Build item graph and group graph
        // -------------------------------------------------------

        vector<vector<int>> itemGraph(n);
        vector<int> itemIndegree(n, 0);

        vector<vector<int>> groupGraph(groupId);
        vector<int> groupIndegree(groupId, 0);

        // Prevent duplicate edges between groups
        set<pair<int,int>> seenEdges;

        for (int item = 0; item < n; item++) {

            for (int prev : beforeItems[item]) {

                // prev -> item
                itemGraph[prev].push_back(item);
                itemIndegree[item]++;

                int g1 = group[prev];
                int g2 = group[item];

                // Different groups create a dependency
                if (g1 != g2 &&
                    !seenEdges.count({g1, g2})) {

                    seenEdges.insert({g1, g2});

                    groupGraph[g1].push_back(g2);
                    groupIndegree[g2]++;
                }
            }
        }

        // -------------------------------------------------------
        // Step 3 : Topological sort groups
        // -------------------------------------------------------

        vector<int> groupOrder =
            topologicalSort(groupGraph, groupIndegree);

        if (groupOrder.empty())
            return {};

        // -------------------------------------------------------
        // Step 4 : Topological sort items
        // -------------------------------------------------------

        vector<int> itemOrder =
            topologicalSort(itemGraph, itemIndegree);

        if (itemOrder.empty())
            return {};

        // -------------------------------------------------------
        // Step 5 : Put every item inside its group
        // according to item topological order
        // -------------------------------------------------------

        unordered_map<int, vector<int>> groupItems;

        for (int item : itemOrder)
            groupItems[group[item]].push_back(item);

        // -------------------------------------------------------
        // Step 6 : Output groups in topological order
        // -------------------------------------------------------

        vector<int> ans;

        for (int g : groupOrder) {

            for (int item : groupItems[g])
                ans.push_back(item);
        }

        return ans;
    }
};