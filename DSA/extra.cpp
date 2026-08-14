#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

// Segment Tree for Range Max queries with binary search
// Time: O(N) build, O(log N) query/update, Space: O(4N)

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

// Fenwick Tree (BIT) for prefix/range sums and tree binary search
// Time: O(log N), Space: O(N)

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

    int find(int k) {
        int lo = 0, hi = n - 1, ans = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (query(mid) >= k) {
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    }
};

// Coordinate Compression helper
// Time: O(N log N), Space: O(N)

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

// Euler Tour / Tree Flattening for subtrees and root-paths
// Time: O(N), Space: O(N)

struct EulerTour {
    int n, timer;
    vi tin, tout;

    EulerTour(int n) : n(n), timer(0), tin(n + 1, 0), tout(n + 1, 0) {}

    void dfsSubtree(int u, int p, const vvi &g, vi &flat, const vi &val) {
        tin[u] = ++timer;
        flat[timer - 1] = val[u];
        for (int v : g[u]) {
            if (v != p) dfsSubtree(v, u, g, flat, val);
        }
        tout[u] = timer;
    }

    void dfsPath(int u, int p, const vvi &g, vi &flat, const vi &val) {
        tin[u] = ++timer;
        flat[timer - 1] = val[u];
        for (int v : g[u]) {
            if (v != p) dfsPath(v, u, g, flat, val);
        }
        tout[u] = ++timer;
        flat[timer - 1] = -val[u];
    }
};

// Subtree Max queries with point updates using Euler Tour + SegTree
// Time: O(N) build, O(log N) query/update, Space: O(N)

struct SubtreeMaxEngine {
    EulerTour et;
    SegTree seg;

    SubtreeMaxEngine(int n, const vi &val, const vvi &g)
        : et(n), seg(vector<int>(n)) {
        vi flat(n);
        et.dfsSubtree(1, 0, g, flat, val);
        seg = SegTree(flat);
    }

    void updateNode(int u, int new_val) {
        seg.update(et.tin[u] - 1, new_val);
    }

    int querySubtreeMax(int u) {
        return seg.query(et.tin[u] - 1, et.tout[u] - 1);
    }
};

// Hierholzer's algorithm for Undirected Eulerian Circuit / Path
// Time: O(V + E), Space: O(V + E)

struct HierholzerUndirected {
    int n, m = 0;
    vector<vector<pair<int, int>>> g;
    vector<bool> used;
    vi deg, path;

    HierholzerUndirected(int n) : n(n), g(n + 1), deg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        used.push_back(false);
        deg[u]++;
        deg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, id] = g[u].back();
            g[u].pop_back();

            if (used[id]) continue;
            used[id] = true;
            dfs(v);
        }
        path.push_back(u);
    }

    vi getEulerianCircuit(int start) {
        for (int i = 1; i <= n; i++) {
            if (deg[i] & 1) return {};
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1) return {};
        return path;
    }

    vi getEulerianPath(int start, int end) {
        if (start == end) return getEulerianCircuit(start);

        for (int i = 1; i <= n; i++) {
            if (i == start || i == end) {
                if (!(deg[i] & 1)) return {};
            } else {
                if (deg[i] & 1) return {};
            }
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1) return {};
        return path;
    }
};

// Hierholzer's algorithm for Directed Eulerian Circuit / Path
// Time: O(V + E), Space: O(V + E)

struct HierholzerDirected {
    int n, m = 0;
    vvi g;
    vi indeg, outdeg, path;

    HierholzerDirected(int n)
        : n(n), g(n + 1), indeg(n + 1, 0), outdeg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        outdeg[u]++;
        indeg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back();
            g[u].pop_back();
            dfs(v);
        }
        path.push_back(u);
    }

    vi getEulerianCircuit(int start) {
        for (int i = 1; i <= n; i++) {
            if (indeg[i] != outdeg[i]) return {};
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1) return {};
        return path;
    }

    vi getEulerianPath(int start, int end) {
        if (start == end) return getEulerianCircuit(start);

        for (int i = 1; i <= n; i++) {
            if (i == start) {
                if (outdeg[i] != indeg[i] + 1) return {};
            } else if (i == end) {
                if (indeg[i] != outdeg[i] + 1) return {};
            } else {
                if (indeg[i] != outdeg[i]) return {};
            }
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1) return {};
        return path;
    }
};

