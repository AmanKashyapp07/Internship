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

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second


class Solution {
public:
    void sortColors(vector<int>& nums) {
        int low = 0, mid = 0;
        int high = nums.size() - 1;

        while (mid <= high) {
            if (nums[mid] == 0) {
                swap(nums[low], nums[mid]);
                low++;
                mid++;
            }
            else if (nums[mid] == 1) {
                mid++;
            }
            else { // nums[mid] == 2
                swap(nums[mid], nums[high]);
                high--;
            }
        }
        for (int i = 0; i < nums.size(); i++) {
            cout << nums[i] << " ";
        }
    }
    // this is dutch national flag problem, we have to sort the array of 0s, 1s and 2s in a single pass and without using any extra space. The idea is to maintain three pointers low, mid and high. The low pointer keeps track of the position where the next 0 should be placed, the mid pointer traverses the array, and the high pointer keeps track of the position where the next 2 should be placed. We swap elements accordingly based on their values.
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Solution solution;
    vector<int> nums = {2, 0, 2, 1, 1, 0};
    solution.sortColors(nums);

    return 0;
}