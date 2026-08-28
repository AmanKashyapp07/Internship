/*
================================================================================
  STRIVER'S 79 + BLIND 75 COMPREHENSIVE INTERVIEW REVISION SHEET
  File: DSA-Code/notebook-11.cpp
  Language: C++17 / C++20
  Covers Striver Sheet + Unique Blind 75 problems (Duplicates & Linked Lists excluded).
  Optimal solutions and full interview explanations included.
================================================================================
  INDEX:
  [STRIVER'S 79 SHEET]
    Section 01: Arrays & Hashing (Problems 01 - 08)
    Section 02: Binary Search (Problems 09 - 16)
    Section 04: Recursion and Backtracking (Problems 23 - 28)
    Section 05: Stacks & Queues (Problems 29 - 34)
    Section 06: Heaps (Problems 35 - 37)
    Section 07: Trees (BT & BST) (Problems 38 - 48)
    Section 08: Graphs (Problems 49 - 60)
    Section 09: Dynamic Programming (Problems 61 - 72)
    Section 10: Tries (Problems 73 - 75)
    Section 11: Strings (Problems 76 - 79)
  
  [BLIND 75 MERGED (NON-DUPLICATE PROBLEMS)]
    Section 12: Blind 75 - Arrays & Hashing (Problems 80 - 85)
    Section 13: Blind 75 - Bit Manipulation (Problems 86 - 90)
    Section 14: Blind 75 - Dynamic Programming (Problems 91 - 97)
    Section 15: Blind 75 - Graphs & Connectivity (Problems 98 - 103)
    Section 16: Blind 75 - Intervals (Problems 104 - 108)
    Section 17: Blind 75 - Matrices (Problems 113 - 115)
    Section 18: Blind 75 - Strings (Problems 116 - 125)
    Section 19: Blind 75 - Trees & Heaps (Problems 126 - 137)
================================================================================
*/

#include <iostream>
#include <vector>
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

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#endif

using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vl = vector<ll>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// ============================================================================
// COMMON DATA STRUCTURES
// ============================================================================

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// ============================================================================
// [PART 1 / 5]
// SECTION 1: ARRAYS AND HASHING (Problems 01 - 08)
// ============================================================================

// ----------------------------------------------------------------------------
// 01. Next Permutation (LeetCode 31)
// ----------------------------------------------------------------------------
void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int i = n - 2;
    // Step 1: Find first decreasing element from right
    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    }
    // Step 2: If found, find element just larger than nums[i] to swap with
    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        swap(nums[i], nums[j]);
    }
    // Step 3: Reverse suffix to get next lexicographically smallest sequence
    reverse(nums.begin() + i + 1, nums.end());
}
// Interview Explanation:
// - Problem Statement: Find the next lexicographically greater permutation of numbers in-place. If no greater permutation exists, rearrange in ascending order.
// - Approach: Find longest non-increasing suffix, identify pivot right before suffix, swap pivot with smallest successor in suffix, then reverse suffix.
// - Intuition: The suffix after the pivot is in descending order; reversing makes it ascending (smallest possible increment).
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 02. 3 Sum (LeetCode 15)
// ----------------------------------------------------------------------------
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    int n = nums.size();
    sort(nums.begin(), nums.end());
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // Skip duplicate first elements
        if (nums[i] > 0) break; // Smallest element > 0 means triplet sum cannot be 0
        int left = i + 1, right = n - 1;
        while (left < right) {
            long long sum = (long long)nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                res.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++;
                right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return all unique triplets [nums[i], nums[j], nums[k]] such that i != j != k and sum is 0.
// - Approach: Sort array + fix first element + two-pointer sweep for remainder. Skip identical elements to avoid duplicates.
// - Intuition: Sorting enables two-pointer convergence in O(N) for each fixed element, avoiding hash map overhead.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.


// ----------------------------------------------------------------------------
// 03. Kadane's Algorithm - Maximum Subarray (LeetCode 53)
// ----------------------------------------------------------------------------
int maxSubArray(vector<int>& nums) {
    int maxSoFar = nums[0];
    int currentMax = nums[0];
    for (size_t i = 1; i < nums.size(); i++) {
        currentMax = max(nums[i], currentMax + nums[i]);
        maxSoFar = max(maxSoFar, currentMax);
    }
    return maxSoFar;
}

pair<int, pair<int, int>> kadaneWithIndices(const vector<int>& nums) {
    int maxSoFar = nums[0], currentMax = nums[0];
    int start = 0, end = 0, tempStart = 0;
    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] > currentMax + nums[i]) {
            currentMax = nums[i];
            tempStart = i;
        } else {
            currentMax += nums[i];
        }
        if (currentMax > maxSoFar) {
            maxSoFar = currentMax;
            start = tempStart;
            end = i;
        }
    }
    return {maxSoFar, {start, end}};
}
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with maximum sum.
// - Approach: DP / Greedy tracking running sum, restarting when running sum becomes worse than starting fresh.
// - Intuition: If the accumulated sum becomes negative, adding it to subsequent elements will only decrease their potential sum.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 04. Majority Element-II (> N/3) (LeetCode 229)
// ----------------------------------------------------------------------------
vector<int> majorityElementII(vector<int>& nums) {
    int n = nums.size();
    int count1 = 0, count2 = 0;
    int candidate1 = 0, candidate2 = 0;

    for (int num : nums) {
        if (num == candidate1) {
            count1++;
        } else if (num == candidate2) {
            count2++;
        } else if (count1 == 0) {
            candidate1 = num;
            count1 = 1;
        } else if (count2 == 0) {
            candidate2 = num;
            count2 = 1;
        } else {
            count1--;
            count2--;
        }
    }

    // Verification pass
    count1 = 0; count2 = 0;
    for (int num : nums) {
        if (num == candidate1) count1++;
        else if (num == candidate2) count2++;
    }

    vector<int> result;
    if (count1 > n / 3) result.push_back(candidate1);
    if (count2 > n / 3) result.push_back(candidate2);
    return result;
}
// Interview Explanation:
// - Problem Statement: Find all elements appearing strictly more than floor(N/3) times.
// - Approach: Extended Boyer-Moore Voting algorithm tracking up to 2 candidates, followed by verification.
// - Intuition: There can be at most 2 elements occurring > N/3 times. Cancelling 3 distinct elements preserves majority candidates.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 05. Count Subarrays with Given XOR K
// ----------------------------------------------------------------------------
long long countSubarraysWithXorK(const vector<int>& nums, int k) {
    unordered_map<int, int> prefixXorFreq;
    prefixXorFreq[0] = 1;
    int currentXor = 0;
    long long count = 0;

    for (int num : nums) {
        currentXor ^= num;
        int required = currentXor ^ k;
        if (prefixXorFreq.find(required) != prefixXorFreq.end()) {
            count += prefixXorFreq[required];
        }
        prefixXorFreq[currentXor]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total number of subarrays whose bitwise XOR equals k.
// - Approach: Prefix XOR with Hash Map frequency table.
// - Intuition: If prefix XOR up to index j is `currXor`, we need a prefix index i-1 where `pref[i-1] = currXor ^ k` because `currXor ^ (currXor ^ k) = k`.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 06. Find the Repeating and Missing Number
// ----------------------------------------------------------------------------
pair<int, int> findRepeatingAndMissing(const vector<int>& nums) {
    long long n = nums.size();
    long long s = 0, s2 = 0;
    for (long long x : nums) {
        s += x;
        s2 += x * x;
    }
    long long sn = n * (n + 1) / 2;
    long long s2n = n * (n + 1) * (2 * n + 1) / 6;

    long long diff = s - sn;                 // X - Y (Repeating - Missing)
    long long sumSquaresDiff = s2 - s2n;     // X^2 - Y^2 = (X - Y)(X + Y)
    long long sumXY = sumSquaresDiff / diff; // X + Y

    long long repeating = (diff + sumXY) / 2;
    long long missing = sumXY - repeating;
    return {(int)repeating, (int)missing};
}

pair<int, int> findRepeatingAndMissingXor(const vector<int>& nums) {
    int n = nums.size();
    int xorAll = 0;
    for (int i = 0; i < n; i++) {
        xorAll ^= nums[i];
        xorAll ^= (i + 1);
    }
    int setBit = xorAll & (-xorAll); // lowest set bit
    int bucket1 = 0, bucket2 = 0;
    for (int num : nums) {
        if (num & setBit) bucket1 ^= num;
        else bucket2 ^= num;
    }
    for (int i = 1; i <= n; i++) {
        if (i & setBit) bucket1 ^= i;
        else bucket2 ^= i;
    }
    for (int num : nums) {
        if (num == bucket1) return {bucket1, bucket2};
    }
    return {bucket2, bucket1};
}
// Interview Explanation:
// - Problem Statement: In an array of size N with elements 1 to N, one number is repeated and one is missing. Find both.
// - Approach: Math system of linear equations (sum diff & sum of squares diff) or XOR partitioning on lowest set bit.
// - Intuition: S - Sn = X - Y and S2 - S2n = X^2 - Y^2 gives X + Y = (S2 - S2n)/(X - Y), solving directly in O(1) extra space.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 07. Count Inversions
// ----------------------------------------------------------------------------
long long mergeAndCount(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    long long invCount = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            invCount += (mid - i + 1); // all elements from i to mid are greater than arr[j]
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; i++) arr[i] = temp[i];
    return invCount;
}

long long mergeSortCount(vector<int>& arr, vector<int>& temp, int left, int right) {
    long long invCount = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        invCount += mergeSortCount(arr, temp, left, mid);
        invCount += mergeSortCount(arr, temp, mid + 1, right);
        invCount += mergeAndCount(arr, temp, left, mid, right);
    }
    return invCount;
}

long long countInversions(vector<int>& arr) {
    vector<int> temp = arr;
    return mergeSortCount(arr, temp, 0, arr.size() - 1);
}
// Interview Explanation:
// - Problem Statement: Count pairs (i, j) where i < j and arr[i] > arr[j].
// - Approach: Divide and Conquer via Merge Sort. Count cross-inversions during merging step.
// - Intuition: When merging two sorted halves, if arr[i] > arr[j], then since the left subarray is sorted, all elements from i to mid are strictly greater than arr[j].
// - Complexity: Time: O(N log N), Space: O(N).


// ----------------------------------------------------------------------------
// 08. Maximum Product Subarray in an Array (LeetCode 152)
// ----------------------------------------------------------------------------
int maxProduct(vector<int>& nums) {
    int maxProd = nums[0];
    int currentMax = nums[0];
    int currentMin = nums[0];

    for (size_t i = 1; i < nums.size(); i++) {
        if (nums[i] < 0) {
            swap(currentMax, currentMin);
        }
        currentMax = max(nums[i], currentMax * nums[i]);
        currentMin = min(nums[i], currentMin * nums[i]);
        maxProd = max(maxProd, currentMax);
    }
    return maxProd;
}
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with maximum product.
// - Approach: DP maintaining current max and min product. Swap max and min when encountering negative number.
// - Intuition: Multiplying a negative number by the minimum (most negative) product yields a potentially large positive value.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================================
// SECTION 2: BINARY SEARCH (Problems 09 - 16)
// ============================================================================

// ----------------------------------------------------------------------------
// 09. Search in Rotated Sorted Array-II (LeetCode 81 - with duplicates)
// ----------------------------------------------------------------------------
bool searchInRotatedSortedArrayII(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return true;

        // Handle duplicates where nums[left] == nums[mid] == nums[right]
        if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
            left++;
            right--;
            continue;
        }

        // Left half is sorted
        if (nums[left] <= nums[mid]) {
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        // Right half is sorted
        else {
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Search target in rotated sorted array containing duplicate values.
// - Approach: Binary Search with duplicate shrinking at boundaries when `nums[left] == nums[mid] == nums[right]`.
// - Intuition: At least one half is always sorted. If duplicates obscure which half is sorted, trim both endpoints safely.
// - Complexity: Time: O(log N) average, O(N) worst case with all duplicates, Space: O(1).


// ----------------------------------------------------------------------------
// 10. Find Minimum in Rotated Sorted Array (LeetCode 153)
// ----------------------------------------------------------------------------
int findMinInRotatedSortedArray(vector<int>& nums) {
    int left = 0, right = nums.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[right]) {
            // Min lies in right unsorted part
            left = mid + 1;
        } else {
            // Min lies at mid or to the left
            right = mid;
        }
    }
    return nums[left];
}
// Interview Explanation:
// - Problem Statement: Find minimum element in rotated sorted array of unique elements.
// - Approach: Binary search comparing `nums[mid]` with `nums[right]`.
// - Intuition: If `nums[mid] > nums[right]`, inflection point is to the right; otherwise, min is at or left of mid.
// - Complexity: Time: O(log N), Space: O(1).


// ----------------------------------------------------------------------------
// 11. Find Peak Element (LeetCode 162)
// ----------------------------------------------------------------------------
int findPeakElement(vector<int>& nums) {
    int left = 0, right = nums.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[mid + 1]) {
            // Descending slope -> peak must be at mid or to the left
            right = mid;
        } else {
            // Ascending slope -> peak must be to the right
            left = mid + 1;
        }
    }
    return left;
}
// Interview Explanation:
// - Problem Statement: Find an index i such that `nums[i] > nums[i-1]` and `nums[i] > nums[i+1]`.
// - Approach: Binary Search on slope.
// - Intuition: If `nums[mid] < nums[mid+1]`, following the upward slope guarantees reaching a peak since boundary values are -infinity.
// - Complexity: Time: O(log N), Space: O(1).


