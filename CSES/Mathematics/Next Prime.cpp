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
