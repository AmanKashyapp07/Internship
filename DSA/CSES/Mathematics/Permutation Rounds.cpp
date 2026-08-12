// Link: https://cses.fi/problemset/task/3398

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
#include <cstring>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1000000007LL;
const ll P = 31;

ll modPow(ll base, ll exp) {
    ll result = 1;
    base %= MOD;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % MOD;

        base = (base * base) % MOD;
        exp >>= 1;
    }

    return result;
}

ll getLCM(const vi &nums) {
    unordered_map<int, int> maxPower;

    for (int x : nums) {
        int temp = x;

        for (int p = 2; 1LL * p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                maxPower[p] = max(maxPower[p], cnt);
            }
        }

        if (temp > 1)
            maxPower[temp] = max(maxPower[temp], 1);
    }

    ll lcm = 1;

    for (auto [prime, power] : maxPower) {
        lcm = (lcm * modPow(prime, power)) % MOD;
    }

    return lcm;
}

ll permutationRounds(int n, const vi &a) {
    vector<bool> vis(n + 1, false);
    vi cycleLengths;

    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;

        int cur = i;
        int len = 0;

        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur];
            len++;
        }

        cycleLengths.push_back(len);
    }

    return getLCM(cycleLengths);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vi a(n + 1);

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    cout << permutationRounds(n, a) << '\n';

    return 0;
}