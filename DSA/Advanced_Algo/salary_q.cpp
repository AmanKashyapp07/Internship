#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// =========================================================================
// TEMPLATE 1: FENWICK TREE (Binary Indexed Tree)
// =========================================================================
// Publicly accepts 0-indexed arguments, but manages them 1-indexed internally 
// to take advantage of the standard, ultra-fast `i & -i` bitwise operations.
struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1) {} // Sized to n + 1 to stay perfectly safe from out-of-bounds
    
    void update(int i, int val) {
        for (++i; i <= n; i += i & -i) bit[i] += val; // Converts 0-index to 1-index, updates frequency
    }
    int query(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & -i) s += bit[i];   // Accumulates prefix sum safely from [0 ... i]
        return s;
    }
    int query(int l, int r) { 
        return (l > r) ? 0 : query(r) - query(l - 1); // Returns total frequency within range [l, r] inclusive
    }
};

// =========================================================================
// TEMPLATE 2: COORDINATE COMPRESSOR
// =========================================================================
// Gathers huge numbers (like salaries up to 10^9) and remaps them down 
// to a tiny scale [0, 1, 2, ... Unique Elements - 1] preserving relative order.
struct CoordinateCompressor {
    vector<int> vals;
    void add(int x) { vals.push_back(x); }
    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end()); // Deduplicates and locks ranks
    }
    int get(int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); } // Returns 0-based compressed index
    int size() { return vals.size(); }
};

// Helper structure to save incoming queries offline
struct Query {
    char type;
    int a, b;
};

int main() {
    // Optimize standard input/output streams for fast competitive programming execution
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> salary(n + 1);
    CoordinateCompressor cc;

    // STEP 1: Collect all original employee salaries to prepare for compression universe
    for (int i = 1; i <= n; i++) {
        cin >> salary[i];
        cc.add(salary[i]);
    }

    // STEP 2: Collect all future query metrics before building our mapping coordinates
    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].type >> queries[i].a >> queries[i].b;
        if (queries[i].type == '!') {
            cc.add(queries[i].b); // Add the new salary value to the compression index map
        } else {
            cc.add(queries[i].a); // Add the lower range target boundary
            cc.add(queries[i].b); // Add the upper range target boundary
        }
    }

    // STEP 3: Complete coordinate compression sorting and unique processing
    cc.build();

    // STEP 4: Size our tracking Fenwick tree perfectly to the compressed universe scale
    Fenwick ft(cc.size());

    // STEP 5: Populate initial frequencies. Every employee adds an occurrence count of 1
    for (int i = 1; i <= n; i++) {
        ft.update(cc.get(salary[i]), +1);
    }

    // STEP 6: Execute the operations sequentially
    for (auto &q : queries) {
        if (q.type == '!') {
            int emp_id = q.a;
            int new_sal = q.b;

            // Step A: Evict the previous salary value from the tracking index (-1 frequency)
            ft.update(cc.get(salary[emp_id]), -1);

            // Step B: Formally modify the local scalar assignment mapping tracking table
            salary[emp_id] = new_sal;

            // Step C: Incorporate the incoming target new salary value (+1 frequency)
            ft.update(cc.get(salary[emp_id]), +1);
        } else {
            // Translate explicit absolute bounds down into safe ordinal compressed arrays
            int L = cc.get(q.a);
            int R = cc.get(q.b);

            // Fetch structural sum boundaries in O(log N) directly
            cout << ft.query(L, R) << '\n';
        }
    }

    return 0;
}