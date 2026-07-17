#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int x = 0;
        for (char c : s) {
            x = (x << 1) | (c - '0');
        }

        a[i] = x;
    }

    int ans = k;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ans = min(ans,
                      __builtin_popcount(a[i] ^ a[j]));

            if (ans == 0) {
                cout << 0 << '\n';
                return 0;
            }
        }
    }

    cout << ans << '\n';
}