// ----------------------------------------------------------------------------
// 12. Koko Eating Bananas (LeetCode 875)
// ----------------------------------------------------------------------------
bool canEatAll(const vector<int>& piles, int speed, int h) {
    long long totalHours = 0;
    for (int p : piles) {
        totalHours += (p + speed - 1) / speed; // ceil division
        if (totalHours > h) return false;
    }
    return totalHours <= h;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int left = 1;
    int right = *max_element(piles.begin(), piles.end());
    int ans = right;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (canEatAll(piles, mid, h)) {
            ans = mid;
            right = mid - 1; // try smaller speed
        } else {
            left = mid + 1; // need higher speed
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum eating speed k (bananas/hour) to eat all piles within h hours.
// - Approach: Binary Search on Answer range [1, max(piles)].
// - Intuition: Total hours required is monotonically non-increasing with respect to speed k, enabling binary search.
// - Complexity: Time: O(N log(max(pile))), Space: O(1).


// ----------------------------------------------------------------------------
// 13. Aggressive Cows (SPOJ / GFG)
// ----------------------------------------------------------------------------
bool canPlaceCows(const vector<int>& stalls, int k, int minDist) {
    int count = 1;
    int lastPos = stalls[0];
    for (size_t i = 1; i < stalls.size(); i++) {
        if (stalls[i] - lastPos >= minDist) {
            count++;
            lastPos = stalls[i];
            if (count >= k) return true;
        }
    }
    return false;
}

int aggressiveCows(vector<int>& stalls, int k) {
    sort(stalls.begin(), stalls.end());
    int left = 1;
    int right = stalls.back() - stalls.front();
    int ans = 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (canPlaceCows(stalls, k, mid)) {
            ans = mid;
            left = mid + 1; // maximize minimum distance
        } else {
            right = mid - 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Place k cows in given stalls such that the minimum distance between any two cows is maximized.
// - Approach: Sort stall coordinates + Binary Search on minimum distance [1, max_coord - min_coord].
// - Intuition: Greedily placing cows at the first available stall >= minDist checks feasibility in O(N).
// - Complexity: Time: O(N log N + N log(max_dist)), Space: O(1).


// ----------------------------------------------------------------------------
// 14. Book Allocation Problem (GFG / InterviewBit)
// ----------------------------------------------------------------------------
bool isAllocationPossible(const vector<int>& arr, int n, int m, long long maxPages) {
    int students = 1;
    long long currentPages = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > maxPages) return false;
        if (currentPages + arr[i] > maxPages) {
            students++;
            currentPages = arr[i];
            if (students > m) return false;
        } else {
            currentPages += arr[i];
        }
    }
    return true;
}

long long findPages(vector<int>& arr, int n, int m) {
    if (m > n) return -1;
    long long left = *max_element(arr.begin(), arr.end());
    long long right = accumulate(arr.begin(), arr.end(), 0LL);
    long long ans = -1;

    while (left <= right) {
        long long mid = left + (right - left) / 2;
        if (isAllocationPossible(arr, n, m, mid)) {
            ans = mid;
            right = mid - 1; // minimize maximum pages
        } else {
            left = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Allocate contiguous books to m students such that the maximum pages allocated to a student is minimized.
// - Approach: Binary Search on Answer range [max(arr), sum(arr)].
// - Intuition: Allocation feasibility is monotonic: larger page limits require fewer students.
// - Complexity: Time: O(N log(sum - max)), Space: O(1).


// ----------------------------------------------------------------------------
// 15. Median of 2 Sorted Arrays (LeetCode 4)
// ----------------------------------------------------------------------------
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) {
        return findMedianSortedArrays(nums2, nums1); // Ensure nums1 is the smaller array
    }
    int n1 = nums1.size(), n2 = nums2.size();
    int left = 0, right = n1;

    while (left <= right) {
        int cut1 = left + (right - left) / 2;
        int cut2 = (n1 + n2 + 1) / 2 - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int r1 = (cut1 == n1) ? INT_MAX : nums1[cut1];
        int r2 = (cut2 == n2) ? INT_MAX : nums2[cut2];

        if (l1 <= r2 && l2 <= r1) {
            if ((n1 + n2) % 2 == 1) {
                return max(l1, l2);
            } else {
                return (max(l1, l2) + min(r1, r2)) / 2.0;
            }
        } else if (l1 > r2) {
            right = cut1 - 1;
        } else {
            left = cut1 + 1;
        }
    }
    return 0.0;
}
// Interview Explanation:
// - Problem Statement: Find the median of two sorted arrays in O(log(min(m, n))) time.
// - Approach: Binary Search on partition cut in the smaller array.
// - Intuition: Partition both arrays into left and right halves of equal total size such that `max(left1, left2) <= min(right1, right2)`.
// - Complexity: Time: O(log(min(N1, N2))), Space: O(1).


// ----------------------------------------------------------------------------
// 16. Minimize Max Distance to Gas Station (LeetCode 774)
// ----------------------------------------------------------------------------
bool isGasStationDistPossible(const vector<int>& stations, int k, double dist) {
    int count = 0;
    for (size_t i = 1; i < stations.size(); i++) {
        double gap = stations[i] - stations[i - 1];
        count += (int)(gap / dist);
        if (count > k) return false;
    }
    return count <= k;
}

double minmaxGasDist(vector<int>& stations, int k) {
    double left = 0.0;
    double right = stations.back() - stations.front();
    double eps = 1e-6;

    while (right - left > eps) {
        double mid = left + (right - left) / 2.0;
        if (isGasStationDistPossible(stations, k, mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}
// Interview Explanation:
// - Problem Statement: Add k new gas stations to minimize the maximum distance between adjacent gas stations.
// - Approach: Binary Search on real values within [0, max_gap] with epsilon precision termination (1e-6).
// - Intuition: For a maximum gap D, the number of stations required between adjacent existing stations is `floor(gap / D)`.
// - Complexity: Time: O(N log((max - min) / eps)), Space: O(1).



// ============================================================================
// SECTION 4: RECURSION AND BACKTRACKING (Problems 23 - 28)
// ============================================================================

// ----------------------------------------------------------------------------
// 23. Power Set / All Subsets (LeetCode 78)
// ----------------------------------------------------------------------------
void generateSubsets(int index, vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
    if (index == (int)nums.size()) {
        result.push_back(current);
        return;
    }
    // Include nums[index]
    current.push_back(nums[index]);
    generateSubsets(index + 1, nums, current, result);
    // Exclude nums[index] (Backtrack)
    current.pop_back();
    generateSubsets(index + 1, nums, current, result);
}

vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    generateSubsets(0, nums, current, result);
    return result;
}

// Bitmask approach
vector<vector<int>> subsetsBitmask(vector<int>& nums) {
    int n = nums.size();
    int total = 1 << n;
    vector<vector<int>> result(total);
    for (int mask = 0; mask < total; mask++) {
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                result[mask].push_back(nums[i]);
            }
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Return all possible subsets (the power set) of an integer array with unique elements.
// - Approach: Pick / Don't pick recursive backtracking or bitmask iteration from 0 to 2^N - 1.
// - Intuition: Each element has two binary choices: either be present in the subset or omitted.
// - Complexity: Time: O(N * 2^N), Space: O(N) recursion stack.


// ----------------------------------------------------------------------------
// 24. Combination Sum (LeetCode 39)
// ----------------------------------------------------------------------------
void findCombinations(int index, int target, vector<int>& candidates, vector<int>& current, vector<vector<int>>& result) {
    if (target == 0) {
        result.push_back(current);
        return;
    }
    if (index == (int)candidates.size() || target < 0) {
        return;
    }

    // Pick current candidate (can pick multiple times)
    if (candidates[index] <= target) {
        current.push_back(candidates[index]);
        findCombinations(index, target - candidates[index], candidates, current, result);
        current.pop_back(); // Backtrack
    }

    // Don't pick current candidate, move to next
    findCombinations(index + 1, target, candidates, current, result);
}

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<int>> result;
    vector<int> current;
    findCombinations(0, target, candidates, current, result);
    return result;
}
// Interview Explanation:
// - Problem Statement: Find all unique combinations of candidates where chosen numbers sum to target. An element may be chosen unlimited times.
// - Approach: Backtracking recursion with reuse of current index on include step.
// - Intuition: Stay at same index on inclusion; advance index on exclusion to explore all valid non-negative integer partitions.
// - Complexity: Time: O(2^T * K), Space: O(K) recursion depth where K = target / min(candidates).


// ----------------------------------------------------------------------------
// 25. N Queen (LeetCode 51)
// ----------------------------------------------------------------------------
void solveNQueensHelper(int col, int n, vector<string>& board, vector<vector<string>>& ans,
                        vector<bool>& leftRow, vector<bool>& upperDiagonal, vector<bool>& lowerDiagonal) {
    if (col == n) {
        ans.push_back(board);
        return;
    }

    for (int row = 0; row < n; row++) {
        if (!leftRow[row] && !lowerDiagonal[row + col] && !upperDiagonal[n - 1 + col - row]) {
            board[row][col] = 'Q';
            leftRow[row] = true;
            lowerDiagonal[row + col] = true;
            upperDiagonal[n - 1 + col - row] = true;

            solveNQueensHelper(col + 1, n, board, ans, leftRow, upperDiagonal, lowerDiagonal);

            board[row][col] = '.';
            leftRow[row] = false;
            lowerDiagonal[row + col] = false;
            upperDiagonal[n - 1 + col - row] = false;
        }
    }
}

vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> ans;
    vector<string> board(n, string(n, '.'));
    vector<bool> leftRow(n, false);
    vector<bool> upperDiagonal(2 * n - 1, false);
    vector<bool> lowerDiagonal(2 * n - 1, false);

    solveNQueensHelper(0, n, board, ans, leftRow, upperDiagonal, lowerDiagonal);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Place N non-attacking queens on an N x N chessboard.
// - Approach: Backtracking column by column with O(1) hash arrays for row, upper diagonal (`n-1 + col - row`), and lower diagonal (`row + col`).
// - Intuition: Hash lookups replace O(N) diagonal sweeps, speeding up valid placement validation.
// - Complexity: Time: O(N!), Space: O(N) auxiliary arrays.


// ----------------------------------------------------------------------------
// 26. Sudoku Solver (LeetCode 37)
// ----------------------------------------------------------------------------
bool isValidSudokuPlacement(const vector<vector<char>>& board, int row, int col, char c) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == c) return false;
        if (board[i][col] == c) return false;
        if (board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == c) return false;
    }
    return true;
}

bool solveSudokuRecursive(vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                for (char c = '1'; c <= '9'; c++) {
                    if (isValidSudokuPlacement(board, i, j, c)) {
                        board[i][j] = c;
                        if (solveSudokuRecursive(board)) return true;
                        board[i][j] = '.'; // Backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void solveSudoku(vector<vector<char>>& board) {
    solveSudokuRecursive(board);
}
// Interview Explanation:
// - Problem Statement: Solve a 9x9 Sudoku puzzle by filling empty cells '.' such that rows, columns, and 3x3 sub-boxes contain digits '1'-'9' once.
// - Approach: Backtracking depth-first search trying digits '1' to '9' at first empty cell.
// - Intuition: Prune search tree immediately whenever a digit violates Sudoku constraints.
// - Complexity: Time: O(9^(empty_cells)), Space: O(empty_cells) recursion stack.


// ----------------------------------------------------------------------------
// 27. M Coloring Problem (GFG)
// ----------------------------------------------------------------------------
bool isColorSafe(int node, int color[], const vector<vector<int>>& adj, int col) {
    for (int neighbor : adj[node]) {
        if (color[neighbor] == col) return false;
    }
    return true;
}

bool solveMColoring(int node, int color[], int m, int n, const vector<vector<int>>& adj) {
    if (node == n) return true;

    for (int c = 1; c <= m; c++) {
        if (isColorSafe(node, color, adj, c)) {
            color[node] = c;
            if (solveMColoring(node + 1, color, m, n, adj)) return true;
            color[node] = 0; // Backtrack
        }
    }
    return false;
}

bool graphColoring(int n, const vector<pair<int, int>>& edges, int m) {
    vector<vector<int>> adj(n);
    for (auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    int color[105] = {0};
    return solveMColoring(0, color, m, n, adj);
}
// Interview Explanation:
// - Problem Statement: Given an undirected graph and integer m, determine if graph can be colored with at most m colors such that no adjacent vertices share a color.
// - Approach: Backtracking assigning color 1..m to vertex `node` and checking adjacent colors.
// - Intuition: Recursive assignment with immediate backtracking on conflict explores valid state space.
// - Complexity: Time: O(m^V), Space: O(V) color array and call stack.


// ----------------------------------------------------------------------------
// 28. Word Search (LeetCode 79)
// ----------------------------------------------------------------------------
bool wordSearchDFS(vector<vector<char>>& board, const string& word, int i, int j, int k) {
    if (k == (int)word.size()) return true;
    if (i < 0 || i >= (int)board.size() || j < 0 || j >= (int)board[0].size() || board[i][j] != word[k]) {
        return false;
    }

    char temp = board[i][j];
    board[i][j] = '#'; // Mark visited in-place

    bool found = wordSearchDFS(board, word, i + 1, j, k + 1) ||
                 wordSearchDFS(board, word, i - 1, j, k + 1) ||
                 wordSearchDFS(board, word, i, j + 1, k + 1) ||
                 wordSearchDFS(board, word, i, j - 1, k + 1);

    board[i][j] = temp; // Unmark (Backtrack)
    return found;
}

bool exist(vector<vector<char>>& board, string word) {
    int m = board.size(), n = board[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == word[0] && wordSearchDFS(board, word, i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if given word exists in 2D character grid by sequentially moving to horizontally/vertically adjacent cells without reusing any cell in the same word.
// - Approach: DFS Backtracking with in-place cell marking ('#').
// - Intuition: In-place modification saves O(M*N) visited memory allocations and simplifies backtracking.
// - Complexity: Time: O(M * N * 4^L), Space: O(L) recursion depth where L is word length.


// ============================================================================
// SECTION 5: STACKS AND QUEUES (Problems 29 - 34)
// ============================================================================

// ----------------------------------------------------------------------------
// 29. Next Greater Element (LeetCode 496 / 503)
// ----------------------------------------------------------------------------
vector<int> nextGreaterElementsCircular(vector<int>& nums) {
    int n = nums.size();
    vector<int> nge(n, -1);
    stack<int> st;

    for (int i = 2 * n - 1; i >= 0; i--) {
        while (!st.empty() && st.top() <= nums[i % n]) {
            st.pop();
        }
        if (i < n) {
            if (!st.empty()) nge[i] = st.top();
        }
        st.push(nums[i % n]);
    }
    return nge;
}
// Interview Explanation:
// - Problem Statement: Find the next strictly greater element for every item in a circular array.
// - Approach: Monotonic decreasing stack traversed from right to left over 2*N elements.
// - Intuition: Stack retains elements in increasing order from top to bottom; popping smaller elements exposes the immediate greater element.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 30. Trapping Rainwater (LeetCode 42)
// ----------------------------------------------------------------------------
int trap(vector<int>& height) {
    int n = height.size();
    if (n == 0) return 0;
    int left = 0, right = n - 1;
    int leftMax = 0, rightMax = 0;
    int trappedWater = 0;

    while (left <= right) {
        if (height[left] <= height[right]) {
            if (height[left] >= leftMax) {
                leftMax = height[left];
            } else {
                trappedWater += leftMax - height[left];
            }
            left++;
        } else {
            if (height[right] >= rightMax) {
                rightMax = height[right];
            } else {
                trappedWater += rightMax - height[right];
            }
            right--;
        }
    }
    return trappedWater;
}
// Interview Explanation:
// - Problem Statement: Compute how much water elevation map can trap after raining.
// - Approach: Two pointers tracking `leftMax` and `rightMax`.
// - Intuition: Water trapped at index i is determined by `min(leftMax, rightMax) - height[i]`. If `height[left] <= height[right]`, `leftMax` is the bottleneck regardless of unknown heights in between.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 31. Largest Rectangle in a Histogram (LeetCode 84)
// ----------------------------------------------------------------------------
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    stack<int> st;
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        int currentHeight = (i == n) ? 0 : heights[i];
        while (!st.empty() && currentHeight < heights[st.top()]) {
            int h = heights[st.top()];
            st.pop();
            int w = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    return maxArea;
}
// Interview Explanation:
// - Problem Statement: Find the area of the largest rectangle in histogram bars.
// - Approach: Single pass with monotonic increasing stack storing bar indices. Append dummy height 0 at end to clear stack.
// - Intuition: When a bar is popped, the current index is its Right Smaller Element, and the new stack top is its Left Smaller Element.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 32. Asteroid Collision (LeetCode 735)
// ----------------------------------------------------------------------------
vector<int> asteroidCollision(vector<int>& asteroids) {
    vector<int> st;
    for (int ast : asteroids) {
        bool destroyed = false;
        while (!st.empty() && ast < 0 && st.back() > 0) {
            if (st.back() < -ast) {
                st.pop_back(); // positive asteroid exploded
                continue;
            } else if (st.back() == -ast) {
                st.pop_back(); // both explode
            }
            destroyed = true;
            break;
        }
        if (!destroyed) {
            st.push_back(ast);
        }
    }
    return st;
}
// Interview Explanation:
// - Problem Statement: Simulate asteroid collisions given array of sizes (sign indicates direction: + right, - left).
// - Approach: Stack / Vector simulation checking collision condition `st.back() > 0 && ast < 0`.
// - Intuition: Only right-moving asteroids preceding a left-moving asteroid can collide.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 33. Sliding Window Maximum (LeetCode 239)
// ----------------------------------------------------------------------------
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // stores indices in monotonically decreasing value order
    vector<int> result;

    for (int i = 0; i < (int)nums.size(); i++) {
        // Remove indices out of current window [i - k + 1, i]
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }

        // Maintain monotonic decreasing order of values
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }

        dq.push_back(i);

        // Window of size >= k reached
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Return maximum element in each sliding window of size k moving from left to right.
// - Approach: Monotonic Decreasing Deque storing array indices.
// - Intuition: Deque front always holds the maximum of the current window. Smaller elements before current index can never be max in any future window containing current element.
// - Complexity: Time: O(N), Space: O(k).


// ----------------------------------------------------------------------------
// 34. LRU Cache (LeetCode 146)
// ----------------------------------------------------------------------------
class LRUCache {
private:
    struct Node {
        int key;
        int val;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;

    void addNode(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node* node) {
        Node* p = node->prev;
        Node* n = node->next;
        p->next = n;
        n->prev = p;
    }

    void moveToHead(Node* node) {
        removeNode(node);
        addNode(node);
    }

    Node* popTail() {
        Node* res = tail->prev;
        removeNode(res);
        return res;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        Node* node = cache[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            Node* newNode = new Node(key, value);
            cache[key] = newNode;
            addNode(newNode);
            if ((int)cache.size() > capacity) {
                Node* lru = popTail();
                cache.erase(lru->key);
                delete lru;
            }
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design Least Recently Used (LRU) Cache supporting get(key) and put(key, val) in O(1) time.
// - Approach: Hash Map + Doubly Linked List with dummy head and tail.
// - Intuition: Hash map provides O(1) key lookups; doubly linked list provides O(1) node detachment and insertion at head.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).

// ============================================================================
// [PART 3 / 5]
// SECTION 6: HEAPS (Problems 35 - 37)
// ============================================================================

// ----------------------------------------------------------------------------
// 35. K-th Largest Element in an Array (LeetCode 215)
// ----------------------------------------------------------------------------
int findKthLargestHeap(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int num : nums) {
        minHeap.push(num);
        if ((int)minHeap.size() > k) {
            minHeap.pop();
        }
    }
    return minHeap.top();
}

// Quickselect Algorithm O(N) average time
int quickSelect(vector<int>& nums, int left, int right, int kSmallest) {
    if (left == right) return nums[left];
    int pivot = nums[left + (right - left) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (nums[i] < pivot) i++;
        while (nums[j] > pivot) j--;
        if (i <= j) {
            swap(nums[i], nums[j]);
            i++;
            j--;
        }
    }
    if (kSmallest <= j) return quickSelect(nums, left, j, kSmallest);
    if (kSmallest >= i) return quickSelect(nums, i, right, kSmallest);
    return nums[kSmallest];
}

int findKthLargest(vector<int>& nums, int k) {
    int n = nums.size();
    return quickSelect(nums, 0, n - 1, n - k);
}
// Interview Explanation:
// - Problem Statement: Find the k-th largest element in an unsorted array.
// - Approach: Min-Heap of size k or Quickselect (Hoare's selection algorithm).
// - Intuition: Min-heap evicts smallest elements, leaving top k largest where the root is the k-th largest.
// - Complexity: Heap: Time O(N log K), Space O(K). Quickselect: Time O(N) average, Space O(1).


// ----------------------------------------------------------------------------
// 36. Task Scheduler (LeetCode 621)
// ----------------------------------------------------------------------------
int leastInterval(vector<char>& tasks, int n) {
    vector<int> freq(26, 0);
    for (char t : tasks) {
        freq[t - 'A']++;
    }
    int maxFreq = *max_element(freq.begin(), freq.end());
    int maxCount = 0;
    for (int f : freq) {
        if (f == maxFreq) maxCount++;
    }

    int partCount = maxFreq - 1;
    int partLength = n - (maxCount - 1);
    int emptySlots = partCount * partLength;
    int availableTasks = (int)tasks.size() - maxFreq * maxCount;
    int idles = max(0, emptySlots - availableTasks);

    return (int)tasks.size() + idles;
}
// Interview Explanation:
// - Problem Statement: Given character array of tasks and cooldown n between identical tasks, find minimum CPU intervals required.
// - Approach: Greedy Math formula using most frequent task count.
// - Intuition: The task with maximum frequency dictates the minimum frame structure `(maxFreq - 1) * (n + 1) + maxCount`. Remaining tasks fill empty cooling slots.
// - Complexity: Time: O(N), Space: O(1) (26 uppercase letters).


// ----------------------------------------------------------------------------
// 37. Implement Min Heap
// ----------------------------------------------------------------------------
class MinHeap {
private:
    vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)] > heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < (int)heap.size() && heap[left] < heap[smallest])
            smallest = left;
        if (right < (int)heap.size() && heap[right] < heap[smallest])
            smallest = right;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap() {}

    void push(int val) {
        heap.push_back(val);
        heapifyUp((int)heap.size() - 1);
    }

    int top() {
        if (heap.empty()) throw runtime_error("Heap is empty");
        return heap[0];
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    int size() const { return heap.size(); }
    bool empty() const { return heap.empty(); }
};
// Interview Explanation:
// - Problem Statement: Implement a complete Min Heap data structure supporting push, pop, top, and size in O(log N).
// - Approach: Array-based complete binary tree using `heapifyUp` for insertion and `heapifyDown` for removal.
// - Intuition: Array indices map directly to tree hierarchy: child of i at 2i+1, 2i+2; parent at (i-1)/2.
// - Complexity: Push: O(log N), Pop: O(log N), Top: O(1), Space: O(N).


// ============================================================================
// SECTION 7: TREES (BINARY TREES & BST) (Problems 38 - 48)
// ============================================================================

// ----------------------------------------------------------------------------
// 38. Diameter of Binary Tree (LeetCode 543)
// ----------------------------------------------------------------------------
int calculateDiameter(TreeNode* root, int& maxDiameter) {
    if (!root) return 0;
    int leftHeight = calculateDiameter(root->left, maxDiameter);
    int rightHeight = calculateDiameter(root->right, maxDiameter);
    maxDiameter = max(maxDiameter, leftHeight + rightHeight);
    return 1 + max(leftHeight, rightHeight);
}

int diameterOfBinaryTree(TreeNode* root) {
    int maxDiameter = 0;
    calculateDiameter(root, maxDiameter);
    return maxDiameter;
}
// Interview Explanation:
// - Problem Statement: Find length of longest path between any two nodes in binary tree (measured in number of edges).
// - Approach: Post-order DFS computing tree height while updating max path through current node (`leftHeight + rightHeight`).
// - Intuition: Longest path through root node is sum of left and right subtree heights.
// - Complexity: Time: O(N), Space: O(H) where H is tree height.


// ----------------------------------------------------------------------------
// 39. Maximum Path Sum in Binary Tree (LeetCode 124)
// ----------------------------------------------------------------------------
int maxPathSumHelper(TreeNode* root, int& globalMax) {
    if (!root) return 0;
    int leftGain = max(0, maxPathSumHelper(root->left, globalMax));
    int rightGain = max(0, maxPathSumHelper(root->right, globalMax));

    // Path turning at current node
    int currentPathSum = root->val + leftGain + rightGain;
    globalMax = max(globalMax, currentPathSum);

    // Return max gain extending to parent
    return root->val + max(leftGain, rightGain);
}

int maxPathSum(TreeNode* root) {
    int globalMax = INT_MIN;
    maxPathSumHelper(root, globalMax);
    return globalMax;
}
// Interview Explanation:
// - Problem Statement: Find maximum path sum of any non-empty path in a binary tree.
// - Approach: DFS returning single branch contribution, ignoring negative subtree gains (`max(0, gain)`).
// - Intuition: At each node, compute potential path combining left, current, and right node, while only returning single strongest branch to parent.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 40. Bottom View of Binary Tree (GFG)
// ----------------------------------------------------------------------------
vector<int> bottomView(TreeNode* root) {
    vector<int> ans;
    if (!root) return ans;
    map<int, int> bottomNodes; // line coordinate -> node value
    queue<pair<TreeNode*, int>> q; // {node, line coordinate}
    q.push({root, 0});

    while (!q.empty()) {
        auto [curr, line] = q.front();
        q.pop();

        bottomNodes[line] = curr->val; // overwrite ensures bottom-most node survives

        if (curr->left) q.push({curr->left, line - 1});
        if (curr->right) q.push({curr->right, line + 1});
    }

    for (auto& [line, val] : bottomNodes) {
        ans.push_back(val);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return bottom view of binary tree seen from bottom (nodes visible at each vertical column).
// - Approach: BFS Level Order Traversal with horizontal distance coordinate tracking.
// - Intuition: Later levels in BFS naturally overwrite earlier levels at same horizontal column line.
// - Complexity: Time: O(N log N) with map or O(N) with range tracking, Space: O(N).


// ----------------------------------------------------------------------------
// 41. Lowest Common Ancestor in Binary Tree (LeetCode 236)
// ----------------------------------------------------------------------------
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    if (left && right) return root; // p and q found in different subtrees
    return left ? left : right;
}
// Interview Explanation:
// - Problem Statement: Find lowest common ancestor (LCA) of two given nodes p and q in binary tree.
// - Approach: Post-order DFS.
// - Intuition: If current root equals p or q, return root. If both left and right recursive calls return non-null, current node is the LCA.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 42. Minimum Time Taken to Burn the Binary Tree from a Given Node
// ----------------------------------------------------------------------------
TreeNode* mapParentsAndFindTarget(TreeNode* root, unordered_map<TreeNode*, TreeNode*>& parentMap, int targetVal) {
    queue<TreeNode*> q;
    q.push(root);
    TreeNode* targetNode = nullptr;

    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        if (curr->val == targetVal) targetNode = curr;
        if (curr->left) {
            parentMap[curr->left] = curr;
            q.push(curr->left);
        }
        if (curr->right) {
            parentMap[curr->right] = curr;
            q.push(curr->right);
        }
    }
    return targetNode;
}

int minTimeToBurnTree(TreeNode* root, int start) {
    if (!root) return 0;
    unordered_map<TreeNode*, TreeNode*> parentMap;
    TreeNode* target = mapParentsAndFindTarget(root, parentMap, start);
    if (!target) return 0;

    unordered_set<TreeNode*> visited;
    queue<TreeNode*> q;
    q.push(target);
    visited.insert(target);
    int time = 0;

    while (!q.empty()) {
        int size = q.size();
        bool burnedNext = false;

        for (int i = 0; i < size; i++) {
            TreeNode* curr = q.front();
            q.pop();

            if (curr->left && visited.find(curr->left) == visited.end()) {
                visited.insert(curr->left);
                q.push(curr->left);
                burnedNext = true;
            }
            if (curr->right && visited.find(curr->right) == visited.end()) {
                visited.insert(curr->right);
                q.push(curr->right);
                burnedNext = true;
            }
            if (parentMap.find(curr) != parentMap.end() && visited.find(parentMap[curr]) == visited.end()) {
                visited.insert(parentMap[curr]);
                q.push(parentMap[curr]);
                burnedNext = true;
            }
        }
        if (burnedNext) time++;
    }
    return time;
}
// Interview Explanation:
// - Problem Statement: Find time required to burn entire binary tree starting from given target node (fire spreads to left, right, parent in 1s).
// - Approach: BFS to build parent pointers + Multi-directional BFS from target node.
// - Intuition: Convert tree into undirected graph via parent map, then run level-order radial fire spread.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 43. Construct a Binary Tree from Preorder and Inorder (LeetCode 105)
// ----------------------------------------------------------------------------
TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd,
                          const vector<int>& inorder, int inStart, int inEnd,
                          unordered_map<int, int>& inMap) {
    if (preStart > preEnd || inStart > inEnd) return nullptr;

    TreeNode* root = new TreeNode(preorder[preStart]);
    int inRoot = inMap[root->val];
    int numsLeft = inRoot - inStart;

    root->left = buildTreeHelper(preorder, preStart + 1, preStart + numsLeft,
                                 inorder, inStart, inRoot - 1, inMap);
    root->right = buildTreeHelper(preorder, preStart + numsLeft + 1, preEnd,
                                  inorder, inRoot + 1, inEnd, inMap);
    return root;
}

TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int, int> inMap;
    for (int i = 0; i < (int)inorder.size(); i++) {
        inMap[inorder[i]] = i;
    }
    return buildTreeHelper(preorder, 0, preorder.size() - 1,
                           inorder, 0, inorder.size() - 1, inMap);
}
// Interview Explanation:
// - Problem Statement: Construct binary tree from preorder and inorder traversal arrays.
// - Approach: Divide and conquer with hash map for O(1) inorder index lookup.
// - Intuition: First element of preorder is the root; its position in inorder splits tree into left and right subtrees.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 44. Morris Preorder Traversal of a Binary Tree
// ----------------------------------------------------------------------------
vector<int> morrisPreorderTraversal(TreeNode* root) {
    vector<int> preorder;
    TreeNode* curr = root;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            preorder.push_back(curr->val);
            curr = curr->right;
        } else {
            TreeNode* prev = curr->left;
            while (prev->right != nullptr && prev->right != curr) {
                prev = prev->right;
            }

            if (prev->right == nullptr) {
                // Establish thread
                prev->right = curr;
                preorder.push_back(curr->val); // visit root before traversing left
                curr = curr->left;
            } else {
                // Break thread
                prev->right = nullptr;
                curr = curr->right;
            }
        }
    }
    return preorder;
}
// Interview Explanation:
// - Problem Statement: Perform preorder traversal of binary tree in O(1) auxiliary space without recursion or stack.
// - Approach: Morris Traversal using temporary threaded binary tree pointers (inorder predecessor's right points to current node).
// - Intuition: Connect rightmost leaf of left subtree to current node to retain return path without stack frames.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 45. Delete a Node in BST (LeetCode 450)
// ----------------------------------------------------------------------------
TreeNode* findMinBST(TreeNode* node) {
    while (node->left) node = node->left;
    return node;
}

TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node with 0 or 1 child
        if (!root->left) {
            TreeNode* rightChild = root->right;
            delete root;
            return rightChild;
        } else if (!root->right) {
            TreeNode* leftChild = root->left;
            delete root;
            return leftChild;
        }
        // Node with 2 children: replace with inorder successor (min of right subtree)
        TreeNode* successor = findMinBST(root->right);
        root->val = successor->val;
        root->right = deleteNode(root->right, successor->val);
    }
    return root;
}
// Interview Explanation:
// - Problem Statement: Delete a node with given key in a Binary Search Tree while maintaining BST property.
// - Approach: Recursive search; for two children, replace with inorder successor and recursively delete successor.
// - Intuition: Inorder successor is the smallest value in right subtree, which preserves binary search invariants when swapped.
// - Complexity: Time: O(H), Space: O(H).


// ----------------------------------------------------------------------------
// 46. Lowest Common Ancestor in BST (LeetCode 235)
// ----------------------------------------------------------------------------
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val) {
            root = root->left;
        } else if (p->val > root->val && q->val > root->val) {
            root = root->right;
        } else {
            return root; // Split point is the LCA
        }
    }
    return nullptr;
}
// Interview Explanation:
// - Problem Statement: Find LCA of two nodes in a Binary Search Tree.
// - Approach: Iterative tree descent using BST ordering property.
// - Intuition: The first node where p and q diverge to different subtrees (or one matches root) is the unique LCA.
// - Complexity: Time: O(H), Space: O(1).


