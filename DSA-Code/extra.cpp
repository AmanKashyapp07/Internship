#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#include <complex>
#include <chrono>
#include <random>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

/*
 ====================================================================================================
     STRICT WARNING: DO NOT READ / PREPARE THESE ALGORITHMS FOR OA OR INTERVIEWS! 
 ----------------------------------------------------------------------------------------------------
  These algorithms and data structures are ADVANCED, NICHE, and SPECIALIZED (typically appearing in
  Codeforces Div 1 / Grandmaster contests, IOI, ICPC World Finals, and Theoretical Computer Science).
  They are UNLIKELY to be asked in standard Tech Company Online Assessments (OA) or technical interviews.
  They are included just for fun.
 ====================================================================================================
                                      ALGORITHM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Algorithm / Data Structure                  | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Heavy-Light Decomposition (HLD)             | Tree Heavy Chains + Lazy SegTree  | O(log^2N)| O(N)     |
 | 2  | Centroid Decomposition of a Tree            | Tree Divide & Conquer (Sz <= N/2) | O(N logN)| O(N)     |
 | 3  | Fully Persistent Segment Tree               | Path-Copying Dynamic Node Storage | O(log N) | O(N+QlogN|
 | 4  | Implicit Treap (Cartesian Tree)             | Randomized BST + Lazy Tag Split   | O(log N) | O(N)     |
 | 5  | Aho-Corasick Automaton                      | Trie + Suffix Failure & Dict Link | O(N Sigma| O(N Sigma|
 | 6  | Suffix Automaton (SAM / DAWG)               | Linear Substring DFA (Endpos Eq)  | O(N)     | O(N Sigma|
 | 7  | Manacher's Algorithm & Z-Algorithm          | Palindrome Symmetry & LCP Match   | O(N)     | O(N)     |
 | 8  | Mo's Algorithm (Hilbert Order)              | 2D Fractal Hilbert Space Curve    | O(NsqrtN)| O(N + Q) |
 | 9  | Dinic's Maximum Network Flow & Min-Cut      | BFS Level Graph + DFS Blocking    | O(V^2 E) | O(V + E) |
 | 10 | Min-Cost Max-Flow (MCMF)                    | Successive Shortest Path + Potent.| O(FElogV)| O(V + E) |
 | 11 | Hopcroft-Karp Bipartite Matching            | Multi-Source BFS + Disjoint DFS   | O(EsqrtV)| O(V + E) |
 | 12 | Number Theoretic Transform (NTT)            | Radix-2 Convolution (Mod 998244353| O(N logN)| O(N)     |
 | 13 | Dynamic Convex Hull Trick & Li Chao Tree    | Dynamic Line Multiset & Li Chao   | O(log N) | O(N)     |
 | 14 | 2-SAT Solver (2-Satisfiability)             | Implication Graph + Tarjan SCCs   | O(V + E) | O(V + E) |
 | 15 | XOR Linear Basis (GF(2) Gaussian Elim)      | Bitwise Basis Independence Vector | O(Bits)  | O(Bits)  |
 | 16 | Hungarian Algorithm (Kuhn-Munkres)          | Primal-Dual Matrix Dual Potentials| O(N^3)   | O(N^2)   |
 | 17 | Tarjan's Bridge Detection                   | DFS Discovery (tin) + Low-Links   | O(V + E) | O(V + E) |
 | 18 | Tarjan's Articulation Points                | Low-Links + Ancestor Binary Search| O(V + E) | O(V + E) |
 | 19 | Segment Tree (Range Max & Find First)       | Recursive Halves + Tree Binary Srch| O(log N)| O(N)     |
 | 20 | Fenwick Tree & Coordinate Compression       | Lowest Set Bit (i & -i) + Ranking | O(log N) | O(N)     |
 ====================================================================================================
*/


// ============================================================================
// 01. HEAVY-LIGHT DECOMPOSITION (HLD) WITH LAZY SEGMENT TREE
// ============================================================================
// Decomposes a tree into vertex-disjoint heavy paths such that any path
// between any two vertices crosses at most O(log N) light edges.
// Allows path queries/updates and subtree queries/updates in O(log^2 N) / O(log N).

struct HLD {
    int n, timer;
    vvi adj;
    vi parent, depth, heavy, head, pos, sub_size;

    // Segment tree on flattened Euler tour indices pos[u]
    vl tree, lazy;

    HLD(int n) : n(n), timer(0), adj(n), parent(n, -1), depth(n, 0),
                 heavy(n, -1), head(n), pos(n), sub_size(n, 1),
                 tree(4 * n, 0), lazy(4 * n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int dfs_size(int u, int p, int d) {
        parent[u] = p;
        depth[u] = d;
        int max_c_size = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            int c_size = dfs_size(v, u, d + 1);
            sub_size[u] += c_size;
            if (c_size > max_c_size) {
                max_c_size = c_size;
                heavy[u] = v;
            }
        }
        return sub_size[u];
    }

    void dfs_hld(int u, int h) {
        head[u] = h;
        pos[u] = timer++;
        if (heavy[u] != -1) {
            dfs_hld(heavy[u], h); // Heavy edge maintains same chain head
        }
        for (int v : adj[u]) {
            if (v != parent[u] && v != heavy[u]) {
                dfs_hld(v, v); // Light edge starts a new heavy path chain
            }
        }
    }

    void build(int root = 0) {
        dfs_size(root, -1, 0);
        dfs_hld(root, root);
    }

