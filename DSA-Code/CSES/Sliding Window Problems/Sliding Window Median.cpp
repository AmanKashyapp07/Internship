// Link: https://cses.fi/problemset/task/1076
#include <bits/stdc++.h>
using namespace std;

multiset<int> lo, hi;

void balance() {
    while (lo.size() > hi.size() + 1) {
        auto it = prev(lo.end()); hi.insert(*it); lo.erase(it);
    }
    while (lo.size() < hi.size()) {
        auto it = hi.begin(); lo.insert(*it); hi.erase(it);
    }
}

void add(int x) {
    if (lo.empty() || x <= *lo.rbegin()) lo.insert(x);
    else hi.insert(x);
    balance();
}

void remove(int x) {
    auto it = lo.find(x);
    if (it != lo.end()) lo.erase(it);
    else { it = hi.find(x); hi.erase(it); }
    balance();
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    for (int i = 0; i < k; i++) add(a[i]);
    cout << *lo.rbegin();
    for (int i = k; i < n; i++) {
        remove(a[i - k]); add(a[i]);
        cout << ' ' << *lo.rbegin();
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the median element in every sliding window of size k (CSES 1076).
// - Approach: Dual Multiset (`lo` and `hi`) maintaining balance condition `lo.size() == hi.size()` or `lo.size() == hi.size() + 1`.
// - Intuition: `lo` stores lower half, `hi` stores upper half; top of `lo` (`*lo.rbegin()`) is always the active median in $O(\log K)$ time.
// - Complexity: Time: O(N \log K), Space: O(K).