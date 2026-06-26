/**
 * LeetCode 1043 - Partition Array for Maximum Sum
 *
 * Description:
 * Given an integer array arr, partition the array into (contiguous) subarrays of length at most k.
 * After partitioning, each subarray has their values changed to become the maximum value of that subarray.
 * Return the largest sum of the given array after partitioning.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Let `solve(i)` represent the maximum partition sum for suffix `[0...i]`.
 * - Transition: For each length `len` from 1 to `k`, partition the last subarray ending at `i`.
 *   - `solve(i) = max(solve(j - 1) + max_ele * (i - j + 1))` where `j` ranges from `i` down to `i - k + 1`.
 *
 * Time Complexity: O(n * k)
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

int solve(int i, vector<int>& arr, int k, vector<int>& dp){
    if(i<0) return 0;
    if(dp[i] != -1) return dp[i];
    int ans=0;
    int max_ele=0;
    int count=k;
    for(int j=i;j>=0 && count>0;j--){
        max_ele=max(max_ele,arr[j]);
        ans=max(ans,solve(j-1,arr,k,dp)+max_ele*(i-j+1));
        count--;
    }
    return dp[i]=ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int k, n;
    cin >> k >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];


    return 0;
}