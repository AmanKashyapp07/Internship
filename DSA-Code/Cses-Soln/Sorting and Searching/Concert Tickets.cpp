// Link: https://cses.fi/problemset/task/1091
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    multiset<int> tickets;
    for (int i = 0; i < n; i++) { int price; cin >> price; tickets.insert(price); }

    for (int i = 0; i < m; i++) {
        int maxPrice; cin >> maxPrice;
        auto it = tickets.upper_bound(maxPrice);
        if (it == tickets.begin()) cout << -1 << '\n';
        else {
            --it;
            cout << *it << '\n';
            tickets.erase(it);
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum ticket price $\le$ customer budget for m customers (CSES 1091).
// - Approach: Multiset Upper Bound Querying (`upper_bound(maxPrice) - 1`).
// - Intuition: `upper_bound` locates first price $> \text{maxPrice}$; decrementing points to largest available price $\le \text{maxPrice}$ in $O(\log N)$ time.
// - Complexity: Time: O((N + M) \log N), Space: O(N).