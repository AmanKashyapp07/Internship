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
// Next Greater Element
//
// Stack: decreasing
// Pop while current > stack.top()
// ------------------------------------------------------------
vi nextGreaterElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Next Smaller Element
//
// Stack: increasing
// Pop while current < stack.top()
// ------------------------------------------------------------
vi nextSmallerElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Previous Greater Element
//
// Stack: decreasing
// Pop while stack.top() <= current
// ------------------------------------------------------------
vi previousGreaterElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] <= nums[i])
            st.pop();

        if (!st.empty())
            ans[i] = nums[st.top()];

        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Previous Smaller Element
//
// Stack: increasing
// Pop while stack.top() >= current
// ------------------------------------------------------------
vi previousSmallerElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] >= nums[i])
            st.pop();

        if (!st.empty())
            ans[i] = nums[st.top()];

        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Stock Span
//
// Previous Greater Element (index version)
// ------------------------------------------------------------
vi stockSpan(const vi& prices) {
    int n = prices.size();
    vi span(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && prices[st.top()] <= prices[i])
            st.pop();

        span[i] = st.empty() ? i + 1 : i - st.top();
        st.push(i);
    }

    return span;
}

// ------------------------------------------------------------
// Next Greater Element II (Circular)
//
// Traverse twice.
// Push indices only during first pass.
// ------------------------------------------------------------
vi nextGreaterCircular(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            ans[st.top()] = nums[i % n];
            st.pop();
        }

        if (i < n)
            st.push(i);
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}