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
    long long n;
    cin >> n;

    long long sum = n * (n + 1) / 2;

    if (sum & 1) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";

    long long target = sum / 2;
    vector<int> a, b;

    for (long long i = n; i >= 1; i--) {
        if (i <= target) {
            a.push_back(i);
            target -= i;
        } else {
            b.push_back(i);
        }
    }

    cout << a.size() << '\n';
    for (int x : a) cout << x << ' ';
    cout << '\n';

    cout << b.size() << '\n';
    for (int x : b) cout << x << ' ';
    cout << '\n';

    return 0;
}
