// Link: https://cses.fi/problemset/task/1722

#include <iostream>
#include <string>
#define int long long
using namespace std;
const int MOD = 1e9 + 7;

/*
    Returns:
        {F(n), F(n + 1)}

    Fast Doubling Identities:

    F(2k)   = F(k) * (2 * F(k + 1) - F(k))
    F(2k+1) = F(k)^2 + F(k + 1)^2

    Time Complexity: O(log n)
*/
pair<int, int> fibonacci(int n) {

    // Base case:
    // F(0) = 0, F(1) = 1
    if (n == 0) {
        return {0, 1};
    }

    // Recursively get:
    // a = F(k)
    // b = F(k + 1)
    auto [a, b] = fibonacci(n / 2);

    // Compute F(2k)
    int c = a * ((2 * b % MOD - a + MOD) % MOD) % MOD;

    // Compute F(2k + 1)
    int d = (a * a % MOD + b * b % MOD) % MOD;

    // If n is even:
    // return {F(2k), F(2k+1)}
    if (n % 2 == 0) {
        return {c, d};
    }

    // If n is odd:
    // return {F(2k+1), F(2k+2)}
    return {d, (c + d) % MOD};
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    cout << fibonacci(n).first << '\n'; // first because we want F(n)

    return 0;
}