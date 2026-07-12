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

bool canDivide(const vector<long long>& a, int k, long long maxSum) {
    int parts = 1;
    long long currentSum = 0;

    for (long long x : a) {
        if (currentSum + x <= maxSum) {
            currentSum += x;
        } else {
            parts++;
            currentSum = x;
        }
    }

    return parts <= k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> a(n);

    long long left = 0;
    long long right = 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        left = max(left, a[i]);
        right += a[i];
    }

    long long answer = right;

    while (left <= right) {
        long long mid = left + (right - left) / 2;

        if (canDivide(a, k, mid)) {
            answer = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    cout << answer << '\n';

    return 0;
}
