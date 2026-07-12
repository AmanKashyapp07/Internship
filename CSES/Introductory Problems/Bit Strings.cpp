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
