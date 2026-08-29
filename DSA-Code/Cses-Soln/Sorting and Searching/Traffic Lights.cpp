// Link: https://cses.fi/problemset/task/1163
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int x, n; cin >> x >> n;
    set<int> lights = {0, x};
    multiset<int> lengths = {x};

    for (int i = 0; i < n; i++) {
        int p; cin >> p;
        auto it = lights.upper_bound(p);
        int r = *it, l = *prev(it);

        lengths.erase(lengths.find(r - l));
        lengths.insert(p - l);
        lengths.insert(r - p);
        lights.insert(p);

        cout << *lengths.rbegin() << (i + 1 == n ? '\n' : ' ');
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum passage length between traffic lights after each new light addition (CSES 1163).
// - Approach: Dual Ordered Containers (`std::set` lights & `std::multiset` segment lengths).
// - Intuition: Inserting position p splits existing segment $[l, r]$ into $[l, p]$ and $[p, r]$; multiset tracks and returns maximum segment length in $O(\log N)$ time.
// - Complexity: Time: O(N \log N), Space: O(N).