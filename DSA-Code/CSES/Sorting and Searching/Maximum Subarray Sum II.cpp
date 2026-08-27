// Link: https://cses.fi/problemset/task/1644
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, A, B; cin >> n >> A >> B;
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        long long x; cin >> x;
        pref[i] = pref[i - 1] + x;
    }

    deque<int> dq;
    long long ans = -1e18;

    for (int i = A; i <= n; i++) {
        int L = i - B, R = i - A;
        while (!dq.empty() && dq.front() < L) dq.pop_front();
        while (!dq.empty() && pref[dq.back()] >= pref[R]) dq.pop_back();
        dq.push_back(R);

        ans = max(ans, pref[i] - pref[dq.front()]);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum subarray sum of length between A and B inclusive (CSES 1644).
// - Approach: Monotonic Queue on Prefix Sums over sliding range $[i-B, i-A]$.
// - Intuition: Subarray sum $pref[i] - pref[j]$ is maximized when $pref[j]$ is minimized over valid window $j \in [i-B, i-A]$.
// - Complexity: Time: O(N), Space: O(N).