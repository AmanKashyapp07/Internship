// Link: https://cses.fi/problemset/task/1140

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Project {
    long long start, end, reward;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (cin >> n) {
        vector<Project> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].start >> p[i].end >> p[i].reward;
        }

        // Sort projects by end time ascending
        sort(p.begin(), p.end(), [](const Project& a, const Project& b) {
            return a.end < b.end;
        });

        vector<long long> dp(n + 1, 0);
        vector<long long> endTimes(n);
        for (int i = 0; i < n; i++) endTimes[i] = p[i].end;

        // Bottom-up Tabulation DP
        for (int i = 1; i <= n; i++) {
            // Find last project ending strictly before current project's start time
            int k = upper_bound(endTimes.begin(), endTimes.begin() + i - 1, p[i - 1].start - 1) - endTimes.begin();
            dp[i] = max(dp[i - 1], dp[k] + p[i - 1].reward);
        }

        cout << dp[n] << "\n";
    }
    return 0;
}