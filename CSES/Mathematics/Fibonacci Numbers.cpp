#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

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
