/**
 * CSES Problem Set
 * 
 * Problem: Sliding Cost
 * Link: https://cses.fi/problemset/task/1077
 * Category: Sorting and Searching
 * 
 * Description:
 * Find the minimum cost to make window elements equal.
 * 
 * Logic/Approach:
 * Two multisets (lo and hi) tracking sum of elements in both halves to compute costs in O(log k).
 */

#include <iostream>
#include <set>
#include <vector>
using namespace std;

multiset<int> lo, hi;
long long sum_lo = 0, sum_hi = 0;

void balance() {
    // If lo has more than 1 extra element compared to hi, move the max of lo to hi
    while (lo.size() > hi.size() + 1) {
        auto it = prev(lo.end());
        hi.insert(*it);
        sum_lo -= *it;
        sum_hi += *it;
        lo.erase(it);
    }

    // If hi has more elements than lo, move the min of hi to lo
    while (lo.size() < hi.size()) {
        auto it = hi.begin();
        lo.insert(*it);
        sum_hi -= *it;
        sum_lo += *it;
        hi.erase(it);
    }
}

void add(int x) {
    if (lo.empty() || x <= *lo.rbegin()) {
        lo.insert(x);
        sum_lo += x;
    } else {
        hi.insert(x);
        sum_hi += x;
    }
    balance(); // Maintains relative size dynamically
}

void remove(int x) {
    auto it = lo.find(x);
    if (it != lo.end()) {
        lo.erase(it);
        sum_lo -= x;
    } else {
        it = hi.find(x);
        hi.erase(it);
        sum_hi -= x;
    }
    balance(); // Rebalances safely
}


long long get_cost() {
    long long median = *lo.rbegin();
    // Formula: sum_hi - sum_lo + Median * (Size of lo - Size of hi)
    return sum_hi - sum_lo + median * (lo.size() - hi.size());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int &x : a)
        cin >> x;

    for (int i = 0; i < k; i++)
        add(a[i]);

    cout << get_cost(); // Print first cost

    for (int i = k; i < n; i++) {
        remove(a[i - k]);
        add(a[i]);

        cout << ' ' << get_cost(); // Print subsequent costs on the same line
    }

    cout << '\n';
    return 0;
}