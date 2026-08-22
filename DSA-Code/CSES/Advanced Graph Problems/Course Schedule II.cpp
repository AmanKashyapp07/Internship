// Link: https://cses.fi/problemset/task/1757

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n + 1);
    vector<int> indegree(n + 1, 0);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;

        graph[b].push_back(a); // reverse edge
        indegree[a]++;
    }

    priority_queue<int> pq; // max heap

    for (int i = 1; i <= n; i++)
    {
        if (indegree[i] == 0)
        {
            pq.push(i);
        }
    }

    vector<int> order;

    while (!pq.empty())
    {
        int node = pq.top();
        pq.pop();

        order.push_back(node);

        for (int next : graph[node])
        {
            indegree[next]--;

            if (indegree[next] == 0)
            {
                pq.push(next);
            }
        }
    }

    reverse(order.begin(), order.end());

    for (int x : order)
    {
        cout << x << ' ';
    }

    cout << '\n';
    return 0;
}