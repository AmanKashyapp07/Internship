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

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int x = 0;
        for (char c : s) {
            x = (x << 1) | (c - '0');
        }

        a[i] = x;
    }

    int ans = k;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ans = min(ans,
                      __builtin_popcount(a[i] ^ a[j]));

            if (ans == 0) {
                cout << 0 << '\n';
                return 0;
            }
        }
    }

    cout << ans << '\n';
}
