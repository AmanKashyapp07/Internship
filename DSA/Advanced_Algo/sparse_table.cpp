/*
What: Static Range Query DS (No Updates)

Time:
- Build  : O(N log N)
- Query  : O(1)
- Update : Not Supported

Use When:
- Array is static (no updates)
- Multiple range queries
- Operation is idempotent (min, max, gcd, bitwise AND/OR)

Common Ops: Min, Max, GCD, AND, OR

When NOT to use:
- Updates exist → Segment Tree
- Non-idempotent operations (like Sum, XOR) → Segment Tree / Fenwick Tree

Core Idea:
Precomputes answers for all ranges of power of 2 lengths.
Queries overlap two intervals of power of 2 length to cover any range in O(1).

To Modify:
- Min → 'min'
- Max → 'max'
- GCD → '__gcd'
- AND → '&'
- OR  → '|'

Usage:
SparseTable st(arr);
st.query(l, r);
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

struct SparseTable {
    int n;
    int LOG;
    vector<vector<int>> st; // st[i][j] = answer for range starting at j of length 2^i
    vector<int> lg; // lg[i] = floor(log2(i))

    SparseTable(vector<int>& a) {
        n = a.size();
        LOG = __lg(n) + 1;

        st.assign(LOG, vector<int>(n));
        lg.assign(n + 1, 0);

        // Precompute log values
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;

        // Base level
        for (int i = 0; i < n; i++) st[0][i] = a[i];

        // Build Sparse Table
        for (int j = 1; j < LOG; j++) {
            int p = 1<<j; // 2^j
            int half = 1<<(j-1); // 2^(j-1)
            for (int i = 0; i + p <= n; i++) {
                int leftInterval = st[j - 1][i];
                int rightInterval = st[j - 1][i + half];
                st[j][i] = min(leftInterval, rightInterval); // Change 'min' for other operations
            }
        }
    }

    // Range Minimum Query [l, r]
    int query(int l, int r) {
        int length = r - l + 1;
        int j = lg[length];
        int leftInterval = st[j][l];
        int p = (1 << j) - 1;
        int rightInterval = st[j][r - p];
        return min(leftInterval, rightInterval); // Change 'min' for other operations
    }
};

/*
============================================================
                    FULL INSTRUCTIONS
============================================================

1. Usage:
   vector<int> arr = {4, 6, 1, 5, 7, 3, 2};
   SparseTable st(arr);
   st.query(2, 5);     // returns minimum from index 2 to 5 (output: 1)

2. How to Modify for Other Operations:

   → Range Maximum:
      - Change 'min' to 'max' in constructor and query

   → Range GCD:
      - Change 'min' to '__gcd' in constructor and query

   → Range Bitwise AND / OR:
      - Change 'min' to '&' or '|' in constructor and query

3. Time Complexity:
   - Build   : O(N log N)
   - Query   : O(1)
   - Update  : Not Supported

4. Tips:
   - Indices are 0-based
   - Requires idempotent operations for O(1) query time. For non-idempotent operations (e.g. Sum), queries require O(log N).
   - Precomputes log values in O(N) to achieve O(1) query time.

5. Coordinate Compression Integration (Old vs. New):
   - SIZING:
     Old: SparseTable st(arr); // array of size n
     New: SparseTable st(comp_arr); // compressed array of size unique_size
   - QUERYING:
     Old: st.query(l, r);
     New: st.query(get_rank(l), get_rank(r)); // Map coordinates first

============================================================
*/