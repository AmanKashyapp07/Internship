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

int n, m;
vector<vector<int>> adj;

vector<int> tin, low;
vector<bool> isArticulation;

int timer = 0;

void dfs(int u, int parent) {
    tin[u] = low[u] = ++timer;

    int children = 0;

    for (int v : adj[u]) {

        if (v == parent) continue;

        if (tin[v]) {
            // Back edge
            low[u] = min(low[u], tin[v]);
        }
        else {
            // Tree edge
            dfs(v, u);

            low[u] = min(low[u], low[v]);

            // Non-root articulation point condition
            if (parent != -1 && low[v] >= tin[u]) {
                isArticulation[u] = true;
            }

            children++;
        }
    }

    // Root articulation point condition
    if (parent == -1 && children > 1) {
        isArticulation[u] = true;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    adj.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    tin.assign(n + 1, 0);
    low.assign(n + 1, 0);
    isArticulation.assign(n + 1, false);

    dfs(1, -1);

    vector<int> answer;

    for (int city = 1; city <= n; city++) {
        if (isArticulation[city]) {
            answer.push_back(city);
        }
    }

    cout << answer.size() << '\n';

    for (int city : answer) {
        cout << city << ' ';
    }

    cout << '\n';

    return 0;
}
