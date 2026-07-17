#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

long long modPow(long long a, long long b) {
    long long result = 1;
    a %= MOD;

    while (b > 0) {
        if (b & 1) {
            result = (result * a) % MOD;
        }
        a = (a * a) % MOD;
        b >>= 1;
    }

    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        long long a, b;
        cin >> a >> b;
        cout << modPow(a, b) << '\n';
    }

    return 0;
}