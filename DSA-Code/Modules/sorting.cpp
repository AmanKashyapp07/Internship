/**
 * ====================================================================================================
 *                 ULTIMATE SORTING, BINARY SEARCH & GREEDY ALGORITHMS BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Core Sorting Algorithms, 1D/2D Binary Search,
 * Median of Two Sorted Arrays, Binary Search on Answer, Dutch National Flag, Boyer-Moore Voting,
 * Sweep-Line, Merge/Insert/Non-overlapping Intervals, and Greedy Invariants.
 */

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
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
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;


// ====================================================================================================
// SECTION 1: CLASSIC SORTING ALGORITHMS
// ====================================================================================================

// 1. Bubble Sort (O(N^2) worst/avg, O(N) best, Stable, In-place)
void bubbleSort(vi &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array by repeatedly swapping adjacent out-of-order elements.
// - Approach: Bubble Sort with early termination flag.
// - Intuition: Each pass bubbles the largest unsorted element to its correct position at the end; if no swaps occur, array is sorted.
// - Complexity: Time: O(N^2) worst/avg, O(N) best case, Space: O(1) in-place.


// 2. Selection Sort (O(N^2) all cases, Unstable, In-place)
void selectionSort(vi &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        swap(arr[i], arr[minIdx]);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array by repeatedly identifying the minimum remaining element.
// - Approach: Selection Sort scanning unsorted suffix.
// - Intuition: Divides array into sorted prefix and unsorted suffix, appending the minimum element of the suffix to the prefix.
// - Complexity: Time: O(N^2) all cases, Space: O(1) in-place.


// 3. Insertion Sort (O(N^2) worst/avg, O(N) best, Stable, In-place)
void insertionSort(vi &arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array by inserting each element into its correct relative position within the sorted prefix.
// - Approach: Insertion Sort via backwards element shifting.
// - Intuition: Mimics sorting playing cards; extremely fast on small (N <= 32) or nearly-sorted datasets.
// - Complexity: Time: O(N^2) worst, O(N) best, Space: O(1) in-place.


// 4. Merge Sort & Inversion Counting (O(N log N), Stable, O(N) Space)
ll mergeAndCount(vi &arr, int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    vi L(arr.begin() + left, arr.begin() + mid + 1);
    vi R(arr.begin() + mid + 1, arr.begin() + right + 1);

    ll invCount = 0;
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
            invCount += (n1 - i); // All remaining elements in L[i..n1-1] are > R[j]
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    return invCount;
}

ll mergeSortCount(vi &arr, int left, int right) {
    ll count = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        count += mergeSortCount(arr, left, mid);
        count += mergeSortCount(arr, mid + 1, right);
        count += mergeAndCount(arr, left, mid, right);
    }
    return count;
}

ll countInversions(vi arr) {
    return arr.empty() ? 0 : mergeSortCount(arr, 0, arr.size() - 1);
}
// Interview Explanation:
// - Problem Statement: Sort an array and count the number of inversions (pairs i < j with arr[i] > arr[j]).
// - Approach: Divide-and-conquer Merge Sort with cross-inversion accumulation.
// - Intuition: When merging two sorted halves, if R[j] < L[i], then R[j] is smaller than all remaining elements in L, contributing n1 - i inversions.
// - Complexity: Time: O(N \log N), Space: O(N) auxiliary space.


// 5. Quick Sort (Randomized Pivot, O(N log N) avg, O(N^2) worst, Unstable)
int partitionLomuto(vi &arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) swap(arr[++i], arr[j]);
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

int randomizedPartition(vi &arr, int low, int high) {
    int randomIdx = low + rand() % (high - low + 1);
    swap(arr[randomIdx], arr[high]);
    return partitionLomuto(arr, low, high);
}

void quickSort(vi &arr, int low, int high) {
    if (low < high) {
        int p = randomizedPartition(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array in-place using randomized divide-and-conquer partitioning.
// - Approach: Randomized Quick Sort with Lomuto partitioning.
// - Intuition: Randomizing the pivot choice avoids adversarial O(N^2) performance on already sorted/reverse-sorted inputs.
// - Complexity: Time: O(N \log N) average, O(N^2) worst case, Space: O(\log N) recursion stack.


// 6. Quick Select (K-th Smallest Element, O(N) average)
int quickSelect(vi &arr, int low, int high, int k) {
    if (low == high) return arr[low];
    int p = randomizedPartition(arr, low, high);
    if (p == k) return arr[p];
    if (p < k) return quickSelect(arr, p + 1, high, k);
    return quickSelect(arr, low, p - 1, k);
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element in an unordered list in O(N) average time.
// - Approach: Quick Select (pruning one partition branch at each step).
// - Intuition: Only recurses into the partition containing the target rank k, reducing the recurrence to T(N) = T(N/2) + O(N) = O(N).
// - Complexity: Time: O(N) average, O(N^2) worst case, Space: O(1) auxiliary (tail recursive).


// 7. Heap Sort (O(N log N) all cases, In-place, Unstable)
void heapify(vi &arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vi &arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array in-place without recursion overhead using a binary max-heap.
// - Approach: In-place Max-Heapify and root-extraction.
// - Intuition: Build a max-heap in O(N) time; repeatedly swap root (maximum) to the end and heapify remaining elements.
// - Complexity: Time: O(N \log N) all cases, Space: O(1) in-place.


// 8. Cycle Sort (O(N) time, O(1) space for [1, N] array range)
void cycleSort(vi &arr) {
    int i = 0, n = arr.size();
    while (i < n) {
        int correctIdx = arr[i] - 1;
        if (correctIdx >= 0 && correctIdx < n && arr[i] != arr[correctIdx]) {
            swap(arr[i], arr[correctIdx]);
        } else {
            i++;
        }
    }
}
// Interview Explanation:
// - Problem Statement: Place each number in range [1, N] into its correct 0-indexed position arr[x-1] == x in O(N) time.
// - Approach: Cyclic Swapping to resolve displacement permutations.
// - Intuition: Each swap places at least one element in its correct final spot; maximum N swaps total across the entire pass.
// - Complexity: Time: O(N), Space: O(1) in-place.


// 9. Counting Sort (O(N + K) non-comparison stable sort)
void countingSort(vi &arr) {
    if (arr.empty()) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    vi count(range, 0);
    for (int num : arr) count[num - minVal]++;
    for (int i = 1; i < range; i++) count[i] += count[i - 1];

    vi output(arr.size());
    for (int i = (int)arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }
    arr = output;
}
// Interview Explanation:
// - Problem Statement: Sort integers with bounded range [minVal, maxVal] in linear time.
// - Approach: Counting sort with prefix sum stability preservation.
// - Intuition: Computes frequencies and cumulative counts, populating output backwards to maintain stability of duplicate values.
// - Complexity: Time: O(N + K) where K = maxVal - minVal + 1, Space: O(N + K).


// ====================================================================================================
// SECTION 2: BINARY SEARCH MASTER TOOLKIT (1D, 2D & DUAL ARRAYS)
// ====================================================================================================

// 1. Lower Bound & Upper Bound primitives
int getLowerBound(const vi &arr, int x) {
    return lower_bound(arr.begin(), arr.end(), x) - arr.begin();
}
int getUpperBound(const vi &arr, int x) {
    return upper_bound(arr.begin(), arr.end(), x) - arr.begin();
}
pair<int, int> firstAndLastOccurrence(const vi &arr, int x) {
    int lb = lower_bound(arr.begin(), arr.end(), x) - arr.begin();
    int ub = upper_bound(arr.begin(), arr.end(), x) - arr.begin() - 1;
    if (lb < (int)arr.size() && arr[lb] == x) return {lb, ub};
    return {-1, -1};
}
// Interview Explanation:
// - Problem Statement: Find lower bound, upper bound, first and last occurrence of target in a sorted array.
// - Approach: Binary Search bound queries.
// - Intuition: lower_bound finds first position where element is >= x; upper_bound finds first strictly > x element.
// - Complexity: Time: O(log N), Space: O(1).


// 2. Median of Two Sorted Arrays (LeetCode 4)
double findMedianSortedArrays(const vi &nums1, const vi &nums2) {
    if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1); // Ensure nums1 is smaller
    int n1 = nums1.size(), n2 = nums2.size();
    int low = 0, high = n1;

    while (low <= high) {
        int cut1 = (low + high) >> 1;
        int cut2 = (n1 + n2 + 1) / 2 - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int r1 = (cut1 == n1) ? INT_MAX : nums1[cut1];
        int r2 = (cut2 == n2) ? INT_MAX : nums2[cut2];

        if (l1 <= r2 && l2 <= r1) {
            if ((n1 + n2) % 2 == 0)
                return (max(l1, l2) + min(r1, r2)) / 2.0;
            else
                return max(l1, l2);
        } else if (l1 > r2) {
            high = cut1 - 1;
        } else {
            low = cut1 + 1;
        }
    }
    return 0.0;
}
// Interview Explanation:
// - Problem Statement: Find the median of two sorted arrays nums1 and nums2 in O(log(min(N, M))) time (LeetCode 4).
// - Approach: Binary Search on the partition index cut1 of the smaller array.
// - Intuition: Partition both arrays such that left half contains (N+M+1)/2 elements; valid partition requires `l1 <= r2` and `l2 <= r1`.
// - Complexity: Time: O(\log(\min(N_1, N_2))), Space: O(1).


// 3. K-th Element of Two Sorted Arrays
int kthElementSortedArrays(const vi &nums1, const vi &nums2, int k) {
    int n1 = nums1.size(), n2 = nums2.size();
    if (n1 > n2) return kthElementSortedArrays(nums2, nums1, k);

    int low = max(0, k - n2), high = min(k, n1);
    while (low <= high) {
        int cut1 = (low + high) >> 1;
        int cut2 = k - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int r1 = (cut1 == n1) ? INT_MAX : nums1[cut1];
        int r2 = (cut2 == n2) ? INT_MAX : nums2[cut2];

        if (l1 <= r2 && l2 <= r1) {
            return max(l1, l2);
        } else if (l1 > r2) {
            high = cut1 - 1;
        } else {
            low = cut1 + 1;
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element from two combined sorted arrays.
// - Approach: Binary Search on partition range [max(0, k - n2), min(k, n1)].
// - Intuition: Generalization of median of two sorted arrays where left partition holds exactly k elements.
// - Complexity: Time: O(\log(\min(N_1, N_2))), Space: O(1).


// 4. Search in Rotated Sorted Array I (Distinct elements)
int searchRotatedI(const vi &arr, int target) {
    int low = 0, high = (int)arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        if (arr[low] <= arr[mid]) {
            if (target >= arr[low] && target < arr[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (target > arr[mid] && target <= arr[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return -1;
}

// 5. Search in Rotated Sorted Array II (Contains duplicates)
bool searchRotatedII(const vi &arr, int target) {
    int low = 0, high = (int)arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return true;
        if (arr[low] == arr[mid] && arr[mid] == arr[high]) {
            low++; high--;
            continue;
        }
        if (arr[low] <= arr[mid]) {
            if (target >= arr[low] && target < arr[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (target > arr[mid] && target <= arr[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return false;
}

// 6. Find Minimum in Rotated Sorted Array
int findMinRotated(const vi &arr) {
    int low = 0, high = (int)arr.size() - 1, ans = INT_MAX;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[low] <= arr[high]) { ans = min(ans, arr[low]); break; }
        if (arr[low] <= arr[mid]) {
            ans = min(ans, arr[low]);
            low = mid + 1;
        } else {
            ans = min(ans, arr[mid]);
            high = mid - 1;
        }
    }
    return ans;
}

// 7. Single Element in a Sorted Array
int singleNonDuplicate(const vi &arr) {
    int n = arr.size();
    if (n == 1) return arr[0];
    if (arr[0] != arr[1]) return arr[0];
    if (arr[n - 1] != arr[n - 2]) return arr[n - 1];

    int low = 1, high = n - 2;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] != arr[mid - 1] && arr[mid] != arr[mid + 1]) return arr[mid];
        if ((mid % 2 == 1 && arr[mid] == arr[mid - 1]) || (mid % 2 == 0 && arr[mid] == arr[mid + 1])) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// 8. Find Peak Element (1D Local Maximum)
int findPeakElement(const vi &arr) {
    int n = arr.size();
    if (n == 1) return 0;
    if (arr[0] > arr[1]) return 0;
    if (arr[n - 1] > arr[n - 2]) return n - 1;

    int low = 1, high = n - 2;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) return mid;
        if (arr[mid] > arr[mid - 1]) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// 9. Search in 2D Matrix I & II
bool searchMatrixI(const vector<vi> &matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    int n = matrix.size(), m = matrix[0].size(), low = 0, high = n * m - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int val = matrix[mid / m][mid % m];
        if (val == target) return true;
        if (val < target) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

bool searchMatrixII(const vector<vi> &matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    int n = matrix.size(), m = matrix[0].size(), row = 0, col = m - 1;
    while (row < n && col >= 0) {
        if (matrix[row][col] == target) return true;
        if (matrix[row][col] > target) col--;
        else row++;
    }
    return false;
}

// 10. Matrix Median
int matrixMedian(const vector<vi> &matrix) {
    int n = matrix.size(), m = matrix[0].size();
    int target_count = (n * m + 1) / 2;
    int low = INT_MAX, high = INT_MIN;
    for (int i = 0; i < n; i++) {
        low = min(low, matrix[i][0]);
        high = max(high, matrix[i][m - 1]);
    }
    int ans = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int count = 0;
        for (int i = 0; i < n; i++) {
            count += upper_bound(matrix[i].begin(), matrix[i].end(), mid) - matrix[i].begin();
        }
        if (count >= target_count) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}


// ====================================================================================================
// SECTION 3: BINARY SEARCH ON ANSWER (PREDICATE / OPTIMIZATION)
// ====================================================================================================

// 1. Split Array Largest Sum / Capacity To Ship Packages (LeetCode 410 / 1011)
bool canSplitArray(const vi &nums, int k, ll maxSum) {
    int count = 1;
    ll currentSum = 0;
    for (int x : nums) {
        if (currentSum + x > maxSum) {
            count++;
            currentSum = x;
            if (count > k) return false;
        } else {
            currentSum += x;
        }
    }
    return true;
}

int splitArray(const vi &nums, int k) {
    ll low = *max_element(nums.begin(), nums.end());
    ll high = accumulate(nums.begin(), nums.end(), 0LL);
    ll ans = high;
    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (canSplitArray(nums, k, mid)) {
            ans = mid;
            high = mid - 1; // Minimize maximum subarray sum
        } else {
            low = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Split array nums into k contiguous subarrays such that the largest sum of any subarray is minimized (LeetCode 410).
// - Approach: Binary Search on Answer over range [max(nums), sum(nums)].
// - Intuition: If max sum `mid` allows partitioning into <= k subarrays, try smaller thresholds `high = mid - 1`.
// - Complexity: Time: O(N \log(\sum - \max)), Space: O(1).


// 2. Aggressive Cows (Maximize Minimum Distance)
bool canPlaceCows(const vi &stalls, int k, int dist) {
    int cows = 1, lastPos = stalls[0];
    for (size_t i = 1; i < stalls.size(); i++) {
        if (stalls[i] - lastPos >= dist) {
            cows++;
            lastPos = stalls[i];
            if (cows >= k) return true;
        }
    }
    return false;
}

int aggressiveCows(vi stalls, int k) {
    sort(stalls.begin(), stalls.end());
    int low = 1, high = stalls.back() - stalls.front(), ans = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (canPlaceCows(stalls, k, mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

// 3. House Robber IV (Minimize Maximum Capability)
bool canRobHouses(const vi &nums, int k, int capability) {
    int robbed = 0, n = nums.size();
    for (int i = 0; i < n; ) {
        if (nums[i] <= capability) {
            robbed++;
            i += 2;
        } else {
            i++;
        }
    }
    return robbed >= k;
}

int minCapability(const vi &nums, int k) {
    int low = *min_element(nums.begin(), nums.end());
    int high = *max_element(nums.begin(), nums.end());
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (canRobHouses(nums, k, mid)) high = mid;
        else low = mid + 1;
    }
    return low;
}


// ====================================================================================================
// SECTION 4: PARTITIONING & VOTING ALGORITHMS
// ====================================================================================================

// 1. Dutch National Flag (Sort 0s, 1s, 2s in one pass)
void sortColors(vi &nums) {
    int low = 0, mid = 0, high = (int)nums.size() - 1;
    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low++], nums[mid++]);
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high--]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array with 0s, 1s, and 2s in-place in a single pass (LeetCode 75).
// - Approach: Dutch National Flag Algorithm (3-way partitioning with low, mid, high pointers).
// - Intuition: Invariants: [0..low-1] are 0s, [low..mid-1] are 1s, [high+1..n-1] are 2s. Process `nums[mid]` and shrink unsorted partition [mid..high].
// - Complexity: Time: O(N) single pass, Space: O(1) in-place.

// 2. Boyer-Moore Majority Element
int majorityElementHalf(const vi &nums) {
    int candidate = nums[0], count = 1;
    for (size_t i = 1; i < nums.size(); i++) {
        if (nums[i] == candidate) count++;
        else count--;
        if (count == 0) {
            candidate = nums[i];
            count = 1;
        }
    }
    return candidate;
}


// ====================================================================================================
// SECTION 5: SWEEP LINE & INTERVAL ALGORITHMS
// ====================================================================================================

// 1. Merge Overlapping Intervals (LeetCode 56)
vector<vi> mergeIntervals(vector<vi> &intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vector<vi> merged;
    for (const auto &interval : intervals) {
        if (merged.empty() || merged.back()[1] < interval[0]) {
            merged.push_back(interval);
        } else {
            merged.back()[1] = max(merged.back()[1], interval[1]);
        }
    }
    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals into non-overlapping interval ranges (LeetCode 56).
// - Approach: Sorting by start time + linear scan merging.
// - Intuition: Sorting ensures intervals can only overlap with the last merged interval; extend `merged.back()[1]` if `interval[0] <= merged.back()[1]`.
// - Complexity: Time: O(N \log N), Space: O(N).


// 2. Insert Interval (LeetCode 57)
vector<vi> insertInterval(vector<vi> &intervals, vi newInterval) {
    vector<vi> result;
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
    // 3. Add all remaining intervals after newInterval
    while (i < n) {
        result.push_back(intervals[i++]);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into a sorted non-overlapping intervals list and merge if necessary (LeetCode 57).
// - Approach: 3-Phase Single Pass (Left non-overlapping, Merging overlap, Right non-overlapping).
// - Intuition: Accumulate min start and max end during overlapping phase, inserting modified `newInterval` in linear time without full re-sorting.
// - Complexity: Time: O(N), Space: O(N).


// 3. Non-overlapping Intervals / Minimum Removals (LeetCode 435)
int eraseOverlapIntervals(vector<vi> &intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const vi &a, const vi &b) {
        return a[1] < b[1]; // Sort by finish time
    });
    int count = 0, prevEnd = intervals[0][1];
    for (size_t i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < prevEnd) {
            count++; // Overlap detected, remove current interval
        } else {
            prevEnd = intervals[i][1];
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of intervals to remove to make the rest non-overlapping (LeetCode 435).
// - Approach: Greedy Activity Selection sorting by end time.
// - Intuition: Retaining the interval that ends earliest leaves maximum room for future intervals, minimizing required removals.
// - Complexity: Time: O(N \log N), Space: O(1).


// 4. Sweep-Line: Maximum Concurrent Events (Meeting Rooms II)
int maxConcurrentEvents(const vector<pii> &intervals) {
    vector<pii> events;
    for (auto &[start, finish] : intervals) {
        events.push_back({start, 1});
        events.push_back({finish, -1});
    }
    sort(events.begin(), events.end());
    int current = 0, maxConcurrent = 0;
    for (auto &[time, delta] : events) {
        current += delta;
        maxConcurrent = max(maxConcurrent, current);
    }
    return maxConcurrent;
}


// ====================================================================================================
// SECTION 6: ADVANCED GREEDY & SEARCHING INVARIANTS
// ====================================================================================================

// 1. Meet-in-the-Middle (LeetCode 2035)
int minPartitionDifference(vi nums) {
    int n = nums.size() / 2;
    vi left(nums.begin(), nums.begin() + n), right(nums.begin() + n, nums.end());
    vector<vll> leftSums(n + 1), rightSums(n + 1);

    for (int mask = 0; mask < (1 << n); mask++) {
        ll sumL = 0, sumR = 0;
        int cnt = __builtin_popcount(mask);
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) { sumL += left[i]; sumR += right[i]; }
        }
        leftSums[cnt].push_back(sumL);
        rightSums[cnt].push_back(sumR);
    }
    for (int i = 0; i <= n; i++) sort(rightSums[i].begin(), rightSums[i].end());

    ll total = accumulate(nums.begin(), nums.end(), 0LL), ans = LLONG_MAX;
    for (int lCnt = 0; lCnt <= n; lCnt++) {
        int rCnt = n - lCnt;
        for (ll lSum : leftSums[lCnt]) {
            ll target = total / 2 - lSum;
            auto &vec = rightSums[rCnt];
            auto it = lower_bound(vec.begin(), vec.end(), target);
            if (it != vec.end()) ans = min(ans, llabs(total - 2 * (lSum + *it)));
            if (it != vec.begin()) ans = min(ans, llabs(total - 2 * (lSum + *prev(it))));
        }
    }
    return ans;
}

// 2. Smallest Impossible Subset Sum
ll smallestImpossibleSubsetSum(vi coins) {
    sort(coins.begin(), coins.end());
    ll target = 1;
    for (int c : coins) {
        if (c > target) break;
        target += c;
    }
    return target;
}

// 3. Candy Distribution (LeetCode 135)
int candy(const vi &ratings) {
    int n = ratings.size();
    vi candies(n, 1);
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1]) candies[i] = candies[i - 1] + 1;
    }
    for (int i = n - 2; i >= 0; i--) {
        if (ratings[i] > ratings[i + 1]) candies[i] = max(candies[i], candies[i + 1] + 1);
    }
    return accumulate(candies.begin(), candies.end(), 0);
}

// 4. Longest Consecutive Sequence (LeetCode 128)
int longestConsecutive(const vi &nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;
    for (int x : s) {
        if (!s.count(x - 1)) {
            int curr = x, len = 1;
            while (s.count(curr + 1)) { curr++; len++; }
            longest = max(longest, len);
        }
    }
    return longest;
}

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: SORTING, SEARCHING & GREEDY
 ====================================================================================================

 1. SORTING ALGORITHMS PROPERTIES & COMPLEXITY MATRIX:
    -------------------------------------------------------------------------------------------------
    Algorithm       | Best Time   | Avg Time    | Worst Time  | Space       | Stable | In-Place
    -------------------------------------------------------------------------------------------------
    Bubble Sort     | O(N)        | O(N^2)      | O(N^2)      | O(1)        | Yes    | Yes
    Selection Sort  | O(N^2)      | O(N^2)      | O(N^2)      | O(1)        | No     | Yes
    Insertion Sort  | O(N)        | O(N^2)      | O(N^2)      | O(1)        | Yes    | Yes
    Merge Sort      | O(N log N)  | O(N log N)  | O(N log N)  | O(N)        | Yes    | No
    Quick Sort      | O(N log N)  | O(N log N)  | O(N^2)      | O(log N)    | No     | Yes
    Heap Sort       | O(N log N)  | O(N log N)  | O(N log N)  | O(1)        | No     | Yes
    Cycle Sort      | O(N)        | O(N)        | O(N)        | O(1)        | No     | Yes
    Counting Sort   | O(N + K)    | O(N + K)    | O(N + K)    | O(N + K)    | Yes    | No
    -------------------------------------------------------------------------------------------------

 2. BINARY SEARCH ON ANSWER & DUAL ARRAYS:
    • Median / K-th Element of 2 Sorted Arrays: Binary search cut1 in smaller array; cut2 = (N+M+1)/2 - cut1.
    • Minimize Maximum Subarray Sum (Split Array / Ship Packages): Binary search range [max, sum].
    • Maximize Minimum Distance (Aggressive Cows): Binary search range [1, max_stalls].

 3. INTERVAL ALGORITHMS MATRIX:
    • Merge Overlapping: Sort by START time -> merge if `next.start <= prev.end`.
    • Insert Interval: 3-phase pass (Left non-overlap, Overlap min/max merge, Right non-overlap).
    • Minimum Removals (Non-overlapping): Sort by END time -> greedily retain earliest ending interval.
 ====================================================================================================
*/


// ====================================================================================================
// SECTION: ARRAYS, HASHING & TWO POINTER INTERVIEW SUITE
// ====================================================================================================

// 1. Two Sum (LeetCode 1)
vi twoSum(const vi& nums, int target) {
    unordered_map<int, int> seen;
    for (int i = 0; i < (int)nums.size(); ++i) {
        int complement = target - nums[i];
        if (seen.count(complement)) return {seen[complement], i};
        seen[nums[i]] = i;
    }
    return {};
}

// 2. Group Anagrams (LeetCode 49)
vvi groupAnagrams(vector<string>& strs) {
    unordered_map<string, vi> map;
    for (int i = 0; i < (int)strs.size(); ++i) {
        string key = strs[i];
        sort(key.begin(), key.end());
        map[key].push_back(i);
    }
    vvi res;
    for (auto& [_, indices] : map) {
        vi group;
        for (int idx : indices) group.push_back(idx);
        res.push_back(group);
    }
    return res;
}

// 3. Product of Array Except Self (LeetCode 238 - O(1) Extra Space)
vi productExceptSelf(const vi& nums) {
    int n = nums.size();
    vi res(n, 1);
    int prefix = 1;
    for (int i = 0; i < n; ++i) {
        res[i] = prefix;
        prefix *= nums[i];
    }
    int suffix = 1;
    for (int i = n - 1; i >= 0; --i) {
        res[i] *= suffix;
        suffix *= nums[i];
    }
    return res;
}

// 4. Find All Duplicates in Array (LeetCode 442 - In-Place Sign Marking)
vi findDuplicates(vi& nums) {
    vi duplicates;
    for (int i = 0; i < (int)nums.size(); ++i) {
        int index = abs(nums[i]) - 1;
        if (nums[index] < 0) duplicates.push_back(index + 1);
        else nums[index] = -nums[index];
    }
    return duplicates;
}

// 5. Valid Sudoku (LeetCode 36)
bool isValidSudoku(const vector<vector<char>>& board) {
    int rowMask[9] = {0}, colMask[9] = {0}, boxMask[9] = {0};
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == '.') continue;
            int bit = 1 << (board[r][c] - '1');
            int b = (r / 3) * 3 + (c / 3);
            if ((rowMask[r] & bit) || (colMask[c] & bit) || (boxMask[b] & bit)) return false;
            rowMask[r] |= bit;
            colMask[c] |= bit;
            boxMask[b] |= bit;
        }
    }
    return true;
}

// 6. Rotate Array by K (LeetCode 189 - Triple Reverse)
void rotateArray(vi& nums, int k) {
    int n = nums.size();
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

// 7. Container With Most Water (LeetCode 11)
int maxArea(const vi& height) {
    int left = 0, right = (int)height.size() - 1, maxWater = 0;
    while (left < right) {
        int currentWater = min(height[left], height[right]) * (right - left);
        maxWater = max(maxWater, currentWater);
        if (height[left] < height[right]) left++;
        else right--;
    }
    return maxWater;
}

// 8. 3Sum (LeetCode 15)
vvi threeSum(vi& nums) {
    vvi res;
    int n = nums.size();
    sort(nums.begin(), nums.end());
    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                res.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++; right--;
            } else if (sum < 0) left++;
            else right--;
        }
    }
    return res;
}

// 9. 4Sum (LeetCode 18)
vvi fourSum(vi& nums, int target) {
    vvi res;
    int n = nums.size();
    sort(nums.begin(), nums.end());
    for (int i = 0; i < n; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1; j < n; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;
            int left = j + 1, right = n - 1;
            while (left < right) {
                ll sum = (ll)nums[i] + nums[j] + nums[left] + nums[right];
                if (sum == target) {
                    res.push_back({nums[i], nums[j], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++; right--;
                } else if (sum < target) left++;
                else right--;
            }
        }
    }
    return res;
}

// 10. Shortest Unsorted Continuous Subarray (LeetCode 581)
int findUnsortedSubarray(const vi& nums) {
    int n = nums.size(), maxSeen = INT_MIN, minSeen = INT_MAX;
    int rightBound = -1, leftBound = -1;
    for (int i = 0; i < n; ++i) {
        maxSeen = max(maxSeen, nums[i]);
        if (nums[i] < maxSeen) rightBound = i;
    }
    for (int i = n - 1; i >= 0; --i) {
        minSeen = min(minSeen, nums[i]);
        if (nums[i] > minSeen) leftBound = i;
    }
    return (rightBound == -1) ? 0 : (rightBound - leftBound + 1);
}

// 11. Time Based Key-Value Store (LeetCode 981)
class TimeMap {
    unordered_map<string, vector<pair<int, string>>> store;
public:
    TimeMap() {}
    void set(string key, string value, int timestamp) {
        store[key].push_back({timestamp, value});
    }
    string get(string key, int timestamp) {
        if (!store.count(key)) return "";
        auto& list = store[key];
        int low = 0, high = (int)list.size() - 1, bestIdx = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (list[mid].first <= timestamp) {
                bestIdx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return (bestIdx == -1) ? "" : list[bestIdx].second;
    }
};


// ====================================================================================================
// SECTION: STRIVER SDE SHEET ARRAYS & TWO POINTERS (CATEGORY 1)
// ====================================================================================================

// 1. Pascal's Triangle (LeetCode 118)
vvi generatePascalTriangle(int numRows) {
    vvi triangle(numRows);
    for (int i = 0; i < numRows; i++) {
        triangle[i].resize(i + 1, 1);
        for (int j = 1; j < i; j++) {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    }
    return triangle;
}
// Interview Explanation:
// - Problem Statement: Generate the first numRows of Pascal's Triangle (LeetCode 118).
// - Approach: Dynamic Programming row-by-row simulation.
// - Intuition: Boundary elements are 1; interior element `triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j]`.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space (excluding returned triangle).

// 2. Merge Two Sorted Arrays Without Extra Space (LeetCode 88)
void mergeSortedArrays(vi& nums1, int m, vi& nums2, int n) {
    int p1 = m - 1, p2 = n - 1, p = m + n - 1;
    while (p1 >= 0 && p2 >= 0) {
        if (nums1[p1] > nums2[p2]) {
            nums1[p--] = nums1[p1--];
        } else {
            nums1[p--] = nums2[p2--];
        }
    }
    while (p2 >= 0) nums1[p--] = nums2[p2--];
}
// Interview Explanation:
// - Problem Statement: Merge sorted array nums2 into nums1 in-place in non-decreasing order (LeetCode 88).
// - Approach: Three Pointers working backwards from index m + n - 1.
// - Intuition: Populating largest elements from the back avoids overwriting elements in nums1 that haven't been processed yet.
// - Complexity: Time: O(M + N), Space: O(1) in-place.

// 4. Find the Duplicate Number (LeetCode 287)
int findDuplicateNumber(const vi& nums) {
    int slow = nums[0], fast = nums[0];
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}
// Interview Explanation:
// - Problem Statement: Find duplicate number in array of n+1 integers in range [1, n] in O(1) extra space without modifying array (LeetCode 287).
// - Approach: Floyd's Tortoise and Hare (Cycle Detection on Functional Graph `i -> nums[i]`).
// - Intuition: Duplicate value creates multiple incoming edges to the same node, forming a cycle entrance. Phase 1 finds collision point; Phase 2 finds cycle entrance.
// - Complexity: Time: O(N), Space: O(1) in-place.

// 5. Pow(x, n) - Binary Exponentiation (LeetCode 50)
double myPow(double x, int n) {
    long long N = n;
    if (N < 0) {
        x = 1.0 / x;
        N = -N;
    }
    double ans = 1.0;
    while (N > 0) {
        if (N & 1) ans *= x;
        x *= x;
        N >>= 1;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Calculate x raised to power n (LeetCode 50).
// - Approach: Binary Exponentiation (Exponent squaring).
// - Intuition: Halve power N at each step while squaring base x; multiply `ans` when lowest bit of N is 1. Handles negative N using `1.0 / x` and `long long` to prevent `INT_MIN` overflow.
// - Complexity: Time: O(\log N), Space: O(1).

// 6. Reverse Pairs (LeetCode 493)
int mergeCountReversePairs(vi& nums, int low, int mid, int high) {
    int count = 0, j = mid + 1;
    for (int i = low; i <= mid; i++) {
        while (j <= high && (ll)nums[i] > 2LL * nums[j]) j++;
        count += (j - (mid + 1));
    }

    vi temp;
    int left = low, right = mid + 1;
    while (left <= mid && right <= high) {
        if (nums[left] <= nums[right]) temp.push_back(nums[left++]);
        else temp.push_back(nums[right++]);
    }
    while (left <= mid) temp.push_back(nums[left++]);
    while (right <= high) temp.push_back(nums[right++]);
    for (int i = low; i <= high; i++) nums[i] = temp[i - low];
    return count;
}

int countReversePairs(vi& nums, int low, int high) {
    if (low >= high) return 0;
    int mid = low + (high - low) / 2;
    int count = countReversePairs(nums, low, mid) + countReversePairs(nums, mid + 1, high);
    count += mergeCountReversePairs(nums, low, mid, high);
    return count;
}

int reversePairs(vi& nums) {
    return countReversePairs(nums, 0, (int)nums.size() - 1);
}
// Interview Explanation:
// - Problem Statement: Return number of reverse pairs where `i < j` and `nums[i] > 2 * nums[j]` (LeetCode 493).
// - Approach: Modified Merge Sort with two-pointer condition counting.
// - Intuition: During merge sort, both left and right halves are sorted. Two pointers count valid pairs `nums[i] > 2 * nums[j]` in O(N) per merge step before standard merging.
// - Complexity: Time: O(N \log N), Space: O(N) temporary merge array.

// 7. Largest Subarray with 0 Sum (GFG / Striver SDE #22)
int maxLenZeroSum(const vi& arr) {
    unordered_map<ll, int> prefixMap;
    ll sum = 0;
    int maxLen = 0;
    for (int i = 0; i < (int)arr.size(); i++) {
        sum += arr[i];
        if (sum == 0) maxLen = i + 1;
        else if (prefixMap.count(sum)) maxLen = max(maxLen, i - prefixMap[sum]);
        else prefixMap[sum] = i;
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of largest subarray with sum equal to 0.
// - Approach: Prefix Sum with Hash Map storing earliest index of each prefix sum.
// - Intuition: If prefix sum `sum` repeats at index `i` (earliest seen at index `j`), subarray `[j+1..i]` sums to 0 with length `i - j`.
// - Complexity: Time: O(N), Space: O(N).

// 8. Remove Duplicates from Sorted Array (LeetCode 26)
int removeDuplicates(vi& nums) {
    if (nums.empty()) return 0;
    int k = 1;
    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] != nums[i - 1]) {
            nums[k++] = nums[i];
        }
    }
    return k;
}
// Interview Explanation:
// - Problem Statement: Remove duplicates from sorted array in-place and return number of unique elements (LeetCode 26).
// - Approach: Two Pointers (Slow write pointer `k`, Fast read pointer `i`).
// - Intuition: Whenever `nums[i] != nums[i-1]`, copy unique element to `nums[k]` and increment `k`.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place.

// 9. Max Consecutive Ones (LeetCode 485)
int findMaxConsecutiveOnes(const vi& nums) {
    int maxCount = 0, curr = 0;
    for (int x : nums) {
        if (x == 1) {
            curr++;
            maxCount = max(maxCount, curr);
        } else {
            curr = 0;
        }
    }
    return maxCount;
}
// Interview Explanation:
// - Problem Statement: Find maximum number of consecutive 1s in a binary array (LeetCode 485).
// - Approach: Single-pass running counter.
// - Intuition: Increment count on 1 and maximize result; reset counter on 0.
// - Complexity: Time: O(N), Space: O(1).


// ====================================================================================================
// SECTION: STRIVER SDE SHEET GREEDY ALGORITHMS (CATEGORY 3)
// ====================================================================================================

// 1. N Meetings in One Room (GFG / Striver SDE #43)
int maxMeetings(vi& start, vi& end) {
    int n = start.size();
    vector<pii> meetings(n);
    for (int i = 0; i < n; i++) meetings[i] = {end[i], start[i]};
    sort(meetings.begin(), meetings.end());

    int count = 0, lastEnd = -1;
    for (auto& [e, s] : meetings) {
        if (s > lastEnd) {
            count++;
            lastEnd = e;
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Maximize number of non-overlapping meetings in a single meeting room.
// - Approach: Activity Selection / Greedy Scheduling by earliest end time.
// - Intuition: Greedily pick meeting with earliest end time to free up the room as early as possible for subsequent meetings.
// - Complexity: Time: O(N \log N) sorting, Space: O(N).

// 2. Minimum Platforms Required for Railway (GFG / Striver SDE #44)
int findPlatform(vi& arr, vi& dep) {
    sort(arr.begin(), arr.end());
    sort(dep.begin(), dep.end());

    int n = arr.size(), platforms = 0, maxPlatforms = 0;
    int i = 0, j = 0;

    while (i < n && j < n) {
        if (arr[i] <= dep[j]) {
            platforms++;
            maxPlatforms = max(maxPlatforms, platforms);
            i++;
        } else {
            platforms--;
            j++;
        }
    }
    return maxPlatforms;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of platforms required at railway station to prevent train collisions.
// - Approach: Two Pointers on independently sorted arrival and departure times.
// - Intuition: When arrival time <= departure time, an overlapping train requires an extra platform (`platforms++`). Otherwise, a platform is vacated (`platforms--`).
// - Complexity: Time: O(N \log N) sorting, Space: O(1) auxiliary space.

// 3. Job Sequencing Problem with Deadlines (GFG / Striver SDE #45)
struct Job {
    int id, dead, profit;
};

pair<int, int> jobSequencing(vector<Job>& jobs) {
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.profit > b.profit;
    });

    int maxDeadline = 0;
    for (const auto& j : jobs) maxDeadline = max(maxDeadline, j.dead);

    vi slot(maxDeadline + 1, -1);
    int totalProfit = 0, countJobs = 0;

    for (const auto& j : jobs) {
        for (int t = j.dead; t > 0; t--) {
            if (slot[t] == -1) {
                slot[t] = j.id;
                countJobs++;
                totalProfit += j.profit;
                break;
            }
        }
    }
    return {countJobs, totalProfit};
}
// Interview Explanation:
// - Problem Statement: Maximize total profit scheduling jobs with unit time duration and deadlines.
// - Approach: Greedy allocation on latest available slot.
// - Intuition: Sort jobs descending by profit. Greedily schedule highest profit job at the latest possible empty slot <= deadline to reserve earlier slots for tighter deadlines.
// - Complexity: Time: O(N \log N + N \cdot D), Space: O(D).

// 4. Fractional Knapsack (GFG / Striver SDE #46)
struct Item {
    int value, weight;
};

double fractionalKnapsack(int W, vector<Item>& items) {
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return (double)a.value / a.weight > (double)b.value / b.weight;
    });

    double totalVal = 0.0;
    int currentWeight = 0;

    for (const auto& item : items) {
        if (currentWeight + item.weight <= W) {
            currentWeight += item.weight;
            totalVal += item.value;
        } else {
            int remain = W - currentWeight;
            totalVal += ((double)item.value / item.weight) * remain;
            break;
        }
    }
    return totalVal;
}
// Interview Explanation:
// - Problem Statement: Maximize total value in knapsack of capacity W allowing fractional items.
// - Approach: Greedy sorting by value-to-weight ratio.
// - Intuition: Greedily pick items with highest value-per-weight density; take whole item if capacity allows, otherwise take exact fractional fraction remaining.
// - Complexity: Time: O(N \log N) sorting, Space: O(1) auxiliary space.


// ====================================================================================================
// SECTION: STRIVER SDE SHEET BINARY SEARCH (CATEGORY 5)
// ====================================================================================================

// 1. N-th Root of an Integer (GFG / Striver SDE #61)
int nthRoot(int n, int m) {
    int low = 1, high = m;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        ll power = 1;
        bool overflow = false;
        for (int i = 0; i < n; i++) {
            power *= mid;
            if (power > m) { overflow = true; break; }
        }

        if (!overflow && power == m) return mid;
        if (overflow || power > m) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find integer n-th root of integer m (return -1 if non-integer).
// - Approach: Binary Search on Answer in range [1, m].
// - Intuition: Function mid^n monotonically increases with mid. Binary search for value satisfying mid^n == m, guarding against integer overflow during power multiplication.
// - Complexity: Time: O(N \cdot \log M), Space: O(1).

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
