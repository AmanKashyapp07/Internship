#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    long long sum, pref, suff, ans;
};

// Merges two adjacent segments in O(1) time
Node combine(Node L, Node R) {
    Node res;
    res.sum = L.sum + R.sum;
    res.pref = max(L.pref, L.sum + R.pref);
    res.suff = max(R.suff, R.sum + L.suff);
    res.ans = max({L.ans, R.ans, L.suff + R.pref});
    return res;
}

// Generates a base leaf node from a single array element
Node make_node(long long val) {
    long long v = max(0LL, val); // Subarray can be empty (sum = 0)
    return {val, v, v, v};
}

struct SegTree {
    int n; vector<Node> t;
    SegTree(vector<int>& a) {
        n = 1;
        while (n < a.size()) n *= 2;
        t.assign(2 * n, make_node(0));
        for (int i = 0; i < a.size(); i++) t[n + i] = make_node(a[i]);
        for (int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
    }
    
    void update(int p, int val) {
        for (t[p += n] = make_node(val); p > 1; p >>= 1) {
            t[p >> 1] = combine(t[p & ~1], t[p | 1]);
        }
    }

    long long getMaxSubarray() {
        return t[1].ans; // Root element represents the global array max subarray sum
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        SegTree st(a);

        while (q--) {
            int idx, val;
            cin >> idx >> val;
            idx--; // Convert to 0-based indexing
            
            st.update(idx, val);
            
            // O(1) query: The root node always holds the status of the entire array
            cout << st.getMaxSubarray() << "\n";
        }
    }
    return 0;
}