#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>
#endif
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0), neighbors(vector<Node*>()) {}
    Node(int _val) : val(_val), neighbors(vector<Node*>()) {}
    Node(int _val, vector<Node*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

// =========================================================
// 46. NUMBER OF ISLANDS
// =========================================================

void dfsSink(vector<vector<char>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') return;
    grid[r][c] = '0';
    dfsSink(grid, r + 1, c);
    dfsSink(grid, r - 1, c);
    dfsSink(grid, r, c + 1);
    dfsSink(grid, r, c - 1);
}

int numIslands(vector<vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int m = grid.size(), n = grid[0].size(), count = 0;
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (grid[r][c] == '1') {
                count++;
                dfsSink(grid, r, c);
            }
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count number of connected islands of '1's.
// - Approach: Grid DFS with In-place Sinking (changing '1' to '0').
// - Intuition: Sinking visited land avoids auxiliary visited matrix.
// - Complexity: Time: O(M * N), Space: O(M * N) recursion stack.


// =========================================================
// 47. ROTTING ORANGES
// =========================================================

int orangesRotting(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    queue<pair<int, int>> q;
    int fresh = 0;

    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (grid[r][c] == 2) q.push({r, c});
            else if (grid[r][c] == 1) fresh++;
        }
    }
    if (fresh == 0) return 0;

    int minutes = 0;
    const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!q.empty() && fresh > 0) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            auto [r, c] = q.front(); q.pop();
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    grid[nr][nc] = 2;
                    fresh--;
                    q.push({nr, nc});
                }
            }
        }
        minutes++;
    }
    return fresh == 0 ? minutes : -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum minutes to rot all fresh oranges.
// - Approach: Multi-Source BFS with Level-by-Level Step Count.
// - Intuition: Push all initial rotten oranges at t=0; propagate rot in parallel.
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 48. COURSE SCHEDULE
// =========================================================

bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);

    for (const auto& pre : prerequisites) {
        adj[pre[1]].push_back(pre[0]);
        inDegree[pre[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }

    int processed = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        processed++;
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    return processed == numCourses;
}
// Interview Explanation:
// - Problem Statement: Determine if all courses can be finished without cyclic dependencies.
// - Approach: Kahn's Algorithm (In-Degree BFS Topological Sort).
// - Intuition: Push 0-in-degree nodes; if total visited == numCourses, no cycle exists.
// - Complexity: Time: O(V + E), Space: O(V + E).


// =========================================================
// 49. CLONE GRAPH
// =========================================================

class SolutionCloneGraph {
    unordered_map<Node*, Node*> copies;
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        if (copies.count(node)) return copies[node];

        Node* copy = new Node(node->val);
        copies[node] = copy;
        for (Node* neighbor : node->neighbors) {
            copy->neighbors.push_back(cloneGraph(neighbor));
        }
        return copy;
    }
};
// Interview Explanation:
// - Problem Statement: Deep copy an undirected graph.
// - Approach: Graph DFS + Visited Hash Map.
// - Intuition: Map original pointer to cloned pointer to handle cycles and shared references.
// - Complexity: Time: O(V + E), Space: O(V).


// =========================================================
// 50. NUMBER OF CONNECTED COMPONENTS IN UNDIRECTED GRAPH
// =========================================================

int countComponents(int n, vector<vector<int>>& edges) {
    vector<vector<int>> adj(n);
    for (auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    vector<bool> visited(n, false);
    int components = 0;

    function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs(v);
        }
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            components++;
            dfs(i);
        }
    }
    return components;
}
// Interview Explanation:
// - Problem Statement: Find number of connected components in graph with n nodes.
// - Approach: Graph DFS Traversal.
// - Intuition: Each unvisited node initiates a DFS covering an entire connected component.
// - Complexity: Time: O(V + E), Space: O(V + E).


// =========================================================
// 51. GRAPH VALID TREE
// =========================================================

