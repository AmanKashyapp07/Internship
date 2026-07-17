#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    set<long long> s;

    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        s.insert(x);
    }

    cout << s.size() << '\n';

    return 0;
}