// Link: https://cses.fi/problemset/task/1621
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    cout << (unique(a.begin(), a.end()) - a.begin()) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count the number of distinct values in an array of n integers (CSES 1621).
// - Approach: Sort + `std::unique` Deduplication.
// - Intuition: Sorting brings duplicate values together; `std::unique` removes contiguous duplicate elements in linear time.
// - Complexity: Time: O(N \log N), Space: O(N).