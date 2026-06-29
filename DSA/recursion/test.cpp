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
    vector<vector<int>> ans;
    void helper(int idx, int prev, vector<int>& nums, vector<int>& temp) {
        if (idx == nums.size()) {
            if (temp.size() >= 2)
                ans.push_back(temp);
            return;
        }

        if (nums[idx] >= prev) {
            temp.push_back(nums[idx]);
            helper(idx + 1, nums[idx], nums, temp);
            temp.pop_back();
        }

        if (nums[idx] != prev)
            helper(idx + 1, prev, nums, temp);
    }
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        ans.clear();
        vector<int> temp;
        helper(0, INT_MIN, nums, temp);
        return ans;
    }
};