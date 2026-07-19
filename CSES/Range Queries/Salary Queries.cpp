// Link: https://cses.fi/problemset/task/1144

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Standard 1-Indexed Fenwick Tree (takes 0-indexed arguments externally)
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

// Coordinate Compressor Helper
struct CoordinateCompressor {
    vector<int> vals;
    void add(int x) { vals.push_back(x); }
    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }
    int get(int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); }
    int size() { return vals.size(); }
};

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
        vector<int> salaries(n + 1);
        CoordinateCompressor cc;
        
        for (int i = 1; i <= n; i++) {
            cin >> salaries[i];
            cc.add(salaries[i]);
        }

        vector<Event> events(q);
        for (int i = 0; i < q; i++) {
            cin >> events[i].type >> events[i].a >> events[i].b;
            if (events[i].type == '!') {
                cc.add(events[i].b);
            } else {
                cc.add(events[i].a);
                cc.add(events[i].b);
            }
        }

        cc.build();
        Fenwick ft(cc.size());

        // Populate initial active salaries
        for (int i = 1; i <= n; i++) {
            ft.update(cc.get(salaries[i]), 1);
        }

        // Process Queries
        for (int i = 0; i < q; i++) {
            if (events[i].type == '!') {
                int emp = events[i].a;
                int new_sal = events[i].b;
                
                ft.update(cc.get(salaries[emp]), -1); // Remove old salary from BIT
                salaries[emp] = new_sal;              // Update local array
                ft.update(cc.get(new_sal), 1);         // Add new salary to BIT
            } else {
                cout << ft.query(cc.get(events[i].a), cc.get(events[i].b)) << "\n";
            }
        }
    }
    return 0;
}
