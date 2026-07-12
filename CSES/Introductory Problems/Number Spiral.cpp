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

void solve() {
    long long y, x;
    cin >> y >> x;
    
    long long z = max(y, x);
    long long ans = 0;
    
    if (z % 2 == 0) {
        if (y <= x) {
            ans = (z - 1) * (z - 1) + y;
        } else {
            ans = z * z - x + 1;
        }
    } else {
        if (y <= x) {
            ans = z * z - y + 1;
        } else {
            ans = (z - 1) * (z - 1) + x;
        }
    }
    
    cout << ans << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}
