/*
What: Range Query + Range Update DS (Segment Tree with Lazy Propagation)

Time:
- Build  : O(N)
- Query  : O(log N)
- Update : O(log N)

Use When:
- Need to update a whole range of index values (e.g., add X to all elements from index L to R)
- Multiple range queries on sum, min, max, etc.

Common Ops: Range Sum, Range Min, Range Max

When NOT to use:
- Only point updates are needed → Standard Segment Tree / Fenwick Tree (BIT)
- Static array → Sparse Table / Prefix Array

Core Idea:
Updates are deferred (held in a `lazy` array) and only propagated down the tree when a query or another update visits the node.

To Modify (For Range Minimum instead of Sum):
- Change `tree[node] += lazy[node] * (r - l + 1)` to `tree[node] += lazy[node]` in push()
- Change '+' to 'min' in update() and query() transitions
- Change identity return value from `0` to `LLONG_MAX` in query()

Usage:
LazySegmentTree st(arr);
st.update(l, r, val); // Adds val to all elements in range [l, r]
st.query(l, r);       // Returns sum of elements in range [l, r]
============================================================
*/
#include <algorithm>
#include <array>
#include <bitset>
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
#define sz(x) ((int)(x).size())

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

struct LazySegmentTree {
    int n;
    vector<long long> tree, lazy;

    LazySegmentTree(vector<int>& a) {
        n = a.size();
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        build(1, 0, n - 1, a);
    }

    void build(int node, int l, int r, vector<int>& a) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Propagates lazy updates to children
    void push(int node, int l, int r) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node] * (r - l + 1); // For Range Min/Max, remove *(r-l+1)
            if (l != r) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update(int node, int l, int r, int ql, int qr, long long val) {
        push(node, l, r);
        if (qr < l || ql > r) return;
        if (ql <= l && r <= qr) {
            lazy[node] += val;
            push(node, l, r);
            return;
        }
        int mid = (l + r) / 2;
        update(2 * node, l, mid, ql, qr, val);
        update(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long query(int node, int l, int r, int ql, int qr) {
        push(node, l, r);
        if (qr < l || ql > r) return 0; // Return identity (e.g. LLONG_MAX for min queries)
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query(2 * node, l, mid, ql, qr) +
               query(2 * node + 1, mid + 1, r, ql, qr);
    }

    // Public functions (0-based indexing)
    void update(int l, int r, long long val) { update(1, 0, n - 1, l, r, val); }
    long long query(int l, int r) { return query(1, 0, n - 1, l, r); }
};

/*
============================================================
                    FULL INSTRUCTIONS
============================================================

1. Usage:
   vector<int> arr = {1, 3, 5, 7, 9};
   LazySegmentTree st(arr);
   st.update(1, 3, 5);  // Adds 5 to all elements from index 1 to 3
   st.query(1, 3);      // Returns sum from index 1 to 3

2. How to Modify for Range Minimum Query (RMQ):

   In push():
   - Change `tree[node] += lazy[node] * (r - l + 1)`
     to `tree[node] += lazy[node]` (since adding X to a range adds exactly X to the minimum value of that range).

   In query():
   - Change `return 0` to `return LLONG_MAX` on non-overlap condition (line 104).
   - Change `return query(...) + query(...)` to `return min(query(...), query(...))` (line 106).

   In update():
   - Change `tree[node] = tree[2 * node] + tree[2 * node + 1]` to `tree[node] = min(tree[2 * node], tree[2 * node + 1])` (line 97).

3. Common Pitfalls:
   - Always run `push()` first inside both `update()` and `query()` recursive steps.
   - For Range Set (assigning value X instead of adding value X), the lazy assignment logic changes slightly (need to track a separate `is_lazy` boolean flag).

4. Coordinate Compression Integration (Old vs. New):
   - SIZING:
     Old: LazySegmentTree st(n);
     New: LazySegmentTree st(unique_size);
   - UPDATING:
     Old: st.update(l, r, val);
     New: st.update(get_rank(l), get_rank(r), val); // Map coordinates first
   - QUERYING:
     Old: st.query(l, r);
     New: st.query(get_rank(l), get_rank(r)); // Map coordinates first

============================================================
*/
