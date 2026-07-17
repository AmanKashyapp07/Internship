// Link: https://cses.fi/problemset/task/1618

#include <iostream>
using namespace std;

int main() {
    long long n;
    cin >> n;

    long long ans = 0;

    for (long long p = 5; p <= n; p *= 5) {
        ans += n / p;
    }

    cout << ans << '\n';

    return 0;
}