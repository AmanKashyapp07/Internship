// Link: https://cses.fi/problemset/task/3151

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int,int>> a;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        a.push_back({x, i});   // {value, original index}
    }

    sort(a.begin(), a.end());  // sorts by value, then original index

    int ans = 0;

    for (int i = 0; i < n; i++) {
        ans = max(ans, a[i].second - i);
    }

    cout << ans << '\n';
}