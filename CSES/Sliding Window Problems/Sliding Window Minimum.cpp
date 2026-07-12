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

struct Node {
    int idx;
    long long val;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    deque<Node> dq;
    long long answer = 0;

    long long current = x;

    for (int i = 0; i < n; i++) {

        long long value = current;

        // Remove larger elements from the back
        while (!dq.empty() && dq.back().val >= value) {
            dq.pop_back();
        }

        dq.push_back({i, value});

        // Remove elements outside the window
        while (!dq.empty() && dq.front().idx <= i - k) {
            dq.pop_front();
        } // we can do this step before the previous step because we are only interested in the minimum value in the current window, and if the front element is outside the window, it cannot be the minimum.

        // First complete window
        if (i >= k - 1) {
            answer ^= dq.front().val;
        }

        current = (a * current + b) % c;
    }

    cout << answer << '\n';
    return 0;
}
