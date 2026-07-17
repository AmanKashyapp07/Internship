// Link: https://cses.fi/problemset/task/1144

/*
How Coordinate Compression Maps to the Fenwick Tree Template (Old vs. New):

1. SIZING:
   Old: Fenwick ft(n);
   New: Fenwick ft(unique_size);

2. UPDATING:
   Old: ft.update(idx, val);
   New: ft.update(get_rank(val), val);

3. QUERYING:
   Old: ft.query(l, r);
   New: ft.query(get_rank(l), get_rank(r));

The Fenwick Tree class/struct itself remains 100% UNMODIFIED.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// 0-Indexed Fenwick Tree (Binary Indexed Tree)
struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n, 0) {}

    // Add val to index idx
    void update(int idx, int val) {
        while (idx < n) {
            bit[idx] += val;
            idx = idx | (idx + 1);
        }
    }

    // Prefix sum query from 0 to idx
    int query(int idx) {
        int sum = 0;
        while (idx >= 0) {
            sum += bit[idx];
            idx = (idx & (idx + 1)) - 1;
        }
        return sum;
    }

    // Range sum query [l, r]
    int query(int l, int r) {
        if (l > r) return 0;
        return query(r) - (l ? query(l - 1) : 0);
    }
};

// Structure to store query events offline
struct Event {
    char type;
    int a, b;
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> salaries(n + 1); // 1-based indexing for employee lookup
        vector<int> coords;          // Vector to collect all coordinates for compression

        for (int i = 1; i <= n; i++) {
            cin >> salaries[i];
            coords.push_back(salaries[i]);
        }

        vector<Event> events(q);
        for (int i = 0; i < q; i++) {
            cin >> events[i].type >> events[i].a >> events[i].b;
            if (events[i].type == '!') {
                coords.push_back(events[i].b); // Collect new salary coordinate
            } else {
                coords.push_back(events[i].a); // Collect lower bound coordinate
                coords.push_back(events[i].b); // Collect upper bound coordinate
            }
        }

        // --- Coordinate Compression ---
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());

        // Lambda to get compressed rank in O(log M)
        auto get_rank = [&](int val) {
            return lower_bound(coords.begin(), coords.end(), val) - coords.begin();
        };

        // Create Fenwick Tree of size equal to unique coordinates
        int unique_size = coords.size();
        Fenwick ft(unique_size);

        // Populate initial salaries in the Fenwick Tree
        for (int i = 1; i <= n; i++) {
            ft.update(get_rank(salaries[i]), 1);
        }

        // Process Queries
        for (int i = 0; i < q; i++) {
            if (events[i].type == '!') {
                int emp = events[i].a;
                int new_sal = events[i].b;

                // 1. Remove old salary count
                ft.update(get_rank(salaries[emp]), -1);
                // 2. Update employee's salary locally
                salaries[emp] = new_sal;
                // 3. Add new salary count
                ft.update(get_rank(new_sal), 1);
            } else {
                int a = events[i].a;
                int b = events[i].b;
                // Range Query: count active elements between compressed ranks of a and b
                cout << ft.query(get_rank(a), get_rank(b)) << "\n";
            }
        }
    }

    return 0;
}
