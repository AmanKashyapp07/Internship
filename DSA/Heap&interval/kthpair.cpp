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

class Solution
{
public:
    int countPairs(vector<int>& nums, int limit) {
    int cnt = 0;

    for (int right = 0; right < nums.size(); right++) {
        int left = lower_bound(nums.begin(), nums.begin() + right+1,
                               nums[right] - limit) - nums.begin();
        // we wanted lowest left that satisfies nums[right] - nums[left] <= limit
        // so using lower bound to find the first index where nums[left] >= nums[right] - limit
        // our search range is [0, right) because we want pairs (left, right) with left < right]

        cnt += right - left;
    }

    return cnt;
}

    int smallestDistancePair(vector<int> &nums, int k)
    {
        sort(nums.begin(), nums.end());

        int low = 0;
        int high = nums.back() - nums.front();
        int ans = -1;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            int totalPairs = countPairs(nums, mid);

            if (totalPairs >= k)
            {
                ans = mid;      // current answer is valid
                high = mid - 1; // search for a smaller valid distance
            }
            else
            {
                low = mid + 1; // need a larger distance
            }
        }

        return ans;
    }
};