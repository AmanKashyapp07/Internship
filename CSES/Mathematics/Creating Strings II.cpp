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

const int64 MOD = 1e9 + 7;

int64 modPow(int64 a, int64 b) {
    int64 res = 1;

    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();

    vector<int64> fact(n + 1), invFact(n + 1);

    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    invFact[n] = modPow(fact[n], MOD - 2);
    for (int i = n; i >= 1; i--) {
        invFact[i - 1] = invFact[i] * i % MOD;
    }

    vector<int> freq(26, 0);
    for (char c : s) {
        freq[c - 'a']++;
    }

    int64 ans = fact[n];

    for (int cnt : freq) {
        ans = ans * invFact[cnt] % MOD;
    }

    cout << ans << '\n';

    return 0;
}
