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

    int x, n;
    cin >> x >> n;

    set<int> lights;
    multiset<int> lengths;

    lights.insert(0);
    lights.insert(x);
    lengths.insert(x);

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        auto it = lights.upper_bound(p);

        int r = *it;
        int l = *prev(it);

        lengths.erase(lengths.find(r - l));

        lengths.insert(p - l);
        lengths.insert(r - p);

        lights.insert(p);

        cout << *lengths.rbegin() << ' ';
    }

    cout << '\n';
    return 0;
}
