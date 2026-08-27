// Link: https://cses.fi/problemset/task/1140
#include <bits/stdc++.h>
using namespace std;

struct Project { long long start, end, reward; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<Project> p(n);
    for (int i = 0; i < n; i++) cin >> p[i].start >> p[i].end >> p[i].reward;

    sort(p.begin(), p.end(), [](const Project& a, const Project& b) { return a.end < b.end; });

    vector<long long> dp(n + 1, 0), endTimes(n);
    for (int i = 0; i < n; i++) endTimes[i] = p[i].end;

    for (int i = 1; i <= n; i++) {
        int k = upper_bound(endTimes.begin(), endTimes.begin() + i - 1, p[i - 1].start - 1) - endTimes.begin();
        dp[i] = max(dp[i - 1], dp[k] + p[i - 1].reward);
    }
    cout << dp[n] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum total reward from non-overlapping projects (CSES 1140).
// - Approach: Sorting by End Time + Binary Search DP (`upper_bound` on end times).
// - Intuition: For project i ending at $E_i$, binary search locates latest project $k$ ending $< S_i$, offering choice `max(dp[i-1], dp[k] + reward)`.
// - Complexity: Time: O(N \log N), Space: O(N).