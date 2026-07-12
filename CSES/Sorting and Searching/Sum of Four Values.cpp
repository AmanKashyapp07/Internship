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

    int n;
    long long x;
    cin >> n >> x;

    vector<long long> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    unordered_map<long long, pair<int, int>> pairSum;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long need = x - (a[i] + a[j]);

            if (pairSum.count(need)) {
                auto [p, q] = pairSum[need];

                if (p != i && p != j && q != i && q != j) {
                    cout << p + 1 << ' '
                         << q + 1 << ' '
                         << i + 1 << ' '
                         << j + 1 << '\n';
                    return 0;
                }
            }
        }

        for (int k = 0; k < i; k++) {
            pairSum[a[k] + a[i]] = {k, i};
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}
