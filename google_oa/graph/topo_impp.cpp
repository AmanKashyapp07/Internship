/**
 * LeetCode 1203 - Sort Items by Groups Respecting Dependencies
 *
 * Description:
 * There are n items divided into m groups. Some items may not belong to
 * any group (-1).
 *
 * Return any ordering of the items such that:
 * 1. Every dependency is satisfied.
 * 2. Items belonging to the same group appear consecutively.
 *
 * Approach:
 * 1. Assign every ungrouped item a unique group ID.
 * 2. Build two DAGs:
 *      - Item graph: item -> dependent item.
 *      - Group graph: group -> dependent group.
 * 3. Compute indegrees for both graphs.
 * 4. Perform Kahn's Topological Sort on:
 *      - Items.
 *      - Groups.
 * 5. If either graph contains a cycle, return an empty array.
 * 6. Group the items according to the item topological order.
 * 7. Traverse the groups in topological order and append the items
 *    belonging to each group.
 *
 * Time Complexity: O(n + m + e)
 *     n = number of items
 *     m = number of groups (after assigning new groups)
 *     e = total dependency relationships
 *
 * Space Complexity: O(n + m + e)
 */

#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

class Solution {
private:
    // Reusable Kahn's algorithm for topological sorting
    vector<int> topoSort(const vector<vector<int>>& adj, vector<int>& indegree, const vector<int>& nodes) {
        queue<int> q;
        for (int node : nodes) {
            if (indegree[node] == 0) {
                q.push(node);
            }
        }
        
        vector<int> ordered;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            ordered.push_back(curr);
            
            for (int neighbor : adj[curr]) {
                indegree[neighbor]--;
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
        
        // If we couldn't sort all nodes, a cycle exists
        return ordered.size() == nodes.size() ? ordered : vector<int>();
    }

public:
    vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
        // Step 1: Assign unique group IDs to items that don't belong to any group
        int group_id = m;
        for (int i = 0; i < n; ++i) {
            if (group[i] == -1) {
                group[i] = group_id++;
            }
        }
        
        // Step 2: Initialize graphs and indegrees for both items and groups
        vector<vector<int>> item_adj(n);
        vector<int> item_indegree(n, 0);
        
        vector<vector<int>> group_adj(group_id);
        vector<int> group_indegree(group_id, 0);
        
        // Populate lists of all unique item nodes and group nodes to pass to topoSort
        vector<int> item_nodes(n);
        for (int i = 0; i < n; ++i) item_nodes[i] = i;
        
        vector<int> group_nodes(group_id);
        for (int i = 0; i < group_id; ++i) group_nodes[i] = i;
        
        // Step 3: Build the dependency edges
        for (int i = 0; i < n; ++i) {
            int to_item = i;
            int to_group = group[i];
            
            for (int from_item : beforeItems[i]) {
                int from_group = group[from_item];
                
                // Add item-level dependency
                item_adj[from_item].push_back(to_item);
                item_indegree[to_item]++;
                
                // Add group-level dependency if they belong to different groups
                if (from_group != to_group) {
                    group_adj[from_group].push_back(to_group);
                    group_indegree[to_group]++;
                }
            }
        }
        
        // Step 4: Run topological sorts at both levels
        vector<int> sorted_items = topoSort(item_adj, item_indegree, item_nodes);
        vector<int> sorted_groups = topoSort(group_adj, group_indegree, group_nodes);
        
        // If a cycle is detected at either level, it's impossible to sort
        if (sorted_items.empty() || sorted_groups.empty()) {
            return {};
        }
        
        // Step 5: Map the sorted items to their respective groups
        unordered_map<int, vector<int>> group_to_items;
        for (int item : sorted_items) {
            group_to_items[group[item]].push_back(item);
        }
        
        // Step 6: Concatenate item blocks using the ordered macro-groups
        vector<int> result;
        for (int group_num : sorted_groups) {
            for (int item : group_to_items[group_num]) {
                result.push_back(item);
            }
        }
        
        return result;
    }
};