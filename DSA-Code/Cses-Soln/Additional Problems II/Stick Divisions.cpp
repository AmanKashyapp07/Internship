// Link: https://cses.fi/problemset/task/1161
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll x;
    int n;
    if (!(cin >> x >> n)) return 0;

    priority_queue<ll, vector<ll>, greater<ll>> pq;
    for (int i = 0; i < n; i++) {
        ll d;
        cin >> d;
        pq.push(d);
    }

    ll ans = 0;
    while (pq.size() > 1) {
        ll a = pq.top(); pq.pop();
        ll b = pq.top(); pq.pop();
        ans += a + b;
        pq.push(a + b);
    }

    cout << ans << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum cost to divide a stick of length X into N given piece lengths (CSES 1161).
// - Approach: Reverse Huffman Coding / Min-Heap Greedy.
// - Intuition: Splitting top-down is equivalent to merging pieces bottom-up; repeatedly merging the two smallest stick lengths minimizes the total division cost.
// - Complexity: Time: O(N log N), Space: O(N).
