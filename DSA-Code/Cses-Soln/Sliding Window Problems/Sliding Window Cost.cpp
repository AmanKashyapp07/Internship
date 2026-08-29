// Link: https://cses.fi/problemset/task/1077
#include <bits/stdc++.h>
using namespace std;

multiset<int> lo, hi;
long long sumLo = 0, sumHi = 0;

void balance() {
    while (lo.size() > hi.size() + 1) {
        auto it = prev(lo.end()); hi.insert(*it);
        sumLo -= *it; sumHi += *it; lo.erase(it);
    }
    while (lo.size() < hi.size()) {
        auto it = hi.begin(); lo.insert(*it);
        sumHi -= *it; sumLo += *it; hi.erase(it);
    }
}

void add(int x) {
    if (lo.empty() || x <= *lo.rbegin()) { lo.insert(x); sumLo += x; }
    else { hi.insert(x); sumHi += x; }
    balance();
}

void remove(int x) {
    auto it = lo.find(x);
    if (it != lo.end()) { lo.erase(it); sumLo -= x; }
    else { it = hi.find(x); hi.erase(it); sumHi -= x; }
    balance();
}

long long getCost() {
    long long median = *lo.rbegin();
    return sumHi - sumLo + median * (lo.size() - hi.size());
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    for (int i = 0; i < k; i++) add(a[i]);
    cout << getCost();
    for (int i = k; i < n; i++) {
        remove(a[i - k]); add(a[i]);
        cout << ' ' << getCost();
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum cost to equalize all elements in every sliding window of size k (CSES 1077).
// - Approach: Dual Multiset (`lo` and `hi`) tracking running sums `sumLo` and `sumHi`.
// - Intuition: The median minimizes total absolute differences $\sum |x_i - \text{median}|$; dual multisets maintain the median in $O(\log K)$ per slide.
// - Complexity: Time: O(N \log K), Space: O(K).