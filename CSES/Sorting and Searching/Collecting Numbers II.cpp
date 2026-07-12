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

bool bad(int x, const vector<int>& pos, int n) {
    if (x < 1 || x >= n) return false;
    return pos[x] > pos[x + 1];
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    vector<int> pos(n + 1);

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[a[i]] = i;
    }

    int rounds = 1;
    for (int x = 1; x < n; x++) {
        if (pos[x] > pos[x + 1]) rounds++;
    }

  

    while (m--) {
        int p, q;
        cin >> p >> q;

        int u = a[p];
        int v = a[q];

        set<int> affected;
        affected.insert(u - 1);
        affected.insert(u);
        affected.insert(v - 1);
        affected.insert(v);

        for (int x : affected) rounds -= bad(x, pos, n);

        swap(a[p], a[q]);
        swap(pos[u], pos[v]);

        for (int x : affected) rounds += bad(x, pos, n);

        cout << rounds << '\n';
    }

    return 0;
}
