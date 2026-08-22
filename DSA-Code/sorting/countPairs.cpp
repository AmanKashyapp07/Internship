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
    #include <cstring>

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
    const ll P = 31;

    class Solution {
    public:
        int helper(int idx, int prev1, int prev2, vector<int>& nums, vector<vector<vector<int>>>& dp) {
            if(idx==nums.size()) return 1;
            if(dp[idx][prev1+1][prev2+1]!=-1) return dp[idx][prev1+1][prev2+1];
            int sum = nums[idx];
            int count=0;
            for(int i=0;i<=sum;i++){
                for(int j=0;j<=sum;j++){
                    if(i+j==sum){
                        if(prev1==-1 && prev2==-1){
                            count+=helper(idx+1,i,j,nums,dp);
                        }
                        else if (i>=prev1 && j<=prev2){
                        count+=helper(idx+1,i,j,nums,dp);
                        }
                    }
                }
            }
            return dp[idx][prev1+1][prev2+1]=count;
        }
        int countOfPairs(vector<int>& nums) {
            vector<vector<vector<int>>> dp(nums.size(), vector<vector<int>>(101, vector<int>(101, -1)));
            return helper(0, -1, -1, nums, dp);
        }
    };