bool validTree(int n, vector<vector<int>>& edges) {
    if ((int)edges.size() != n - 1) return false; // Tree property: exactly n-1 edges
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int i) {
        return parent[i] == i ? i : parent[i] = find(parent[i]);
    };

    for (auto& e : edges) {
        int r1 = find(e[0]), r2 = find(e[1]);
        if (r1 == r2) return false; // Cycle detected
        parent[r1] = r2;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Check if edges form a valid single tree.
// - Approach: Disjoint Set Union (DSU / Union-Find).
// - Intuition: A graph is a tree iff it has exactly n-1 edges and contains 0 cycles.
// - Complexity: Time: O(N * alpha(N)), Space: O(N).


// =========================================================
// 52. PACIFIC ATLANTIC WATER FLOW
// =========================================================

void dfsOcean(const vector<vector<int>>& heights, vector<vector<bool>>& vis, int r, int c) {
    vis[r][c] = true;
    int m = heights.size(), n = heights[0].size();
    const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        int nr = r + d[0], nc = c + d[1];
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && !vis[nr][nc]) {
            if (heights[nr][nc] >= heights[r][c]) {
                dfsOcean(heights, vis, nr, nc);
            }
        }
    }
}

vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
    if (heights.empty() || heights[0].empty()) return {};
    int m = heights.size(), n = heights[0].size();
    vector<vector<bool>> pac(m, vector<bool>(n, false));
    vector<vector<bool>> atl(m, vector<bool>(n, false));

    for (int r = 0; r < m; ++r) {
        dfsOcean(heights, pac, r, 0);
        dfsOcean(heights, atl, r, n - 1);
    }
    for (int c = 0; c < n; ++c) {
        dfsOcean(heights, pac, 0, c);
        dfsOcean(heights, atl, m - 1, c);
    }

    vector<vector<int>> res;
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (pac[r][c] && atl[r][c]) res.push_back({r, c});
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find grid coordinates where water can flow to both Pacific and Atlantic oceans.
// - Approach: Reverse DFS from Ocean Boundaries (moving uphill).
// - Intuition: Instead of testing every cell to ocean, start from oceans and find reachable intersections.
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 53. OPEN THE LOCK
// =========================================================

