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

class Solution {
public:
    vector<int> subsetSum(vector<int>& nums) {
        int n = nums.size();
        vector<int> sums;

        for (int mask = 0; mask < (1 << n); ++mask) {
            int currentSum = 0;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) {
                    currentSum += nums[i];
                }
            }
            sums.push_back(currentSum);
        }

        return sums;
    }
    int minAbsDifference(vector<int>& nums, int goal) {
        vector<int> left(nums.begin(), nums.begin() + nums.size() / 2);
        vector<int> right(nums.begin() + nums.size() / 2, nums.end());

        vector<int> leftSums = subsetSum(left);
        vector<int> rightSums = subsetSum(right);

        sort(leftSums.begin(), leftSums.end());
        sort(rightSums.begin(), rightSums.end());

        int minDiff = INT_MAX;
        for (int sum : leftSums) {
            // we have to minimise abs(sum-goal) = abs(sum + rightSum - goal) = abs(rightSum - (goal - sum))
            int target = goal - sum;
            auto it = lower_bound(rightSums.begin(), rightSums.end(), target);
            int index = it - rightSums.begin();
            if(index!= rightSums.size()) {
                minDiff = min(minDiff, abs(sum + rightSums[index] - goal));
            }
            if(index > 0) {
                minDiff = min(minDiff, abs(sum + rightSums[index - 1] - goal));
            }
        }

        return minDiff;
    }
};