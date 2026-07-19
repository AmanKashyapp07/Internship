// Link: https://cses.fi/problemset/task/1159

#include <bits/stdc++.h>
using namespace std;

// ---------- Binary Splitting ----------
void addItem(int w, int v, int k,
             vector<int>& weights,
             vector<int>& values) {
    for (int take = 1; k > 0; take <<= 1) {
        int cnt = min(take, k);

        weights.push_back(cnt * w);
        values.push_back(cnt * v);

        k -= cnt;
    }
}

// ---------- 0/1 Knapsack ----------
int knapsack_01(const vector<int>& weights,
                const vector<int>& values,
                int W) {

    vector<int> dp(W + 1, 0);

    for (int i = 0; i < weights.size(); i++) {
        for (int w = W; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }

    return dp[W];
}

int main() {
    int n, x;
    cin >> n >> x;

    vector<int> price(n), pages(n), copies(n);

    for (int &p : price) cin >> p;
    for (int &p : pages) cin >> p;
    for (int &c : copies) cin >> c;

    vector<int> weights;
    vector<int> values;

    // Convert bounded knapsack -> 0/1 knapsack
    for (int i = 0; i < n; i++) {
        addItem(price[i], pages[i], copies[i], weights, values);
    }

    cout << knapsack_01(weights, values, x) << '\n';

    return 0;
}