// Link: https://cses.fi/problemset/task/1158

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> price(n), pages(n);

    for (int i = 0; i < n; i++) cin >> price[i];
    for (int i = 0; i < n; i++) cin >> pages[i];

    vector<int> dp(x + 1, 0);

    for (int i = 0; i < n; i++) {
        for (int money = x; money >= price[i]; money--) {
            dp[money] = max(
                dp[money],
                dp[money - price[i]] + pages[i]
            );
        }
    }

    cout << dp[x] << '\n';
}