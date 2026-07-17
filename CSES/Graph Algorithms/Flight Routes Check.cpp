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

vector<vector<int>> g, rg;
vector<bool> vis;
stack<int> st;

void dfs1(int u)
{
    vis[u] = true;

    for (int v : g[u])
        if (!vis[v])
            dfs1(v);

    st.push(u);
} // this function performs a DFS on the original graph and pushes nodes onto a stack in the order of their finishing times. This is the first step of Kosaraju's algorithm for finding strongly connected components (SCCs).

void dfs2(int u, vector<int>& comp)
{
    vis[u] = true;
    comp.push_back(u);

    for (int v : rg[u])
        if (!vis[v])
            dfs2(v, comp);
} // this function performs a DFS on the transposed graph (rg) and collects all nodes reachable from u into the comp vector. This identifies one strongly connected component.

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    g.resize(n + 1);
    rg.resize(n + 1);

    while (m--)
    {
        int a, b;
        cin >> a >> b;

        g[a].push_back(b);
        rg[b].push_back(a);
    }

    vis.assign(n + 1, false);

    for (int i = 1; i <= n; i++)
        if (!vis[i])
            dfs1(i);

    vis.assign(n + 1, false);

    vector<vector<int>> sccs;

    while (!st.empty())
    {
        int u = st.top();
        st.pop();

        if (vis[u]) continue;

        vector<int> comp;
        dfs2(u, comp); // this will fill the comp vector with all nodes in the strongly connected component containing u
        sccs.push_back(comp);
    }

    if (sccs.size() == 1)
    {
        cout << "YES\n";
        return 0;
    }

    cout << "NO\n";
    cout << sccs[1][0] << ' ' << sccs[0][0] << '\n'; // print one node from the second SCC and one node from the first SCC, indicating that there is no path from the second SCC to the first SCC in the original graph.
}