// ----------------------------------------------------------------------------
// 47. Two Sum In BST | Pair with Sum K (LeetCode 653)
// ----------------------------------------------------------------------------
class BSTIterator {
private:
    stack<TreeNode*> st;
    bool reverse; // false -> inorder (next), true -> reverse inorder (before)

    void pushAll(TreeNode* node) {
        while (node) {
            st.push(node);
            node = reverse ? node->right : node->left;
        }
    }

public:
    BSTIterator(TreeNode* root, bool isReverse) : reverse(isReverse) {
        pushAll(root);
    }

    int next() {
        TreeNode* curr = st.top();
        st.pop();
        if (!reverse) pushAll(curr->right);
        else pushAll(curr->left);
        return curr->val;
    }

    bool hasNext() {
        return !st.empty();
    }
};

bool findTarget(TreeNode* root, int k) {
    if (!root) return false;
    BSTIterator l(root, false); // produces smallest elements
    BSTIterator r(root, true);  // produces largest elements

    int i = l.next();
    int j = r.next();

    while (i < j) {
        if (i + j == k) return true;
        else if (i + j < k) {
            if (l.hasNext()) i = l.next();
            else break;
        } else {
            if (r.hasNext()) j = r.next();
            else break;
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if there exist two nodes in BST whose sum equals k.
// - Approach: Dual BST Iterators (forward and reverse) simulating two-pointer sweep in O(H) space.
// - Intuition: Forward iterator gives sorted ascending values, reverse gives descending values, enabling two pointers without flattening tree.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 48. Largest BST in Binary Tree (GFG / LeetCode 333)
// ----------------------------------------------------------------------------
struct NodeInfo {
    int minVal;
    int maxVal;
    int maxSize;
    bool isBST;
};

NodeInfo largestBSTHelper(TreeNode* root) {
    if (!root) {
        return {INT_MAX, INT_MIN, 0, true};
    }

    NodeInfo left = largestBSTHelper(root->left);
    NodeInfo right = largestBSTHelper(root->right);

    if (left.isBST && right.isBST && left.maxVal < root->val && root->val < right.minVal) {
        return {
            min(root->val, left.minVal),
            max(root->val, right.maxVal),
            1 + left.maxSize + right.maxSize,
            true
        };
    }

    return {0, 0, max(left.maxSize, right.maxSize), false};
}

int largestBST(TreeNode* root) {
    return largestBSTHelper(root).maxSize;
}
// Interview Explanation:
// - Problem Statement: Find the size (number of nodes) of the largest subtree which is a valid BST.
// - Approach: Bottom-up Post-order DFS returning `{minVal, maxVal, maxSize, isBST}` for every subtree.
// - Intuition: Subtree is BST if left and right subtrees are BSTs and `left.max < root->val < right.min`.
// - Complexity: Time: O(N), Space: O(H).

// ============================================================================
// [PART 4 / 5]
// SECTION 8: GRAPHS (Problems 49 - 60)
// ============================================================================

// ----------------------------------------------------------------------------
// Disjoint Set Union (DSU / Union-Find) Utility
// ----------------------------------------------------------------------------
class DisjointSet {
public:
    vector<int> parent, rank, size;
    DisjointSet(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        size.resize(n + 1, 1);
        for (int i = 0; i <= n; i++) parent[i] = i;
    }

    int findUPar(int node) {
        if (node == parent[node]) return node;
        return parent[node] = findUPar(parent[node]); // Path compression
    }

    void unionByRank(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v;
        } else if (rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u;
        } else {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }

    void unionBySize(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};


// ----------------------------------------------------------------------------
// 49. Rotten Oranges (LeetCode 994)
// ----------------------------------------------------------------------------
int orangesRotting(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    queue<pair<int, int>> q;
    int freshCount = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 2) {
                q.push({i, j});
            } else if (grid[i][j] == 1) {
                freshCount++;
            }
        }
    }

    if (freshCount == 0) return 0;
    int minutes = 0;
    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};

    while (!q.empty()) {
        int size = q.size();
        bool rottedAny = false;

        for (int i = 0; i < size; i++) {
            auto [r, c] = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                int nr = r + dRow[d];
                int nc = c + dCol[d];

                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    grid[nr][nc] = 2;
                    freshCount--;
                    q.push({nr, nc});
                    rottedAny = true;
                }
            }
        }
        if (rottedAny) minutes++;
    }

    return (freshCount == 0) ? minutes : -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum minutes until no fresh oranges remain (adjacent fresh oranges rot each minute).
