// Link: https://cses.fi/problemset/task/1749

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Fenwick Tree (BIT) with binary search find() helper for List Removals
struct Fenwick {
    int n; 
    vector<int> bit;
    
    Fenwick(int n) : n(n), bit(n + 1) {}
    
    void update(int i, int val) {
        i++; // because Fenwick is 1-indexed
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
    } // find first index with prefix sum >= k in O(log N * log N) time
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (cin >> n) {
        vector<int> a(n);
        Fenwick ft(n);

        for (int i = 0; i < n; i++) {
            cin >> a[i];
            ft.update(i, 1); // Mark all initial indices as active (value 1)
        }

        for (int i = 0; i < n; i++) {
            int k;
            cin >> k;
            int idx = ft.find(k);
            cout << a[idx] << (i == n - 1 ? "" : " ");
            ft.update(idx, -1); // Remove the element at idx (active count decreases by 1)
        }
        cout << "\n";
    }
    return 0;
}
