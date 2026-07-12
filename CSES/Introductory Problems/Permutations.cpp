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
    int n;
    cin >> n;

    if (n == 1) {
        cout << 1 << '\n';
    } else if (n == 2 || n == 3) {
        cout << "NO SOLUTION\n";
    } else {
        for (int i = 2; i <= n; i += 2) {
            cout << i << ' ';
        }

        for (int i = 1; i <= n; i += 2) {
            cout << i << ' ';
        }

        cout << '\n';
    }

    return 0;
}
