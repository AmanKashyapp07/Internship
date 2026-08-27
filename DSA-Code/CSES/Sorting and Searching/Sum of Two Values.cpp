// Link: https://cses.fi/problemset/task/1640
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    map<long long, int> pos;

    for (int i = 1; i <= n; i++) {
        long long v; cin >> v;
        if (pos.count(x - v)) {
            cout << pos[x - v] << ' ' << i << '\n';
            return 0;
        }
        pos[v] = i;
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find 2 distinct 1-based indices such that $a[i] + a[j] = x$ (CSES 1640).
// - Approach: Single Pass Hash Map Lookup.
// - Intuition: Storing visited elements and checking for target complement $(x - v)$ yields early exit solution.
// - Complexity: Time: O(N \log N), Space: O(N).