// Bipartite Graph utilities (Coloring, Kuhn's Max Matching, Konig's Min Vertex Cover, Max Independent Set)
// Time: O(V * E), Space: O(V + E)

struct BipartiteGraph {
    int n, L, R;
    vvi g;
    vvi g_match;
    vi color;
    vi match_R, match_L, vis;

    BipartiteGraph(int total_nodes)
        : n(total_nodes), L(0), R(0), g(total_nodes + 1), color(total_nodes + 1, -1) {}

    BipartiteGraph(int left_size, int right_size)
        : n(left_size + right_size), L(left_size), R(right_size),
          g_match(left_size + 1), match_R(right_size + 1, 0), match_L(left_size + 1, 0) {}

    void addUndirectedEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void addMatchingEdge(int u, int v) {
        g_match[u].push_back(v);
    }

    bool dfsColor(int u, int c) {
        color[u] = c;
        for (int v : g[u]) {
            if (color[v] == -1) {
                if (!dfsColor(v, c ^ 1)) return false;
            } else if (color[v] == c) {
                return false;
            }
        }
        return true;
    }

    bool isBipartite() {
        for (int i = 1; i <= n; i++) {
            if (color[i] == -1 && !dfsColor(i, 0)) return false;
        }
        return true;
    }

    bool augment(int u) {
        if (vis[u]) return false;
        vis[u] = 1;
        for (int v : g_match[u]) {
            if (match_R[v] == 0 || augment(match_R[v])) {
                match_R[v] = u;
                match_L[u] = v;
                return true;
            }
        }
        return false;
    }

    int maxMatching() {
        int matches = 0;
        match_R.assign(R + 1, 0);
        match_L.assign(L + 1, 0);

        for (int i = 1; i <= L; i++) {
            vis.assign(L + 1, 0);
            if (augment(i)) matches++;
        }
        return matches;
    }

    void dfsVertexCover(int u, vector<bool> &vis_L, vector<bool> &vis_R) {
        vis_L[u] = true;
        for (int v : g_match[u]) {
            if (!vis_R[v]) {
                vis_R[v] = true;
                if (match_R[v] != 0 && !vis_L[match_R[v]]) {
                    dfsVertexCover(match_R[v], vis_L, vis_R);
                }
            }
        }
    }

    pair<vi, vi> minVertexCover() {
        maxMatching();
        vector<bool> vis_L(L + 1, false), vis_R(R + 1, false);

        for (int i = 1; i <= L; i++) {
            if (match_L[i] == 0 && !vis_L[i]) {
                dfsVertexCover(i, vis_L, vis_R);
            }
        }

        vi cover_L, cover_R;
        for (int i = 1; i <= L; i++) {
            if (!vis_L[i]) cover_L.push_back(i);
        }
        for (int i = 1; i <= R; i++) {
            if (vis_R[i]) cover_R.push_back(i);
        }
        return {cover_L, cover_R};
    }

    pair<vi, vi> maxIndependentSet() {
        auto [cover_L, cover_R] = minVertexCover();
        vector<bool> in_cover_L(L + 1, false), in_cover_R(R + 1, false);

        for (int u : cover_L) in_cover_L[u] = true;
        for (int v : cover_R) in_cover_R[v] = true;

        vi ind_L, ind_R;
        for (int i = 1; i <= L; i++) {
            if (!in_cover_L[i]) ind_L.push_back(i);
        }
        for (int i = 1; i <= R; i++) {
            if (!in_cover_R[i]) ind_R.push_back(i);
        }
        return {ind_L, ind_R};
    }
};

// AVL Tree implementation
// Time: O(log N) insert, Space: O(N)

struct AVLNode {
    int val, height;
    AVLNode *left, *right;
    AVLNode(int x) : val(x), height(1), left(nullptr), right(nullptr) {}
};

struct AVLTree {
    int getHeight(AVLNode *node) {
        return node ? node->height : 0;
    }

