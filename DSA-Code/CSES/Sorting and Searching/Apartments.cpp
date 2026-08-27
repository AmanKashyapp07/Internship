// Link: https://cses.fi/problemset/task/1084
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<int> a(n), b(m);
    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    int i = 0, j = 0, count = 0;
    while (i < n && j < m) {
        if (abs(a[i] - b[j]) <= k) { count++; i++; j++; }
        else if (a[i] < b[j]) i++;
        else j++;
    }
    cout << count << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Allocate m apartments to n applicants such that size difference $|a_i - b_j| \le k$ (CSES 1084).
// - Approach: Two Pointers on Sorted Arrays.
// - Intuition: Sorting both applicant desired sizes and available apartment sizes allows greedy matching via two pointers.
// - Complexity: Time: O(N \log N + M \log M), Space: O(N + M).
