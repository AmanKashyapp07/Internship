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
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    if (cin >> n) {
        for (long long k = 1; k <= n; ++k) {
            // Total ways to place 2 knights on a k * k board
            long long total_ways = (k * k * (k * k - 1)) / 2;
            
            // Ways two knights can attack each other
            long long attacking_ways = 4 * (k - 1) * (k - 2);
            
            // Safe ways
            cout << total_ways - attacking_ways << "\n";
        }
    }
    
    return 0;
}