// - Approach: Multi-source Breadth-First Search starting simultaneously from all initially rotten oranges.
// - Intuition: BFS level count corresponds directly to elapsed minutes of simultaneous radial spread.
// - Complexity: Time: O(M * N), Space: O(M * N).


// ----------------------------------------------------------------------------
// 50. Word Ladder I (LeetCode 127)
// ----------------------------------------------------------------------------
int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (dict.find(endWord) == dict.end()) return 0;

    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    dict.erase(beginWord);

    while (!q.empty()) {
        auto [word, steps] = q.front();
        q.pop();

        if (word == endWord) return steps;

        for (int i = 0; i < (int)word.size(); i++) {
            char original = word[i];
            for (char ch = 'a'; ch <= 'z'; ch++) {
                word[i] = ch;
                if (dict.find(word) != dict.end()) {
                    dict.erase(word);
                    q.push({word, steps + 1});
                }
            }
            word[i] = original;
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Find length of shortest transformation sequence from beginWord to endWord where adjacent words differ by 1 letter.
// - Approach: Unweighted Shortest Path BFS on implicit graph generated by mutating characters.
// - Intuition: BFS guarantees first visit to endWord is via the minimal number of transformations. Erasing visited words prevents cycles.
// - Complexity: Time: O(N * L * 26), Space: O(N * L).


// ----------------------------------------------------------------------------
// 51. Number of Islands (LeetCode 200)
// ----------------------------------------------------------------------------
void islandBFS(int startR, int startC, vector<vector<char>>& grid) {
    int m = grid.size(), n = grid[0].size();
    queue<pair<int, int>> q;
    grid[startR][startC] = '0'; // mark visited
    q.push({startR, startC});

    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int d = 0; d < 4; d++) {
            int nr = r + dRow[d];
            int nc = c + dCol[d];

            if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == '1') {
                grid[nr][nc] = '0';
                q.push({nr, nc});
            }
        }
    }
}

int numIslands(vector<vector<char>>& grid) {
    int m = grid.size(), n = grid[0].size();
    int count = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                count++;
                islandBFS(i, j, grid);
            }
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count the number of islands in a 2D binary grid ('1' land, '0' water).
// - Approach: Connected Components BFS / DFS traversal sinking visited land cells in-place to '0'.
// - Intuition: Each unvisited '1' triggers a BFS flooding all reachable land in that island component.
// - Complexity: Time: O(M * N), Space: O(min(M, N)) queue size.


// ----------------------------------------------------------------------------
// 52. Course Schedule II (LeetCode 210)
// ----------------------------------------------------------------------------
vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);

    for (auto& edge : prerequisites) {
        int u = edge[1], v = edge[0]; // u -> v
        adj[u].push_back(v);
        inDegree[v]++;
    }

    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    vector<int> topoOrder;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        topoOrder.push_back(node);

        for (int neighbor : adj[node]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if ((int)topoOrder.size() == numCourses) {
        return topoOrder;
    }
    return {}; // Cycle detected, impossible to finish all courses
}
// Interview Explanation:
// - Problem Statement: Return ordering of courses you should take to finish all courses given prerequisite pairs.
// - Approach: Kahn's Algorithm for Topological Sort using In-degree reduction.
// - Intuition: Nodes with in-degree 0 have all prerequisites met. Processing them unlocks downstream dependent courses.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ----------------------------------------------------------------------------
// 53. Alien Dictionary (LeetCode 269 / GFG)
// ----------------------------------------------------------------------------
string findAlienOrder(vector<string>& words, int k) {
    vector<vector<int>> adj(k);
    vector<int> inDegree(k, 0);

    for (size_t i = 0; i < words.size() - 1; i++) {
        const string& w1 = words[i];
        const string& w2 = words[i + 1];
        int len = min(w1.size(), w2.size());
        bool foundDiff = false;

        for (int j = 0; j < len; j++) {
            if (w1[j] != w2[j]) {
                adj[w1[j] - 'a'].push_back(w2[j] - 'a');
                inDegree[w2[j] - 'a']++;
                foundDiff = true;
                break;
            }
        }
        // Invalid prefix order check: e.g. "abc" before "ab"
        if (!foundDiff && w1.size() > w2.size()) return "";
    }

    queue<int> q;
    for (int i = 0; i < k; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    string order = "";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order += (char)(u + 'a');

        for (int v : adj[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
        }
    }

    return ((int)order.size() == k) ? order : "";
}
// Interview Explanation:
// - Problem Statement: Determine unique character order in an alien language given sorted dictionary of words.
// - Approach: Compare adjacent words to build directed graph of character precedence + Kahn's Topo Sort.
// - Intuition: The first mismatch between adjacent sorted words defines a directed precedence edge `c1 -> c2`.
// - Complexity: Time: O(N * L + K), Space: O(K + E).


// ----------------------------------------------------------------------------
// 54. Dijkstra's Algorithm (Single Source Shortest Path)
// ----------------------------------------------------------------------------
vector<int> dijkstra(int V, const vector<vector<pair<int, int>>>& adj, int src) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(V, 1e9);

    dist[src] = 0;
    pq.push({0, src}); // {distance, node}

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue; // Stale heap entry

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Find shortest path from source node to all other nodes in a graph with non-negative edge weights.
// - Approach: Min-Heap priority queue greedily settling the closest unvisited vertex.
// - Intuition: With non-negative edges, once a vertex is extracted with minimal distance from priority queue, its shortest distance is finalized.
// - Complexity: Time: O((V + E) log V), Space: O(V + E).


// ----------------------------------------------------------------------------
// 55. Cheapest Flights Within K Stops (LeetCode 787)
// ----------------------------------------------------------------------------
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<vector<pair<int, int>>> adj(n);
    for (auto& f : flights) {
        adj[f[0]].push_back({f[1], f[2]});
    }

    // Queue stores {stops, {node, cost}}
    queue<pair<int, pair<int, int>>> q;
    q.push({0, {src, 0}});
    vector<int> minCost(n, 1e9);
    minCost[src] = 0;

    while (!q.empty()) {
        auto [stops, nodeCost] = q.front();
        q.pop();
        auto [u, cost] = nodeCost;

        if (stops > k) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int price = edge.second;

            if (cost + price < minCost[v]) {
                minCost[v] = cost + price;
                q.push({stops + 1, {v, cost + price}});
            }
        }
    }

    return (minCost[dst] == 1e9) ? -1 : minCost[dst];
}
// Interview Explanation:
// - Problem Statement: Find cheapest flight from src to dst with at most k intermediate stops.
// - Approach: Modified BFS / SPFA ordering by stops taken.
// - Intuition: Since stops increase monotonically by 1 per step, standard queue naturally processes flight levels without needing priority queue by cost.
// - Complexity: Time: O(K * E), Space: O(V + E).


// ----------------------------------------------------------------------------
// 56. Bellman Ford Algorithm
// ----------------------------------------------------------------------------
vector<int> bellmanFord(int V, const vector<vector<int>>& edges, int src) {
    vector<int> dist(V, 1e8);
    dist[src] = 0;

    // Relax all edges V - 1 times
    for (int i = 0; i < V - 1; i++) {
        for (auto& edge : edges) {
            int u = edge[0], v = edge[1], wt = edge[2];
            if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }

    // Nth relaxation to detect negative weight cycles
    for (auto& edge : edges) {
        int u = edge[0], v = edge[1], wt = edge[2];
        if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
            return {-1}; // Negative cycle detected
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Find single-source shortest paths in graphs with negative edge weights and detect negative weight cycles.
// - Approach: Dynamic Programming edge relaxation V - 1 times.
// - Intuition: A shortest simple path has at most V - 1 edges. If dist can still be reduced on the V-th iteration, a negative cycle exists.
// - Complexity: Time: O(V * E), Space: O(V).


// ----------------------------------------------------------------------------
// 57. Floyd Warshall Algorithm
// ----------------------------------------------------------------------------
void floydWarshall(vector<vector<int>>& matrix) {
    int n = matrix.size();
    const int INF = 1e9;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == -1) matrix[i][j] = INF;
            if (i == j) matrix[i][j] = 0;
        }
    }

    // k is intermediate pivot vertex
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][k] != INF && matrix[k][j] != INF) {
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == INF) matrix[i][j] = -1;
        }
    }
}
// Interview Explanation:
// - Problem Statement: Find shortest paths between all pairs of vertices in a directed weighted graph.
// - Approach: 3-nested DP loops considering every vertex k as intermediate transit vertex: `dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j])`.
// - Intuition: Incrementally expands allowed intermediate transit nodes from {0} up to {0..N-1}.
// - Complexity: Time: O(V^3), Space: O(1) auxiliary space (modifies matrix in-place).


// ----------------------------------------------------------------------------
// 58. Find MST Weight (Prim's & Kruskal's Algorithm)
// ----------------------------------------------------------------------------
int spanningTreePrims(int V, const vector<vector<pair<int, int>>>& adj) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<bool> visited(V, false);
    pq.push({0, 0}); // {weight, node}
    int mstSum = 0;

    while (!pq.empty()) {
        auto [wt, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;
        mstSum += wt;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int edgeWt = edge.second;
            if (!visited[v]) {
                pq.push({edgeWt, v});
            }
        }
    }
    return mstSum;
}

int spanningTreeKruskals(int V, vector<vector<int>>& edges) {
    // edges: {wt, u, v}
    sort(edges.begin(), edges.end());
    DisjointSet ds(V);
    int mstSum = 0;

    for (auto& edge : edges) {
        int wt = edge[0], u = edge[1], v = edge[2];
        if (ds.findUPar(u) != ds.findUPar(v)) {
            mstSum += wt;
            ds.unionBySize(u, v);
        }
    }
    return mstSum;
}
// Interview Explanation:
// - Problem Statement: Find the sum of weights of edges in a Minimum Spanning Tree (MST).
// - Approach: Prim's algorithm (greedy vertex cut with min-heap) or Kruskal's algorithm (greedy edge addition with DSU).
// - Intuition: Prim grows a single tree from source node; Kruskal joins disjoint forest components without forming cycles.
// - Complexity: Prim: O(E log V), Kruskal: O(E log E), Space: O(V + E).


// ----------------------------------------------------------------------------
// 59. Accounts Merge (LeetCode 721)
// ----------------------------------------------------------------------------
vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    DisjointSet ds(n);
    unordered_map<string, int> emailToAcc;

    for (int i = 0; i < n; i++) {
        for (size_t j = 1; j < accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (emailToAcc.find(email) == emailToAcc.end()) {
                emailToAcc[email] = i;
            } else {
                ds.unionBySize(i, emailToAcc[email]);
            }
        }
    }

    unordered_map<int, vector<string>> mergedEmails;
    for (auto& [email, accIdx] : emailToAcc) {
        int root = ds.findUPar(accIdx);
        mergedEmails[root].push_back(email);
    }

    vector<vector<string>> result;
    for (auto& [root, emails] : mergedEmails) {
        sort(emails.begin(), emails.end());
        vector<string> accountRow;
        accountRow.push_back(accounts[root][0]); // Name
        accountRow.insert(accountRow.end(), emails.begin(), emails.end());
        result.push_back(accountRow);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Merge user accounts sharing at least one common email address.
// - Approach: Disjoint Set Union over account indices mapped from email strings.
// - Intuition: Treat accounts as nodes in a graph and shared emails as connecting edges, grouping connected components via DSU.
// - Complexity: Time: O(N * K log(N * K)), Space: O(N * K).


// ----------------------------------------------------------------------------
// 60. Bridges in Graph / Critical Connections (LeetCode 1192)
// ----------------------------------------------------------------------------
void tarjanBridgeDFS(int node, int parent, int& timer, vector<int>& tin, vector<int>& low,
                     vector<bool>& visited, const vector<vector<int>>& adj, vector<vector<int>>& bridges) {
    visited[node] = true;
    tin[node] = low[node] = timer++;

    for (int neighbor : adj[node]) {
        if (neighbor == parent) continue;

        if (!visited[neighbor]) {
            tarjanBridgeDFS(neighbor, node, timer, tin, low, visited, adj, bridges);
            low[node] = min(low[node], low[neighbor]);

            // Bridge condition: neighbor cannot reach node or any ancestor of node
            if (low[neighbor] > tin[node]) {
                bridges.push_back({node, neighbor});
            }
        } else {
            // Back-edge
            low[node] = min(low[node], tin[neighbor]);
        }
    }
}

vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
    vector<vector<int>> adj(n);
    for (auto& edge : connections) {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    vector<int> tin(n, -1), low(n, -1);
    vector<bool> visited(n, false);
    vector<vector<int>> bridges;
    int timer = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            tarjanBridgeDFS(i, -1, timer, tin, low, visited, adj, bridges);
        }
    }
    return bridges;
}
// Interview Explanation:
// - Problem Statement: Find all critical connections (bridges) in an undirected network whose removal disconnects the graph.
// - Approach: Tarjan's Algorithm tracking DFS entry time `tin` and lowest reachable ancestor time `low`.
// - Intuition: An edge `(u, v)` is a bridge if `low[v] > tin[u]`, meaning vertex v has no back-edge to u or ancestors of u.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ============================================================================
// SECTION 9: DYNAMIC PROGRAMMING (Part A: Problems 61 - 64)
// ============================================================================

