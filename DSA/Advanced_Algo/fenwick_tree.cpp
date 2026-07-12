/*
What: Range Prefix Query + Point Update DS (Binary Indexed Tree / BIT)

Time:
- Build  : O(N log N) or O(N)
- Query  : O(log N)
- Update : O(log N)

Use When:
- Point updates on array
- Prefix sum/XOR queries
- Extremely tight memory limit (uses O(N) memory vs Segment Tree's O(4N))
- Quick to write (only ~15 lines of core code)

Common Ops: Sum, XOR, Product (Invertible operations only)

When NOT to use:
- Non-invertible operations (e.g., Range Min, Range Max, Range GCD) → Segment Tree / Sparse Table
- Complex Range Updates (Range Add/Set) with Range Queries → Lazy Segment Tree

Core Idea:
Each index stores sum of a range determined by its least significant set bit (LSB): `idx & -idx`.
Queries traverse down by stripping the LSB; updates traverse up by adding the LSB.

To Modify (Requires invertible operations):
- Sum  → '+' , identity 0  (inverse is '-')
- XOR  → '^' , identity 0  (inverse is '^')

Usage:
FenwickTree ft(arr);
ft.query(l, r);
ft.update(idx, delta); // Add delta to index idx
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

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n) {}

    void update(int idx, int val) {
        while (idx < n) {
            bit[idx] += val;
            idx = idx | (idx + 1);
        }
    }

    int query(int idx) {
        int sum = 0;
        while (idx >= 0) {
            sum += bit[idx];
            idx = (idx & (idx + 1)) - 1;
        }
        return sum;
    }

    int query(int l, int r) {
        return query(r) - (l ? query(l - 1) : 0);
    }
};

/*
============================================================
                FENWICK TREE (BIT) - NOTES
============================================================

1. Usage

vector<int> arr = {1,3,5,7,9};

Fenwick ft(arr.size());

for (int i = 0; i < arr.size(); i++)
    ft.update(i, arr[i]);

ft.query(1,3);      // sum = 15
ft.update(2,5);     // arr[2] += 5

============================================================
2. Supported Operations

✔ Point Update
    arr[idx] += val

✔ Prefix Sum
    query(idx)

✔ Range Sum
    query(l,r)

Build  : O(N log N)
Update : O(log N)
Query  : O(log N)
Memory : O(N)

============================================================
3. How to Modify

Fenwick Tree only works for operations having an inverse.

✔ Sum
combine : +
range   : query(r) - query(l-1)

✔ XOR
Update : bit[idx] ^= val
Query  : sum ^= bit[idx]
Range  : query(r) ^ query(l-1)

✘ Min
✘ Max
✘ GCD

Use Segment Tree for these.

============================================================
4. Fenwick vs Segment Tree

Use Fenwick when:
✔ Point Updates
✔ Prefix Sum
✔ Range Sum
✔ Range XOR

Advantages:
• Very short template
• O(N) memory
• Small constant factor

Use Segment Tree when:
✔ Range Min
✔ Range Max
✔ Range GCD
✔ Lazy Propagation
✔ More flexible queries

============================================================
5. Coordinate Compression

Use when values are huge (1e9, 1e12, 1e18)
but only ordering matters.

vector<int> vals = a;

sort(vals.begin(), vals.end());
vals.erase(unique(vals.begin(), vals.end()), vals.end());

for (int &x : a)
    x = lower_bound(vals.begin(), vals.end(), x)
        - vals.begin();

Fenwick ft(vals.size());

============================================================
6. OA Recognition

Think Fenwick Tree when you see:

✔ Point Updates
✔ Prefix Sum
✔ Range Sum
✔ Count Inversions
✔ Count Smaller Elements
✔ Frequency Counting
✔ Coordinate Compression

============================================================
7. Remember

✔ Simpler than Segment Tree
✔ O(log N) Update/Query
✔ O(N) Memory
✔ Best for Sum/XOR

✘ Cannot handle Min/Max/GCD

If operation is NOT invertible
→ Use Segment Tree.

============================================================
*/