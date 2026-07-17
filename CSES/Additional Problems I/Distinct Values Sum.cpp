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
using pii = pair<ll, ll>;
using pll = pair<ll, ll>;
using vi = vector<ll>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const ll INF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

int main()
{
    ll n;
    cin >> n;

    vector<ll> a(n);
    for (ll i = 0; i < n; i++)
        cin >> a[i];

    ll ans = 0;
    ll cur = 0;
    map<ll, ll> last;

    for (ll i = 0; i < n; i++)
    {
        ll p = last.count(a[i]) ? last[a[i]] : -1;
        cur += (i - p);
        ans += cur;
        last[a[i]] = i;
    }

    cout << ans << '\n';
    return 0;
}