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
    vector<int> indegree(n + 1);
 
    while (m--)
    {
        int a, b;
        cin >> a >> b;
 
        adj[a].push_back(b);
        indegree[b]++;
    }
 
    queue<int> q;
 
    for (int i = 1; i <= n; i++)
    {
        if (indegree[i] == 0)
            q.push(i);
    }
 
    vector<int> topo;
 
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
 
        topo.push_back(u);
 
        for (int v : adj[u])
        {
            if (--indegree[v] == 0)
                q.push(v);
        }
    }
 
    if ((int)topo.size() != n)
    {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
 
    for (int x : topo)
        cout << x << ' ';
    cout << '\n';
}