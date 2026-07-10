# Essential C++ OA Templates (Min LOC)

High-performance, minimal LOC implementations of DSU, Polynomial Rolling Hash (FastHash), Bridge Detection, Strongly Connected Components (SCC), and Digit DP optimized for competitive programming and Online Assessments.

---

## 1. Disjoint Set Union (DSU)

*   **Complexity:** $O(1)$ amortized find/unite operations ($O(\alpha(N))$ using path compression and union by size).
*   **Indexing:** 1-based indexing.
*   **Return Value:** `unite(a, b)` returns the size of the combined component.

```cpp
struct DSU {
    vector<int> parent, sz;
    
    DSU(int n) {
        parent.resize(n + 1); sz.assign(n + 1, 1);
        for(int i=1;i<=n;i++) parent[i] = i; // 1-based indexing
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]); // Path compression
    }

    int unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return sz[a];
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a; sz[a] += sz[b]; // Union by size
        return sz[a];
    } // returns the size of the new component after uniting a and b
};
```

---

## 2. Polynomial Rolling Hash (FastHash)

*   **Complexity:** Preprocessing $O(N)$ time, substring hash query $O(1)$ time.
*   **Collision Prevention:** Uses polynomial rolling hash. Recommended values: $P = 31$, $MOD = 10^9 + 7$.

```cpp
typedef long long ll;
const int P = 31; // Common primes: 31, 53
const int MOD = 1e9 + 7; // Or 1e9 + 9, or 998244353

class FastHash {
private:
    vector<ll> power, pref;

public:
    FastHash(const string& s) {
        int n = s.size();
        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    // Returns the polynomial hash of substring s[l..r] in O(1) time
    ll get(int l, int r) const {
        return (pref[r + 1] - pref[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }

    // Computes the hash of an entire string in O(m) time
    static ll hashWord(const string& s) {
        ll h = 0;
        for (char c : s) {
            h = (h * P + (c - 'a' + 1)) % MOD;
        }
        return h;
    }
};
```

---

## 3. Bridge Detection (Tarjan's Algorithm)

*   **Complexity:** $O(V + E)$ time and space.
*   **Output:** Returns a list of pairs `{{u, v}, ...}` representing all bridges (critical connections) in an undirected graph.

```cpp
struct BridgeDetection {
    int timer = 0;
    vector<int> tin, low;
    vector<pair<int, int>> bridges;

    void dfs(int u, int p, const vector<vector<int>>& adj) {
        tin[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) { // Back-edge
                low[u] = min(low[u], tin[v]);
            } else { // Tree-edge
                dfs(v, u, adj);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    bridges.push_back({u, v});
                }
            }
        }
    }

    vector<pair<int, int>> findBridges(int n, const vector<vector<int>>& adj) {
        tin.assign(n + 1, 0);
        low.assign(n + 1, 0);
        timer = 0;
        bridges.clear();
        for (int i = 1; i <= n; i++) {
            if (!tin[i]) dfs(i, 0, adj);
        }
        return bridges;
    }
};
```

---

## 4. Strongly Connected Components (Kosaraju's Algorithm)

*   **Complexity:** $O(V + E)$ time and space.
*   **Output:** Generates components where each component contains a set of nodes belonging to that cycle block. Also supports 1-based indexing.

```cpp
#include <vector>
#include <stack>
#include <numeric>
#include <algorithm>

using namespace std;

struct SCC {
    int n;
    vector<vector<int>> adj, radj;
    stack<int> todo;
    vector<int> comp, comps;
    vector<bool> vis;

    SCC(int n) : n(n), adj(n + 1), radj(n + 1), comp(n + 1, -1), vis(n + 1, false) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : adj[u]) if (!vis[v]) dfs1(v);
        todo.push(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        comps.push_back(u);
        for (int v : radj[u]) if (comp[v] == -1) dfs2(v, c);
    }

    vector<vector<int>> build() {
        for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
        vector<vector<int>> sccs;
        while (!todo.empty()) {
            int u = todo.top();
            todo.pop();
            if (comp[u] == -1) {
                comps.clear();
                dfs2(u, sccs.size());
                sccs.push_back(comps);
            }
        }
        return sccs;
    }
};
```

---

## 5. Digit DP General Template

*   **Complexity:** $O(\text{digits} \times \text{custom\_states} \times 10)$.
*   **Usage:** For counting numbers in a range with dynamic digit parameters. Handles leading zero checks (`started`) and bounds checking (`tight`).

```cpp
class DigitDP {
private:
    string s;
    int memo[20][2][2][100]; // Dimensions: [pos][started][tight][custom_state]

    long long dp(int pos, bool started, bool tight, int custom_state) {
        if (pos == s.size()) {
            return custom_state; // Or evaluate custom_state against condition
        }
        if (memo[pos][started][tight][custom_state] != -1) {
            return memo[pos][started][tight][custom_state];
        }

        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        // Option 1: Skip placing a digit at this position (leading zeros)
        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == limit), custom_state);
        }

        // Option 2: Place a digit
        int start_digit = started ? 0 : 1;
        for (int d = start_digit; d <= limit; d++) {
            int next_state = custom_state + d; // Adjust logic for custom properties
            ans += dp(pos + 1, true, tight && (d == limit), next_state);
        }

        return memo[pos][started][tight][custom_state] = ans;
    }

public:
    long long solve(long long n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};
```
