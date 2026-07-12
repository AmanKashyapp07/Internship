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

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    map<int, int> freq;
    int distinct = 0;

    // Build the first window
    for (int i = 0; i < k; i++) {
        if (++freq[a[i]] == 1) {
            distinct++;
        }
    }

    cout << distinct;

    // Slide the window
    for (int right = k; right < n; right++) {
        int outgoing = a[right - k];
        int incoming = a[right];

        // Remove outgoing element
        if (--freq[outgoing] == 0) {
            distinct--;
        }

        // Add incoming element
        if (++freq[incoming] == 1) {
            distinct++;
        }

        cout << ' ' << distinct;
    }

    cout << '\n';
    return 0;
}
