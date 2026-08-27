// Link: https://cses.fi/problemset/task/1619
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pair<int, int>> events;
    for (int i = 0; i < n; i++) {
        int a, b; cin >> a >> b;
        events.push_back({a, 1});
        events.push_back({b, -1});
    }
    sort(events.begin(), events.end());

    int cur = 0, ans = 0;
    for (auto &[time, change] : events) {
        cur += change;
        ans = max(ans, cur);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum number of customers present at any point in time (CSES 1619).
// - Approach: Event Sweep Line (+1 on arrival, -1 on departure).
// - Intuition: Sorting events chronologically lets a running counter track simultaneous customers at all timestamps.
// - Complexity: Time: O(N \log N), Space: O(N).