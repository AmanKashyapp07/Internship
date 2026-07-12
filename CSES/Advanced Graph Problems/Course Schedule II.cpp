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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1, 0);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;

        adj[b].push_back(a); // reverse edge
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

        for (int next : adj[node])
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
