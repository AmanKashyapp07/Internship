// Link: https://cses.fi/problemset/task/1158
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x; cin >> n >> x;
    vector<int> price(n), pages(n);
    for (int &p : price) cin >> p;
    for (int &pg : pages) cin >> pg;

    vector<int> dp(x + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int money = x; money >= price[i]; money--) {
            dp[money] = max(dp[money], dp[money - price[i]] + pages[i]);
        }
    }
    cout << dp[x] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum pages that can be bought with maximum price x (0/1 Knapsack) (CSES 1158).
// - Approach: Space-Optimized 1D 0/1 Knapsack Dynamic Programming.
// - Intuition: Iterating capacity backwards (`money` from x down to `price[i]`) ensures each book is bought at most once.
// - Complexity: Time: O(N \cdot X), Space: O(X).