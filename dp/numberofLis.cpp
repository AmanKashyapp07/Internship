/**
 * CSES Problem Set / LeetCode 673 - Number of Longest Increasing Subsequence
 *
 * Description:
 * Given an array of integers, find the number of longest increasing subsequences.
 *
 * Approach:
 * - Dynamic programming using two arrays:
 *   - `dp[i]` stores the length of LIS ending at index `i`.
 *   - `count[i]` stores the number of LIS of length `dp[i]` ending at index `i`.
 * - Transition: For each element, iterate over all predecessors. If `nums[i] > nums[j]`:
 *   - If `dp[j] + 1 > dp[i]`, we found a longer LIS: update `dp[i] = dp[j] + 1` and reset `count[i] = count[j]`.
 *   - If `dp[j] + 1 == dp[i]`, we found another subsequence of the same length: add `count[j]` to `count[i]`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */

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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    vector<int> nums = {1, 2, 3, 4, 5};
    vector<int>dp(nums.size(), 1); // dp[i] = length of longest increasing subsequence ending at index i
    vector<int>count(nums.size(), 1); // count[i] = number of longest increasing subsequences ending at index i

    for(int i=1; i<nums.size(); i++){
        for(int j=0; j<i; j++){
            if(nums[i]>nums[j]){
                if(dp[i]<dp[j]+1){ // if we found a longer increasing subsequence ending at i
                    dp[i]=max(dp[i], dp[j]+1); // update the length of the longest increasing subsequence ending at i   
                    count[i] = count[j]; // reset the count to the count of the longest increasing subsequences ending at j
                }
                else if(dp[i]==dp[j]+1){
                    count[i] += count[j];
                }
            }
        }
    }
    int maxLength = *max_element(dp.begin(), dp.end());
    int totalCount = 0;
    for(int i=0; i<nums.size(); i++) if(dp[i]==maxLength) totalCount += count[i];

    return 0;
}