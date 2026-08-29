#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 1 (Problems 01 to 17)
// ============================================================================

void nextPermutation(vi& nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;
    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}
// Interview Explanation:
// - Problem Statement: Rearrange numbers into the lexicographically next greater permutation of numbers.
// - Approach: Single-pass scan from right to find pivot dip, swap with next larger element in suffix, then reverse suffix.
// - Intuition: A descending suffix is at its maximum permutation; finding the first drop `nums[i] < nums[i+1]` gives the rightmost element to increment with the smallest possible increase, and reversing the suffix resets it to ascending order (minimum).
// - Complexity: Time: O(N) at most 3 linear passes, Space: O(1) in-place.

vvi threeSum(vi& nums) {
    sort(nums.begin(), nums.end());
    vvi ans;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        if (nums[i] > 0) break; // smallest element > 0 means sum cannot be 0
        int l = i + 1, r = n - 1;
        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];
            if (sum == 0) {
                ans.push_back({nums[i], nums[l], nums[r]});
                while (l < r && nums[l] == nums[l + 1]) l++;
                while (l < r && nums[r] == nums[r - 1]) r--;
                l++; r--;
            } else if (sum < 0) {
                l++;
            } else {
                r--;
            }
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all unique triplets [nums[i], nums[j], nums[k]] such that i != j != k and nums[i] + nums[j] + nums[k] == 0.
// - Approach: Sorting + Two Pointers (converting 3-Sum to 2-Sum on the remaining suffix).
// - Intuition: Sorting allows deduplication by skipping identical elements for both the outer loop `i` and the two pointers `l, r`.
// - Complexity: Time: O(N^2) (O(N log N) sort + O(N^2) two pointer scans), Space: O(1) auxiliary space (excluding returned result).

int maxSubArray(vi& nums) {
    int max_sum = nums[0], curr = 0;
    for (int x : nums) {
        curr += x;
        max_sum = max(max_sum, curr);
        if (curr < 0) curr = 0;
    }
    return max_sum;
}
// Interview Explanation:
// - Problem Statement: Find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.
// - Approach: Kadane's Algorithm (Dynamic Programming with state compression).
// - Intuition: A negative prefix sum can never contribute positively to any future subarray; whenever the running sum becomes negative, discard it by resetting to 0.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

