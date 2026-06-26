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


#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> lewisQuiet(int n, vector<vector<int>>& richer, vector<int>& quiet) {
        // Step 1: Build the graph (Rich -> Poor) and track indegrees
        vector<vector<int>> adj(n);
        vector<int> indegree(n, 0);
        
        for (const auto& edge : richer) {
            int rich = edge[0];
            int poor = edge[1];
            adj[rich].push_back(poor);
            indegree[poor]++;
        }
        
        // Step 2: Initialize answers where everyone is their own baseline
        vector<int> answer(n);
        for (int i = 0; i < n; ++i) {
            answer[i] = i;
        }
        
        // Step 3: Queue all people who have NO ONE richer than them
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        // Step 4: Topological Sort (Kahn's Algorithm)
        while (!q.empty()) {
            int rich = q.front();
            q.pop();
            
            for (int poor : adj[rich]) {
                // If the best candidate from the rich person is quieter than 
                // the best candidate found so far for the poor person, update it.
                if (quiet[answer[rich]] < quiet[answer[poor]]) {
                    answer[poor] = answer[rich];
                }
                
                // Decrement indegree; push to queue if all richer ancestors are processed
                indegree[poor]--;
                if (indegree[poor] == 0) {
                    q.push(poor);
                }
            }
        }
        
        return answer;
    }
};