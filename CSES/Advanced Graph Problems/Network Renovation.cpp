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

#define int long long

using namespace std;

// Simplified DFS: No visited array needed for a tree, just pass the parent 'p'
void dfs(int u, int p, const vector<vector<int>>& adj, vector<int>& leaves_list) {
    // If a node has exactly 1 connection, it is a leaf
    // (This correctly includes the starting node if it happens to be a leaf)
    if (adj[u].size() == 1) {
        leaves_list.push_back(u);
    }

    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, adj, leaves_list);
        }
    }
}

signed main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    
    // Edge case: If there is only 1 computer, 0 new connections are needed.
    if (n == 1) {
        cout << 0 << "\n";
        return 0;
    }

    vector<vector<int>> adj(n + 1);
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> leaves_list;
    // Start DFS from node 1, with parent -1
    dfs(1, -1, adj, leaves_list);
    
    int total_leaves = leaves_list.size();
    int k = (total_leaves + 1) / 2; // Ceiling division
    
    cout << k << "\n";
    
    // Half-split pairing trick
    int half = total_leaves / 2;
    for(int i = 0; i < k; i++){
        // Pair the i-th leaf with the leaf exactly halfway across the list.
        // The modulo operator safely handles the case where total_leaves is odd
        // and the last element wraps around to connect with the middle element.
        cout << leaves_list[i] << " " << leaves_list[(i + half) % total_leaves] << "\n"; // 
    }

    return 0;
}
