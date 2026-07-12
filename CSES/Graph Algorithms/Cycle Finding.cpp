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

struct Edge
{
    int u, v;
    long long w;
};

void bellman_ford(int n, vector<Edge>& edges)
{
    vector<long long> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);

    int x = -1;

    // Run Bellman-Ford for n iterations
    for (int i = 1; i <= n; i++)
    {
        x = -1;

        for (const auto& edge : edges)
        {
            int u = edge.u;
            int v = edge.v;
            long long w = edge.w;

            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                x = v; // remember a vertex updated in this iteration
            }
        }
    }

    if (x == -1)
    {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";

    // Move inside the cycle
    for (int i = 0; i < n; i++)
    {
        x = parent[x];
    }

    vector<int> cycle;
    int start = x;

    cycle.push_back(start);
    x = parent[start];

    while (x != start)
    {
        cycle.push_back(x);
        x = parent[x];
    }

    cycle.push_back(start);

    reverse(cycle.begin(), cycle.end());

    for (int node : cycle)
    {
        cout << node << " ";
    }
    cout << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);

    for (int i = 0; i < m; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    bellman_ford(n, edges);

    return 0;
}
