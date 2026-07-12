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

multiset<int> lo, hi;

void balance() {
    // If lo has more than 1 extra element compared to hi, move the max of lo to hi
    while (lo.size() > hi.size() + 1) {
        hi.insert(*lo.rbegin());
        lo.erase(prev(lo.end()));
    }

    // If hi has more elements than lo, move the min of hi to lo
    while (lo.size() < hi.size()) {
        lo.insert(*hi.begin());
        hi.erase(hi.begin());
    }
}

void add(int x) {
    if (lo.empty() || x <= *lo.rbegin()) 
        lo.insert(x);
    else 
        hi.insert(x);

    balance(); // Maintains relative size dynamically
}

void remove(int x) {
    auto it = lo.find(x);

    if (it != lo.end())
        lo.erase(it);
    else 
        hi.erase(hi.find(x)); // We are guaranteed x is in the window, so it must be in hi

    balance(); // Rebalances after the total size temporarily drops
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int &x : a)
        cin >> x;

    for (int i = 0; i < k; i++)
        add(a[i]);

    // Use .rbegin() which is slightly cleaner than prev(lo.end()) for grabbing the max value
    cout << *lo.rbegin(); 

    for (int i = k; i < n; i++) {
        remove(a[i - k]);
        add(a[i]);

        cout << ' ' << *lo.rbegin(); 
    }

    cout << '\n';
    return 0;
}
