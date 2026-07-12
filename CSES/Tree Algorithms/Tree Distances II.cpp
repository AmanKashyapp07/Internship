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

int n;
vector<vector<int>> adj;
vector<ll> subtreeSize;
vector<ll> ans;

void dfs1(int node, int parent, int depth)
{
    ans[1] += depth;
    subtreeSize[node] = 1;

    for (int child : adj[node])
    {
        if (child == parent)
            continue;

        dfs1(child, node, depth + 1);
        subtreeSize[node] += subtreeSize[child];
    }
}

void dfs2(int node, int parent)
{
    for (int child : adj[node])
    {
        if (child == parent)
            continue;

        ans[child] = ans[node] + n - 2 * subtreeSize[child];
        dfs2(child, node);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    adj.resize(n + 1);
    subtreeSize.resize(n + 1);
    ans.resize(n + 1, 0);

    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs1(1, 0, 0);
    dfs2(1, 0);

    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << " ";
    }
    cout << '\n';

    return 0;
}