int openLock(vector<string>& deadends, string target) {
    unordered_set<string> dead(deadends.begin(), deadends.end());
    if (dead.count("0000")) return -1;
    if (target == "0000") return 0;

    queue<pair<string, int>> q;
    unordered_set<string> visited;
    q.push({"0000", 0});
    visited.insert("0000");

    while (!q.empty()) {
        auto [curr, steps] = q.front(); q.pop();
        if (curr == target) return steps;

        for (int i = 0; i < 4; ++i) {
            for (int diff : {-1, 1}) {
                string nextComb = curr;
                nextComb[i] = (nextComb[i] - '0' + diff + 10) % 10 + '0';
                if (!dead.count(nextComb) && !visited.count(nextComb)) {
                    visited.insert(nextComb);
                    q.push({nextComb, steps + 1});
                }
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum turns to reach target combination on a 4-wheel lock avoiding deadends.
// - Approach: Level-Order BFS Shortest Path on State Graph (10^4 states).
// - Intuition: Each state has 8 adjacent neighbors; BFS guarantees minimum turns.
// - Complexity: Time: O(10^4 * 8), Space: O(10^4).


// =========================================================
// 54. VALID PARENTHESES
// =========================================================

bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(') st.push(')');
        else if (c == '{') st.push('}');
        else if (c == '[') st.push(']');
        else {
            if (st.empty() || st.top() != c) return false;
            st.pop();
        }
    }
    return st.empty();
}
// Interview Explanation:
// - Problem Statement: Validate string of bracket pairs '()', '{}', '[]'.
// - Approach: Stack Bracket Matching.
// - Intuition: Push expected closing bracket on open; verify matching top on close.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 55. MIN STACK
// =========================================================

class MinStack {
    stack<pair<int, int>> st; // {val, currentMin}
public:
    MinStack() {}
    void push(int val) {
        int curMin = st.empty() ? val : min(val, st.top().second);
        st.push({val, curMin});
    }
    void pop() { st.pop(); }
    int top() { return st.top().first; }
    int getMin() { return st.top().second; }
};
// Interview Explanation:
// - Problem Statement: Stack supporting push, pop, top, and getMin in O(1) time.
// - Approach: Value + Current-Min Pair Stack.
// - Intuition: Every element records the minimum value present in the stack up to that element's height.
// - Complexity: Time: O(1) all ops, Space: O(N).


// =========================================================
// 56. DAILY TEMPERATURES
// =========================================================

vector<int> dailyTemperatures(vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> res(n, 0);
    stack<int> st; // monotonic decreasing stack of indices

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
            int prevIdx = st.top(); st.pop();
            res[prevIdx] = i - prevIdx;
        }
        st.push(i);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find number of days until a warmer temperature.
// - Approach: Monotonic Decreasing Stack of Unresolved Indices.
// - Intuition: When current temp exceeds stack top, pop and calculate distance.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 57. NEXT GREATER ELEMENT I
// =========================================================

vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
    unordered_map<int, int> nextGreater;
    stack<int> st;

    for (int x : nums2) {
        while (!st.empty() && x > st.top()) {
            nextGreater[st.top()] = x;
            st.pop();
        }
        st.push(x);
    }

    vector<int> res;
    for (int x : nums1) {
        res.push_back(nextGreater.count(x) ? nextGreater[x] : -1);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find next greater element in nums2 for each element in nums1.
// - Approach: Monotonic Stack on nums2 + Hash Map Lookup.
// - Intuition: Build mapping of each element to its next greater using stack in O(N).
// - Complexity: Time: O(N + M), Space: O(N).


// =========================================================
// 58. LARGEST RECTANGLE IN HISTOGRAM
// =========================================================

int largestRectangleArea(vector<int>& heights) {
    int n = heights.size(), maxArea = 0;
    stack<int> st; // monotonic increasing stack of bar indices

    for (int i = 0; i <= n; ++i) {
        int h = (i == n) ? 0 : heights[i];
        while (!st.empty() && h < heights[st.top()]) {
            int height = heights[st.top()]; st.pop();
            int width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    return maxArea;
}
// Interview Explanation:
// - Problem Statement: Find largest rectangular area in histogram.
// - Approach: Monotonic Increasing Stack of Indices.
// - Intuition: Each popped bar's height is bounded by the current smaller bar on right and stack top on left.
// - Complexity: Time: O(N), Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 46 | Number of Islands                           | Grid DFS / In-Place Visited Sink  | O(M * N) | O(M * N) |
 | 47 | Rotting Oranges                             | Multi-Source BFS with Level Steps | O(M * N) | O(M * N) |
 | 48 | Course Schedule                             | Kahn's Algorithm (Topological Sort| O(V + E) | O(V + E) |
 | 49 | Clone Graph                                 | Graph DFS + Visited Clone Map     | O(V + E) | O(V)     |
 | 50 | Number of Connected Components              | Graph DFS Traversal               | O(V + E) | O(V + E) |
 | 51 | Graph Valid Tree                            | Disjoint Set Union (DSU) Cycle    | O(N a(N))| O(N)     |
 | 52 | Pacific Atlantic Water Flow                 | Reverse Multi-Source DFS Uphill   | O(M * N) | O(M * N) |
 | 53 | Open the Lock                               | Level-Order BFS Shortest Path     | O(10^4*8)| O(10^4)  |
 | 54 | Valid Parentheses                           | Stack Bracket Matching            | O(N)     | O(N)     |
 | 55 | Min Stack                                   | Value + Min Pair Stack            | O(1)     | O(N)     |
 | 56 | Daily Temperatures                          | Monotonic Decreasing Stack        | O(N)     | O(N)     |
 | 57 | Next Greater Element I                      | Monotonic Stack + Hash Map        | O(N + M) | O(N)     |
 | 58 | Largest Rectangle in Histogram              | Monotonic Increasing Stack        | O(N)     | O(N)     |
 ====================================================================================================
*/
