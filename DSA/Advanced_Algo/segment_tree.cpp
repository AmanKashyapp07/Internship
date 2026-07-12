/*
What: Range Query + Update DS

Time:
- Build  : O(N)
- Query  : O(log N)
- Update : O(log N)

Use When:
- Updates on array
- Multiple range queries
- Need fast operations

Common Ops: Sum, Min, Max, GCD

When NOT to use:
- Static array     → Sparse Table
- Only prefix sums → Prefix Array / Fenwick

Core Idea:
Each node represents a range.
- Leaf   : single element
- Parent : merged result of children

Query Cases:
1. No overlap      → return identity (0 / INF / -INF)
2. Full overlap    → return node value
3. Partial overlap → query both children + combine

To Modify:
- Sum  → '+'     , identity 0
- Min  → 'min'   , identity INT_MAX
- Max  → 'max'   , identity INT_MIN
- GCD  → '__gcd' , identity 0

Usage:
SegmentTree st(arr);
st.query(l, r);
st.update(idx, val);
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


struct SegmentTree {
    int n;
    vector<int> tree;

    SegmentTree(vector<int>& a) {
        n = a.size();
        tree.assign(4 * n, 0);
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

    int query(int node, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return 0;           // No overlap
        if (ql <= l && r <= qr) return tree[node]; // Complete overlap
        int mid = (l + r) / 2;                     // Partial overlap
        return query(2 * node, l, mid, ql, qr) +
               query(2 * node + 1, mid + 1, r, ql, qr);
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            update(2 * node, l, mid, idx, val);
        else
            update(2 * node + 1, mid + 1, r, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Public functions
    int query(int l, int r) { return query(1, 0, n - 1, l, r); }
    void update(int idx, int val) { update(1, 0, n - 1, idx, val); }
};

/*
============================================================
                    FULL INSTRUCTIONS
============================================================

1. Usage:
   vector<int> arr = {1, 3, 5, 7, 9};
   SegmentTree st(arr);
   st.query(1, 3);     // returns sum from index 1 to 3
   st.update(2, 10);   // set arr[2] = 10

2. How to Modify for Other Operations:

   → Range Minimum:
      - Change '+' to 'min'
      - Change 'return 0' to 'return INT_MAX' in query

   → Range Maximum:
      - Change '+' to 'max'
      - Change 'return 0' to 'return INT_MIN' in query

   → Range GCD:
      - Change '+' to '__gcd'
      - Change 'return 0' to 'return 0' (already fine)

3. Time Complexity:
   - Build   : O(N)
   - Query   : O(log N)
   - Update  : O(log N)

4. Tips:
   - Indices are 0-based
   - Always pass vector by reference in constructor
   - For Lazy Propagation (Range Update), a more advanced version is needed

5. Coordinate Compression Integration (Old vs. New):
   - SIZING:
     Old: SegmentTree st(n);
     New: SegmentTree st(unique_size);
   - UPDATING:
     Old: st.update(idx, val);
     New: st.update(get_rank(val), new_val); // Map coordinate first
   - QUERYING:
     Old: st.query(l, r);
     New: st.query(get_rank(l), get_rank(r)); // Map range bounds first

============================================================
*/
