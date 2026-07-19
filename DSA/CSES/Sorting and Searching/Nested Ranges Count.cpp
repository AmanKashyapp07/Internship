// Link: https://cses.fi/problemset/task/2169

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief A 1D Fenwick Tree (Binary Indexed Tree) for efficient prefix sum queries and point updates.
 * 
 * Provides O(log N) updates and queries, making it ideal for counting frequencies 
 * during coordinate-compressed sweep-line algorithms.
 * 
 * // Fenwick Tree stores frequencies/prefix sums over indices.
// If a problem asks "how many values <= x" or "sum from 0 to x", think of a prefix sum -> use query(x).
// For a range [l, r], answer = query(r) - query(l - 1).
// Update each element with update(idx, val), where val is usually +1/-1 (frequency) or the value to add.

 */

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1) {}

    /**
     * @brief Adds a value to the element at 0-indexed position i.
     * Converted internally to 1-based indexing for the BIT structure.
     */
    void update(int i, int val) {
        for (++i; i <= n; i += i & -i)
            bit[i] += val;
    } // adds 'val' to index 'i' (0-based) in the Fenwick Tree

    /**
     * @brief Computes the prefix sum from index 0 to i (inclusive).
     */
    int query(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & -i)
            s += bit[i];
        return s;
    }

    /**
     * @brief Computes the range sum between l and r (inclusive).
     */
    int query(int l, int r) {
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};

/**
 * @brief Maps sparse coordinate values to a dense, continuous range [0, unique_count - 1].
 * 
 * Necessary because original coordinates can be up to 10^9, which is too large 
 * to use directly as indices in a standard Fenwick Tree array.
 */
struct CoordinateCompressor {
    vector<int> vals;

    void add(int x) {
        vals.push_back(x);
    }

    /**
     * @brief Sorts and removes duplicates to prepare the compression mapping.
     */
    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }

    /**
     * @brief Returns the compressed 0-indexed rank of value x.
     */
    int get(int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    }

    int size() {
        return vals.size();
    }
};

/**
 * @brief Represents a line segment with its initial tracking index.
 */
struct Range {
    int l, r, idx;
};

int main() {
    // Optimize standard I/O operations for performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Range> ranges(n);
    CoordinateCompressor cc;

    for (int i = 0; i < n; i++) {
        cin >> ranges[i].l >> ranges[i].r;
        ranges[i].idx = i;
        cc.add(ranges[i].r); // Only compressing R coordinates since L is handled by sorting
    }

    cc.build();

    // Transform right endpoints to their compressed rank representations
    for (auto &x : ranges)
        x.r = cc.get(x.r);

    /**
     * @brief Sweep-line sorting strategy:
     * 1. Sort primarily by left endpoint (l) ascending. 
     *    This guarantees that as we iterate forwards, any subsequent range has a start 
     *    point >= the current range.
     * 2. If left endpoints are identical, sort by right endpoint (r) descending.
     *    This ensures larger ranges are processed first, correctly counting nested subsets 
     *    when bounds completely overlap.
     */
    sort(ranges.begin(), ranges.end(), [](const Range &a, const Range &b) {
        if (a.l == b.l)
            return a.r > b.r;
        return a.l < b.l;
    });

    vector<int> contains(n), contained(n);

    // -------------------------------------------------------------------------
    // TASK 1: Count how many ranges each individual range CONTAINS
    // -------------------------------------------------------------------------
    Fenwick bit1(cc.size());

    /**
     * Iterating backwards (from right to left):
     * Since the array is sorted by L ascending, moving backwards means we are seeing 
     * ranges with progressively smaller L values.
     * 
     * For a current range i to completely contain a previously processed range j (where j > i):
     * We already know L_i <= L_j. Thus, we only need to count how many active ranges 
     * have their R_j <= R_i. This matches a standard prefix sum query up to R_i.
     */
    for (int i = n - 1; i >= 0; i--) {
        contains[ranges[i].idx] = bit1.query(ranges[i].r); // bit1.query(x) returns the count of ranges with R <= x, as fenwick tree is 0-indexed and compressed and stores the counts of R values seen so far.
        bit1.update(ranges[i].r, 1);
    }

    // -------------------------------------------------------------------------
    // TASK 2: Count how many ranges CONTAIN each individual range
    // -------------------------------------------------------------------------
    Fenwick bit2(cc.size());

    /**
     * Iterating forwards (from left to right):
     * Moving forwards means we are seeing ranges with progressively larger L values.
     * 
     * For a previously processed range j (where j < i) to completely contain the current range i:
     * We already know L_j <= L_i. Thus, we only need to count how many active ranges 
     * have their R_j >= R_i. This is computed by taking the total active elements 
     * and subtracting those whose R values are strictly less than R_i.
     */
    for (int i = 0; i < n; i++) {
        contained[ranges[i].idx] =
            bit2.query(cc.size() - 1) - // why cc.size() - 1? 
            (ranges[i].r ? bit2.query(ranges[i].r - 1) : 0);

        bit2.update(ranges[i].r, 1);
    }

    // Output results in the original input sequence order
    for (int x : contains)
        cout << x << " ";
    cout << "\n";

    for (int x : contained)
        cout << x << " ";
    cout << "\n";

    return 0;
}