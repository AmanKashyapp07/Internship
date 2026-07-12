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

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    int unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return sz[a];

        if (sz[a] < sz[b]) swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];

        return sz[a];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DSU dsu(n);

    int components = n;
    int largest = 1;

    while (m--) {
        int a, b;
        cin >> a >> b;

        if (dsu.find(a) != dsu.find(b)) {
            largest = max(largest, dsu.unite(a, b));
            components--;
        }

        cout << components << ' ' << largest << '\n';
    }
}
