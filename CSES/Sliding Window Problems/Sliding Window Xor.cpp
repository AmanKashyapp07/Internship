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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    cin >> n >> k;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    deque<long long> window;

    long long curXor = 0;

    // Build first window
    long long cur = x;
    for (long long i = 1; i <= k; i++) {
        window.push_back(cur);
        curXor ^= cur;

        if (i < n) {
            cur = (a * cur + b) % c;
        }
    }

    long long answer = curXor;

    // Process remaining elements
    for (long long i = k + 1; i <= n; i++) {
        long long nextVal = cur;

        curXor ^= window.front(); // remove
        window.pop_front();

        curXor ^= nextVal;        // add
        window.push_back(nextVal);

        answer ^= curXor;

        if (i < n) {
            cur = (a * cur + b) % c;
        }
    }

    cout << answer << '\n';
    return 0;
}
