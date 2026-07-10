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

// ------------------------------------------------------------
// LC 907 - Sum of Subarray Minimums
//
// Contribution:
// arr[i] is minimum in
// (i - previous smaller) * (next smaller/equal - i) subarrays.
//
// Stack:
// - increasing
// - pop while arr[top] >= arr[i]
// ------------------------------------------------------------
ll sumSubarrayMins(const vi& arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            ll leftCnt = mid - left;
            ll rightCnt = right - mid;

            ans = (ans + 1LL * arr[mid] * leftCnt % MOD * rightCnt) % MOD;
        }
        if (i < n) st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// LC 1793 - Maximum Score of a Good Subarray
//
// For each element, find the largest interval where it is the
// minimum. If that interval contains k, update the answer.
// ------------------------------------------------------------
int maximumScore(vector<int>& nums, int k) {
    int n = nums.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || nums[st.top()] >= nums[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            if (left < k && k < right)
                ans = max(ans, 1LL * nums[mid] * (right - left - 1));
        }
        if (i < n) st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Sum of Subarray Maximums
//
// Contribution:
// arr[i] is maximum in
// (i - previous greater) * (next greater/equal - i) subarrays.
//
// Stack:
// - decreasing
// - pop while arr[top] <= arr[i]
// ------------------------------------------------------------
ll sumSubarrayMaxs(const vi& arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            ll leftCnt = mid - left;
            ll rightCnt = right - mid;

            ans = (ans + 1LL * arr[mid] * leftCnt % MOD * rightCnt) % MOD;
        }
        if (i < n) st.push(i);
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}