// Link: https://cses.fi/problemset/task/1162

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
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1000000007LL;
const ll P = 31;


/*---------------------------------------------------------------
1. Minimum adjacent swaps = Number of inversions
Time: O(n log n)
----------------------------------------------------------------*/

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, int val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    int sum(int idx) {
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }
};

ll minAdjacentSwaps(vector<int> &a) {
    int n = a.size();
    Fenwick ft(n);

    ll inv = 0;

    for (int i = n - 1; i >= 0; i--) {
        inv += ft.sum(a[i] - 1);
        ft.add(a[i], 1);
    }

    return inv;
}

/*---------------------------------------------------------------
2. Minimum arbitrary swaps = n - number of cycles
Time: O(n)
----------------------------------------------------------------*/

int minAnySwaps(vector<int> &a) {
    int n = a.size();
    vector<bool> vis(n, false);

    int cycles = 0;

    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;

        cycles++;

        int cur = i;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur] - 1; // why -1? because the array a is 1-indexed, so we need to subtract 1 to get the correct index in the 0-indexed array.
        }
    }

    return n - cycles;
}

/*---------------------------------------------------------------
3. Minimum move-to-any-position operations = n - LIS
Time: O(n log n)
----------------------------------------------------------------*/

int minMoveAnywhere(vector<int> &a) {
    vector<int> lis;

    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);

        if (it == lis.end())
            lis.push_back(x);
        else
            *it = x;
    }

    return (int)a.size() - (int)lis.size();
}

/*---------------------------------------------------------------
4. Minimum move-to-front operations
Time: O(n)
----------------------------------------------------------------*/

int minMoveToFront(vector<int> &a) {
    int n = a.size();

    int need = n;

    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == need)
            need--;
    }

    return need;
}

int main(){
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    cout << minAdjacentSwaps(a) << " " << minAnySwaps(a) << " " << minMoveAnywhere(a) << " " << minMoveToFront(a) << endl;
}