// ----------------------------------------------------------------------------
// 61. Maximum Sum of Non-Adjacent Elements / House Robber (LeetCode 198)
// ----------------------------------------------------------------------------
int rob(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    if (n == 1) return nums[0];

    int prev2 = nums[0];
    int prev1 = max(nums[0], nums[1]);

    for (int i = 2; i < n; i++) {
        int curr = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Determine maximum amount of money you can rob without robbing two adjacent houses.
// - Approach: 1D DP with space optimization tracking `prev1` (rob up to i-1) and `prev2` (rob up to i-2).
// - Intuition: At house i, choices are: skip house i (`prev1`) or rob house i (`prev2 + nums[i]`).
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 62. Ninja's Training (2D DP)
// ----------------------------------------------------------------------------
int ninjaTraining(int n, vector<vector<int>>& points) {
    // dp[lastTask] storing max points
    vector<int> prev(4, 0);

    prev[0] = max(points[0][1], points[0][2]);
    prev[1] = max(points[0][0], points[0][2]);
    prev[2] = max(points[0][0], points[0][1]);
    prev[3] = max({points[0][0], points[0][1], points[0][2]});

    for (int day = 1; day < n; day++) {
        vector<int> curr(4, 0);
        for (int last = 0; last < 4; last++) {
            curr[last] = 0;
            for (int task = 0; task < 3; task++) {
                if (task != last) {
                    curr[last] = max(curr[last], points[day][task] + prev[task]);
                }
            }
        }
        prev = curr;
    }
    return prev[3];
}
// Interview Explanation:
// - Problem Statement: Ninja trains for N days choosing 1 of 3 activities daily. Cannot perform same activity on consecutive days. Maximize points.
// - Approach: 2D DP tabulation with previous day state space optimization.
// - Intuition: Transition `dp[day][lastTask] = max_{task != lastTask}(points[day][task] + dp[day-1][task])`.
// - Complexity: Time: O(N * 4 * 3) = O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 63. Minimum Path Sum in Grid (LeetCode 64)
// ----------------------------------------------------------------------------
int minPathSum(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    vector<int> prev(n, 0);

    for (int i = 0; i < m; i++) {
        vector<int> curr(n, 0);
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) {
                curr[j] = grid[0][0];
            } else {
                int up = (i > 0) ? prev[j] : 1e9;
                int left = (j > 0) ? curr[j - 1] : 1e9;
                curr[j] = grid[i][j] + min(up, left);
            }
        }
        prev = curr;
    }
    return prev[n - 1];
}
// Interview Explanation:
// - Problem Statement: Find path from top-left to bottom-right of grid minimizing sum of numbers along path (only move right or down).
// - Approach: 2D Grid DP with 1D rolling row optimization.
// - Intuition: Cell `(i, j)` is reachable only from `(i-1, j)` (up) or `(i, j-1)` (left).
// - Complexity: Time: O(M * N), Space: O(N).


// ----------------------------------------------------------------------------
// 64. Subset Sum Equal to Target (0/1 Knapsack DP)
// ----------------------------------------------------------------------------
bool subsetSumToK(int n, int k, const vector<int>& arr) {
    vector<bool> prev(k + 1, false);
    prev[0] = true;

    if (arr[0] <= k) prev[arr[0]] = true;

    for (int ind = 1; ind < n; ind++) {
        vector<bool> curr(k + 1, false);
        curr[0] = true;
        for (int target = 1; target <= k; target++) {
            bool notTake = prev[target];
            bool take = false;
            if (arr[ind] <= target) {
                take = prev[target - arr[ind]];
            }
            curr[target] = notTake || take;
        }
        prev = curr;
    }
    return prev[k];
}
// Interview Explanation:
// - Problem Statement: Determine if there exists a subset of array whose sum equals given target k.
// - Approach: 0/1 Knapsack DP with boolean array space optimization.
// - Intuition: Target sum `s` is achievable either by not taking element (`prev[s]`) or taking element (`prev[s - arr[i]]`).
// - Complexity: Time: O(N * K), Space: O(K).

// ============================================================================
// [PART 5 / 5]
// SECTION 9: DYNAMIC PROGRAMMING (Part B: Problems 65 - 72)
// ============================================================================

// ----------------------------------------------------------------------------
// 65. Assign Cookies (LeetCode 455)
// ----------------------------------------------------------------------------
int findContentChildren(vector<int>& g, vector<int>& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());
    int child = 0, cookie = 0;
    while (child < (int)g.size() && cookie < (int)s.size()) {
        if (s[cookie] >= g[child]) {
            child++; // Greedily satisfy child with smallest sufficient cookie
        }
        cookie++;
    }
    return child;
}
// Interview Explanation:
// - Problem Statement: Maximize number of children content given greed factors `g` and cookie sizes `s`.
// - Approach: Greedy Two Pointers on sorted arrays.
// - Intuition: Give the smallest cookie that can satisfy the least greedy child first, preserving larger cookies for greedier children.
// - Complexity: Time: O(N log N + M log M), Space: O(1).


// ----------------------------------------------------------------------------
// 66. Rod Cutting Problem (Unbounded Knapsack DP)
// ----------------------------------------------------------------------------
int cutRod(vector<int>& price, int n) {
    vector<int> prev(n + 1, 0);

    for (int len = 0; len <= n; len++) {
        prev[len] = len * price[0];
    }

    for (int ind = 1; ind < n; ind++) {
        vector<int> curr(n + 1, 0);
        int rodLength = ind + 1;
        for (int len = 0; len <= n; len++) {
            int notTake = prev[len];
            int take = INT_MIN;
            if (rodLength <= len) {
                take = price[ind] + curr[len - rodLength]; // Unbounded choice: stays on curr
            }
            curr[len] = max(notTake, take);
        }
        prev = curr;
    }
    return prev[n];
}
// Interview Explanation:
// - Problem Statement: Given a rod of length n and array of prices for pieces of length 1..n, determine maximum value obtainable by cutting rod.
// - Approach: Unbounded Knapsack DP with rolling array optimization.
// - Intuition: Because rod pieces can be reused multiple times, the 'take' transition references the current row `curr[len - rodLength]`.
// - Complexity: Time: O(N^2), Space: O(N).


// ----------------------------------------------------------------------------
// 67. Longest Common Subsequence (LeetCode 1143)
// ----------------------------------------------------------------------------
int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vector<int> prev(n + 1, 0);

    for (int i = 1; i <= m; i++) {
        vector<int> curr(n + 1, 0);
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                curr[j] = 1 + prev[j - 1];
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        prev = curr;
    }
    return prev[n];
}

string printLongestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    int i = m, j = n;
    string lcs = "";
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs.push_back(text1[i - 1]);
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}
// Interview Explanation:
// - Problem Statement: Find length of longest common subsequence of two strings (and backtrack to construct the string).
// - Approach: 2D DP tabulation with 1D space optimization; table backtracking for reconstruction.
// - Intuition: If characters match, `1 + dp[i-1][j-1]`; else, take `max(dp[i-1][j], dp[i][j-1])`.
// - Complexity: Time: O(M * N), Space: O(N) for length, O(M * N) for string reconstruction.


// ----------------------------------------------------------------------------
// 68. Longest Palindromic Subsequence (LeetCode 516)
// ----------------------------------------------------------------------------
int longestPalindromeSubseq(string s) {
    string revS = s;
    reverse(revS.begin(), revS.end());
    return longestCommonSubsequence(s, revS);
}
// Interview Explanation:
// - Problem Statement: Find length of longest palindromic subsequence in string s.
// - Approach: Compute Longest Common Subsequence between string `s` and its reverse `rev(s)`.
// - Intuition: A palindrome reads identically forward and backward; its longest subsequence common to its reverse is its longest palindromic subsequence.
// - Complexity: Time: O(N^2), Space: O(N).


// ----------------------------------------------------------------------------
// 69. Edit Distance (LeetCode 72)
// ----------------------------------------------------------------------------
int minDistance(string word1, string word2) {
    int m = word1.size(), n = word2.size();
    vector<int> prev(n + 1, 0);

    for (int j = 0; j <= n; j++) prev[j] = j;

    for (int i = 1; i <= m; i++) {
        vector<int> curr(n + 1, 0);
        curr[0] = i;
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                curr[j] = prev[j - 1]; // characters match, no operation
            } else {
                int insertOp = curr[j - 1];
                int deleteOp = prev[j];
                int replaceOp = prev[j - 1];
                curr[j] = 1 + min({insertOp, deleteOp, replaceOp});
            }
        }
        prev = curr;
    }
    return prev[n];
}
// Interview Explanation:
// - Problem Statement: Return minimum number of operations (insert, delete, replace) to convert word1 to word2.
// - Approach: 2D DP with 1D rolling array optimization.
// - Intuition: Transitions correspond to each operation: Insert `(i, j-1)`, Delete `(i-1, j)`, Replace `(i-1, j-1)`.
// - Complexity: Time: O(M * N), Space: O(N).


// ----------------------------------------------------------------------------
// 70. Best Time to Buy and Sell Stock IV (LeetCode 188)
// ----------------------------------------------------------------------------
int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    if (n == 0 || k == 0) return 0;

    // dp[cap][buy]: cap from 1..k, buy in {0, 1}
    vector<vector<int>> after(k + 1, vector<int>(2, 0));
    vector<vector<int>> curr(k + 1, vector<int>(2, 0));

    for (int ind = n - 1; ind >= 0; ind--) {
        for (int cap = 1; cap <= k; cap++) {
            // Can Buy
            curr[cap][1] = max(-prices[ind] + after[cap][0], after[cap][1]);
            // Can Sell
            curr[cap][0] = max(prices[ind] + after[cap - 1][1], after[cap][0]);
        }
        after = curr;
    }
    return after[k][1];
}
// Interview Explanation:
// - Problem Statement: Find maximum profit with at most k buy-and-sell stock transactions.
// - Approach: DP state `dp[day][capacity][buyState]` with next-day array space optimization.
// - Intuition: Buying deducts price from profit; selling adds price to profit and consumes 1 unit of transaction capacity.
// - Complexity: Time: O(N * K), Space: O(K).


// ----------------------------------------------------------------------------
// 71. Longest Increasing Subsequence (LeetCode 300 - O(N log N))
// ----------------------------------------------------------------------------
int lengthOfLIS(vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x; // Replace smallest element >= x to allow smaller subsequent extensions
        }
    }
    return tails.size();
}
// Interview Explanation:
// - Problem Statement: Find length of the longest strictly increasing subsequence in an integer array.
// - Approach: Binary Search (Patience Sorting) maintaining `tails` array where `tails[i]` stores smallest tail of all increasing subsequences of length i+1.
// - Intuition: Greedily keeping tails as small as possible maximizes future opportunities to extend subsequences.
// - Complexity: Time: O(N log N), Space: O(N).


// ----------------------------------------------------------------------------
// 72. Burst Balloons (LeetCode 312)
// ----------------------------------------------------------------------------
int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> arr(n + 2, 1);
    for (int i = 0; i < n; i++) arr[i + 1] = nums[i];

    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));

    for (int i = n; i >= 1; i--) {
        for (int j = i; j <= n; j++) {
            int maxi = INT_MIN;
            for (int k = i; k <= j; k++) {
                int coins = arr[i - 1] * arr[k] * arr[j + 1] + dp[i][k - 1] + dp[k + 1][j];
                maxi = max(maxi, coins);
            }
            dp[i][j] = maxi;
        }
    }
    return dp[1][n];
}
// Interview Explanation:
// - Problem Statement: Burst balloons indexed i to j to maximize coins `nums[left-1] * nums[i] * nums[right+1]`.
// - Approach: Matrix Chain Multiplication / Partition DP thinking in reverse: choose balloon k as the LAST balloon to burst in subarray `[i..j]`.
// - Intuition: Thinking bottom-up in reverse ensures subproblems `[i..k-1]` and `[k+1..j]` are completely independent of each other.
// - Complexity: Time: O(N^3), Space: O(N^2).


// ============================================================================
// SECTION 10: TRIES (Problems 73 - 75)
// ============================================================================

// ----------------------------------------------------------------------------
// 73. Trie Implementation and Advanced Operations (Trie II)
// ----------------------------------------------------------------------------
struct AdvancedTrieNode {
    AdvancedTrieNode* links[26];
    int countPrefix = 0;
    int countEndsWith = 0;

    AdvancedTrieNode() {
        for (int i = 0; i < 26; i++) links[i] = nullptr;
    }

    bool containsKey(char ch) { return links[ch - 'a'] != nullptr; }
    AdvancedTrieNode* get(char ch) { return links[ch - 'a']; }
    void put(char ch, AdvancedTrieNode* node) { links[ch - 'a'] = node; }
};

class AdvancedTrie {
private:
    AdvancedTrieNode* root;

public:
    AdvancedTrie() { root = new AdvancedTrieNode(); }

    void insert(const string& word) {
        AdvancedTrieNode* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                node->put(ch, new AdvancedTrieNode());
            }
            node = node->get(ch);
            node->countPrefix++;
        }
        node->countEndsWith++;
    }

    int countWordsEqualTo(const string& word) {
        AdvancedTrieNode* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) return 0;
            node = node->get(ch);
        }
        return node->countEndsWith;
    }

    int countWordsStartingWith(const string& prefix) {
        AdvancedTrieNode* node = root;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) return 0;
            node = node->get(ch);
        }
        return node->countPrefix;
    }

    void erase(const string& word) {
        AdvancedTrieNode* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) return;
            node = node->get(ch);
            node->countPrefix--;
        }
        node->countEndsWith--;
    }
};
// Interview Explanation:
// - Problem Statement: Implement Trie II with insert, countWordsEqualTo, countWordsStartingWith, and erase in O(L) time.
// - Approach: 26-way Trie node with `countPrefix` and `countEndsWith` counters.
// - Intuition: Increment counters on insertion along character path; decrement on deletion.
// - Complexity: Time: O(L) for all operations, Space: O(N * L * 26).


// ----------------------------------------------------------------------------
// 74. Maximum XOR with an Element from Array (LeetCode 1707)
// ----------------------------------------------------------------------------
struct BitTrieNode {
    BitTrieNode* links[2] = {nullptr, nullptr};
    bool containsKey(int bit) { return links[bit] != nullptr; }
    BitTrieNode* get(int bit) { return links[bit]; }
    void put(int bit, BitTrieNode* node) { links[bit] = node; }
};

class BitTrie {
public:
    BitTrieNode* root;
    BitTrie() { root = new BitTrieNode(); }

    void insert(int num) {
        BitTrieNode* node = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!node->containsKey(bit)) {
                node->put(bit, new BitTrieNode());
            }
            node = node->get(bit);
        }
    }

    int getMaxXor(int num) {
        BitTrieNode* node = root;
        int maxXor = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int opposite = 1 - bit;
            if (node->containsKey(opposite)) {
                maxXor |= (1 << i);
                node = node->get(opposite);
            } else if (node->containsKey(bit)) {
                node = node->get(bit);
            } else {
                return -1;
            }
        }
        return maxXor;
    }
};

vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
    int q = queries.size();
    vector<int> ans(q, -1);
    // Offline query sorting: {m_i, x_i, original_index}
    vector<pair<int, pair<int, int>>> offlineQueries;
    for (int i = 0; i < q; i++) {
        offlineQueries.push_back({queries[i][1], {queries[i][0], i}});
    }

    sort(nums.begin(), nums.end());
    sort(offlineQueries.begin(), offlineQueries.end());

    BitTrie trie;
    int numIdx = 0, n = nums.size();

    for (int i = 0; i < q; i++) {
        int m = offlineQueries[i].first;
        int x = offlineQueries[i].second.first;
        int qIdx = offlineQueries[i].second.second;

        // Insert all numbers <= m into bitwise Trie
        while (numIdx < n && nums[numIdx] <= m) {
            trie.insert(nums[numIdx]);
            numIdx++;
        }

        if (numIdx == 0) {
            ans[qIdx] = -1; // No numbers <= m available
        } else {
            ans[qIdx] = trie.getMaxXor(x);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: For each query `(x_i, m_i)`, find maximum XOR of `x_i` with any array element `<= m_i`.
// - Approach: Offline Query Processing + 32-bit Binary Trie. Sort queries and array by threshold `m_i`.
// - Intuition: Incrementally insert valid array elements `<= m_i` into Trie, then greedily traverse opposite bits for max XOR.
// - Complexity: Time: O(N log N + Q log Q + (N + Q) * 32), Space: O(N * 32).


// ----------------------------------------------------------------------------
// 75. Number of Distinct Substrings in a String
// ----------------------------------------------------------------------------
struct SimpleTrieNode {
    SimpleTrieNode* links[26] = {nullptr};
    bool containsKey(char ch) { return links[ch - 'a'] != nullptr; }
    SimpleTrieNode* get(char ch) { return links[ch - 'a']; }
    void put(char ch, SimpleTrieNode* node) { links[ch - 'a'] = node; }
};

int countDistinctSubstrings(string s) {
    SimpleTrieNode* root = new SimpleTrieNode();
    int count = 0;
    int n = s.size();

    for (int i = 0; i < n; i++) {
        SimpleTrieNode* node = root;
        for (int j = i; j < n; j++) {
            if (!node->containsKey(s[j])) {
                count++; // New distinct substring discovered
                node->put(s[j], new SimpleTrieNode());
            }
            node = node->get(s[j]);
        }
    }
    return count + 1; // +1 for empty string if required by problem
}
// Interview Explanation:
// - Problem Statement: Count total number of distinct substrings in a given string s.
// - Approach: Trie Insertion of all suffixes of s; count newly allocated Trie nodes.
// - Intuition: Each node in the suffix Trie represents a unique substring starting from root to that node.
// - Complexity: Time: O(N^2), Space: O(N^2 * 26).


// ============================================================================
// SECTION 11: STRINGS (Problems 76 - 79)
// ============================================================================

// ----------------------------------------------------------------------------
// 76. Minimum Bracket Reversals to Balance Expression
// ----------------------------------------------------------------------------
int countMinBracketReversals(string s) {
    if (s.length() % 2 != 0) return -1; // Odd length cannot be balanced

    int open = 0, close = 0;
    for (char c : s) {
        if (c == '{') {
            open++;
        } else {
            if (open > 0) {
                open--; // Matched valid pair
            } else {
                close++; // Unmatched closing brace
            }
        }
    }
    // ceil(open/2) + ceil(close/2)
    return (open + 1) / 2 + (close + 1) / 2;
}
// Interview Explanation:
// - Problem Statement: Find minimum bracket reversals to make expression balanced (e.g. `}{{}}{{{`). Return -1 if odd length.
// - Approach: Count unmatched open and close brackets using single pass counter.
// - Intuition: After removing balanced pairs, string is `}}...}{{...{`. Reversing half of each group produces balanced configuration.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 77. Rabin Karp Algorithm (Rolling Hash Pattern Matching)
// ----------------------------------------------------------------------------
vector<int> rabinKarpSearch(const string& text, const string& pattern) {
    vector<int> matches;
    int n = text.size(), m = pattern.size();
    if (m > n) return matches;

    const ll PRIME = 31;
    const ll MOD_VAL = 1e9 + 9;

    ll patternHash = 0, windowHash = 0, power = 1;

    for (int i = 0; i < m; i++) {
        patternHash = (patternHash * PRIME + (pattern[i] - 'a' + 1)) % MOD_VAL;
        windowHash = (windowHash * PRIME + (text[i] - 'a' + 1)) % MOD_VAL;
        if (i < m - 1) power = (power * PRIME) % MOD_VAL;
    }

    for (int i = 0; i <= n - m; i++) {
        if (patternHash == windowHash) {
            if (text.substr(i, m) == pattern) {
                matches.push_back(i);
            }
        }
        if (i < n - m) {
            windowHash = (windowHash - (text[i] - 'a' + 1) * power % MOD_VAL + MOD_VAL) % MOD_VAL;
            windowHash = (windowHash * PRIME + (text[i + m] - 'a' + 1)) % MOD_VAL;
        }
    }
    return matches;
}
// Interview Explanation:
// - Problem Statement: Find all starting indices of pattern in text using rolling polynomial hashing.
// - Approach: Rabin-Karp Algorithm with modular arithmetic rolling hash update.
// - Intuition: Sliding window updates hash in O(1) by subtracting leading character and adding trailing character.
// - Complexity: Time: O(N + M) average, O(N * M) worst case with hash collisions, Space: O(1).


// ----------------------------------------------------------------------------
// 78. Z-Function / Z-Algorithm
// ----------------------------------------------------------------------------
vector<int> calculateZ(const string& s) {
    int n = s.length();
    vector<int> Z(n, 0);
    int l = 0, r = 0;

    for (int i = 1; i < n; i++) {
        if (i <= r) {
            Z[i] = min(r - i + 1, Z[i - l]);
        }
        while (i + Z[i] < n && s[Z[i]] == s[i + Z[i]]) {
            Z[i]++;
        }
        if (i + Z[i] - 1 > r) {
            l = i;
            r = i + Z[i] - 1;
        }
    }
    return Z;
}

vector<int> zAlgorithmSearch(const string& text, const string& pattern) {
    string combined = pattern + "$" + text;
    vector<int> Z = calculateZ(combined);
    vector<int> occurrences;
    int pLen = pattern.size();

    for (size_t i = pLen + 1; i < Z.size(); i++) {
        if (Z[i] == pLen) {
            occurrences.push_back(i - pLen - 1);
        }
    }
    return occurrences;
}
// Interview Explanation:
// - Problem Statement: Construct Z-array where `Z[i]` is length of longest common prefix between `s` and suffix `s[i..n-1]`. Find pattern occurrences.
// - Approach: Z-Algorithm maintaining sliding window `[l, r]` of matching characters.
// - Intuition: Reuse previously computed prefix matches within the current `[l, r]` boundary to avoid re-comparisons.
// - Complexity: Time: O(N + M), Space: O(N + M).


// ----------------------------------------------------------------------------
// 79. KMP Algorithm / LPS Array (Knuth-Morris-Pratt)
// ----------------------------------------------------------------------------
vector<int> computeLPSArray(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // Fallback to previous longest prefix suffix
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

vector<int> KMPSearch(const string& text, const string& pattern) {
    vector<int> occurrences;
    int n = text.length(), m = pattern.length();
    if (m == 0) return occurrences;

    vector<int> lps = computeLPSArray(pattern);
    int i = 0; // index for text
    int j = 0; // index for pattern

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            occurrences.push_back(i - j); // Found match at index i - j
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return occurrences;
}
// Interview Explanation:
// - Problem Statement: Find all occurrences of pattern in text in linear O(N + M) time.
// - Approach: KMP Algorithm using Longest Prefix Suffix (LPS / Pi) array.
// - Intuition: When a mismatch occurs, LPS table allows shifting pattern to the longest matching proper prefix without backtracking text index i.
// - Complexity: Time: O(N + M), Space: O(M).

// ============================================================================
// MERGED BLIND 75 LEETCODE PROBLEMS (SKIPPING DUPLICATES WITH STRIVER 79)
// Problems 80 - 137 (58 Unique Problems)
// ============================================================================

// ============================================================================
// SECTION 12: BLIND 75 - ARRAYS & HASHING (Problems 80 - 85)
// ============================================================================

// ----------------------------------------------------------------------------
// 80. Two Sum (LeetCode 1)
// ----------------------------------------------------------------------------
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> numMap;
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        if (numMap.find(complement) != numMap.end()) {
            return {numMap[complement], i};
        }
        numMap[nums[i]] = i;
    }
    return {};
}
// Interview Explanation:
// - Problem Statement: Return indices of two numbers in array that add up to target.
// - Approach: Hash Map storing value-to-index mappings in a single pass.
// - Intuition: For each number `x`, check if `target - x` was already encountered in O(1) average time.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 81. Best Time to Buy and Sell Stock (LeetCode 121)
// ----------------------------------------------------------------------------
int maxProfitStock(vector<int>& prices) {
    int minPrice = INT_MAX;
    int maxProfit = 0;
    for (int price : prices) {
        minPrice = min(minPrice, price);
        maxProfit = max(maxProfit, price - minPrice);
    }
    return maxProfit;
}
// Interview Explanation:
// - Problem Statement: Maximize profit by choosing a single day to buy one stock and a different day in the future to sell.
// - Approach: Single pass tracking minimum price seen so far and maximum potential profit upon selling today.
// - Intuition: The optimal day to sell after any point is best paired with the lowest historical price before that day.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 82. Contains Duplicate (LeetCode 217)
// ----------------------------------------------------------------------------
bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen;
    for (int num : nums) {
        if (seen.find(num) != seen.end()) return true;
        seen.insert(num);
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Return true if any value appears at least twice in the array.
// - Approach: Hash Set membership tracking.
// - Intuition: Inserting into hash set detects duplicate existence immediately upon collision.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 83. Product of Array Except Self (LeetCode 238)
// ----------------------------------------------------------------------------
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> output(n, 1);

    int prefix = 1;
    for (int i = 0; i < n; i++) {
        output[i] = prefix;
        prefix *= nums[i];
    }

    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        output[i] *= suffix;
        suffix *= nums[i];
    }
    return output;
}
// Interview Explanation:
// - Problem Statement: Return array where `output[i]` is product of all elements except `nums[i]` without using division in O(N) time.
// - Approach: Two passes: Prefix running product stored in output array + Suffix running product variable.
// - Intuition: `productExceptSelf[i] = (prefix product of nums[0..i-1]) * (suffix product of nums[i+1..n-1])`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space (excluding output).