vi majorityElementII(vi& nums) {
    int c1 = 0, c2 = 0, cnt1 = 0, cnt2 = 0, n = nums.size();
    for (int x : nums) {
        if (cnt1 > 0 && x == c1) cnt1++;
        else if (cnt2 > 0 && x == c2) cnt2++;
        else if (cnt1 == 0) { c1 = x; cnt1 = 1; }
        else if (cnt2 == 0) { c2 = x; cnt2 = 1; }
        else { cnt1--; cnt2--; }
    }
    cnt1 = cnt2 = 0;
    for (int x : nums) {
        if (x == c1) cnt1++;
        else if (x == c2) cnt2++;
    }
    vi ans;
    if (cnt1 > n / 3) ans.push_back(c1);
    if (cnt2 > n / 3) ans.push_back(c2);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all elements that appear more than floor(n/3) times in an array of size n.
// - Approach: Extended Boyer-Moore Voting Algorithm (tracking up to 2 candidates).
// - Intuition: At most 2 elements can appear > floor(n/3) times. Pass 1 identifies 2 potential candidates by canceling out triplets of distinct elements; Pass 2 verifies their actual counts.
// - Complexity: Time: O(N) two linear passes, Space: O(1) auxiliary space.

ll countSubarraysWithXorK(vi& nums, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;
    ll count = 0;
    int xr = 0;
    for (int x : nums) {
        xr ^= x;
        int target = xr ^ k;
        if (freq.count(target)) count += freq[target];
        freq[xr]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count the total number of subarrays having bitwise XOR equal to k.
// - Approach: Prefix XOR with Hash Map frequency counting (similar to Prefix Sum for subarray sum equal to k).
// - Intuition: If prefix XOR up to index `i` is `xr`, and prefix XOR up to index `j-1` is `xr ^ k`, then the XOR of subarray `nums[j...i]` is `(xr ^ k) ^ xr = k`.
// - Complexity: Time: O(N) average time using hash map, Space: O(N) to store prefix XOR frequencies.

pii findMissingAndRepeating(vi& nums) {
    int n = nums.size(), xr = 0;
    for (int i = 0; i < n; i++) xr ^= nums[i] ^ (i + 1);

    int diff_bit = xr & (-xr); // lowest set bit distinguishing X and Y
    int b1 = 0, b2 = 0;
    for (int i = 0; i < n; i++) {
        if (nums[i] & diff_bit) b1 ^= nums[i];
        else b2 ^= nums[i];

        if ((i + 1) & diff_bit) b1 ^= (i + 1);
        else b2 ^= (i + 1);
    }

    int count_b1 = 0;
    for (int x : nums) if (x == b1) count_b1++;
    return (count_b1 == 2) ? make_pair(b1, b2) : make_pair(b2, b1); // {Repeating, Missing}
}
// Interview Explanation:
// - Problem Statement: Given an array of size n containing numbers from 1 to n with one number missing and one repeating, find both.
// - Approach: Bit Manipulation (XOR bucket separation) or Math equations (sum and sum of squares).
// - Intuition: XOR of all elements and 1..n gives `X ^ Y`. The lowest set bit isolates the bit where X and Y differ. Partition numbers into two buckets by this bit to isolate X and Y without integer overflow risk.
// - Complexity: Time: O(N) linear scan, Space: O(1) auxiliary space.

ll mergeAndCount(vi& arr, vi& temp, int l, int mid, int r) {
    int i = l, j = mid + 1, k = l;
    ll inv_count = 0;
    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            inv_count += (mid - i + 1); // all remaining elements in left half form inversions with arr[j]
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= r) temp[k++] = arr[j++];
    for (i = l; i <= r; i++) arr[i] = temp[i];
    return inv_count;
}

ll mergeSortCount(vi& arr, vi& temp, int l, int r) {
    ll inv = 0;
    if (l < r) {
        int mid = l + (r - l) / 2;
        inv += mergeSortCount(arr, temp, l, mid);
        inv += mergeSortCount(arr, temp, mid + 1, r);
        inv += mergeAndCount(arr, temp, l, mid, r);
    }
    return inv;
}

ll countInversions(vi arr) {
    int n = arr.size();
    vi temp(n);
    return mergeSortCount(arr, temp, 0, n - 1);
}
// Interview Explanation:
// - Problem Statement: Count how far the array is from being sorted (number of pairs (i, j) such that i < j and arr[i] > arr[j]).
// - Approach: Modified Merge Sort (Divide and Conquer).
// - Intuition: During the merge step, if `arr[i] > arr[j]`, then because the left subarray `[l...mid]` is sorted, every element from index `i` through `mid` is also greater than `arr[j]`, contributing `mid - i + 1` inversions.
// - Complexity: Time: O(N log N) standard merge sort recursion, Space: O(N) auxiliary buffer for merging.

int maxProduct(vi& nums) {
    int n = nums.size();
    int max_prod = nums[0], min_prod = nums[0], ans = nums[0];

    for (int i = 1; i < n; i++) {
        if (nums[i] < 0) swap(max_prod, min_prod);
        max_prod = max(nums[i], max_prod * nums[i]);
        min_prod = min(nums[i], min_prod * nums[i]);
        ans = max(ans, max_prod);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find a contiguous non-empty subarray that has the largest product and return the product.
// - Approach: Dynamic Programming tracking both maximum and minimum products ending at each index.
// - Intuition: A negative number can flip a large negative product into the maximum positive product; swapping `max_prod` and `min_prod` when `nums[i] < 0` accounts for sign reversal.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

bool searchRotatedII(vi& nums, int target) {
    int l = 0, r = nums.size() - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target) return true;

        // Ambiguous duplicate boundary: cannot determine sorted half
        if (nums[l] == nums[mid] && nums[mid] == nums[r]) {
            l++; r--;
            continue;
        }

        // Left half is sorted
        if (nums[l] <= nums[mid]) {
            if (nums[l] <= target && target < nums[mid]) r = mid - 1;
            else l = mid + 1;
        }
        // Right half is sorted
        else {
            if (nums[mid] < target && target <= nums[r]) l = mid + 1;
            else r = mid - 1;
        }
    }

    return false;
}
// Interview Explanation:
// - Problem Statement: Search for target in a rotated sorted array containing duplicates; return true if found, false otherwise.
// - Approach: Modified Binary Search with duplicate trimming.
// - Intuition: At least one half is always sorted. When `nums[l] == nums[mid] == nums[r]`, rotation pivot ambiguity occurs, so safely shrink search bounds with `l++` and `r--`.
// - Complexity: Time: O(log N) average, O(N) worst case (when all elements are identical), Space: O(1) auxiliary space.

int findMinRotated(vi& nums) {
    int l = 0, r = nums.size() - 1;

    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] > nums[r]) {
            l = mid + 1; // Inflection point is in right half
        } else {
            r = mid;     // Inflection point is at mid or left half
        }
    }

    return nums[l];
}
// Interview Explanation:
// - Problem Statement: Find the minimum element in a sorted rotated array of unique elements in O(log n) time.
// - Approach: Binary Search comparing `nums[mid]` against `nums[r]`.
// - Intuition: If `nums[mid] > nums[r]`, the unsorted drop (minimum element) must be strictly right of `mid` (search `[mid+1, r]`); otherwise, the right half is sorted and the minimum is within `[l, mid]`.
// - Complexity: Time: O(log N) strictly halving search space, Space: O(1) auxiliary space.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

