/**
 * Problem: Next Permutation
 * Link: https://leetcode.com/problems/next-permutation/
 * Category: Array / Permutations
 * 
 * Description:
 * Rearrange numbers into the lexicographically next greater permutation.
 * 
 * Logic/Approach:
 * Find pivot from right, swap with next larger, reverse tail.
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

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

void nextPermutation(vi& nums) {
    int n = nums.size();

    int i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--; // this code breaks at the first index from right where nums[i] < nums[i + 1], this while loop will return i


    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--; // this code breaks at the first index from right where nums[j] > nums[i], this while loop will return j
        swap(nums[i], nums[j]);
        reverse(nums.begin() + i + 1, nums.end()); // reverse the suffix starting at i+1
        return;
    }
    else{
        reverse(all(nums));
        return;
    }

   
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vi nums(n);
    for (int& x : nums)
        cin >> x;

    nextPermutation(nums);

    for (int x : nums)
        cout << x << ' ';
    cout << '\n';

    return 0;
}