    // Segment Tree Lazy Operations
    void push(int node, int l, int r) {
        if (lazy[node] != 0) {
            int mid = (l + r) / 2;
            tree[2 * node] += lazy[node] * (mid - l + 1);
            lazy[2 * node] += lazy[node];
            tree[2 * node + 1] += lazy[node] * (r - mid);
            lazy[2 * node + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    void update_range(int node, int l, int r, int ql, int qr, ll val) {
        if (ql <= l && r <= qr) {
            tree[node] += val * (r - l + 1);
            lazy[node] += val;
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) update_range(2 * node, l, mid, ql, qr, val);
        if (qr > mid) update_range(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    ll query_range(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[node];
        push(node, l, r);
        int mid = (l + r) / 2;
        ll res = 0;
        if (ql <= mid) res += query_range(2 * node, l, mid, ql, qr);
        if (qr > mid) res += query_range(2 * node + 1, mid + 1, r, ql, qr);
        return res;
    }

    // Path Operations
    void update_path(int u, int v, ll val) {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
            update_range(1, 0, n - 1, pos[head[v]], pos[v], val);
            v = parent[head[v]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        update_range(1, 0, n - 1, pos[u], pos[v], val);
    }

    ll query_path(int u, int v) {
        ll res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
            res += query_range(1, 0, n - 1, pos[head[v]], pos[v]);
            v = parent[head[v]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        res += query_range(1, 0, n - 1, pos[u], pos[v]);
        return res;
    }

    // Subtree Operations (contiguous range in Euler tour [pos[u], pos[u] + sub_size[u] - 1])
    void update_subtree(int u, ll val) {
        update_range(1, 0, n - 1, pos[u], pos[u] + sub_size[u] - 1, val);
    }

    ll query_subtree(int u) {
        return query_range(1, 0, n - 1, pos[u], pos[u] + sub_size[u] - 1);
    }
};
// Algorithm Deep Dive:
// - Concept: Heavy-Light Decomposition classifies edges as heavy (leading to subtree of max size) or light.
// - Intuition: Traversing a light edge cuts the remaining subtree size by at least half; thus, any u -> v path
//   traverses at most O(log N) light edges and O(log N) continuous heavy paths.
// - Complexity:
//   * Build: O(N)
//   * Path Query / Update: O(log^2 N)
//   * Subtree Query / Update: O(log N)
//   * Space: O(N)


// ============================================================================
// 02. CENTROID DECOMPOSITION OF A TREE
// ============================================================================
// Divide and Conquer on trees. Recursively finds the tree centroid (node whose
// removal leaves subtrees of size <= N/2) to build a Centroid Tree of depth O(log N).

struct CentroidDecomposition {
    int n;
    vvi adj;
    vector<bool> removed;
    vi sub_size, parent_c;

    CentroidDecomposition(int n) : n(n), adj(n), removed(n, false), sub_size(n, 0), parent_c(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int get_sizes(int u, int p) {
        sub_size[u] = 1;
        for (int v : adj[u]) {
            if (v != p && !removed[v]) {
                sub_size[u] += get_sizes(v, u);
            }
        }
        return sub_size[u];
    }

    int get_centroid(int u, int p, int tree_size) {
        for (int v : adj[u]) {
            if (v != p && !removed[v] && sub_size[v] > tree_size / 2) {
                return get_centroid(v, u, tree_size);
            }
        }
        return u;
    }

    int build_tree(int u, int p = -1) {
        int sz = get_sizes(u, -1);
        int centroid = get_centroid(u, -1, sz);
        removed[centroid] = true;
        parent_c[centroid] = p;

        for (int v : adj[centroid]) {
            if (!removed[v]) {
                build_tree(v, centroid);
            }
        }
        return centroid;
    }
};
// Algorithm Deep Dive:
// - Concept: Decomposes a tree of arbitrary topology into a balanced Centroid Tree of height O(log N).
// - Intuition: Any simple path between two nodes u and v in the original tree passes through their Lowest Common
//   Ancestor in the centroid tree. We can process all paths passing through the current centroid in O(Size).
// - Complexity:
//   * Construction: O(N log N)
//   * Query / Update per node: O(log N)
//   * Space: O(N)


// ============================================================================
// 03. FULLY PERSISTENT SEGMENT TREE (WITH SUBARRAY K-TH SMALLEST)
// ============================================================================
// Maintains history of all modifications. Instead of mutating nodes, updates clone
// only the O(log N) nodes on the path from root to leaf, sharing unchanged subtrees.

struct PersistentSegTree {
    struct Node {
        int count;
        int left, right;
    };

    int n;
    vector<Node> tree;
    vi roots;

    PersistentSegTree(int n) : n(n) {
        tree.push_back({0, 0, 0}); // 0 index is null node
    }

    int new_node(int count, int left, int right) {
        tree.push_back({count, left, right});
        return (int)tree.size() - 1;
    }

    int update(int prev_root, int l, int r, int val) {
        if (l == r) {
            return new_node(tree[prev_root].count + 1, 0, 0);
        }
        int mid = (l + r) / 2;
        if (val <= mid) {
            int new_left = update(tree[prev_root].left, l, mid, val);
            return new_node(tree[prev_root].count + 1, new_left, tree[prev_root].right);
        } else {
            int new_right = update(tree[prev_root].right, mid + 1, r, val);
            return new_node(tree[prev_root].count + 1, tree[prev_root].left, new_right);
        }
    }

    // Subarray [L, R] K-th smallest element query in O(log N)
    // Compares version root[R] with root[L-1]
    int query_kth(int root_r, int root_l_minus_1, int l, int r, int k) {
        if (l == r) return l;
        int left_count = tree[tree[root_r].left].count - tree[tree[root_l_minus_1].left].count;
        int mid = (l + r) / 2;
        if (k <= left_count) {
            return query_kth(tree[root_r].left, tree[root_l_minus_1].left, l, mid, k);
        } else {
            return query_kth(tree[root_r].right, tree[root_l_minus_1].right, mid + 1, r, k - left_count);
        }
    }
};
// Algorithm Deep Dive:
// - Concept: Persistent data structures preserve previous versions after updates.
// - Intuition: A point update alters at most one path of length O(log N). By copying only this path and pointing
//   unaltered child pointers to previous version nodes, we obtain complete snapshotting in O(log N) space per update.
// - Complexity:
//   * Update: O(log N) time, O(log N) space per version.
//   * Range K-th Query: O(log N) time.
//   * Total Space for Q updates: O(N + Q log N).


// ============================================================================
// 04. IMPLICIT TREAP (RANDOMIZED CARTESIAN TREE WITH LAZY REVERSAL)
// ============================================================================
// Dynamic array supporting arbitrary insertions, deletions, range sum queries,
// and range reversals in O(log N) expected time using Treap Split & Merge.

mt19937 rng(1337);

struct ImplicitTreap {
    struct Node {
        int val, priority, size;
        ll sum;
        bool lazy_rev;
        Node *l, *r;
        Node(int v) : val(v), priority(rng()), size(1), sum(v), lazy_rev(false), l(nullptr), r(nullptr) {}
    };

    Node* root = nullptr;

    int sz(Node* t) { return t ? t->size : 0; }
    ll get_sum(Node* t) { return t ? t->sum : 0; }

    void update(Node* t) {
        if (!t) return;
        t->size = 1 + sz(t->l) + sz(t->r);
        t->sum = t->val + get_sum(t->l) + get_sum(t->r);
    }

    void push(Node* t) {
        if (t && t->lazy_rev) {
            swap(t->l, t->r);
            if (t->l) t->l->lazy_rev ^= 1;
            if (t->r) t->r->lazy_rev ^= 1;
            t->lazy_rev = false;
        }
    }

    // Splits treap t into L (first k elements) and R (remaining elements)
    void split(Node* t, int k, Node*& l, Node*& r) {
        if (!t) { l = r = nullptr; return; }
        push(t);
        if (sz(t->l) >= k) {
            split(t->l, k, l, t->l);
            r = t;
        } else {
            split(t->r, k - sz(t->l) - 1, t->r, r);
            l = t;
        }
        update(t);
    }

    // Merges treaps l and r (all elements in l come before r)
    void merge(Node*& t, Node* l, Node* r) {
        push(l);
        push(r);
        if (!l || !r) {
            t = l ? l : r;
            return;
        }
        if (l->priority > r->priority) {
            merge(l->r, l->r, r);
            t = l;
        } else {
            merge(r->l, l, r->l);
            t = r;
        }
        update(t);
    }

    void insert(int pos, int val) {
        Node *l, *r;
        split(root, pos, l, r);
        merge(l, l, new Node(val));
        merge(root, l, r);
    }

    void reverse(int ql, int qr) { // 0-indexed [ql, qr]
        Node *l, *mid, *r;
        split(root, ql, l, mid);
        split(mid, qr - ql + 1, mid, r);
        if (mid) mid->lazy_rev ^= 1;
        merge(root, l, mid);
        merge(root, root, r);
    }

    ll query_sum(int ql, int qr) {
        Node *l, *mid, *r;
        split(root, ql, l, mid);
        split(mid, qr - ql + 1, mid, r);
        ll ans = get_sum(mid);
        merge(root, l, mid);
        merge(root, root, r);
        return ans;
    }
};
// Algorithm Deep Dive:
// - Concept: Implicit Treap maintains sequence indices via subtree sizes instead of explicit keys.
// - Intuition: Heap priorities ensure balanced O(log N) depth on average. Any interval [L, R] can be isolated
//   into a dedicated subtree by two split operations, modified/queried, and merged back.
// - Complexity:
//   * Insert / Delete: O(log N) expected.
//   * Range Reverse / Range Sum / Range Shift: O(log N) expected.
//   * Space: O(N).


// ============================================================================
// 05. AHO-CORASICK AUTOMATON (MULTI-PATTERN STRING MATCHING)
// ============================================================================
// Constructs a Trie with suffix failure links (`fail`) and dictionary links (`dict_link`).
// Matches multiple dictionary patterns simultaneously in linear time O(|T| + sum(|P_i|)).

struct AhoCorasick {
    static const int ALPHABET = 26;
    struct Node {
        int next[ALPHABET];
        int fail = 0;
        int dict_link = 0; // Quick jump to nearest matching ancestor
        vi pattern_indices;
        Node() { fill(begin(next), end(next), 0); }
    };

    vector<Node> trie;

    AhoCorasick() { trie.emplace_back(); }

    void insert(const string& s, int id) {
        int u = 0;
        for (char c : s) {
            int ch = c - 'a';
            if (!trie[u].next[ch]) {
                trie[u].next[ch] = trie.size();
                trie.emplace_back();
            }
            u = trie[u].next[ch];
        }
        trie[u].pattern_indices.push_back(id);
    }

    void build() {
        queue<int> q;
        for (int ch = 0; ch < ALPHABET; ch++) {
            if (trie[0].next[ch]) {
                q.push(trie[0].next[ch]);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();

            // Set dictionary link
            int f = trie[u].fail;
            trie[u].dict_link = (!trie[f].pattern_indices.empty()) ? f : trie[f].dict_link;

            for (int ch = 0; ch < ALPHABET; ch++) {
                if (trie[u].next[ch]) {
                    trie[trie[u].next[ch]].fail = trie[f].next[ch];
                    q.push(trie[u].next[ch]);
                } else {
                    trie[u].next[ch] = trie[f].next[ch]; // Automaton transition optimization
                }
            }
        }
    }

    // Matches text T and returns for each pattern its match count
    vi match_all(const string& text, int num_patterns) {
        vi matches(num_patterns, 0);
        int u = 0;
        for (char c : text) {
            u = trie[u].next[c - 'a'];
            int curr = u;
            while (curr > 0) {
                for (int id : trie[curr].pattern_indices) {
                    matches[id]++;
                }
                curr = trie[curr].dict_link;
            }
        }
        return matches;
    }
};
// Algorithm Deep Dive:
// - Concept: Extension of KMP algorithm to a dictionary of patterns represented as a Trie.
// - Intuition: Failure link of node u points to the node representing the longest proper suffix of the string
//   at u that is also a prefix in the trie. Text is processed in a single pass without backtracking.
// - Complexity:
//   * Construction: O(sum(|P_i|) * Sigma)
//   * Search: O(|T| + total match occurrences)
//   * Space: O(sum(|P_i|) * Sigma)


// ============================================================================
// 06. SUFFIX AUTOMATON (SAM / DIRECTED ACYCLIC WORD GRAPH)
// ============================================================================
// Minimal Deterministic Finite Automaton (DFA) recognizing all substrings of a string.
// Built online in strictly linear time O(N). Has at most 2N-1 states and 3N-4 transitions.

struct SuffixAutomaton {
    struct State {
        int len, link;
        map<char, int> next;
        ll cnt = 0; // Number of endpos occurrences
        bool is_clone = false;
    };

    vector<State> st;
    int sz, last;

    SuffixAutomaton(int maxlen = 1) {
        st.resize(maxlen * 2);
        st[0].len = 0;
        st[0].link = -1;
        sz = 1;
        last = 0;
    }

    void extend(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                st[clone].is_clone = true;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string& s) {
        for (char c : s) extend(c);
    }

    // Number of distinct substrings in S in O(N)
    ll count_distinct_substrings() {
        ll total = 0;
        for (int i = 1; i < sz; i++) {
            total += st[i].len - st[st[i].link].len;
        }
        return total;
    }
};
// Algorithm Deep Dive:
// - Concept: Suffix Automaton compresses all substrings of a string into a minimal DAG based on equivalent right end-positions (endpos equivalence classes).
// - Intuition: Each state corresponds to a continuous length range of substrings sharing the exact same set of end-positions.
// - Complexity:
//   * Build Time: O(N log Sigma) or O(N Sigma)
//   * Space: O(N Sigma) (at most 2N-1 states)
//   * Substring queries: O(|Pattern|) exact matching.


// ============================================================================
// 07. MANACHER'S ALGORITHM (LINEAR PALINDROMES) & Z-ALGORITHM
// ============================================================================
// Manacher computes palindrome radii for all centers in strictly O(N) time.
// Z-Algorithm computes longest common prefix array between S and S[i...N-1] in O(N).

struct Manacher {
    string t;
    vi p; // p[i] = radius of palindrome centered at i in transformed string t

    Manacher(const string& s) {
        // Transform s -> "^#a#b#c#$" to handle both even and odd length palindromes uniformly
        t = "^";
        for (char c : s) { t += "#"; t += c; }
        t += "#$";
        p.assign(t.size(), 0);
        int c = 0, r = 0;
        for (int i = 1; i < (int)t.size() - 1; i++) {
            int i_mirror = 2 * c - i;
            if (r > i) p[i] = min(r - i, p[i_mirror]);
            while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
            if (i + p[i] > r) {
                c = i;
                r = i + p[i];
            }
        }
    }

    // Check if substring s[l...r] (0-indexed) is a palindrome in O(1)
    bool is_palindrome(int l, int r) {
        int center = l + r + 2; // Map to transformed coordinates
        int radius = r - l + 1;
        return p[center] >= radius;
    }
};

vi z_function(const string& s) {
    int n = s.size();
    vi z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}
// Algorithm Deep Dive:
// - Manacher: Uses previous palindrome symmetry to avoid re-examining characters within current right boundary.
// - Z-Algorithm: Maintains [l, r] match interval to skip redundant comparisons.
// - Complexity: Both strictly O(N) time and O(N) space.


// ============================================================================
// 08. MO'S ALGORITHM WITH HILBERT ORDERING (OFFLINE RANGE QUERIES)
// ============================================================================
// Answers offline range queries on arrays in O((N + Q) sqrt(N)) by ordering queries
// along a space-filling Hilbert curve (or block parity) to minimize L and R pointer sweeps.

int64_t hilbert_order(int x, int y, int pow = 21, int rotate = 0) {
    if (pow == 0) return 0;
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
    seg = (seg + rotate) & 3;
    const int rotate_delta[4] = {3, 0, 0, 1};
    int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
    int nrot = (rotate + rotate_delta[seg]) & 3;
    int64_t sub_square_size = int64_t(1) << (2 * pow - 2);
    int64_t ans = seg * sub_square_size;
    int64_t add = hilbert_order(nx, ny, pow - 1, nrot);
    ans += (seg == 1 || seg == 2) ? add : (sub_square_size - add - 1);
    return ans;
}

struct MoQuery {
    int l, r, id;
    int64_t order;
    MoQuery(int l, int r, int id) : l(l), r(r), id(id) {
        order = hilbert_order(l, r);
    }
    bool operator<(const MoQuery& other) const {
        return order < other.order;
    }
};

struct MoSolver {
    int distinct_elements = 0;
    vi freq;

    MoSolver(int max_val) : freq(max_val + 1, 0) {}

    void add(int val) {
        if (freq[val] == 0) distinct_elements++;
        freq[val]++;
    }

    void remove(int val) {
        freq[val]--;
        if (freq[val] == 0) distinct_elements--;
    }

    vi solve(const vi& a, vector<MoQuery>& queries) {
        sort(queries.begin(), queries.end());
        vi answers(queries.size());
        int cur_l = 0, cur_r = -1;

        for (const auto& q : queries) {
            while (cur_l > q.l) add(a[--cur_l]);
            while (cur_r < q.r) add(a[++cur_r]);
            while (cur_l < q.l) remove(a[cur_l++]);
            while (cur_r > q.r) remove(a[cur_r--]);
            answers[q.id] = distinct_elements;
        }
        return answers;
    }
};
// Algorithm Deep Dive:
// - Concept: Offline query reordering where interval transitions [cur_l, cur_r] -> [next_l, next_r] are batched.
// - Intuition: Hilbert curve maps 2D query points (l, r) onto a 1D fractal that strictly minimizes Manhattan distance.
// - Complexity:
//   * Time: O((N + Q) sqrt(N))
//   * Space: O(N + Q)


// ============================================================================
// 09. DINIC'S MAXIMUM FLOW ALGORITHM WITH MIN-CUT EXTRACTION
// ============================================================================
// Computes maximum network flow using BFS layered level graphs and DFS blocking flows
// with current-edge pointer optimization.

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap, flow;
    };

    int n, s, t;
    vector<vector<Edge>> adj;
    vi level, ptr;

    Dinic(int n, int s, int t) : n(n), s(s), t(t), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, ll cap) {
        adj[from].push_back({to, (int)adj[to].size(), cap, 0});
        adj[to].push_back({from, (int)adj[from].size() - 1, 0, 0}); // 0 capacity for reverse edge in directed flow
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto& e : adj[u]) {
                if (e.cap - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(int u, ll pushed) {
        if (pushed == 0 || u == t) return pushed;
        for (int& cid = ptr[u]; cid < (int)adj[u].size(); cid++) {
            auto& e = adj[u][cid];
            int tr = e.to;
            if (level[u] + 1 != level[tr] || e.cap - e.flow == 0) continue;
            ll tr_pushed = dfs(tr, min(pushed, e.cap - e.flow));
            if (tr_pushed == 0) continue;
            e.flow += tr_pushed;
            adj[tr][e.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    ll max_flow() {
        ll flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(s, LLONG_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }

    // Min-Cut partition: returns true for all vertices in source-side S of the minimum cut
    vector<bool> get_min_cut_source_set() {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto& e : adj[u]) {
                if (e.cap - e.flow > 0 && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return visited;
    }
};
// Algorithm Deep Dive:
// - Concept: Finds blocking flow in a level graph at each phase.
// - Intuition: Constructing layered BFS trees guarantees path lengths strictly increase each phase (at most V phases).
// - Complexity:
//   * General Networks: O(V^2 E)
//   * Unit Networks / Bipartite: O(E sqrt(V))
//   * Space: O(V + E)


// ============================================================================
// 10. MIN-COST MAX-FLOW (MCMF VIA SUCCESSIVE SHORTEST PATH WITH POTENTIALS)
// ============================================================================
// Finds the maximum flow from s to t that minimizes the total edge cost.
// Employs Johnson potentials so that Dijkstra's algorithm can be used iteratively.

struct MCMF {
    struct Edge {
        int to, rev;
        ll cap, flow, cost;
    };

    int n;
    vector<vector<Edge>> adj;
    vl dist, potential;
    vi parent_node, parent_edge;

    MCMF(int n) : n(n), adj(n), dist(n), potential(n, 0), parent_node(n), parent_edge(n) {}

    void add_edge(int from, int to, ll cap, ll cost) {
        adj[from].push_back({to, (int)adj[to].size(), cap, 0, cost});
        adj[to].push_back({from, (int)adj[from].size() - 1, 0, 0, -cost});
    }

    pair<ll, ll> min_cost_max_flow(int s, int t) {
        ll total_flow = 0, total_cost = 0;

        while (true) {
            // Priority Queue Dijkstra with reduced costs
            fill(dist.begin(), dist.end(), LLONG_MAX);
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d > dist[u]) continue;

                for (int i = 0; i < (int)adj[u].size(); i++) {
                    const auto& e = adj[u][i];
                    if (e.cap - e.flow > 0) {
                        ll reduced_cost = e.cost + potential[u] - potential[e.to];
                        if (dist[u] + reduced_cost < dist[e.to]) {
                            dist[e.to] = dist[u] + reduced_cost;
                            parent_node[e.to] = u;
                            parent_edge[e.to] = i;
                            pq.push({dist[e.to], e.to});
                        }
                    }
                }
            }

            if (dist[t] == LLONG_MAX) break; // No more augmenting paths

            // Update potentials
            for (int i = 0; i < n; i++) {
                if (dist[i] != LLONG_MAX) potential[i] += dist[i];
            }

            // Find bottleneck capacity along shortest path
            ll push = LLONG_MAX;
            for (int u = t; u != s; u = parent_node[u]) {
                const auto& e = adj[parent_node[u]][parent_edge[u]];
                push = min(push, e.cap - e.flow);
            }

            // Augment flow
            for (int u = t; u != s; u = parent_node[u]) {
                auto& e = adj[parent_node[u]][parent_edge[u]];
                e.flow += push;
                adj[u][e.rev].flow -= push;
                total_cost += push * e.cost;
            }
            total_flow += push;
        }

        return {total_flow, total_cost};
    }
};
// Algorithm Deep Dive:
// - Concept: Augments flow along the shortest path in the residual graph with respect to edge costs.
// - Intuition: Potentials ensure reduced edge costs are non-negative, enabling Dijkstra instead of SPFA.
// - Complexity:
//   * Time: O(Flow * E log V)
//   * Space: O(V + E)


// ============================================================================
// 11. HOPCROFT-KARP ALGORITHM (MAXIMUM BIPARTITE MATCHING)
// ============================================================================
// Finds Maximum Cardinality Matching in bipartite graphs in O(E sqrt(V)) time.

struct HopcroftKarp {
    int n, m; // n: size of left partition, m: size of right partition
    vvi adj;
    vi pair_u, pair_v, dist;

    HopcroftKarp(int n, int m) : n(n), m(m), adj(n + 1), pair_u(n + 1, 0), pair_v(m + 1, 0), dist(n + 1) {}

    void add_edge(int u, int v) { // 1-indexed u in [1..n], v in [1..m]
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (pair_u[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INT_MAX;
            }
        }
        dist[0] = INT_MAX;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    if (dist[pair_v[v]] == INT_MAX) {
                        dist[pair_v[v]] = dist[u] + 1;
                        q.push(pair_v[v]);
                    }
                }
            }
        }
        return dist[0] != INT_MAX;
    }

    bool dfs(int u) {
        if (u == 0) return true;
        for (int v : adj[u]) {
            if (dist[pair_v[v]] == dist[u] + 1) {
                if (dfs(pair_v[v])) {
                    pair_v[v] = u;
                    pair_u[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INT_MAX;
        return false;
    }

    int max_matching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 1; u <= n; u++) {
                if (pair_u[u] == 0 && dfs(u)) {
                    matching++;
                }
            }
        }
        return matching;
    }
};
// Algorithm Deep Dive:
// - Concept: Multi-source BFS discovers shortest alternating augmenting paths; DFS extracts maximal set of vertex-disjoint paths in a single phase.
// - Complexity:
//   * Time: O(E sqrt(V))
//   * Space: O(V + E)


// ============================================================================
// 12. NUMBER THEORETIC TRANSFORM (NTT) & POLYNOMIAL MULTIPLICATION
// ============================================================================
// Fast polynomial convolution modulo 998244353 (primitive root g = 3) in O(N log N) time.

struct NTT {
    const ll MOD = 998244353;
    const ll G = 3; // Primitive root modulo 998244353

    ll power(ll base, ll exp) {
        ll res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    ll mod_inverse(ll n) {
        return power(n, MOD - 2);
    }

    void transform(vl& a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = power(G, (MOD - 1) / len);
            if (invert) wlen = mod_inverse(wlen);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j];
                    ll v = (a[i + j + len / 2] * w) % MOD;
                    a[i + j] = (u + v < MOD ? u + v : u + v - MOD);
                    a[i + j + len / 2] = (u - v >= 0 ? u - v : u - v + MOD);
                    w = (w * wlen) % MOD;
                }
            }
        }

        if (invert) {
            ll n_inv = mod_inverse(n);
            for (ll& x : a) x = (x * n_inv) % MOD;
        }
    }

    vl multiply(vl a, vl b) {
        int n = 1;
        while (n < (int)(a.size() + b.size())) n <<= 1;
        a.resize(n);
        b.resize(n);
        transform(a, false);
        transform(b, false);
        for (int i = 0; i < n; i++) a[i] = (a[i] * b[i]) % MOD;
        transform(a, true);
        while (a.size() > 1 && a.back() == 0) a.pop_back();
        return a;
    }
};
// Algorithm Deep Dive:
// - Concept: Discrete Fourier Transform over finite fields GF(MOD). Replaces complex roots of unity with primitive roots.
// - Intuition: Converts polynomial multiplication from O(N^2) coefficient representation to O(N) point-value representation.
// - Complexity:
//   * Multiplication Time: O(N log N)
//   * Space: O(N)


// ============================================================================
// 13. DYNAMIC CONVEX HULL TRICK (LINE CONTAINER) & LI CHAO TREE
// ============================================================================
// Line Container: Maintains a set of lines y = k*x + m dynamically, querying max k*x + m in O(log N).
// Li Chao Tree: Segment Tree storing linear functions on continuous or discrete coordinate domains.

struct Line {
    mutable ll k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    // Computes intersection of two lines
    static const ll INF = LLONG_MAX;
    ll div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = INF; return false; }
        if (x->k == y->k) x->p = (x->m > y->m) ? INF : -INF;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }

    void add(ll k, ll m) { // Add line y = k*x + m
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }

    ll query(ll x) { // Queries max(k*x + m)
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};

struct LiChaoTree {
    struct LineFunc {
        ll m, c;
        ll eval(ll x) const { return m * x + c; }
    };

    int n;
    static constexpr ll INF = 1e18;
    vector<LineFunc> tree;

    LiChaoTree(int n) : n(n), tree(4 * n, {0, -INF}) {}

    void insert(int node, int l, int r, LineFunc nw) {
        int mid = (l + r) / 2;
        bool left_better = nw.eval(l) > tree[node].eval(l);
        bool mid_better = nw.eval(mid) > tree[node].eval(mid);

        if (mid_better) swap(tree[node], nw);

        if (l == r) return;

        if (left_better != mid_better) {
            insert(2 * node, l, mid, nw);
        } else {
            insert(2 * node + 1, mid + 1, r, nw);
        }
    }

    ll query(int node, int l, int r, ll x) {
        ll current_val = tree[node].eval(x);
        if (l == r) return current_val;
        int mid = (l + r) / 2;
        if (x <= mid) return max(current_val, query(2 * node, l, mid, x));
        else return max(current_val, query(2 * node + 1, mid + 1, r, x));
    }
};
// Algorithm Deep Dive:
// - Concept: Speeds up 1D DP state transitions dp[i] = max_j(dp[j] + slope * x) from O(N^2) to O(N log N).
// - Complexity:
//   * Line insertion: O(log N) amortized
//   * Query: O(log N)
//   * Space: O(N)


// ============================================================================
// 14. 2-SAT SOLVER (2-SATISFIABILITY VIA TARJAN'S SCC)
// ============================================================================
// Solves Boolean Satisfiability where clauses have <= 2 literals: (x_i or x_j).
// Builds implication graph (not x_i -> x_j and not x_j -> x_i).
// If variable x and not x belong to same SCC -> UNSATISFIABLE.

struct TwoSAT {
    int n; // Number of boolean variables (1 to n)
    vvi adj;
    vi tin, low, scc, scc_assignment;
    vector<bool> in_stack;
    stack<int> st;
    int timer = 0, scc_count = 0;

    TwoSAT(int n) : n(n), adj(2 * n + 2), tin(2 * n + 2, 0), low(2 * n + 2, 0),
                    scc(2 * n + 2, 0), scc_assignment(n + 1, 0), in_stack(2 * n + 2, false) {}

    // Variable mapping: x -> 2*x, not x -> 2*x + 1
    int var_pos(int x) { return (x > 0) ? 2 * x : 2 * (-x) + 1; }
    int var_neg(int x) { return (x > 0) ? 2 * x + 1 : 2 * (-x); }

    void add_clause_or(int u, int v) {
        // (u or v) <=> (not u -> v) and (not v -> u)
        adj[var_neg(u)].push_back(var_pos(v));
        adj[var_neg(v)].push_back(var_pos(u));
    }

    void tarjan_dfs(int u) {
        tin[u] = low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (!tin[v]) {
                tarjan_dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                low[u] = min(low[u], tin[v]);
            }
        }

        if (low[u] == tin[u]) {
            scc_count++;
            while (true) {
                int node = st.top(); st.pop();
                in_stack[node] = false;
                scc[node] = scc_count;
                if (node == u) break;
            }
        }
    }

    bool solve() {
        for (int i = 2; i <= 2 * n + 1; i++) {
            if (!tin[i]) tarjan_dfs(i);
        }

        for (int i = 1; i <= n; i++) {
            if (scc[2 * i] == scc[2 * i + 1]) {
                return false; // Contradiction: x and not x in same SCC
            }
            // In Tarjan, lower SCC index corresponds to topologically later SCC
            scc_assignment[i] = (scc[2 * i] < scc[2 * i + 1]);
        }
        return true;
    }
};
// Algorithm Deep Dive:
// - Concept: Implication graph maps logical constraints (A or B) to directed edges (~A -> B).
// - Intuition: A valid truth assignment exists iff no variable shares an SCC with its negation.
// - Complexity:
//   * Time: O(V + E) strictly linear in number of clauses and variables.
//   * Space: O(V + E).


// ============================================================================
// 15. XOR LINEAR BASIS (GAUSSIAN ELIMINATION OVER GF(2))
// ============================================================================
// Vector space over GF(2). Maintains minimal linearly independent basis under XOR.
// Solves: max XOR subset, min XOR subset, check span, K-th distinct XOR sum.

template<int BITS = 60>
struct XorBasis {
    ll basis[BITS];
    int sz = 0;

    XorBasis() { fill(begin(basis), end(basis), 0); }

    bool insert(ll mask) {
        for (int i = BITS - 1; i >= 0; i--) {
            if ((mask >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = mask;
                    sz++;
                    return true; // Linearly independent element added
                }
                mask ^= basis[i];
            }
        }
        return false; // Vector is already in the span of the basis
    }

    ll query_max(ll base = 0) {
        ll ans = base;
        for (int i = BITS - 1; i >= 0; i--) {
            ans = max(ans, ans ^ basis[i]);
        }
        return ans;
    }

    ll query_min(ll base = 0) {
        ll ans = base;
        for (int i = BITS - 1; i >= 0; i--) {
            if ((ans >> i) & 1) ans ^= basis[i];
        }
        return ans;
    }
};
// Algorithm Deep Dive:
// - Concept: Gaussian Elimination over the binary field F_2.
// - Intuition: Triangulates a system of bit vectors so each basis element has a unique highest set bit.
// - Complexity:
//   * Insert / Query: O(BITS) where BITS <= 60.
//   * Space: O(BITS).


// ============================================================================
// 16. HUNGARIAN ALGORITHM (KUHN-MUNKRES MIN-WEIGHT BIPARTITE MATCHING)
// ============================================================================
// Solves the Assignment Problem (Minimum weight perfect matching in weighted bipartite graphs)
// in strictly O(N^3) time.

struct Hungarian {
    int n;
    vvl cost;

    Hungarian(int n) : n(n), cost(n + 1, vl(n + 1, 0)) {}

    void set_cost(int i, int j, ll c) { // 1-indexed
        cost[i][j] = c;
    }

    pair<ll, vi> solve() {
        vl u(n + 1, 0), v(n + 1, 0);
        vi p(n + 1, 0), way(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            p[0] = i;
            int j0 = 0;
            vl minv(n + 1, LLONG_MAX);
            vector<bool> used(n + 1, false);

            do {
                used[j0] = true;
                int i0 = p[j0], j1 = 0;
                ll delta = LLONG_MAX;

                for (int j = 1; j <= n; j++) {
                    if (!used[j]) {
                        ll cur = cost[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }

                for (int j = 0; j <= n; j++) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != 0);
        }

        vi match(n + 1);
        for (int j = 1; j <= n; j++) match[p[j]] = j;

        ll min_cost = -v[0];
        return {min_cost, match};
    }
};
// Algorithm Deep Dive:
// - Concept: Dual formulation of the maximum weight bipartite matching problem (Primal-Dual algorithm).
// - Intuition: Maintains vertex potentials u[i] and v[j] such that cost[i][j] - u[i] - v[j] >= 0; iteratively reduces slack until a perfect matching is found.
// - Complexity:
//   * Time: O(N^3)
//   * Space: O(N^2)


// ============================================================================
// 17. TARJAN'S BRIDGE DETECTION & COMPONENT SEPARATION
// ============================================================================

struct Bridge {
    int timer;
    vi tin, tout, low, parent;
    vector<pair<int, int>> bridges;

    void dfs(int u, int p, vvi &g) {
        parent[u] = p;
        tin[u] = low[u] = ++timer;

        for (int v : g[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u, g);
                low[u] = min(low[u], low[v]);

                if (low[v] > tin[u])
                    bridges.push_back({u, v});
            }
        }
        tout[u] = timer;
    }

    void build(vvi &g) {
        int n = g.size();
        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        parent.assign(n, -1);
        bridges.clear();

        for (int i = 0; i < n; i++) {
            if (!tin[i]) dfs(i, -1, g);
        }
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    bool separated(int u, int v, int a, int b) {
        if (parent[u] == v) swap(u, v);
        return isAncestor(v, a) != isAncestor(v, b);
    }
};


// ============================================================================
// 18. TARJAN'S ARTICULATION POINTS & CUT-VERTEX SEPARATION
// ============================================================================

struct Articulation {
    int timer;
    vi tin, tout, low, par;
    vvi child;

    void dfs(int u, int p, vvi &g) {
        par[u] = p;
        tin[u] = low[u] = ++timer;

        for (int v : g[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                child[u].push_back(v);
                dfs(v, u, g);
                low[u] = min(low[u], low[v]);
            }
        }
        tout[u] = timer;
    }

    void build(vvi &g) {
        int n = g.size();
        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        par.assign(n, -1);
        child.assign(n, {});

        for (int i = 0; i < n; i++) {
            if (!tin[i]) dfs(i, -1, g);
        }
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    int findChild(int u, int x) {
        int l = 0, r = (int)child[u].size() - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            int v = child[u][mid];
            if (tin[x] < tin[v]) r = mid - 1;
            else if (isAncestor(v, x)) return v;
            else l = mid + 1;
        }
        return -1;
    }

    int component(int cut, int x) {
        if (cut == x) return -1;
        if (par[cut] == x) return cut;
        if (!isAncestor(cut, x)) return par[cut];
        return findChild(cut, x);
    }

    bool separated(int cut, int a, int b) {
        if (a == cut || b == cut) return true;
        return component(cut, a) != component(cut, b);
    }
};


// ============================================================================
// 19. SEGMENT TREE (RANGE MAX & BINARY SEARCH)
// ============================================================================

struct SegNode {
    int val;
};

struct SegTree {
    int n;
    vector<SegNode> tree;

    SegTree(const vi &a) : n(a.size()), tree(4 * n) {
        build(1, 0, n - 1, a);
    }

    SegNode identity() { return {INT_MIN}; }
    SegNode make_node(int x) { return {x}; }
    SegNode merge(SegNode L, SegNode R) { return {max(L.val, R.val)}; }

    void build(int u, int lo, int hi, const vi &a) {
        if (lo == hi) {
            tree[u] = make_node(a[lo]);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        build(2 * u, lo, mid, a);
        build(2 * u + 1, mid + 1, hi, a);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1]);
    }

    void update(int idx, int val) { update(1, 0, n - 1, idx, val); }

    void update(int u, int lo, int hi, int idx, int val) {
        if (lo == hi) {
            tree[u] = make_node(val);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        if (idx <= mid) update(2 * u, lo, mid, idx, val);
        else update(2 * u + 1, mid + 1, hi, idx, val);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1]);
    }

    int query(int L, int R) { return query(1, 0, n - 1, L, R).val; }

    SegNode query(int u, int lo, int hi, int L, int R) {
        if (hi < L || R < lo) return identity();
        if (L <= lo && hi <= R) return tree[u];
        int mid = lo + (hi - lo) / 2;
        return merge(query(2 * u, lo, mid, L, R), query(2 * u + 1, mid + 1, hi, L, R));
    }

    int find_first(int val) { return find_first(1, 0, n - 1, val); }

    int find_first(int u, int lo, int hi, int val) {
        if (tree[u].val < val) return -1;
        if (lo == hi) return lo;
        int mid = lo + (hi - lo) / 2;
        if (tree[2 * u].val >= val) return find_first(2 * u, lo, mid, val);
        return find_first(2 * u + 1, mid + 1, hi, val);
    }
};


// ============================================================================
// 20. FENWICK TREE (BINARY INDEXED TREE) & COORDINATE COMPRESSION
// ============================================================================

struct Fenwick {
    int n;
    vi bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, int val) {
        i++;
        while (i <= n) {
            bit[i] += val;
            i += i & -i;
        }
    }

    int query(int i) {
        int s = 0;
        i++;
        while (i >= 1) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }

    int query(int l, int r) {
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};

struct CoordinateCompressor {
    vi vals;

    void add(int x) { vals.push_back(x); }

    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }

    int get(int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    }

    int size() { return vals.size(); }
};
