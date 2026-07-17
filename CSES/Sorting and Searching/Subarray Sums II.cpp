#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    map<long long, long long> mp;
    mp[0] = 1;

    long long ans = 0;
    long long pre = 0;

    for (int i = 0; i < n; i++) {
        long long v;
        cin >> v;

        pre += v;

        if (mp.count(pre - x)) {
            ans += mp[pre - x];
        }

        mp[pre]++;
    }

    cout << ans << '\n';

    return 0;
}