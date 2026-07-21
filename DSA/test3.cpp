#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_N = 10000000;
    const int LIM = sqrt(MAX_N);

    vector<pair<int, int>> twoSquares(MAX_N + 1, {-1, -1});

    for (int a = 0; a <= LIM; a++) {
        int a2 = a * a;
        for (int b = a; b <= LIM; b++) {
            int sum = a2 + b * b;
            if (sum > MAX_N) break;
            if (twoSquares[sum].first == -1)
                twoSquares[sum] = {a, b};
        }
    }

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        for (int s = 0; s <= n; s++) {
            if (twoSquares[s].first == -1) continue;
            if (twoSquares[n - s].first == -1) continue;

            auto [a, b] = twoSquares[s];
            auto [c, d] = twoSquares[n - s];

            cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
            break;
        }
    }

    return 0;
}