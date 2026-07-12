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
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Handle base cases directly
    if (n == 1) {
        cout << 0 << "\n";
        return 0;
    }
    if (n == 2) {
        cout << 1 << "\n";
        return 0;
    }

    long long MOD = 1e9 + 7;
    long long prev2 = 0; // Represents D_1
    long long prev1 = 1; // Represents D_2
    long long current = 0; // Represents D_i

    // Compute derangements iteratively from 3 up to n
    for (int i = 3; i <= n; i++) {
        current = (i - 1) * (prev1 + prev2) % MOD;
        
        // Slide the tracking variables forward
        prev2 = prev1;
        prev1 = current;
    }

    cout << current << "\n";
    return 0;
}
