// Link: https://cses.fi/problemset/task/3150
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    long long totalSum = 0, currentGain = 0;
    map<long long, int> lastSeen;

    for (int i = 0; i < n; i++) {
        int prevIdx = lastSeen.count(a[i]) ? lastSeen[a[i]] : -1;
        currentGain += (i - prevIdx);
        totalSum += currentGain;
        lastSeen[a[i]] = i;
    }
    cout << totalSum << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Calculate the sum of distinct element counts across all contiguous subarrays (CSES 3150).
// - Approach: Contribution Technique using last seen element map.
// - Intuition: Element a[i] adds +1 distinct contribution to all subarrays starting after its previous occurrence `prevIdx`, adding `(i - prevIdx)` to rolling sum.
// - Complexity: Time: O(N \log N), Space: O(N).