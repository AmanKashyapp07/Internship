// Link: https://cses.fi/problemset/task/1734

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Standard 1-Indexed Fenwick Tree template (takes 0-indexed arguments externally)
struct Fenwick {
    int n; 
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    
    void update(int i, int val) {
        i++; // Convert 0-indexed parameter to 1-indexed BIT internally
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
        if (l > r) return 0;
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};

struct Query { 
    int l, r, id; 
};

int main() {
    // Fast I/O
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        // Group queries by their right endpoint for offline processing
        vector<vector<Query>> endingAt(n);
        for (int i = 0; i < q; i++) {
            int l, r;
            cin >> l >> r;
            endingAt[r - 1].push_back({l - 1, r - 1, i});
        }

        Fenwick bit(n);
        unordered_map<int, int> last; // value -> latest index seen
        vector<int> ans(q);

        for (int i = 0; i < n; i++) {
            // If the element has been seen before, remove its previous occurrence from BIT
            if (last.count(a[i])) {
                bit.update(last[a[i]], -1);
            }
            
            // Add the current occurrence to BIT
            bit.update(i, 1);
            last[a[i]] = i;

            // Answer all queries ending at the current index i
            for (const auto &qu : endingAt[i]) {
                ans[qu.id] = bit.query(qu.l, qu.r);
            }
        }

        for (int x : ans) {
            cout << x << '\n';
        }
    }
    return 0;
}