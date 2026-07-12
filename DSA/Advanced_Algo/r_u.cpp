#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

/*
================================================================================
          EXPLANATION: RANGE UPDATES VIA DIFFERENCE ARRAYS IN A BIT
================================================================================
A standard Fenwick Tree handles Point Updates and Prefix Queries in O(log N). 
To perform Range Updates [L, R] and Point Queries efficiently, we load a 
"Difference Array" into the Fenwick Tree.

1. WHAT IS A DIFFERENCE ARRAY?
   A difference array D stores the relative change between adjacent elements:
   - D[0] = A[0]
   - D[i] = A[i] - A[i-1] (for i > 0)

   PROPERTY: You can reconstruct the original value of A[i] at any point by 
   taking the PREFIX SUM of the difference array from 0 to i.
   - A[i] = D[0] + D[1] + ... + D[i]

2. THE RANGE UPDATE TRICK
   If you add a value 'u' to every element in a range [L, R] in array A, the 
   relative difference between elements INSIDE the range does not change. 
   Only the boundaries break:
   - The jump from A[L-1] to A[L] increases by 'u'   -> Add 'u' to D[L]
   - The jump from A[R] to A[R+1] decreases by 'u'   -> Sub 'u' from D[R+1]

   Thus, an O(N) range update on a normal array turns into exactly TWO O(1) 
   point updates on a Difference Array!

3. HOW OPERATIONS MAP TO THE FENWICK TREE:
   - Range Update [L, R] with +u  => Call ft.update(L, u) and ft.update(R + 1, -u)
   - Point Query at index K       => Call ft.query(K) (computes prefix sum of D)
================================================================================
*/

// Sized to n+1 internally for safety, but accepts 0-indexed parameters publicly.
struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1) {}
    
    void update(int i, int val) {
        for (++i; i <= n; i += i & -i) bit[i] += val;
    }
    int query(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & -i) s += bit[i];
        return s;
    }
    int query(int l, int r) { 
        return (l > r) ? 0 : query(r) - query(l - 1); 
    }
};

int main() {
    // Optimize standard input/output streams for fast competitive programming execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        Fenwick ft(n);

        // Build the Difference Array representation directly inside the Fenwick Tree
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            if (i == 0) {
                ft.update(i, a[i]);
            } else {
                ft.update(i, a[i] - a[i - 1]); // Store the delta between consecutive elements
            }
        }

        // Process Queries
        for (int i = 0; i < q; i++) {
            int type;
            cin >> type;
            if (type == 1) {
                int l, r, u;
                cin >> l >> r >> u;
                l--; r--; // Convert 1-based bounds to 0-based index targets
                
                // Perform the 2-point boundary update trick on the difference array
                ft.update(l, u);
                ft.update(r + 1, -u);
            } else if (type == 2) {
                int k;
                cin >> k;
                k--; // Convert 1-based index to 0-based point query
                
                // Prefix sum of the difference array yields the exact current value of element A[k]
                cout << ft.query(k) << "\n";
            }
        }
    }

    return 0;
}