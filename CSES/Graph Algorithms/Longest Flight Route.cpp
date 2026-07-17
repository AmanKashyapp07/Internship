// CSES Problem: <problem name>
// https://cses.fi/problemset/task/<id>

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
// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    vector<int> inDegree(n + 1, 0);
    vector<int> answer(n + 1, 0);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        inDegree[v]++;
    }
    answer[1] = 1;
    queue<int> q;
    for (int i = 1; i <= n; i++)
    {
        if (inDegree[i] == 0)
        {
            q.push(i);
        }
    }
    vector<int> result;
    vector<int> parent(n + 1, -1);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u])
        {
            if (answer[u]>0 && answer[v] < answer[u] + 1) 
            {
                answer[v] = answer[u] + 1;
                parent[v] = u;
            }
            inDegree[v]--;
            if(inDegree[v] == 0 && v==n)
            {
                int cur = v;
                while (cur != -1)
                {
                    result.push_back(cur);
                    cur = parent[cur];
                }
                reverse(result.begin(), result.end());
            }
            if (inDegree[v] == 0)
            {
                q.push(v);
            }
        }
    }
    if (answer[n] == 0) cout << "IMPOSSIBLE\n";

    else
    {
        cout << answer[n] << endl;
        for (int x : result)
        {
            cout << x << " ";
        }
        cout << endl;
   
    }
    return 0;
}
