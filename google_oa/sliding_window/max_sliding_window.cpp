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
// Sliding Window Maximum
//
// Deque: decreasing
// Front = maximum of current window
//
// Time: O(N)
// ------------------------------------------------------------
vi maxSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;

    for (int R = 0; R < nums.size(); R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L)
            dq.pop_front();

        while (!dq.empty() && nums[dq.back()] <= nums[R])
            dq.pop_back();

        dq.push_back(R);

        if (L >= 0)
            ans.push_back(nums[dq.front()]);
    }

    return ans;
}

// ------------------------------------------------------------
// Sliding Window Minimum
//
// Deque: increasing
// Front = minimum of current window
//
// Time: O(N)
// ------------------------------------------------------------
vi minSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;

    for (int R = 0; R < nums.size(); R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L)
            dq.pop_front();

        while (!dq.empty() && nums[dq.back()] >= nums[R])
            dq.pop_back();

        dq.push_back(R);

        if (L >= 0)
            ans.push_back(nums[dq.front()]);
    }

    return ans;
}