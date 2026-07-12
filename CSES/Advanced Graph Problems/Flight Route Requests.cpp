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

void findComponent(
    int u,
    int compId,
    const vector<vector<int>>& undirected,
    vector<int>& component
) {
    component[u] = compId;

    for (int v : undirected[u]) {
        if (component[v] == -1) {
            findComponent(v, compId, undirected, component);
        }
    }
}

void detectCycle(
    int u,
    const vector<vector<int>>& directed,
    vector<int>& state,
    vector<int>& component,
    vector<bool>& hasCycle
) {
    state[u] = 1;

    for (int v : directed[u]) {
        if (state[v] == 0) {
            detectCycle(v, directed, state, component, hasCycle);
        }
        else if (state[v] == 1) {
            hasCycle[component[u]] = true;
        }
    }

    state[u] = 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> directed(n + 1);
    vector<vector<int>> undirected(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;

        directed[u].push_back(v);

        // For weakly connected components
        undirected[u].push_back(v);
        undirected[v].push_back(u);
    }

    vector<int> component(n + 1, -1);
    vector<int> compSize;
    int compCount = 0;

    // Find weakly connected components
    for (int i = 1; i <= n; i++) {
        if (component[i] == -1) {
            compSize.push_back(0);

            findComponent(i, compCount, undirected, component);

            compCount++;
        }
    }

    for (int i = 1; i <= n; i++) {
        compSize[component[i]]++;
    }

    vector<int> state(n + 1, 0);
    vector<bool> hasCycle(compCount, false);

    // Detect cycles
    for (int i = 1; i <= n; i++) {
        if (state[i] == 0) {
            detectCycle(i, directed, state, component, hasCycle);
        }
    }

    long long answer = 0;

    for (int c = 0; c < compCount; c++) {
        answer += hasCycle[c] ? compSize[c] : compSize[c] - 1;
    }

    cout << answer << '\n';
}
