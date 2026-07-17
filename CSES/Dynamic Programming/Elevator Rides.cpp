#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

int n, x;
vector<int> w;

vector<pair<int,int>> dp;

pair<int,int> solve(int mask) {

    if (mask == 0)
        return {1, 0};

    if (dp[mask].first != -1)
        return dp[mask];

    pair<int,int> best = {n + 1, 0};

    for (int i = 0; i < n; i++) {

        if (!(mask & (1 << i)))
            continue;

        auto prev = solve(mask ^ (1 << i));

        pair<int,int> cur;

        if (prev.second + w[i] <= x) {
            cur = {prev.first,
                   prev.second + w[i]};
        }
        else {
            cur = {prev.first + 1,
                   w[i]};
        }

        best = min(best, cur);
    }

    return dp[mask] = best;
}

int main() {

    cin >> n >> x;

    w.resize(n);

    for (int i = 0; i < n; i++)
        cin >> w[i];

    dp.assign(1 << n, {-1, -1});

    cout << solve((1 << n) - 1).first << '\n';
}