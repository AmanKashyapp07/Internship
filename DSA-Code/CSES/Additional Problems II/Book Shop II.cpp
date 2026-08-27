// Link: https://cses.fi/problemset/task/1159
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x; cin >> n >> x;
    vector<int> price(n), pages(n), copies(n);
    for (int &p : price) cin >> p;
    for (int &p : pages) cin >> p;
    for (int &c : copies) cin >> c;

    vector<int> weights, values;
    for (int i = 0; i < n; i++) {
        int w = price[i], v = pages[i], k = copies[i];
        for (int take = 1; k > 0; take <<= 1) {
            int cnt = min(take, k);
            weights.push_back(cnt * w);
            values.push_back(cnt * v);
            k -= cnt;
        }
    }

    vector<int> dp(x + 1, 0);
    for (size_t i = 0; i < weights.size(); i++) {
        for (int w = x; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    cout << dp[x] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum pages attainable with budget x buying up to copies[i] of book i (CSES 1159).
// - Approach: Bounded Knapsack reduction to 0/1 Knapsack via Binary Grouping ($1, 2, 4, \dots$).
// - Intuition: Decomposing copies $k$ into powers of 2 reduces $k$ items to $\lfloor \log_2 k \rfloor$ items while covering all subset sums.
// - Complexity: Time: O(X \sum \log_2 \text{copies}_i), Space: O(X).