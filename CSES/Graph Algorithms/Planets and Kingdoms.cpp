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
    map<int, int> mp;
    cout << sccs.size() << "\n";
    for(int i=0; i<sccs.size(); i++) {
        for(auto x : sccs[i]) {
            mp[x] = i+1;
        }
    }
    for(int i=1; i<=n; i++) {
        cout << mp[i] << " ";
    }
    cout << "\n";
}
// main logic of kosaraju :
// A->B->C->D->E
// these are super nodes, we will run dfs on original graph and push nodes in stack, then we will run dfs on transposed graph and pop nodes from stack, this will give us strongly connected components, because
// after dfs1 on original graph, we will have nodes in stack in order of finishing time, and when we pop nodes from stack and run dfs on transposed graph, we will get strongly connected components, because
// if there is a path from A to B in original graph, then there is a path from B to A in transposed graph, and if there is a path from B to C in original graph, then there is a path from C to B in transposed graph, and so on, and if there is a path from E to A in original graph, then there is a path from A to E in transposed graph, so all nodes are reachables
