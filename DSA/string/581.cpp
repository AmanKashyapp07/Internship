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
    bool checkSorted(vector<int>& nums) {
        for(int i=1;i<nums.size();i++){
            if(nums[i]<nums[i-1]) return false;
        }
        return true;
    }
    int findUnsortedSubarray(vector<int>& nums) {
        if(checkSorted(nums)) return 0;
        int n=nums.size();
        int low=0,high=n-1;
        vector<int> sorted=nums;
        sort(sorted.begin(),sorted.end());
        while(low<n && nums[low]==sorted[low]) low++;
        while(high>=0 && nums[high]==sorted[high]) high--;
        return high-low+1;
    }
};

// time complexity is O(nlogn) due to sorting the array, and space complexity is O(n) due to storing the sorted copy of the array.