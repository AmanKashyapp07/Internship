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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

class Solution {
public:
    int merge(vector<int>& nums, int l, int mid, int r) {
        int count = 0;
        int j = mid + 1;

        for (int i = l; i <= mid; i++) {
            while (j <= r && (long long)nums[i] > 2LL * nums[j]) j++;
            count += (j - (mid + 1)); // all elements from mid+1 to j-1 are valid pairs with nums[i], so we add that count, j-1 - (mid+1) + 1 = j - (mid + 1)
        }

        vector<int> temp;
        int i = l;
        j = mid + 1;

        while (i <= mid && j <= r) {
            if (nums[i] <= nums[j]) temp.push_back(nums[i++]);
            else temp.push_back(nums[j++]);
        }

        while (i <= mid) temp.push_back(nums[i++]);
        while (j <= r) temp.push_back(nums[j++]);

        for (int k = l; k <= r; k++) {
            nums[k] = temp[k - l];
        }

        return count;
    }

    int mergeSort(vector<int>& nums, int l, int r) {
        if (l >= r) return 0;

        int mid = (l + r) / 2;
        int count = 0;

        count += mergeSort(nums, l, mid);
        count += mergeSort(nums, mid + 1, r);
        count += merge(nums, l, mid, r);

        return count;
    }

    int reversePairs(vector<int>& nums) {
        return mergeSort(nums, 0, nums.size() - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Solution solution;
    vector<int> nums = {1, 3, 2, 3, 1};
    int result = solution.reversePairs(nums);
    cout << result << endl; // Output: 2

    return 0;
}