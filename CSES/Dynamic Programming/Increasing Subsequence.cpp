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

    vector<long long> tails;
    // tails hold the smallest tail of all increasing subsequences of length i+1 in tails[i].
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;

        auto it = lower_bound(tails.begin(), tails.end(), x); // Find the first element in tails that is >= x , we can use binary search here because tails is always sorted in increasing order because we are always replacing elements with smaller values.

        if (it == tails.end()) {
            tails.push_back(x); // if x is greater than all elements in tails, append it to the end
        } else {
            *it = x; // Otherwise, replace the existing element with x. This maintains the property of tails that it is sorted and contains the smallest possible tail for increasing subsequences of that length.
        }
    }

    cout << tails.size() << '\n';
    return 0;
}
