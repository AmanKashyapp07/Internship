#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long MOD1 = 1000000007;
const long long MOD2 = 1000000009;
const long long BASE1 = 313;
const long long BASE2 = 317;

// A tree of N nodes will have a bracket sequence of length 2*N
// So we need powers up to 200,000 for N = 100,000
const int MAX_LEN = 200005; 
long long pow1[MAX_LEN], pow2[MAX_LEN];

void precompute() {
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i < MAX_LEN; i++) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1;
        pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }
}

struct HashResult {
    long long h1, h2;
    int len; // Tracks the length of the bracket sequence, not just node count

    bool operator<(const HashResult& other) const {
        if (h1 != other.h1) return h1 < other.h1;
        if (h2 != other.h2) return h2 < other.h2;
        return len < other.len;
    }

    bool operator==(const HashResult& other) const {
        return h1 == other.h1 && h2 == other.h2 && len == other.len;
    }
};

HashResult dfs(int u, int p, const vector<vector<int>>& adj) {
    vector<HashResult> child_results;

    for (int v : adj[u]) {
        if (v != p) {
            child_results.push_back(dfs(v, u, adj));
        }
    }

    // Sort to handle canonical child ordering (ignore branch order)
    sort(child_results.begin(), child_results.end());

    // Start with '1' simulating an opening bracket '('
    long long h1 = 1, h2 = 1; 
    int total_len = 1;

    for (const auto& child : child_results) {
        h1 = (h1 * pow1[child.len] + child.h1) % MOD1;
        h2 = (h2 * pow2[child.len] + child.h2) % MOD2;
        total_len += child.len;
    }

    // End with '2' simulating a closing bracket ')'
    h1 = (h1 * BASE1 + 2) % MOD1;
    h2 = (h2 * BASE2 + 2) % MOD2;
    total_len += 1;

    return {h1, h2, total_len};
}

void solve() {
    int n;
    cin >> n;

    vector<vector<int>> adj1(n + 1), adj2(n + 1);

    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj1[u].push_back(v);
        adj1[v].push_back(u);
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj2[u].push_back(v);
        adj2[v].push_back(u);
    }

    HashResult hash1 = dfs(1, 0, adj1);
    HashResult hash2 = dfs(1, 0, adj2);

    if (hash1 == hash2) cout << "YES\n";
    else cout << "NO\n";
}

int main() {
    // Fast I/O is required for CSES
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    precompute();

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}