// Link: https://cses.fi/problemset/task/1646

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, q;
    cin >> n >> q;

    vector<long long> pref(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        long long x;
        cin >> x;
        pref[i] = pref[i - 1] + x;
    }

    while (q--) {
        int a, b;
        cin >> a >> b;

        cout << pref[b] - pref[a - 1] << '\n';
    }

    return 0;
}