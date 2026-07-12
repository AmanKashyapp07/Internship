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
    cin >> n;

    vector<ll> a(n);
    ll count = 0;

    for (int i = 0; i < n; i++) {
        ll c, d;
        cin >> c >> d;
        a[i] = c;
        count += d;
    }

    sort(a.begin(), a.end());

    vector<ll> prefix(n);
    prefix[0] = a[0];

    for (int i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + a[i];
    }

    ll pref_sum = 0;
    for (int i = 0; i < n; i++) {
        pref_sum += prefix[i];
    }

    cout << count - pref_sum << '\n';

    return 0;
}
