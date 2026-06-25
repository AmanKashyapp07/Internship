/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution
{
public:
    int total = 0;

    void dfs(int node, int parent, int depth, vector<vector<int>> &graph)
    {
        total += depth;
        for (int child : graph[node])
        {
            if (child == parent)
                continue;
            dfs(child, node, depth + 1, graph);
        }
    }

    void dfs_subtree_size(int node, int parent,
                          vector<vector<int>> &graph,
                          vector<int> &subtree_size)
    {
        subtree_size[node] = 1;

        for (int child : graph[node])
        {
            if (child == parent)
                continue;

            dfs_subtree_size(child, node, graph, subtree_size);
            subtree_size[node] += subtree_size[child];
        }
    }

    void dfs_ans(int node, int parent,
                 vector<vector<int>> &graph,
                 vector<int> &subtree_size,
                 vector<int> &ans)
    {
        for (int child : graph[node])
        {
            if (child == parent)
                continue;

            ans[child] = ans[node] - subtree_size[child] + (graph.size() - subtree_size[child]);

            dfs_ans(child, node, graph, subtree_size, ans);
        }
    }
    void bfs(vector<vector<int>> &graph,
             vector<int> &subtree_size,
             vector<int> &ans)
    {
        int n = graph.size();
        queue<pair<int, int>> q; // {node, parent}
        q.push({0, -1});

        while (!q.empty())
        {
            auto [node, parent] = q.front();
            q.pop();

            for (int child : graph[node])
            {
                if (child == parent)
                    continue;

                ans[child] = ans[node] - subtree_size[child] + (n - subtree_size[child]);
                q.push({child, node});
            }
        }
    }
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>> &edges)
    {
        vector<vector<int>> graph(n);

        for (auto &edge : edges)
        {
            graph[edge[0]].push_back(edge[1]);
            graph[edge[1]].push_back(edge[0]);
        }

        vector<int> subtree_size(n);
        vector<int> ans(n);

        dfs(0, -1, 0, graph); // compute total distance from root
        ans[0] = total;

        dfs_subtree_size(0, -1, graph, subtree_size); // compute subtree sizes

        bfs(graph, subtree_size, ans); // compute answers for all nodes

        return ans;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}
