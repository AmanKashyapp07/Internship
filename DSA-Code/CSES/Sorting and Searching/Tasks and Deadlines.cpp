// Link: https://cses.fi/problemset/task/1630

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<ll> a(n);
    ll count = 0;

    for (int i = 0; i < n; i++) {
        ll c, d;
        cin >> c >> d;
        a[i] = c;
        count += d;
    }

    sort(a.begin(), a.end());

    vector<ll> prefix(n);
    prefix[0] = a[0];

    for (int i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + a[i];
    }

    ll pref_sum = 0;
    for (int i = 0; i < n; i++) {
        pref_sum += prefix[i];
    }

    cout << count - pref_sum << '\n';

    return 0;
}