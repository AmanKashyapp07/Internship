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
