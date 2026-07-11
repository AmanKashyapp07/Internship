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
    int comp;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        comp = n;
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        comp--;
        return true;
    }

    int size(int x) {
        return sz[find(x)];
    }

    DSU dsu(n);

    dsu.unite(u, v);          // merge
    dsu.find(x);              // component ID (root)
    dsu.size(x);              // size of x's component
    dsu.comp;                 // number of connected components
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

    bool equal(const FastHash& H, int l1, int r1, int l2, int r2) {
        return H.get(l1, r1) == H.get(l2, r2);
    }
    FastHash H(s);
    cout << equal(H, 0, 2, 5, 7);
};
```

---

## 3. Bridge Detection (Tarjan's Algorithm)

*   **Complexity:** $O(V + E)$ time and space.
*   **Output:** Returns a list of pairs `{{u, v}, ...}` representing all bridges (critical connections) in an undirected graph.

```cpp
struct Bridge {
    int t = 0;
    vector<int> tin, low;
    vector<pair<int,int>> bridges;

    void dfs(int u, int p, vector<vector<int>>& adj) {
        tin[u] = low[u] = ++t;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u, adj);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges.push_back({u, v});
            }
        }
    }

    vector<pair<int,int>> get(vector<vector<int>>& adj) {
        int n = adj.size();
        tin.assign(n, 0);
        low.assign(n, 0);
        t = 0;
        bridges.clear();
        for (int i = 0; i < n; i++)
            if (!tin[i]) dfs(i, -1, adj);
        return bridges;
    }

    Bridge B;
    vector<pair<int,int>> bridges = B.get(adj);
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

    SCC scc(n);
    scc.addEdge(u, v);
    vector<vector<int>> sccs = scc.build();
    scc.comp[u]; // component ID of u
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

    DigitDP dp;
    long long count = dp.solve(n);
};
```

## 6. Binary Lifting (LCA & K-th Ancestor)

- **Complexity:** `O(N log N)` preprocessing time, `O(log N)` per LCA / K-th ancestor / distance query.
- **Indexing:** 1-based indexing.
- **Dummy Node:** Node `0` is used as a dummy node for out-of-bounds ancestors.

```cpp
void dfs(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p; // first ancestor (parent)

    for (int j = 1; j < LOG; j++) {
        int prev = up[u][j - 1];
        if (prev != -1)
            up[u][j] = up[prev][j - 1];
    }

    for (int v : adj[u]) {
        if (v != p)
            dfs(v, u, d + 1);
    }
}

int lift(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) {
            u = up[u][j];

            // Tried to lift beyond the root
            if (u == -1)
                return -1;
        }
    }

    // Lift using the set bits in k
    return u;
}

int lca(int a, int b) {
    // Ensure 'a' is the deeper node
    if (depth[a] < depth[b])
        swap(a, b);

    // Bring both nodes to the same depth
    a = lift(a, depth[a] - depth[b]);

    if (a == b)
        return a;

    // Lift both nodes until their ancestors differ
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }

    // Parent of either node is the LCA
    return up[a][0];
}

// Initialisation
LOG = 32 - __builtin_clz(n);
up.assign(n + 1, vector<int>(LOG, -1));
depth.assign(n + 1, 0);
dfs(root, -1, 0);

// Usage
int ancestor = lift(node, k);
int lca_node = lca(u, v);
int dist = depth[u] + depth[v] - 2 * depth[lca(u, v)];
```

# Binary Trie (OA Template)

```cpp
struct Node {
    Node *c[2] = {};
    int cnt = 0;
};

struct Trie {
    Node *root = new Node();

    void insert(int x) {
        Node *cur = root;
        for (int b = 31; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (!cur->c[i]) cur->c[i] = new Node();
            cur = cur->c[i];
            cur->cnt++;
        }
    }

    int maxXor(int x) {
        Node *cur = root;
        int ans = 0;
        for (int b = 31; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (cur->c[i ^ 1]) ans |= 1 << b, cur = cur->c[i ^ 1];
            else cur = cur->c[i];
        }
        return ans;
    }

    int countLess(int x, int k) {
        Node *cur = root;
        int ans = 0;
        for (int b = 31; b >= 0 && cur; b--) {
            int xb = (x >> b) & 1, kb = (k >> b) & 1;
            if (kb) {
                if (cur->c[xb]) ans += cur->c[xb]->cnt;
                cur = cur->c[xb ^ 1];
            } else cur = cur->c[xb];
        }
        return ans;
    }
};
```

## Usage

### Create Trie

```cpp
Trie T;
```

### Insert Number

```cpp
T.insert(x);
```

### Maximum XOR with `x`

```cpp
int ans = T.maxXor(x);
```

Returns the maximum value of:

```text
x ^ y
```

where `y` is a number already inserted.

---

### Count Numbers with `(x ^ y) < k`

```cpp
int cnt = T.countLess(x, k);
```

Returns the number of inserted values `y` satisfying:

```text
(x ^ y) < k
```

---

## Complexity

| Operation | Time |
|-----------|------|
| `insert(x)` | O(32) |
| `maxXor(x)` | O(32) |
| `countLess(x, k)` | O(32) |

**Space:** `O(32 × N)` nodes in the worst case.

# Trie (OA Template)

```cpp
struct Node {
    Node *c[26] = {};
    bool end = 0;
    int endCnt = 0, preCnt = 0;
};

struct Trie {
    Node *root = new Node();

    void insert(string s) {
        Node *cur = root;
        for (char ch : s) {
            int i = ch - 'a';
            if (!cur->c[i]) cur->c[i] = new Node();
            cur = cur->c[i];
            cur->preCnt++;
        }
        cur->end = 1;
        cur->endCnt++;
    }

    bool search(string s) {
        Node *cur = root;
        for (char ch : s) {
            if (!(cur = cur->c[ch - 'a'])) return 0;
        }
        return cur->end;
    }

    bool startsWith(string s) {
        Node *cur = root;
        for (char ch : s) {
            if (!(cur = cur->c[ch - 'a'])) return 0;
        }
        return 1;
    }

    int countEqual(string s) {
        Node *cur = root;
        for (char ch : s) {
            if (!(cur = cur->c[ch - 'a'])) return 0;
        }
        return cur->endCnt;
    }

    int countPrefix(string s) {
        Node *cur = root;
        for (char ch : s) {
            if (!(cur = cur->c[ch - 'a'])) return 0;
        }
        return cur->preCnt;
    }
};
```

## Usage

### Create Trie

```cpp
Trie T;
```

### Insert a Word

```cpp
T.insert(word);
```

---

### Search Exact Word

```cpp
bool found = T.search(word);
```

Returns `true` if the word exists.

---

### Check Prefix

```cpp
bool ok = T.startsWith(prefix);
```

Returns `true` if at least one inserted word starts with `prefix`.

---

### Count Exact Occurrences

```cpp
int cnt = T.countEqual(word);
```

Returns the number of times `word` was inserted.

---

### Count Words with Given Prefix

```cpp
int cnt = T.countPrefix(prefix);
```

Returns the number of inserted words that start with `prefix`.

---

## Complexity

| Operation | Time |
|-----------|------|
| `insert()` | O(L) |
| `search()` | O(L) |
| `startsWith()` | O(L) |
| `countEqual()` | O(L) |
| `countPrefix()` | O(L) |

where **L** is the length of the string.

**Space:** `O(26 × total inserted characters)` in the worst case.