// ----------------------------------------------------------------------------
// 84. Search in Rotated Sorted Array (LeetCode 33 - Distinct Elements)
// ----------------------------------------------------------------------------
int searchRotatedSortedArray(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;

        if (nums[left] <= nums[mid]) {
            // Left half is sorted
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            // Right half is sorted
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Search for target value in rotated sorted array of distinct integers in O(log N) time.
// - Approach: Binary Search checking which half is strictly sorted.
// - Intuition: Mid always splits rotated array into one normally sorted half and one rotated half.
// - Complexity: Time: O(log N), Space: O(1).


// ----------------------------------------------------------------------------
// 85. Container With Most Water (LeetCode 11)
// ----------------------------------------------------------------------------
int maxArea(vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int maxWater = 0;
    while (left < right) {
        int width = right - left;
        int h = min(height[left], height[right]);
        maxWater = max(maxWater, width * h);
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }
    return maxWater;
}
// Interview Explanation:
// - Problem Statement: Find two lines that together with x-axis form a container containing maximum water.
// - Approach: Two Pointers starting at outer boundaries, moving the shorter line inward.
// - Intuition: Width decreases on every step; keeping the shorter line cannot yield a larger area with smaller widths.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================================
// SECTION 13: BLIND 75 - BIT MANIPULATION (Problems 86 - 90)
// ============================================================================

// ----------------------------------------------------------------------------
// 86. Sum of Two Integers (LeetCode 371)
// ----------------------------------------------------------------------------
int getSum(int a, int b) {
    while (b != 0) {
        unsigned int carry = (unsigned int)(a & b) << 1;
        a = a ^ b; // sum without carry
        b = carry; // carry bits
    }
    return a;
}
// Interview Explanation:
// - Problem Statement: Calculate sum of two integers a and b without using '+' or '-' operators.
// - Approach: Bitwise XOR for half-addition + Bitwise AND left-shifted by 1 for carry propagation.
// - Intuition: `a ^ b` computes bitwise addition without carry; `(a & b) << 1` computes carries.
// - Complexity: Time: O(32) = O(1), Space: O(1).


// ----------------------------------------------------------------------------
// 87. Number of 1 Bits / Hamming Weight (LeetCode 191)
// ----------------------------------------------------------------------------
int hammingWeight(uint32_t n) {
    int count = 0;
    while (n) {
        n &= (n - 1); // Clears lowest set bit
        count++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Return number of set bits ('1's) in binary representation of unsigned integer.
// - Approach: Brian Kernighan's Algorithm clearing lowest set bit in each iteration.
// - Intuition: `n & (n - 1)` directly zeroes out the rightmost set bit in O(1) operations.
// - Complexity: Time: O(set_bits) <= O(32) = O(1), Space: O(1).


// ----------------------------------------------------------------------------
// 88. Counting Bits (LeetCode 338)
// ----------------------------------------------------------------------------
vector<int> countBits(int n) {
    vector<int> ans(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        ans[i] = ans[i >> 1] + (i & 1); // DP relation based on right shift
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: For every number 0 <= i <= n, compute number of 1's in its binary representation.
// - Approach: Dynamic Programming using relation `bits[i] = bits[i / 2] + (i % 2)`.
// - Intuition: Right-shifting a number drops its least significant bit, which was already computed.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// ----------------------------------------------------------------------------
// 89. Missing Number (LeetCode 268)
// ----------------------------------------------------------------------------
int missingNumber(vector<int>& nums) {
    int n = nums.size();
    int xorVal = n;
    for (int i = 0; i < n; i++) {
        xorVal ^= i ^ nums[i];
    }
    return xorVal;
}
// Interview Explanation:
// - Problem Statement: Given array containing n distinct numbers in range [0, n], find the one missing from array.
// - Approach: Bitwise XOR of all indices and array values.
// - Intuition: Since `x ^ x = 0`, every paired number cancels out, leaving only the missing number.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 90. Reverse Bits (LeetCode 190)
// ----------------------------------------------------------------------------
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Reverse bits of a given 32-bit unsigned integer.
// - Approach: Bit manipulation extracting LSB of n and shifting into result.
// - Intuition: Append each lowest bit of n to the right of result and shift result left.
// - Complexity: Time: O(32) = O(1), Space: O(1).


// ============================================================================
// SECTION 14: BLIND 75 - DYNAMIC PROGRAMMING (Problems 91 - 97)
// ============================================================================

// ----------------------------------------------------------------------------
// 91. Climbing Stairs (LeetCode 70)
// ----------------------------------------------------------------------------
int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1, prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Distinct ways to climb n stairs taking 1 or 2 steps at a time.
// - Approach: Fibonacci DP with rolling variables.
// - Intuition: Ways to step i = ways to step i-1 + ways to step i-2.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 92. Coin Change I & II (LeetCode 322 & 518)
// ----------------------------------------------------------------------------
// Fewest coins to make up amount (LeetCode 322)
int coinChangeMin(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (coin <= i) {
                dp[i] = min(dp[i], 1 + dp[i - coin]);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

// Number of combinations to make up amount (LeetCode 518)
int changeCombinations(int amount, vector<int>& coins) {
    vector<int> dp(amount + 1, 0);
    dp[0] = 1;
    for (int coin : coins) {
        for (int i = coin; i <= amount; i++) {
            dp[i] += dp[i - coin];
        }
    }
    return dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find min coins to make amount (LC 322) and total unique combinations to make amount (LC 518).
// - Approach: Unbounded Knapsack 1D DP array. Outer loop on coins avoids duplicate permutations for LC 518.
// - Intuition: Each coin can be reused indefinitely; updating `dp[i]` using `dp[i - coin]` propagates unbounded choices.
// - Complexity: Time: O(N * Amount), Space: O(Amount).


// ----------------------------------------------------------------------------
// 93. Word Break (LeetCode 139)
// ----------------------------------------------------------------------------
bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && dict.find(s.substr(j, i - j)) != dict.end()) {
                dp[i] = true;
                break;
            }
        }
    }
    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Return true if string s can be segmented into space-separated dictionary words.
// - Approach: 1D DP where `dp[i]` represents whether prefix `s[0..i-1]` can be segmented.
// - Intuition: If `dp[j] == true` and substring `s[j..i]` is in dictionary, then `dp[i] = true`.
// - Complexity: Time: O(N^2 * L), Space: O(N).


// ----------------------------------------------------------------------------
// 94. House Robber II (LeetCode 213 - Circular Houses)
// ----------------------------------------------------------------------------
int robLinear(const vector<int>& nums, int start, int end) {
    int prev2 = 0, prev1 = 0;
    for (int i = start; i <= end; i++) {
        int curr = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int robCircular(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    if (n == 1) return nums[0];
    return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
}
// Interview Explanation:
// - Problem Statement: Maximize money robbed from circular houses where first and last houses are neighbors.
// - Approach: Solve two independent linear subproblems: `[0..n-2]` (excluding last) and `[1..n-1]` (excluding first).
// - Intuition: First and last houses cannot both be robbed. The optimum must be in either excluding first or excluding last.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 95. Decode Ways (LeetCode 91)
// ----------------------------------------------------------------------------
int numDecodings(string s) {
    int n = s.size();
    if (n == 0 || s[0] == '0') return 0;

    int prev2 = 1, prev1 = 1;
    for (int i = 1; i < n; i++) {
        int curr = 0;
        int singleDigit = s[i] - '0';
        int twoDigits = stoi(s.substr(i - 1, 2));

        if (singleDigit >= 1) curr += prev1;
        if (twoDigits >= 10 && twoDigits <= 26) curr += prev2;

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Decode string of digits mapped to 'A'-'Z' ('1'-'26'). Return number of valid decodings.
// - Approach: 1D DP with space optimization considering single-digit (1-9) and two-digit (10-26) decode transitions.
// - Intuition: Character at index i can decode independently if != '0', or combine with previous digit if in range [10, 26].
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 96. Unique Paths (LeetCode 62)
// ----------------------------------------------------------------------------
int uniquePaths(int m, int n) {
    vector<int> dp(n, 1);
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[j] += dp[j - 1]; // dp[j] is top, dp[j-1] is left
        }
    }
    return dp[n - 1];
}

// Combinatorics O(min(m, n)) solution: (m+n-2) C (m-1)
int uniquePathsCombinatorics(int m, int n) {
    int N = m + n - 2;
    int r = min(m - 1, n - 1);
    long long res = 1;
    for (int i = 1; i <= r; i++) {
        res = res * (N - r + i) / i;
    }
    return (int)res;
}
// Interview Explanation:
// - Problem Statement: Count unique paths from top-left to bottom-right in m x n grid moving only right or down.
// - Approach: 1D Rolling DP or Combinatorics formula `(m + n - 2) choose (m - 1)`.
// - Intuition: A total of `(m-1)` down moves and `(n-1)` right moves are needed in any order.
// - Complexity: Time: O(M * N) with DP or O(min(M, N)) with combinatorics, Space: O(N).


// ----------------------------------------------------------------------------
// 97. Jump Game (LeetCode 55)
// ----------------------------------------------------------------------------
bool canJump(vector<int>& nums) {
    int maxReach = 0;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i > maxReach) return false; // cannot reach index i
        maxReach = max(maxReach, i + nums[i]);
        if (maxReach >= n - 1) return true;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if you can reach the last index starting from index 0 given max jump lengths.
// - Approach: Greedy tracking the farthest reachable index `maxReach`.
// - Intuition: If current index exceeds `maxReach`, future indices are unreachable.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================================
// SECTION 15: BLIND 75 - GRAPHS & CONNECTIVITY (Problems 98 - 103)
// ============================================================================

// ----------------------------------------------------------------------------
// Graph Node Struct for Clone Graph
// ----------------------------------------------------------------------------
class GraphNode {
public:
    int val;
    vector<GraphNode*> neighbors;
    GraphNode() : val(0), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val) : val(_val), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val, vector<GraphNode*> _neighbors) : val(_val), neighbors(_neighbors) {}
};


// ----------------------------------------------------------------------------
// 98. Clone Graph (LeetCode 133)
// ----------------------------------------------------------------------------
GraphNode* cloneGraphDFS(GraphNode* node, unordered_map<GraphNode*, GraphNode*>& visited) {
    if (!node) return nullptr;
    if (visited.find(node) != visited.end()) {
        return visited[node];
    }

    GraphNode* clone = new GraphNode(node->val);
    visited[node] = clone;

    for (GraphNode* neighbor : node->neighbors) {
        clone->neighbors.push_back(cloneGraphDFS(neighbor, visited));
    }
    return clone;
}

GraphNode* cloneGraph(GraphNode* node) {
    unordered_map<GraphNode*, GraphNode*> visited;
    return cloneGraphDFS(node, visited);
}
// Interview Explanation:
// - Problem Statement: Return deep copy of connected undirected graph.
// - Approach: DFS Traversal with Hash Map mapping original nodes to their newly cloned counterparts.
// - Intuition: Hash map prevents infinite cycles and ensures graph topology is cloned faithfully.
// - Complexity: Time: O(V + E), Space: O(V).


// ----------------------------------------------------------------------------
// 99. Course Schedule I - Cycle Detection (LeetCode 207)
// ----------------------------------------------------------------------------
bool canFinishCourses(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);

    for (auto& edge : prerequisites) {
        adj[edge[1]].push_back(edge[0]);
        inDegree[edge[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    int completed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        completed++;

        for (int v : adj[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }
    return completed == numCourses;
}
// Interview Explanation:
// - Problem Statement: Determine if it is possible to finish all courses given prerequisite dependencies (detect cycle in directed graph).
// - Approach: Kahn's Algorithm / BFS In-degree reduction.
// - Intuition: If graph contains a directed cycle, nodes in the cycle will never reach in-degree 0 and won't be processed.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ----------------------------------------------------------------------------
// 100. Pacific Atlantic Water Flow (LeetCode 417)
// ----------------------------------------------------------------------------
void pacificAtlanticDFS(int r, int c, vector<vector<bool>>& ocean, const vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    ocean[r][c] = true;
    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};

    for (int d = 0; d < 4; d++) {
        int nr = r + dRow[d], nc = c + dCol[d];
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && !ocean[nr][nc] && heights[nr][nc] >= heights[r][c]) {
            pacificAtlanticDFS(nr, nc, ocean, heights);
        }
    }
}

vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    vector<vector<bool>> pacific(m, vector<bool>(n, false));
    vector<vector<bool>> atlantic(m, vector<bool>(n, false));

    for (int i = 0; i < m; i++) {
        pacificAtlanticDFS(i, 0, pacific, heights);
        pacificAtlanticDFS(i, n - 1, atlantic, heights);
    }
    for (int j = 0; j < n; j++) {
        pacificAtlanticDFS(0, j, pacific, heights);
        pacificAtlanticDFS(m - 1, j, atlantic, heights);
    }

    vector<vector<int>> result;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (pacific[i][j] && atlantic[i][j]) {
                result.push_back({i, j});
            }
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Find all grid coordinates where rain water can flow to both Pacific (top/left) and Atlantic (bottom/right) oceans.
// - Approach: Reverse DFS starting from ocean borders moving inward to cells of equal or greater height.
// - Intuition: Reverse water flow from ocean edges guarantees all visited cells can drain into that ocean.
// - Complexity: Time: O(M * N), Space: O(M * N).


// ----------------------------------------------------------------------------
// 101. Longest Consecutive Sequence (LeetCode 128)
// ----------------------------------------------------------------------------
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());
    int longestStreak = 0;

    for (int num : numSet) {
        // Only start counting if num is the start of a sequence
        if (numSet.find(num - 1) == numSet.end()) {
            int currentNum = num;
            int currentStreak = 1;

            while (numSet.find(currentNum + 1) != numSet.end()) {
                currentNum++;
                currentStreak++;
            }
            longestStreak = max(longestStreak, currentStreak);
        }
    }
    return longestStreak;
}
// Interview Explanation:
// - Problem Statement: Find length of longest consecutive elements sequence in unsorted array in O(N) time.
// - Approach: Hash Set lookup checking sequence head condition `!set.count(num - 1)`.
// - Intuition: Only iterating sequences from their true starting numbers ensures each element is visited at most twice.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 102. Graph Valid Tree (LeetCode 261)
// ----------------------------------------------------------------------------
bool validTree(int n, vector<vector<int>>& edges) {
    if ((int)edges.size() != n - 1) return false; // Tree must have exactly N-1 edges
    DisjointSet ds(n);

    for (auto& edge : edges) {
        if (ds.findUPar(edge[0]) == ds.findUPar(edge[1])) {
            return false; // Cycle detected
        }
        ds.unionBySize(edge[0], edge[1]);
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine whether given undirected graph of n nodes forms a valid single tree.
// - Approach: Disjoint Set Union (DSU) verifying exactly n - 1 edges and 0 cycles.
// - Intuition: An undirected graph of N nodes is a tree iff it is acyclic and has exactly N - 1 edges.
// - Complexity: Time: O(N alpha(N)), Space: O(N).


// ----------------------------------------------------------------------------
// 103. Number of Connected Components in an Undirected Graph (LeetCode 323)
// ----------------------------------------------------------------------------
int countComponents(int n, vector<vector<int>>& edges) {
    DisjointSet ds(n);
    int components = n;

    for (auto& edge : edges) {
        if (ds.findUPar(edge[0]) != ds.findUPar(edge[1])) {
            components--;
            ds.unionBySize(edge[0], edge[1]);
        }
    }
    return components;
}
// Interview Explanation:
// - Problem Statement: Find number of connected components in an undirected graph with n vertices.
// - Approach: DSU decrementing component count upon every successful union.
// - Intuition: Start with n isolated components; each valid merge reduces total component count by 1.
// - Complexity: Time: O(E alpha(N)), Space: O(N).


// ============================================================================
// SECTION 16: BLIND 75 - INTERVALS (Problems 104 - 108)
// ============================================================================

// ----------------------------------------------------------------------------
// 104. Insert Interval (LeetCode 57)
// ----------------------------------------------------------------------------
vector<vector<int>> insertInterval(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> result;
    int i = 0, n = intervals.size();

    // 1. Add all intervals ending before newInterval starts
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }

    // 2. Merge all overlapping intervals with newInterval
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);

    // 3. Add all remaining intervals
    while (i < n) {
        result.push_back(intervals[i++]);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into non-overlapping sorted intervals and merge if necessary.
// - Approach: Three-phase single pass: left non-overlapping, merged overlapping region, right non-overlapping.
// - Intuition: Overlap condition `intervals[i][0] <= newInterval[1]` dynamically expands `newInterval` boundaries.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// ----------------------------------------------------------------------------
// 105. Merge Intervals (LeetCode 56)
// ----------------------------------------------------------------------------
vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;

    for (const auto& interval : intervals) {
        if (merged.empty() || merged.back()[1] < interval[0]) {
            merged.push_back(interval);
        } else {
            merged.back()[1] = max(merged.back()[1], interval[1]);
        }
    }
    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals and return array of non-overlapping intervals covering all input intervals.
// - Approach: Sort intervals by start time + linear sweep extending right endpoint of current interval.
// - Intuition: Sorting by start time guarantees overlapping intervals appear consecutively.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// ----------------------------------------------------------------------------
// 106. Non-overlapping Intervals (LeetCode 435)
// ----------------------------------------------------------------------------
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1]; // sort by end time
    });

    int count = 0;
    int lastEnd = intervals[0][1];

    for (size_t i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < lastEnd) {
            count++; // overlap detected, remove current interval
        } else {
            lastEnd = intervals[i][1];
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of intervals to remove to make the remainder non-overlapping.
// - Approach: Greedy Interval Scheduling by sorting by earliest finish time `end`.
// - Intuition: Choosing interval that finishes earliest leaves maximum room for future intervals, minimizing required removals.
// - Complexity: Time: O(N log N), Space: O(1).


// ----------------------------------------------------------------------------
// 107. Meeting Rooms (LeetCode 252)
// ----------------------------------------------------------------------------
bool canAttendMeetings(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    for (size_t i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < intervals[i - 1][1]) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if a person can attend all scheduled meetings without conflict.
// - Approach: Sort meetings by start time and check adjacent overlap condition `intervals[i][0] < intervals[i-1][1]`.
// - Complexity: Time: O(N log N), Space: O(1).


// ----------------------------------------------------------------------------
// 108. Meeting Rooms II (LeetCode 253)
// ----------------------------------------------------------------------------
int minMeetingRooms(vector<vector<int>>& intervals) {
    int n = intervals.size();
    vector<int> starts(n), ends(n);
    for (int i = 0; i < n; i++) {
        starts[i] = intervals[i][0];
        ends[i] = intervals[i][1];
    }
    sort(starts.begin(), starts.end());
    sort(ends.begin(), ends.end());

    int rooms = 0, endIdx = 0;
    for (int i = 0; i < n; i++) {
        if (starts[i] < ends[endIdx]) {
            rooms++; // Need new room
        } else {
            endIdx++; // Reused existing room that ended
        }
    }
    return rooms;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of conference rooms required to host all meetings.
// - Approach: Two pointers on sorted start and end times / Chronological sweep.
// - Intuition: If a meeting starts before the earliest meeting ends, a new room is needed; otherwise, reuse released room.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================================
// SECTION 17: BLIND 75 - MATRICES (Problems 113 - 115)
// ============================================================================

// ----------------------------------------------------------------------------
// 113. Set Matrix Zeroes (LeetCode 73)
// ----------------------------------------------------------------------------
void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    int col0 = 1;

    for (int i = 0; i < m; i++) {
        if (matrix[i][0] == 0) col0 = 0;
        for (int j = 1; j < n; j++) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }
    }

    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 1; j--) {
            if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                matrix[i][j] = 0;
            }
        }
        if (col0 == 0) matrix[i][0] = 0;
    }
}
// Interview Explanation:
// - Problem Statement: If an element in m x n matrix is 0, set its entire row and column to 0 in-place in O(1) space.
// - Approach: Use 1st row and 1st col as markers; use single variable `col0` for 1st col marker.
// - Intuition: Reusing first row/column eliminates need for external marker arrays.
// - Complexity: Time: O(M * N), Space: O(1).


// ----------------------------------------------------------------------------
// 114. Spiral Matrix (LeetCode 54)
// ----------------------------------------------------------------------------
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    vector<int> res;
    if (matrix.empty()) return res;
    int top = 0, bottom = matrix.size() - 1;
    int left = 0, right = matrix[0].size() - 1;

    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; j++) res.push_back(matrix[top][j]);
        top++;

        for (int i = top; i <= bottom; i++) res.push_back(matrix[i][right]);
        right--;

        if (top <= bottom) {
            for (int j = right; j >= left; j--) res.push_back(matrix[bottom][j]);
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) res.push_back(matrix[i][left]);
            left++;
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return all elements of matrix in spiral order.
// - Approach: Boundary shrinking with 4 directional boundary pointers: `top`, `bottom`, `left`, `right`.
// - Intuition: Traverse perimeter rings from outside in, shrinking boundary margins after each side traversal.
// - Complexity: Time: O(M * N), Space: O(1) auxiliary space.


// ----------------------------------------------------------------------------
// 115. Rotate Image / Matrix by 90 Degrees Clockwise (LeetCode 48)
// ----------------------------------------------------------------------------
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    // 1. Transpose matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    // 2. Reverse each row
    for (int i = 0; i < n; i++) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}
// Interview Explanation:
// - Problem Statement: Rotate n x n 2D matrix by 90 degrees clockwise in-place.
// - Approach: Transpose matrix + Reverse every row.
// - Intuition: Transpose converts columns to rows; horizontal reflection produces 90-degree clockwise rotation.
// - Complexity: Time: O(N^2), Space: O(1).


// ============================================================================
// SECTION 18: BLIND 75 - STRINGS (Problems 116 - 125)
// ============================================================================

