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
    int n, A, B;
    cin >> n >> A >> B;

    vector<long long> arr(n + 1);
    vector<long long> pref(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        pref[i] = pref[i - 1] + arr[i];
    }

    deque<int> dq;
    long long ans = LLONG_MIN;

    for (int i = A; i <= n; i++) {

        // Valid range of j:
        // i - B <= j <= i - A
        int L = i - B;
        int R = i - A;
         // Remove indices outside the left boundary
        while (!dq.empty() && dq.front() < L) {
            dq.pop_front();
        }
        // Insert the new right boundary index
        while (!dq.empty() && pref[dq.back()] >= pref[R]) {
            dq.pop_back();
        }
        dq.push_back(R);


        // Minimum prefix sum in [L, R] is at the front
        ans = max(ans, pref[i] - pref[dq.front()]);
    }

    cout << ans << '\n';
}
