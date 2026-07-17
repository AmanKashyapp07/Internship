// Link: https://cses.fi/problemset/task/1713

#include <bits/stdc++.h>
using namespace std;

const int MAXX = 1'000'000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> divCnt(MAXX + 1, 0);

    for (int d = 1; d <= MAXX; d++) {
        for (int multiple = d; multiple <= MAXX; multiple += d) {
            divCnt[multiple]++;
        }
    }

    int n;
    cin >> n;

    while (n--) {
        int x;
        cin >> x;
        cout << divCnt[x] << '\n';
    }

    return 0;
}