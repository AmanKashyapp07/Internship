/**
 * CSES Problem Set
 *
 * Problem: Merge K Sorted Arrays
 */

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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// Time: O(N log K)
// Space: O(K)

vi mergeKSortedArrays(vector<vi>& a) {
    priority_queue<
        pair<int, pii>,
        vector<pair<int, pii>>,
        greater<pair<int, pii>>
    > pq;

    int k = a.size();

    // {value, {array_index, element_index}}
    for (int i = 0; i < k; i++) {
        if (!a[i].empty()) {
            pq.push({a[i][0], {i, 0}});
        }
    }

    vi ans;

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        int val = cur.ff;
        int arr = cur.ss.ff;
        int idx = cur.ss.ss;

        ans.pb(val);

        if (idx + 1 < (int)a[arr].size()) {
            pq.push({a[arr][idx + 1], {arr, idx + 1}});
        }
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vi> a = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };

    vi ans = mergeKSortedArrays(a);

    for (int x : ans) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}