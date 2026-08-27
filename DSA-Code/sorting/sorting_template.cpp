/**
 * ====================================================================================================
 *                 ULTIMATE SORTING, BINARY SEARCH & GREEDY ALGORITHMS BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Core Sorting Algorithms, 1D/2D Binary Search,
 * Binary Search on Answer, Dutch National Flag, Boyer-Moore Voting, Sweep-Line, and Greedy Invariants.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vll = vector<ll>;

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
// SECTION 2: BINARY SEARCH MASTER TOOLKIT (1D & 2D)
// ====================================================================================================

// 1. Lower Bound & Upper Bound primitives
int getLowerBound(const vi &arr, int x) {
    return lower_bound(arr.begin(), arr.end(), x) - arr.begin(); // Smallest index with arr[idx] >= x
}
int getUpperBound(const vi &arr, int x) {
    return upper_bound(arr.begin(), arr.end(), x) - arr.begin(); // Smallest index with arr[idx] > x
}
pair<int, int> firstAndLastOccurrence(const vi &arr, int x) {
    int lb = lower_bound(arr.begin(), arr.end(), x) - arr.begin();
    int ub = upper_bound(arr.begin(), arr.end(), x) - arr.begin() - 1;
    if (lb < (int)arr.size() && arr[lb] == x) return {lb, ub};
    return {-1, -1};
}
// Interview Explanation:
// - Problem Statement: Find lower bound, upper bound, first and last occurrence, and total occurrences of target in a sorted array.
// - Approach: Binary Search bound queries.
// - Intuition: lower_bound finds first position where element is not less than x; upper_bound finds first strictly greater element.
// - Complexity: Time: O(log N), Space: O(1).


// 2. Search in Rotated Sorted Array I (Distinct elements)
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
// Interview Explanation:
// - Problem Statement: Search for target in a rotated sorted array of distinct elements.
// - Approach: Binary Search checking sorted half.
// - Intuition: At least one half [low...mid] or [mid...high] is always strictly sorted; check if target falls inside that sorted range.
// - Complexity: Time: O(log N), Space: O(1).


// 3. Search in Rotated Sorted Array II (Contains duplicates)
bool searchRotatedII(const vi &arr, int target) {
    int low = 0, high = (int)arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return true;
        if (arr[low] == arr[mid] && arr[mid] == arr[high]) {
            low++; high--; // Indeterminate half, shrink bounds
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
// Interview Explanation:
// - Problem Statement: Search for target in a rotated sorted array containing duplicates.
// - Approach: Binary Search with boundary trimming when arr[low] == arr[mid] == arr[high].
// - Intuition: When endpoints match mid, we cannot determine which half is sorted; safely advance low and decrement high.
// - Complexity: Time: O(log N) avg, O(N) worst case with all duplicate values, Space: O(1).


// 4. Find Minimum & Rotation Count in Rotated Sorted Array
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

int findRotationCount(const vi &arr) {
    int low = 0, high = (int)arr.size() - 1, mini = INT_MAX, ans = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[low] <= arr[high]) {
            if (arr[low] < mini) { mini = arr[low]; ans = low; }
            break;
        }
        if (arr[low] <= arr[mid]) {
            if (arr[low] < mini) { mini = arr[low]; ans = low; }
            low = mid + 1;
        } else {
            if (arr[mid] < mini) { mini = arr[mid]; ans = mid; }
            high = mid - 1;
        }
    }
    return ans; // Rotation count is the index of the minimum element
}
// Interview Explanation:
// - Problem Statement: Find the minimum value and total rotation count of a rotated sorted array.
// - Approach: Binary search tracking minimum candidate in sorted halves.
// - Intuition: In a sorted half, the minimum is always its first element; eliminate that half and explore the unsorted half.
// - Complexity: Time: O(log N), Space: O(1).


// 5. Single Element in a Sorted Array (Every other element appears twice)
int singleNonDuplicate(const vi &arr) {
    int n = arr.size();
    if (n == 1) return arr[0];
    if (arr[0] != arr[1]) return arr[0];
    if (arr[n - 1] != arr[n - 2]) return arr[n - 1];

    int low = 1, high = n - 2;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] != arr[mid - 1] && arr[mid] != arr[mid + 1]) return arr[mid];
        // Before unique element: (even, odd) index pairs match. After unique element: (odd, even) match.
        if ((mid % 2 == 1 && arr[mid] == arr[mid - 1]) || (mid % 2 == 0 && arr[mid] == arr[mid + 1])) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the unique element in a sorted array where every other element appears twice.
// - Approach: Binary search on index parity.
// - Intuition: Before the single element, pairs start at even indices: (0,1), (2,3); after the single element, the order flips to (odd, even).
// - Complexity: Time: O(log N), Space: O(1).


// 6. Find Peak Element (1D Local Maximum)
int findPeakElement(const vi &arr) {
    int n = arr.size();
    if (n == 1) return 0;
    if (arr[0] > arr[1]) return 0;
    if (arr[n - 1] > arr[n - 2]) return n - 1;

    int low = 1, high = n - 2;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) return mid;
        if (arr[mid] > arr[mid - 1]) low = mid + 1; // Climbing uphill, peak must exist on right
        else high = mid - 1; // Downhill, peak must exist on left
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find an index i such that arr[i] > arr[i-1] and arr[i] > arr[i+1].
// - Approach: Binary Search following gradient slope.
// - Intuition: If arr[mid] > arr[mid-1], the slope is increasing to the right; a peak is guaranteed to exist on the right side.
// - Complexity: Time: O(log N), Space: O(1).


// 7. K-th Missing Positive Number
int findKthPositive(vi &arr, int k) {
    int low = 0, high = (int)arr.size() - 1, idx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int missing = arr[mid] - (mid + 1);
        if (missing < k) {
            idx = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return k + idx + 1;
}
// Interview Explanation:
// - Problem Statement: Find the k-th positive integer missing from a strictly increasing array.
// - Approach: Binary search on the count of missing numbers before index mid (`arr[mid] - (mid + 1)`).
// - Intuition: Find the largest index with < k missing elements; the answer is directly computed as k + idx + 1.
// - Complexity: Time: O(log N), Space: O(1).


// 8. Search in 2D Matrix I (Row-major fully sorted) & II (Individually sorted rows and cols)
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
    int n = matrix.size(), m = matrix[0].size(), row = 0, col = m - 1; // Start top-right
    while (row < n && col >= 0) {
        if (matrix[row][col] == target) return true;
        if (matrix[row][col] > target) col--;
        else row++;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Search for target in (I) fully flattened sorted matrix, and (II) matrix with sorted rows and columns.
// - Approach: (I) Virtual 1D binary search. (II) Top-right staircase walk.
// - Intuition: (I) Index mid maps to (mid / m, mid % m). (II) Top-right corner acts as a binary search tree root; left is smaller, down is larger.
// - Complexity: (I) Time: O(log(N * M)), (II) Time: O(N + M), Space: O(1).


// 9. Find Peak Element II (2D Peak strictly greater than 4 neighbors)
vi findPeakElementII(const vector<vi> &matrix) {
    int n = matrix.size(), m = matrix[0].size(), low = 0, high = m - 1;
    while (low <= high) {
        int mid_col = low + (high - low) / 2, max_row = 0;
        for (int i = 0; i < n; i++) {
            if (matrix[i][mid_col] > matrix[max_row][mid_col]) max_row = i;
        }
        int left = (mid_col - 1 >= 0) ? matrix[max_row][mid_col - 1] : -1;
        int right = (mid_col + 1 < m) ? matrix[max_row][mid_col + 1] : -1;

        if (matrix[max_row][mid_col] > left && matrix[max_row][mid_col] > right) {
            return {max_row, mid_col};
        } else if (matrix[max_row][mid_col] < left) {
            high = mid_col - 1;
        } else {
            low = mid_col + 1;
        }
    }
    return {-1, -1};
}
// Interview Explanation:
// - Problem Statement: Find a 2D peak element in an N x M matrix strictly greater than top, bottom, left, right neighbors.
// - Approach: Binary Search on column indices + Global column maximum.
// - Intuition: Finding the column maximum guarantees it is greater than top and bottom neighbors; then binary search horizontally on left/right neighbors.
// - Complexity: Time: O(N \log M), Space: O(1).


// 10. Matrix Median (Row-wise sorted matrix with odd dimensions)
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
// Interview Explanation:
// - Problem Statement: Find the median element of a row-wise sorted matrix of odd dimensions.
// - Approach: Binary search on the answer range [min_val, max_val] with upper_bound counting.
// - Intuition: The median has at least (N * M + 1) / 2 elements <= median; monotonic count allows standard binary search on value range.
// - Complexity: Time: O(32 * N \log M), Space: O(1).


// ====================================================================================================
// SECTION 3: BINARY SEARCH ON ANSWER (PREDICATE / OPTIMIZATION)
// ====================================================================================================

// 1. Aggressive Cows (Maximize Minimum Distance)
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
            low = mid + 1; // Try to maximize distance
        } else {
            high = mid - 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Place k cows in stall coordinates such that the minimum distance between any two cows is maximized.
// - Approach: Binary Search on Answer [1, max_coordinate - min_coordinate].
// - Intuition: If we can place cows with separation >= mid, we can always place with smaller separation; greedily place cows left-to-right.
// - Complexity: Time: O(N \log N + N \log(\text{range})), Space: O(1).


// 2. House Robber IV (Minimize Maximum Capability)
bool canRobHouses(const vi &nums, int k, int capability) {
    int robbed = 0, n = nums.size();
    for (int i = 0; i < n; ) {
        if (nums[i] <= capability) {
            robbed++;
            i += 2; // Skip adjacent house
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
// Interview Explanation:
// - Problem Statement: Rob at least k non-adjacent houses such that the maximum house value robbed is minimized.
// - Approach: Binary Search on Answer over house values.
// - Intuition: For a given capability threshold, greedily rob every eligible house skipping its neighbor to maximize total robbed houses.
// - Complexity: Time: O(N \log(\text{range})), Space: O(1).


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
            swap(nums[mid], nums[high--]); // Do not advance mid; swapped element must be inspected
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array of 0s, 1s, and 2s in-place in a single pass.
// - Approach: Dutch National Flag 3-pointer partition (low, mid, high).
// - Intuition: Maintains invariants: [0..low-1] are 0s, [low..mid-1] are 1s, and [high+1..n-1] are 2s.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place.


// 2. Boyer-Moore Majority Element (N/2 and N/K Generalization)
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

vi majorityElementK(const vi &nums, int k) {
    unordered_map<int, int> candidates;
    for (int x : nums) {
        if (candidates.count(x)) candidates[x]++;
        else if ((int)candidates.size() < k - 1) candidates[x] = 1;
        else {
            vi toErase;
            for (auto &[num, cnt] : candidates) {
                if (--cnt == 0) toErase.push_back(num);
            }
            for (int num : toErase) candidates.erase(num);
        }
    }
    unordered_map<int, int> freq;
    for (int x : nums) if (candidates.count(x)) freq[x]++;
    vi ans;
    for (auto &[num, cnt] : freq) {
        if (cnt > (int)nums.size() / k) ans.push_back(num);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all elements that appear strictly more than n/2 (or n/k) times in an array.
// - Approach: Boyer-Moore Majority Voting Algorithm with at most k-1 candidates.
// - Intuition: Different elements cancel each other out; if an element appears > n/k times, it cannot be completely eliminated.
// - Complexity: Time: O(N \times K), Space: O(K) candidate map.


// ====================================================================================================
// SECTION 5: SWEEP LINE & INTERVAL SCHEDULING
// ====================================================================================================

// 1. Sweep-Line: Maximum Concurrent Events (Restaurant Customers / Meeting Rooms II)
int maxConcurrentEvents(const vector<pii> &intervals) {
    vector<pii> events;
    for (auto &[start, finish] : intervals) {
        events.push_back({start, 1});    // Arrival
        events.push_back({finish, -1});  // Departure
    }
    // If departure and arrival coincide, process departure first by sorting (-1 before 1)
    sort(events.begin(), events.end());
    int current = 0, maxConcurrent = 0;
    for (auto &[time, delta] : events) {
        current += delta;
        maxConcurrent = max(maxConcurrent, current);
    }
    return maxConcurrent;
}
// Interview Explanation:
// - Problem Statement: Find the maximum number of overlapping intervals / concurrent customers at any point in time.
// - Approach: Sweep-Line algorithm mapping intervals into arrival (+1) and departure (-1) events.
// - Intuition: Sorting events chronologically lets a running counter track active overlaps, recording the peak counter value.
// - Complexity: Time: O(N \log N) sorting, Space: O(N) events array.


// 2. Activity Selection: Maximum Non-Overlapping Meetings
int maxNonOverlappingMeetings(vector<pii> &meetings) {
    // Sort meetings strictly by finish time
    sort(meetings.begin(), meetings.end(), [](const pii &a, const pii &b) {
        return a.second < b.second;
    });
    int count = 0, lastEnd = -1;
    for (const auto &[start, finish] : meetings) {
        if (start > lastEnd) {
            count++;
            lastEnd = finish;
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Find the maximum number of meetings that can be scheduled in a single room without overlap.
// - Approach: Greedy Activity Selection sorting by earliest finish time.
// - Intuition: Choosing the meeting that finishes earliest leaves the maximal remaining time for future meetings.
// - Complexity: Time: O(N \log N), Space: O(1) auxiliary.


// 3. Job Sequencing with Deadlines via DSU (O(N * alpha(M)))
struct Job { char id; int deadline; int profit; };
struct DSU_Slots {
    vi parent;
    DSU_Slots(int n) : parent(n + 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int i) { return (parent[i] == i) ? i : (parent[i] = find(parent[i])); }
    void merge(int u, int v) { parent[v] = u; }
};

pair<int, int> jobSequencingDSU(vector<Job> &jobs) {
    sort(jobs.begin(), jobs.end(), [](const Job &a, const Job &b) { return a.profit > b.profit; });
    int maxDeadline = 0;
    for (const auto &j : jobs) maxDeadline = max(maxDeadline, j.deadline);

    DSU_Slots dsu(maxDeadline);
    int countJobs = 0, totalProfit = 0;
    for (const auto &job : jobs) {
        int availableSlot = dsu.find(job.deadline);
        if (availableSlot > 0) {
            dsu.merge(dsu.find(availableSlot - 1), availableSlot);
            countJobs++;
            totalProfit += job.profit;
        }
    }
    return {countJobs, totalProfit};
}
// Interview Explanation:
// - Problem Statement: Schedule jobs with deadlines and profits to maximize total profit, each job taking 1 unit of time.
// - Approach: Greedy sorting by descending profit + Disjoint Set Union (DSU) to find latest available slot <= deadline.
// - Intuition: Greedily place high-profit jobs at their latest possible valid slot; DSU points each filled slot to the next free left slot.
// - Complexity: Time: O(N \log N + N \alpha(\text{maxDeadline})), Space: O(\text{maxDeadline}) DSU table.


// ====================================================================================================
// SECTION 6: ADVANCED GREEDY & SEARCHING INVARIANTS
// ====================================================================================================

// 1. Meet-in-the-Middle (Partition Array to Minimize Difference - LeetCode 2035)
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
// Interview Explanation:
// - Problem Statement: Partition 2N integers into two arrays of size N minimizing the absolute difference of their sums.
// - Approach: Meet-in-the-Middle generation of subset sums + Binary Search (`lower_bound`).
// - Intuition: Splitting array into two halves of size N allows computing subset sums in O(2^N) instead of O(2^{2N}); binary search matches best halves.
// - Complexity: Time: O(N \cdot 2^N), Space: O(2^N).


// 2. Smallest Impossible Subset Sum (CSES Missing Coin Sum)
ll smallestImpossibleSubsetSum(vi coins) {
    sort(coins.begin(), coins.end());
    ll target = 1;
    for (int c : coins) {
        if (c > target) break;
        target += c;
    }
    return target;
}
// Interview Explanation:
// - Problem Statement: Find the smallest positive integer sum that cannot be formed by any subset of coin values.
// - Approach: Greedy sorting and prefix reachability invariant.
// - Intuition: If sorted coins can form every sum in [1, target-1], adding coin c extends reach to [1, target + c - 1] iff c <= target.
// - Complexity: Time: O(N \log N) sorting, Space: O(1).


// 3. Candy Distribution (LeetCode 135 - Two-Pass Slope Greedy)
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
// Interview Explanation:
// - Problem Statement: Distribute candies to children such that each child has at least 1, and higher rated children get more candies than neighbors.
// - Approach: Two-Pass Greedy (Left-to-Right and Right-to-Left).
// - Intuition: Forward pass satisfies left-neighbor condition; backward pass satisfies right-neighbor condition while taking max to satisfy both.
// - Complexity: Time: O(N) two linear passes, Space: O(N) candies array.


// 4. Longest Consecutive Sequence (LeetCode 128 - O(N) Hash Set)
int longestConsecutive(const vi &nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;
    for (int x : s) {
        // Only start sequence if x is the beginning (x-1 is not in set)
        if (!s.count(x - 1)) {
            int curr = x, len = 1;
            while (s.count(curr + 1)) { curr++; len++; }
            longest = max(longest, len);
        }
    }
    return longest;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest consecutive elements sequence in an unsorted array in O(N) time.
// - Approach: Hash Set lookup anchored at sequence beginnings (`!set.count(x - 1)`).
// - Intuition: Only numbers that start a streak are traversed; each number is visited at most twice (once in outer loop, once in inner loop).
// - Complexity: Time: O(N) average, Space: O(N) hash set.


// 5. CSES Reading Books Invariant
ll readingBooksTotalTime(const vi &books) {
    ll totalSum = 0, maxTime = 0;
    for (int t : books) {
        totalSum += t;
        maxTime = max(maxTime, (ll)t);
    }
    return max(2 * maxTime, totalSum);
}
// Interview Explanation:
// - Problem Statement: Find minimum total time for two people to read n books where each book can be read by only one person at a time.
// - Approach: Maximum element vs. sum of remaining elements comparison.
// - Intuition: If maxTime > sum - maxTime, one person is bottlenecked on the largest book while the other finishes all other books (taking 2 * maxTime); else totalSum.
// - Complexity: Time: O(N), Space: O(1).


// 6. Dynamic Street Intervals (CSES Traffic Lights)
vi trafficLights(int x, const vi &positions) {
    set<int> lights = {0, x};
    multiset<int> lengths = {x};
    vi ans;
    for (int p : positions) {
        auto it = lights.upper_bound(p);
        int r = *it, l = *prev(it);
        lengths.erase(lengths.find(r - l));
        lengths.insert(p - l);
        lengths.insert(r - p);
        lights.insert(p);
        ans.push_back(*lengths.rbegin());
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the longest street segment after each new traffic light is sequentially added.
// - Approach: Ordered set for light coordinates + multiset for active segment lengths.
// - Intuition: Upper_bound locates the enclosing interval [l, r]; remove old length r - l and insert split lengths p - l and r - p in O(log N).
// 7. CSES Collecting Numbers II (Inversion Tracking under Element Swaps)
bool isBadPair(int x, const vi &pos, int n) {
    if (x < 1 || x >= n) return false;
    return pos[x] > pos[x + 1];
}

vi collectingNumbersQueries(int n, vi a, const vector<pii> &swaps) {
    vi pos(n + 1);
    for (int i = 1; i <= n; i++) pos[a[i]] = i;
    int rounds = 1;
    for (int x = 1; x < n; x++) {
        if (pos[x] > pos[x + 1]) rounds++;
    }
    vi result;
    for (auto &[p, q] : swaps) {
        int u = a[p], v = a[q];
        set<int> affected = {u - 1, u, v - 1, v};
        for (int x : affected) rounds -= isBadPair(x, pos, n);
        swap(a[p], a[q]);
        swap(pos[u], pos[v]);
        for (int x : affected) rounds += isBadPair(x, pos, n);
        result.push_back(rounds);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Track the number of rounds needed to collect numbers 1..n sequentially when pairs of array elements are swapped dynamically.
// - Approach: Position array inversion checking before and after swap on affected pairs.
// - Intuition: Swapping elements at indices p and q only alters the relative order of adjacent values {u-1, u, v-1, v}; update rounds in O(1) per query.
// - Complexity: Time: O(N + Q), Space: O(N).

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

 2. BINARY SEARCH ON ANSWER (THE 3 TEMPLATES):
    • Minimize the Maximum (e.g. House Robber IV, Painter's Partition, Ship Packages):
      - Invariant: `check(mid) == true` means mid is feasible, so search left: `high = mid;` else `low = mid + 1;`
    • Maximize the Minimum (e.g. Aggressive Cows, Magnetic Force):
      - Invariant: `check(mid) == true` means mid is feasible, so record and search right: `ans = mid; low = mid + 1;`
    • Monotonic Property Check:
      - Always ensure `check(x)` is monotonic: F F F F T T T (first true) or T T T T F F F (last true).

 3. SWEEP-LINE & GREEDY RULES OF THUMB:
    • Concurrent Interval Overlap: Split into events `(start, +1)` and `(end, -1)`. If interval boundaries are
      touching (e.g. [1, 2] and [2, 3]), decide whether departure happens BEFORE arrival (use -1 before +1 in sort).
    • Activity Selection (Max Non-Overlapping): Always sort by END TIME (`finish`).
    • Interval Covering (Min Arrows to burst balloons): Always sort by END TIME (`finish`).
    • Job Sequencing with Deadlines: Sort by PROFIT descending; assign latest free slot using DSU.
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
