#include <bits/stdc++.h>
#include <atcoder/convolution>

using namespace std;
using namespace atcoder;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();

    vector<long long> a(n), b(n);
    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            a[i] = 1;
            b[n - 1 - i] = 1;
        }
    }

    auto c = convolution(a, b);

    for (int k = 1; k < n; k++) {
        cout << c[n - 1 - k] << " ";
    }
    cout << '\n';
}