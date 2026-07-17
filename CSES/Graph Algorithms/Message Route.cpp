// Link: https://cses.fi/problemset/task/1667

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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);

    while (m--)
    {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> parent(n + 1, -1);
    queue<int> q;

    q.push(1);
    parent[1] = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (parent[v] != -1)
                continue;

            parent[v] = u;
            q.push(v);
        }
    }

    if (parent[n] == -1) // if we cannot reach node n from node 1, print NO and exit
    {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    vector<int> path;
    int end = n;
    int count = 0;
    while (end != 0) // backtrack from node n to node 1 using the parent array to construct the path
    {
        path.push_back(end);
        count++;
        end = parent[end];
    }
    reverse(path.begin(), path.end()); // reverse the path to get the correct order from node 1 to node n
    cout << count << '\n';
    for (int node : path)
    {
        cout << node << ' ';
    }
    cout << '\n';
}