// Link: https://cses.fi/problemset/task/1652

#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> pref(n + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;

        for (int j = 1; j <= n; j++) {
            int tree = (s[j - 1] == '*');

            pref[i][j] = tree
                       + pref[i - 1][j]
                       + pref[i][j - 1]
                       - pref[i - 1][j - 1];
        }
    }

    while (q--) {
        int y1, x1, y2, x2;
        cin >> y1 >> x1 >> y2 >> x2;

        int ans = pref[y2][x2]
                - pref[y1 - 1][x2]
                - pref[y2][x1 - 1]
                + pref[y1 - 1][x1 - 1];

        cout << ans << '\n';
    }

    return 0;
}