    void updateHeight(AVLNode *node) {
        if (node) node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    int getBalance(AVLNode *node) {
        return getHeight(node->left) - getHeight(node->right);
    }

    AVLNode* rotateRight(AVLNode *root) {
        AVLNode *new_root = root->left;
        AVLNode *subtree = new_root->right;

        new_root->right = root;
        root->left = subtree;

        updateHeight(root);
        updateHeight(new_root);
        return new_root;
    }

    AVLNode* rotateLeft(AVLNode *root) {
        AVLNode *new_root = root->right;
        AVLNode *subtree = new_root->left;

        new_root->left = root;
        root->right = subtree;

        updateHeight(root);
        updateHeight(new_root);
        return new_root;
    }

    AVLNode* insert(AVLNode *root, int val) {
        if (!root) return new AVLNode(val);

        if (val < root->val) root->left = insert(root->left, val);
        else if (val > root->val) root->right = insert(root->right, val);
        else return root;

        updateHeight(root);

        if (getBalance(root) > 1 && val < root->left->val) return rotateRight(root);
        if (getBalance(root) < -1 && val > root->right->val) return rotateLeft(root);
        if (getBalance(root) > 1 && val > root->left->val) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (getBalance(root) < -1 && val < root->right->val) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }
        return root;
    }

    int sumOfLeafNodes(AVLNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return root->val;
        return sumOfLeafNodes(root->left) + sumOfLeafNodes(root->right);
    }
};

// Dinic's Algorithm for Maximum Flow / Minimum Cut
// Time: O(V^2 * E), Space: O(V + E)

struct Dinic {
    struct FlowEdge {
        int to, rev;
        ll cap;
    };

    int nodes;
    vector<vector<FlowEdge>> adj;
    vi level, it;

    Dinic(int n) : nodes(n), adj(n + 1), level(n + 1), it(n + 1) {}

    void addEdge(int u, int v, ll cap) {
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }

