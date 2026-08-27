// Link: https://cses.fi/problemset/task/3151
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i].first;
        a[i].second = i;
    }
    sort(a.begin(), a.end());
    int rounds = 0;
    for (int i = 0; i < n; i++) {
        rounds = max(rounds, a[i].second - i);
    }
    cout << rounds << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the number of pass rounds required to sort an array using standard Bubble Sort (CSES 3151).
// - Approach: Coordinate sorting & maximum leftwards index shift calculation.
// - Intuition: Each pass of Bubble Sort shifts every element at most 1 position to the left; max displacement `orig_idx - sorted_idx` determines total rounds.
// - Complexity: Time: O(N \log N), Space: O(N).