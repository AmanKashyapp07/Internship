// Link: https://cses.fi/problemset/task/1090
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x; cin >> n >> x;
    vector<int> p(n);
    for (int &w : p) cin >> w;
    sort(p.begin(), p.end());

    int l = 0, r = n - 1, gondolas = 0;
    while (l <= r) {
        if (l < r && p[l] + p[r] <= x) { l++; r--; }
        else r--;
        gondolas++;
    }
    cout << gondolas << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum gondolas of capacity x to accommodate n children (max 2 children per gondola) (CSES 1090).
// - Approach: Two Pointers Greedy Pair Matching on Sorted Weights.
// - Intuition: Pair heaviest remaining child with lightest remaining child if sum $\le x$; otherwise heaviest child goes alone.
// - Complexity: Time: O(N \log N), Space: O(N).