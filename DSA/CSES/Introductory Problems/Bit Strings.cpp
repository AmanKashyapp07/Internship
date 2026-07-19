// Link: https://cses.fi/problemset/task/1617

#include <iostream>

using namespace std;

const long long MOD = 1000000007;

long long power(long long base, long long exp) {
    long long result = 1;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % MOD;
        }

        base = (base * base) % MOD;
        exp >>= 1;
    }

    return result;
}

int main() {
    long long n;
    cin >> n;

    cout << power(2, n) << '\n';

    return 0;
}