vi twoSum(vi& nums, int target) {
    unordered_map<int, int> mp;
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        if (mp.count(complement)) return {mp[complement], i};
        mp[nums[i]] = i;
    }
    return {};
}
// Interview Explanation:
// - Problem Statement: Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
// - Approach: One-pass Hash Map storing element indices.
// - Intuition: For each element `nums[i]`, check if its complement `target - nums[i]` has already been seen in the hash map. If so, return pair of indices; otherwise, store `nums[i] -> i`.
// - Complexity: Time: O(N) single pass, Space: O(N) hash map storage.

int maxProfit(vi& prices) {
    int min_price = INT_MAX, max_profit = 0;
    for (int p : prices) {
        min_price = min(min_price, p);
        max_profit = max(max_profit, p - min_price);
    }
    return max_profit;
}
// Interview Explanation:
// - Problem Statement: Find maximum profit possible by buying on one day and selling on a future day.
// - Approach: Single-pass Greedy tracking running minimum buy price.
// - Intuition: At each day `i`, calculate profit if sold today (`prices[i] - min_price`) and update `max_profit`. Simultaneously maintain the lowest price observed so far.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

bool containsDuplicate(vi& nums) {
    unordered_set<int> seen;
    for (int x : nums) {
        if (!seen.insert(x).second) return true;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Return true if any value appears at least twice in the array, and false if every element is distinct.
// - Approach: Hash Set insertion check.
// - Intuition: Insert elements one by one into an `unordered_set`. If `insert()` indicates the element already exists, return true immediately.
// - Complexity: Time: O(N) average time, Space: O(N) hash set.

vi productExceptSelf(vi& nums) {
    int n = nums.size();
    vi ans(n, 1);

    int prefix = 1;
    for (int i = 0; i < n; i++) {
        ans[i] = prefix;
        prefix *= nums[i];
    }

    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        ans[i] *= suffix;
        suffix *= nums[i];
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Return an array answer such that answer[i] is equal to the product of all the elements of nums except nums[i], without using division in O(N) time.
// - Approach: Prefix and Suffix Running Products in O(1) extra space (excluding result).
// - Intuition: First pass fills `ans[i]` with prefix product of all elements to the left of `i`. Second backwards pass accumulates running suffix product and multiplies it with `ans[i]`.
// - Complexity: Time: O(N) two linear passes, Space: O(1) auxiliary space (excluding returned vector).

int getSum(int a, int b) {
    while (b != 0) {
        unsigned int carry = (unsigned int)(a & b) << 1;
        a = a ^ b; // sum without carry
        b = carry; // carry
    }
    return a;
}
// Interview Explanation:
// - Problem Statement: Calculate sum of two integers a and b without using operators + and -.
// - Approach: Bitwise XOR and AND bit manipulation.
// - Intuition: Bitwise XOR (`a ^ b`) calculates sum without carry. Bitwise AND shifted left (`(a & b) << 1`) calculates the carry. Repeat until carry becomes 0.
// - Complexity: Time: O(32) = O(1) integer bit length, Space: O(1) auxiliary space.

int hammingWeight(uint32_t n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1); // clears lowest set bit
        count++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Return number of set bits ('1's) in the binary representation of an unsigned integer.
// - Approach: Brian Kernighan's Algorithm.
// - Intuition: The bitwise operation `n & (n - 1)` directly clears the lowest set bit of `n`. The loop runs exactly as many times as there are set bits.
// - Complexity: Time: O(K) where K is number of set bits (<= 32), Space: O(1) auxiliary space.

vi countBits(int n) {
    vi ans(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        ans[i] = ans[i >> 1] + (i & 1); // dp transition
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Given an integer n, return an array of length n + 1 where ans[i] is the number of 1's in the binary representation of i.
// - Approach: Dynamic Programming using bit shifting (`i >> 1`).
// - Intuition: Number of set bits in `i` equals number of set bits in `i / 2` (`i >> 1`) plus 1 if `i` is odd (`i & 1`).
// - Complexity: Time: O(N) single linear pass, Space: O(1) auxiliary space (excluding result).
