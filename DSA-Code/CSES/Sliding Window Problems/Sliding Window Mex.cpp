// Link: https://cses.fi/problemset/task/3219
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    vector<int> freq(k + 1, 0);
    set<int> missing;
    for (int i = 0; i <= k; i++) missing.insert(i);

    for (int i = 0; i < k; i++) {
        if (a[i] <= k && ++freq[a[i]] == 1) missing.erase(a[i]);
    }
    cout << *missing.begin();

    for (int i = k; i < n; i++) {
        int out = a[i - k], in = a[i];
        if (out <= k && --freq[out] == 0) missing.insert(out);
        if (in <= k && ++freq[in] == 1) missing.erase(in);
        cout << " " << *missing.begin();
    }
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the Minimum Excluded Value (MEX) in every sliding window of size k (CSES 3219).
// - Approach: Frequency Array + `std::set` tracking available non-negative integers up to k.
// - Intuition: The MEX cannot exceed k; maintaining missing values in a set allows querying the minimum missing element `*missing.begin()` in $O(1)$.
// - Complexity: Time: O(N \log K), Space: O(K).