    bool bfs(int src, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(src);
        level[src] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &e : adj[u]) {
                if (e.cap && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[sink] != -1;
    }

    ll dfs(int u, int sink, ll flow) {
        if (u == sink || !flow) return flow;
        for (int &i = it[u]; i < (int)adj[u].size(); i++) {
            auto &e = adj[u][i];
            if (level[e.to] != level[u] + 1 || !e.cap) continue;
            ll pushed = dfs(e.to, sink, min(flow, e.cap));
            if (pushed) {
                e.cap -= pushed;
                adj[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
        return 0;
    }

    ll maxFlow(int src, int sink) {
        ll flow = 0, pushed;
        while (bfs(src, sink)) {
            fill(it.begin(), it.end(), 0);
            while ((pushed = dfs(src, sink, LLONG_MAX))) flow += pushed;
        }
        return flow;
    }
};

// Z-algorithm / Z-array calculation
// Time: O(N log N) via FastHash, Space: O(N)

vi z_array(const string &s) {
    int n = s.size();
    vi z(n, 0);
    if (n == 0) return z;

    FastHash fh(s);
    z[0] = n;
    for (int i = 1; i < n; i++) {
        int l = 1, r = n - i, ans = 0;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (fh.get(0, mid - 1) == fh.get(i, i + mid - 1)) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        z[i] = ans;
    }
    return z;
}

// KMP prefix function (pi-array) using Z-array
// Time: O(N), Space: O(N)

vi pi_array(const string &s) {
    vi z = z_array(s);
    int n = s.size();
    vi pi(n, 0);

    for (int i = 1; i < n; i++) {
        for (int len = z[i] - 1; len >= 0; len--) {
            if (pi[i + len] != 0) break;
            pi[i + len] = len + 1;
        }
    }
    return pi;
}

// Manacher's algorithm variant via FastHash for palindrome radii
// Time: O(N log N), Space: O(N)

pair<vi, vi> manacherr(const string &s) {
    int n = s.size();
    vi d1(n, 1), d2(n, 0);
    if (n == 0) return {d1, d2};

    FastHash fh(s);
    string s_rev = s;
    reverse(s_rev.begin(), s_rev.end());
    FastHash fh_rev(s_rev);

    auto is_pal = [&](int l, int r) {
        return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l);
    };

    for (int i = 0; i < n; i++) {
        int l = 2, r = min(i + 1, n - i);
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) {
                d1[i] = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        l = 1; r = min(i, n - i);
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (is_pal(i - mid, i + mid - 1)) {
                d2[i] = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
    }
    return {d1, d2};
}

// Linked List implementation with common interview patterns
// Time: O(N) operations, Space: O(1)

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int v) : val(v), next(nullptr) {}
};

struct LinkedList {
    ListNode *head;

    LinkedList() : head(nullptr) {}

    void pushFront(int val) {
        ListNode *node = new ListNode(val);
        node->next = head;
        head = node;
    }

    void pushBack(int val) {
        ListNode *node = new ListNode(val);
        if (!head) {
            head = node;
            return;
        }
        ListNode *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

    void remove(int val) {
        if (!head) return;
        ListNode dummy(0);
        dummy.next = head;
        ListNode *prev = &dummy, *cur = head;

        while (cur) {
            if (cur->val == val) {
                prev->next = cur->next;
                delete cur;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        head = dummy.next;
    }

    void reverseIterative() {
        ListNode *prev = nullptr, *cur = head, *nxt = nullptr;
        while (cur) {
            nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        head = prev;
    }

    ListNode* reverseRecursive(ListNode *node) {
        if (!node || !node->next) return node;
        ListNode *new_head = reverseRecursive(node->next);
        node->next->next = node;
        node->next = nullptr;
        return new_head;
    }

    ListNode* findMiddle() {
        if (!head) return nullptr;
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    bool hasCycle() {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }

    ListNode* detectCycleStart() {
        ListNode *slow = head, *fast = head;
        bool found = false;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                found = true;
                break;
            }
        }
        if (!found) return nullptr;

        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }

    void removeNthFromEnd(int n) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode *fast = &dummy, *slow = &dummy;

        for (int i = 0; i <= n; i++) {
            if (!fast) return;
            fast = fast->next;
        }

        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }

        ListNode *to_delete = slow->next;
        slow->next = slow->next->next;
        delete to_delete;
        head = dummy.next;
    }

    bool isPalindrome() {
        if (!head || !head->next) return true;
        ListNode *rev = nullptr, *slow = head, *fast = head;

        while (fast && fast->next) {
            fast = fast->next->next;
            ListNode *nxt = slow->next;
            slow->next = rev;
            rev = slow;
            slow = nxt;
        }
        if (fast) slow = slow->next;

        bool is_pal = true;
        while (rev && slow) {
            if (rev->val != slow->val) {
                is_pal = false;
                break;
            }
            rev = rev->next;
            slow = slow->next;
        }
        return is_pal;
    }

    ~LinkedList() {
        ListNode *cur = head;
        while (cur) {
            ListNode *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
};

// Tarjan's algorithm for finding bridges
// Time: O(V + E), Space: O(V + E)

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

// Tarjan's algorithm for articulation points and component separation
// Time: O(V + E), Space: O(V + E)

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

// Sum Over Subsets (SOS) DP for bitmask queries
// Time: O(B * 2^B), Space: O(2^B)

struct SOS {
    int B = 20, N = 1 << B;
    vi f, sub, sup;

    SOS() : f(N), sub(N), sup(N) {}

    void add(int x) { f[x]++; }

    void build() {
        sub = sup = f;
        for (int b = 0; b < B; b++) {
            for (int m = 0; m < N; m++) {
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)];
                else sup[m] += sup[m | (1 << b)];
            }
        }
    }

    int subsets(int x) { return sub[x]; }
    int supersets(int x) { return sup[x]; }
    int disjoint(int x) { return sub[(N - 1) ^ x]; }
    int intersect(int x, int n) { return n - disjoint(x); }
};

// Custom Max-Heap implementation
// Time: O(log N) push/pop, O(N) build, Space: O(N)

struct MaxHeap {
    vi heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) { swap(heap[i], heap[j]); }

    void heapifyDown(int i) {
        int largest = i, l = left(i), r = right(i);
        if (l < (int)heap.size() && heap[l] > heap[largest]) largest = l;
        if (r < (int)heap.size() && heap[r] > heap[largest]) largest = r;
        if (largest != i) {
            swapNodes(i, largest);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    void insert(int val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    int top() { return heap[0]; }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }

    void buildHeap(const vi &nums) {
        heap = nums;
        for (int i = (int)heap.size() / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    vi heapSort() {
        vi original = heap, ans;
        while (!heap.empty()) {
            ans.push_back(top());
            pop();
        }
        heap = original;
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
