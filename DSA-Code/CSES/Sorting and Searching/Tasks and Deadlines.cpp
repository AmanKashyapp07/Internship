// Link: https://cses.fi/problemset/task/1630
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pair<long long, long long>> tasks(n);
    long long totalReward = 0;
    for (int i = 0; i < n; i++) {
        cin >> tasks[i].first >> tasks[i].second;
        totalReward += tasks[i].second;
    }
    sort(tasks.begin(), tasks.end());

    long long curTime = 0;
    for (int i = 0; i < n; i++) {
        curTime += tasks[i].first;
        totalReward -= curTime;
    }
    cout << totalReward << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Schedule n tasks to maximize total reward $\sum (d_i - f_i)$ (CSES 1630).
// - Approach: Greedy Sorting by Duration (Shortest Processing Time First).
// - Intuition: Minimizing total accumulated completion time $\sum f_i$ by executing shorter tasks first maximizes total reward.
// - Complexity: Time: O(N \log N), Space: O(N).