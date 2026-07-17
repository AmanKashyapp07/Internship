// CSES Problem: <problem name>
// https://cses.fi/problemset/task/<id>

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

using int64 = long long;

const long long MOD = 1e9 + 7;
const long long INV2 = 500000004; // modular inverse of 2 modulo MOD

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    long long answer = 0;

    // We process ranges [l, r] where floor(n/i)
    // remains constant.
    long long l = 1;

    while (l <= n) {

        // Current quotient value
        long long q = n / l;

        // Largest index r such that
        // floor(n/x) = q for every x in [l, r]
        long long r = n / q;

        /*
            We need:

            q * (l + (l+1) + ... + r)

            Sum of arithmetic progression:

            (l + r) * (r - l + 1) / 2

            All computations are done modulo MOD.
        */

        long long count = (r - l + 1) % MOD;

        long long sumRange =
            ((l + r) % MOD) * count % MOD * INV2 % MOD;

        long long contribution =
            (q % MOD) * sumRange % MOD;

        answer = (answer + contribution) % MOD;

        // Jump directly to next block
        l = r + 1;
    }

    cout << answer << '\n';

    return 0;
}