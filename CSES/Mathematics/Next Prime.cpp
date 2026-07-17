#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

bool isPrime(int64 n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;

    for (int64 d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }

    return true;
}

int64 nextPrime(int64 n) {
    int64 x = n + 1;

    if (x <= 2) return 2;
    if (x % 2 == 0) x++;

    while (!isPrime(x)) {
        x += 2;
    }

    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int64 n;
        cin >> n;

        cout << nextPrime(n) << '\n';
    }

    return 0;
}