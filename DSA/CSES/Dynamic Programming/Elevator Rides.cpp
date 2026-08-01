#include <bits/stdc++.h>
using namespace std;

int n, x;
vector<int> w;
vector<pair<int,int>> dp;

pair<int,int> solve(int mask) {
    if (!mask) return {1, 0};
    if (dp[mask].first != -1) return dp[mask];

    pair<int,int> best = {n + 1, 0};

    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) continue;

        auto prev = solve(mask ^ (1 << i));

        best = min(best,
            prev.second + w[i] <= x
                ? pair(prev.first, prev.second + w[i])
                : pair(prev.first + 1, w[i]));
    }

    return dp[mask] = best;
}

int main() {
    cin >> n >> x;

    w.resize(n);
    for (int &i : w) cin >> i;

    dp.assign(1 << n, {-1, -1});

    cout << solve((1 << n) - 1).first;
}