// ----------------------------------------------------------------------------
// 116. Longest Substring Without Repeating Characters (LeetCode 3)
// ----------------------------------------------------------------------------
int lengthOfLongestSubstring(string s) {
    vector<int> lastIndex(256, -1);
    int maxLen = 0, start = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (lastIndex[(unsigned char)s[i]] >= start) {
            start = lastIndex[(unsigned char)s[i]] + 1;
        }
        lastIndex[(unsigned char)s[i]] = i;
        maxLen = max(maxLen, i - start + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of longest substring without duplicate characters.
// - Approach: Sliding Window with last-seen index array.
// - Intuition: When duplicate is encountered, advance window start directly past duplicate's previous index.
// - Complexity: Time: O(N), Space: O(1) (256 ASCII map).


// ----------------------------------------------------------------------------
// 117. Longest Repeating Character Replacement (LeetCode 424)
// ----------------------------------------------------------------------------
int characterReplacement(string s, int k) {
    vector<int> count(26, 0);
    int maxFreq = 0, left = 0, maxLen = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        count[s[right] - 'A']++;
        maxFreq = max(maxFreq, count[s[right] - 'A']);

        // If characters to replace (window_size - maxFreq) > k, shrink window
        if ((right - left + 1) - maxFreq > k) {
            count[s[left] - 'A']--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find longest substring of same letter after replacing at most k characters.
// - Approach: Sliding window maintaining maximum frequency character in current window.
// - Intuition: Window is valid as long as `(window_len - maxFreq) <= k`.
// - Complexity: Time: O(N), Space: O(1) (26 letters).


// ----------------------------------------------------------------------------
// 118. Minimum Window Substring (LeetCode 76)
// ----------------------------------------------------------------------------
string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";
    vector<int> targetMap(128, 0);
    for (char c : t) targetMap[(unsigned char)c]++;

    int required = t.size();
    int left = 0, minStart = 0, minLen = INT_MAX;

    for (int right = 0; right < (int)s.size(); right++) {
        if (targetMap[(unsigned char)s[right]] > 0) required--;
        targetMap[(unsigned char)s[right]]--;

        while (required == 0) {
            if (right - left + 1 < minLen) {
                minLen = right - left + 1;
                minStart = left;
            }
            targetMap[(unsigned char)s[left]]++;
            if (targetMap[(unsigned char)s[left]] > 0) required++;
            left++;
        }
    }
    return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
}
// Interview Explanation:
// - Problem Statement: Find minimum window substring in s that contains all characters of t.
// - Approach: Sliding Window with frequency table and match count `required`.
// - Intuition: Expand right until all characters matched, then shrink left to find minimal valid window.
// - Complexity: Time: O(N + M), Space: O(1) (128 ASCII array).


// ----------------------------------------------------------------------------
// 119. Valid Anagram (LeetCode 242)
// ----------------------------------------------------------------------------
bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    vector<int> count(26, 0);
    for (size_t i = 0; i < s.size(); i++) {
        count[s[i] - 'a']++;
        count[t[i] - 'a']--;
    }
    for (int c : count) {
        if (c != 0) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if string t is an anagram of string s.
// - Approach: Frequency counter array of size 26.
// - Intuition: Strings are anagrams if every character count matches exactly.
// - Complexity: Time: O(N), Space: O(1).


// ----------------------------------------------------------------------------
// 120. Group Anagrams (LeetCode 49)
// ----------------------------------------------------------------------------
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, list] : groups) {
        result.push_back(list);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Group an array of strings into anagram clusters.
// - Approach: Hash Map with sorted character string as bucket key.
// - Intuition: Anagrams sort into identical key strings.
// - Complexity: Time: O(N * L log L), Space: O(N * L).


// ----------------------------------------------------------------------------
// 121. Valid Parentheses (LeetCode 20)
// ----------------------------------------------------------------------------
bool isValidParentheses(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            if (c == ')' && st.top() != '(') return false;
            if (c == '}' && st.top() != '{') return false;
            if (c == ']' && st.top() != '[') return false;
            st.pop();
        }
    }
    return st.empty();
}
// Interview Explanation:
// - Problem Statement: Validate matching brackets '()', '{}', '[]'.
// - Approach: Stack matching closing brackets with corresponding opening top element.
// - Intuition: Last opened bracket must be the first closed bracket (LIFO).
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 122. Valid Palindrome & Palindrome Number (LeetCode 125 & 9)
// ----------------------------------------------------------------------------
bool isPalindromeString(string s) {
    int left = 0, right = s.size() - 1;
    while (left < right) {
        while (left < right && !isalnum(s[left])) left++;
        while (left < right && !isalnum(s[right])) right--;
        if (tolower(s[left]) != tolower(s[right])) return false;
        left++;
        right--;
    }
    return true;
}

bool isPalindromeNumber(int x) {
    if (x < 0 || (x % 10 == 0 && x != 0)) return false;
    int reversedHalf = 0;
    while (x > reversedHalf) {
        reversedHalf = reversedHalf * 10 + x % 10;
        x /= 10;
    }
    return x == reversedHalf || x == reversedHalf / 10;
}
// Interview Explanation:
// - Problem Statement: Check if alphanumeric string is palindrome ignoring cases (LC 125) and check if integer is palindrome without converting to string (LC 9).
// - Approach: Two pointers skipping non-alphanumeric chars; reverse right half of integer until equal/greater.
// - Complexity: Time: O(N) / O(log10 X), Space: O(1).


// ----------------------------------------------------------------------------
// 123. Longest Palindromic Substring (LeetCode 5)
// ----------------------------------------------------------------------------
string longestPalindrome(string s) {
    if (s.empty()) return "";
    int start = 0, maxLen = 1;

    auto expandAroundCenter = [&](int left, int right) {
        while (left >= 0 && right < (int)s.size() && s[left] == s[right]) {
            if (right - left + 1 > maxLen) {
                maxLen = right - left + 1;
                start = left;
            }
            left--;
            right++;
        }
    };

    for (int i = 0; i < (int)s.size(); i++) {
        expandAroundCenter(i, i);     // Odd length center
        expandAroundCenter(i, i + 1); // Even length center
    }
    return s.substr(start, maxLen);
}
// Interview Explanation:
// - Problem Statement: Return the longest palindromic substring in string s.
// - Approach: Expand Around Center for 2N-1 possible centers (odd and even length).
// - Intuition: Palindromes expand symmetrically from single-letter and double-letter centers.
// - Complexity: Time: O(N^2), Space: O(1).


// ----------------------------------------------------------------------------
// 124. Palindromic Substrings (LeetCode 647)
// ----------------------------------------------------------------------------
int countSubstrings(string s) {
    int count = 0;
    auto countFromCenter = [&](int left, int right) {
        while (left >= 0 && right < (int)s.size() && s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
    };

    for (int i = 0; i < (int)s.size(); i++) {
        countFromCenter(i, i);     // Odd center
        countFromCenter(i, i + 1); // Even center
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total number of palindromic substrings in string s.
// - Approach: Expand Around Center incrementing counter for each valid expansion.
// - Complexity: Time: O(N^2), Space: O(1).


// ----------------------------------------------------------------------------
// 125. Encode and Decode Strings (LeetCode 271)
// ----------------------------------------------------------------------------
class CodecStrings {
public:
    string encode(vector<string>& strs) {
        string encoded = "";
        for (const string& s : strs) {
            encoded += to_string(s.size()) + "#" + s;
        }
        return encoded;
    }

    vector<string> decode(string s) {
        vector<string> decoded;
        int i = 0, n = s.size();
        while (i < n) {
            int delimiterPos = s.find('#', i);
            int len = stoi(s.substr(i, delimiterPos - i));
            decoded.push_back(s.substr(delimiterPos + 1, len));
            i = delimiterPos + 1 + len;
        }
        return decoded;
    }
};
// Interview Explanation:
// - Problem Statement: Design algorithm to encode list of strings to a single string and decode back without delimiter ambiguity.
// - Approach: Length-prefix framing (`<length>#<string>`).
// - Intuition: Explicit string length prefix eliminates ambiguity even if string content contains special delimiters.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// ============================================================================
// SECTION 19: BLIND 75 - TREES & HEAPS (Problems 126 - 137)
// ============================================================================

// ----------------------------------------------------------------------------
// 126. Maximum Depth of Binary Tree (LeetCode 104)
// ----------------------------------------------------------------------------
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
// Interview Explanation:
// - Problem Statement: Find maximum depth (number of nodes along longest path from root to leaf).
// - Approach: Post-order DFS `1 + max(leftDepth, rightDepth)`.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 127. Same Tree (LeetCode 100)
// ----------------------------------------------------------------------------
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
// Interview Explanation:
// - Problem Statement: Check if two binary trees are structurally identical and have identical node values.
// - Approach: Simultaneous pre-order DFS on both trees.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 128. Invert / Flip Binary Tree (LeetCode 226)
// ----------------------------------------------------------------------------
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
// Interview Explanation:
// - Problem Statement: Invert a binary tree (mirror reflection).
// - Approach: Recursive swap of left and right child pointers at every node.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 129. Binary Tree Level Order Traversal (LeetCode 102)
// ----------------------------------------------------------------------------
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        vector<int> level;
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Return level-order traversal of binary tree nodes' values level by level.
// - Approach: BFS Queue with level batching using `q.size()`.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 130. Serialize and Deserialize Binary Tree (LeetCode 297)
// ----------------------------------------------------------------------------
class CodecTree {
public:
    string serialize(TreeNode* root) {
        if (!root) return "#";
        string s = "";
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            if (!curr) {
                s += "#,";
            } else {
                s += to_string(curr->val) + ",";
                q.push(curr->left);
                q.push(curr->right);
            }
        }
        return s;
    }

    TreeNode* deserialize(string data) {
        if (data.empty() || data[0] == '#') return nullptr;
        stringstream ss(data);
        string token;
        getline(ss, token, ',');

        TreeNode* root = new TreeNode(stoi(token));
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();

            if (getline(ss, token, ',')) {
                if (token != "#") {
                    curr->left = new TreeNode(stoi(token));
                    q.push(curr->left);
                }
            }
            if (getline(ss, token, ',')) {
                if (token != "#") {
                    curr->right = new TreeNode(stoi(token));
                    q.push(curr->right);
                }
            }
        }
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Design algorithms to serialize a binary tree into a string and deserialize back into the tree.
// - Approach: BFS Level Order Traversal with null sentinel '#' and comma delimiter.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 131. Subtree of Another Tree (LeetCode 572)
// ----------------------------------------------------------------------------
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if (!root) return false;
    if (isSameTree(root, subRoot)) return true;
    return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}
// Interview Explanation:
// - Problem Statement: Check if tree `subRoot` is a subtree of tree `root`.
// - Approach: DFS Traversal testing `isSameTree` match at every candidate node.
// - Complexity: Time: O(N * M), Space: O(H).


// ----------------------------------------------------------------------------
// 132. Construct Binary Tree from Inorder and Postorder (LeetCode 106)
// ----------------------------------------------------------------------------
TreeNode* buildTreeInPostHelper(const vector<int>& inorder, int inStart, int inEnd,
                                const vector<int>& postorder, int postStart, int postEnd,
                                unordered_map<int, int>& inMap) {
    if (inStart > inEnd || postStart > postEnd) return nullptr;

    TreeNode* root = new TreeNode(postorder[postEnd]);
    int inRoot = inMap[root->val];
    int numsLeft = inRoot - inStart;

    root->left = buildTreeInPostHelper(inorder, inStart, inRoot - 1,
                                       postorder, postStart, postStart + numsLeft - 1, inMap);
    root->right = buildTreeInPostHelper(inorder, inRoot + 1, inEnd,
                                        postorder, postStart + numsLeft, postEnd - 1, inMap);
    return root;
}

TreeNode* buildTreeInPost(vector<int>& inorder, vector<int>& postorder) {
    unordered_map<int, int> inMap;
    for (int i = 0; i < (int)inorder.size(); i++) {
        inMap[inorder[i]] = i;
    }
    return buildTreeInPostHelper(inorder, 0, inorder.size() - 1,
                                 postorder, 0, postorder.size() - 1, inMap);
}
// Interview Explanation:
// - Problem Statement: Construct binary tree from inorder and postorder traversal arrays.
// - Approach: Divide and conquer with hash map. Last element of postorder is root.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 133. Validate Binary Search Tree (LeetCode 98)
// ----------------------------------------------------------------------------
bool isValidBSTHelper(TreeNode* root, long long minVal, long long maxVal) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isValidBSTHelper(root->left, minVal, root->val) &&
           isValidBSTHelper(root->right, root->val, maxVal);
}

bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}
// Interview Explanation:
// - Problem Statement: Determine whether a binary tree is a valid Binary Search Tree.
// - Approach: DFS with valid value interval `(minVal, maxVal)`.
// - Intuition: Every left child must be strictly `< root->val` and every right child strictly `> root->val`.
// - Complexity: Time: O(N), Space: O(H).


// ----------------------------------------------------------------------------
// 134. Kth Smallest Element in a BST (LeetCode 230)
// ----------------------------------------------------------------------------
void kthSmallestInorder(TreeNode* root, int& k, int& ans) {
    if (!root || k == 0) return;
    kthSmallestInorder(root->left, k, ans);
    k--;
    if (k == 0) {
        ans = root->val;
        return;
    }
    kthSmallestInorder(root->right, k, ans);
}

int kthSmallest(TreeNode* root, int k) {
    int ans = -1;
    kthSmallestInorder(root, k, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find k-th smallest element in a BST (1-indexed).
// - Approach: Inorder Traversal (produces ascending sorted order), stopping when k reaches 0.
// - Complexity: Time: O(H + K), Space: O(H).


// ----------------------------------------------------------------------------
// 135. Implement Trie - Prefix Tree (LeetCode 208)
// ----------------------------------------------------------------------------
struct PrefixTrieNode {
    PrefixTrieNode* children[26] = {nullptr};
    bool isEnd = false;
};

class Trie {
private:
    PrefixTrieNode* root;

public:
    Trie() { root = new PrefixTrieNode(); }

    void insert(string word) {
        PrefixTrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new PrefixTrieNode();
            }
            node = node->children[idx];
        }
        node->isEnd = true;
    }

    bool search(string word) {
        PrefixTrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        return node->isEnd;
    }

    bool startsWith(string prefix) {
        PrefixTrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Implement standard Prefix Tree (Trie) supporting insert, search, and startsWith.
// - Approach: 26-ary tree node with boolean `isEnd` flag.
// - Complexity: Time: O(L) for all operations, Space: O(N * L * 26).


// ----------------------------------------------------------------------------
// 136. Top K Frequent Elements (LeetCode 347)
// ----------------------------------------------------------------------------
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> count;
    for (int num : nums) count[num]++;

    int n = nums.size();
    vector<vector<int>> buckets(n + 1);
    for (auto& [num, freq] : count) {
        buckets[freq].push_back(num);
    }

    vector<int> result;
    for (int i = n; i >= 1 && (int)result.size() < k; i--) {
        for (int num : buckets[i]) {
            result.push_back(num);
            if ((int)result.size() == k) break;
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Return k most frequent elements in array in O(N) time.
// - Approach: Bucket Sort mapping frequency to list of elements.
// - Intuition: Maximum frequency is at most N; traversing frequency buckets in reverse yields top-k elements in O(N) without heap sorting.
// - Complexity: Time: O(N), Space: O(N).


// ----------------------------------------------------------------------------
// 137. Find Median from Data Stream (LeetCode 295)
// ----------------------------------------------------------------------------
class MedianFinder {
private:
    priority_queue<int> maxHeap; // stores smaller half of numbers
    priority_queue<int, vector<int>, greater<int>> minHeap; // stores larger half of numbers

public:
    MedianFinder() {}

    void addNum(int num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // Rebalance heaps so size difference is at most 1
        if ((int)maxHeap.size() > (int)minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if ((int)minHeap.size() > (int)maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
// Interview Explanation:
// - Problem Statement: Design data structure that supports adding numbers from a data stream and finding the median in O(1).
// - Approach: Two Heaps (Max-Heap for lower half, Min-Heap for upper half).
// - Intuition: Max-heap top gives largest of lower half, min-heap top gives smallest of upper half; median is directly accessible from heap tops.
// - Complexity: addNum: O(log N), findMedian: O(1), Space: O(N).
