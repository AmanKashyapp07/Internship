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
#include <random>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Bubble Sort with Early Stop & Round Count   | Adjacent Swaps + Max Displacement | O(N^2)   | O(1)     |
 | 2  | Selection Sort (Min-Element Swap)           | Suffix Minimum Index Extraction   | O(N^2)   | O(1)     |
 | 3  | Insertion Sort (Array & Linked List)        | Shift Larger Prefix In-Place      | O(N + I) | O(1)     |
 | 4  | Merge Sort (Classic Divide & Conquer)       | Recursive 2-Way Merge Subroutine  | O(N logN)| O(N)     |
 | 5  | In-Place Merge Sort (Gap Method)            | Shell-like Gap Shelling           | O(N log^2N)| O(1)   |
 | 6  | QuickSort (Lomuto & Hoare Partitioning)     | Two-Way Pivot Partitioning        | O(N logN)| O(log N) |
 | 7  | Randomized QuickSort (Introsort Hybrid)     | Random Pivot Selection            | O(N logN)| O(log N) |
 | 8  | 3-Way QuickSort (Bentley-McIlroy DNF)       | Duplicates Pivot Segregation      | O(N logN)| O(log N) |
 | 9  | HeapSort (In-Place Max-Heapify)             | Sift-Down In-Place Max Extraction | O(N logN)| O(1)     |
 | 10 | ShellSort (Knuth's Gap Sequence)            | Diminishing Increment Insertion   | O(N^(3/2))| O(1)    |
 | 11 | TimSort Concept (Small Runs Insertion Merge)| Run Detection + Insertion + Merge | O(N logN)| O(N)     |
 | 12 | Counting Sort (Stable Frequency Table)      | Cumulative Prefix Sum Table       | O(N + K) | O(N + K) |
 | 13 | Radix Sort LSD (Base-10 Digit-by-Digit)     | Digit-by-Digit Stable Counting    | O(D(N+B))| O(N + B) |
 | 14 | Radix Sort MSD (Recursive Bucket Sort)      | Trie-like MSD Prefix Partitioning | O(D(N+B))| O(N + B) |
 | 15 | Bucket Sort (Uniform Real Range [0, 1))     | Scatter-Gather Bucketing + Sort   | O(N) avg | O(N)     |
 | 16 | Cyclic Sort [1 to N] (In-Place Swaps)       | Direct Index Mapping Swap         | O(N)     | O(1)     |
 | 17 | Pigeonhole Sort (Direct Range Slots)        | Range-Sized Bucket Placement      | O(N + R) | O(N + R) |
 | 18 | Pancake Sorting (LeetCode 969)              | Greedy Suffix Placement (2-Flips) | O(N^2)   | O(N)     |
 | 19 | Sort Colors / DNF 3-Way (LeetCode 75)       | In-Place 3-Pointer Partition      | O(N)     | O(1)     |
 | 20 | Sort List (LeetCode 148 — Merge on SLL)     | Fast/Slow Split + SLL Merge       | O(N logN)| O(log N) |
 | 21 | Largest Number from Concatenation (LC 179)  | Custom Transitive String Comp     | O(N logN)| O(N)     |
 | 22 | Wiggle Sort I & II (LeetCode 280 / 324)     | Greedy Swap / Median + DNF Index  | O(N)     | O(1)/O(N)|
 | 23 | Topological Sort vs Comparison Sort         | Kahn's In-Degree BFS DAG Sort     | O(V + E) | O(V + E) |
 | 24 | Patience Sorting & LIS (Greene's Theorem)   | Pile Insertion via lower_bound    | O(N logN)| O(N)     |
 | 25 | Sort Characters By Frequency (LeetCode 451) | Bucket Sorting on Frequency Count | O(N)     | O(N)     |
 | 26 | Sort Matrix Diagonally (LeetCode 1329)      | Hash Map Diagonal Bucketing       | O(MN logM)| O(M * N)|
 | 27 | Multi-Criteria Custom Sorting Lambda        | Multi-Field Tie-Breaking Predicate| O(N logN)| O(1)     |
 | 28 | Min Swaps to Sort Permutation               | Disjoint Cycle Decomposition      | O(N)     | O(N)     |
 | 29 | Count Inversions & Reverse Pairs (LC 493)   | Merge Sort Inversion Accumulator  | O(N logN)| O(N)     |
 | 30 | Sort Transformed Array (LeetCode 360)       | Parabola Concavity Two-Pointers   | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. BUBBLE SORT WITH EARLY STOP & ROUND COUNT
// ============================================================

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Array is already sorted
    }
}

int countBubbleSortRounds(const vector<int>& arr) {
    int n = arr.size();
    vector<pair<int, int>> indexed(n);
    for (int i = 0; i < n; i++) indexed[i] = {arr[i], i};
    sort(indexed.begin(), indexed.end());

    int maxLeftShift = 0;
    for (int sortedIdx = 0; sortedIdx < n; sortedIdx++) {
        int origIdx = indexed[sortedIdx].second;
        maxLeftShift = max(maxLeftShift, origIdx - sortedIdx);
    }
    return maxLeftShift + 1;
}
// Interview Explanation:
// - Problem Statement: Implement Bubble Sort with early termination and compute the minimum number of rounds required to sort an array.
// - Approach: Standard adjacent swapping with `swapped` boolean flag. Inversion displacement math gives round count in O(N log N).
// - Intuition: In each pass, every element moves at most 1 step to the left. The maximum distance any element needs to travel left (`orig_idx - sorted_idx`) dictates total rounds.
// - Complexity: Time: O(N^2) worst/avg, O(N) best; Space: O(1) auxiliary.

// ============================================================
// 2. SELECTION SORT
// ============================================================

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array by repeatedly finding the minimum element from the unsorted segment and placing it at the beginning.
// - Approach: Suffix Minimum Scan + Prefix Boundary Swap.
// - Intuition: Partition array into sorted prefix and unsorted suffix. Find minimum in unsorted suffix and swap with prefix head. Performs at most N - 1 swaps.
// - Complexity: Time: O(N^2) all cases, Space: O(1) auxiliary.

// ============================================================
// 3. INSERTION SORT (ARRAY & LINKED LIST)
// ============================================================

void insertionSortArray(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* insertionSortList(ListNode* head) {
    ListNode dummy(0);
    ListNode* curr = head;
    while (curr) {
        ListNode* prevNode = &dummy;
        ListNode* nextNode = curr->next;
        while (prevNode->next && prevNode->next->val < curr->val) {
            prevNode = prevNode->next;
        }
        curr->next = prevNode->next;
        prevNode->next = curr;
        curr = nextNode;
    }
    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Sort an array or singly linked list using Insertion Sort.
// - Approach: Maintain sorted prefix; take next element and shift all larger prefix elements right until proper slot is found.
// - Intuition: Adaptive algorithm: takes O(N + I) time where I is the number of inversions. Extremely fast for small (N < 32) or nearly-sorted datasets.
// - Complexity: Time: O(N^2) worst, O(N) best for sorted array; Space: O(1).

// ============================================================
// 4. MERGE SORT (CLASSIC DIVIDE & CONQUER)
// ============================================================

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> leftArr(n1), rightArr(n2);
    for (int i = 0; i < n1; i++) leftArr[i] = arr[l + i];
    for (int j = 0; j < n2; j++) rightArr[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);
    merge(arr, l, mid, r);
}
// Interview Explanation:
// - Problem Statement: Sort an array using divide-and-conquer Merge Sort.
// - Approach: Divide array into halves recursively, sort each half, and merge using two pointers.
// - Intuition: Subproblems are independent and equal in size. Merging two sorted lists takes linear time, leading to guaranteed O(N log N) runtime and stable order.
// - Complexity: Time: O(N log N) guaranteed, Space: O(N) auxiliary.

// ============================================================
// 5. IN-PLACE MERGE SORT (GAP METHOD)
// ============================================================

void inPlaceMerge(vector<int>& arr, int l, int m, int r) {
    int len = r - l + 1;
    int gap = (len + 1) / 2;
    while (gap > 0) {
        int i = l;
        while (i + gap <= r) {
            int j = i + gap;
            if (arr[i] > arr[j]) {
                swap(arr[i], arr[j]);
            }
            i++;
        }
        gap = (gap > 1) ? (gap + 1) / 2 : 0;
    }
}

void inPlaceMergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    inPlaceMergeSort(arr, l, mid);
    inPlaceMergeSort(arr, mid + 1, r);
    inPlaceMerge(arr, l, mid, r);
}
// Interview Explanation:
// - Problem Statement: Merge two sorted adjacent subarrays in-place without allocating O(N) extra buffer memory.
// - Approach: Shell's Gap Reduction Method across boundary.
// - Intuition: Treat the two halves as a single contiguous array and compare elements separated by `gap = ceil(len / 2)`. Halving the gap successively sorts the combined range in O(N log N) merge time.
// - Complexity: Time: O(N log^2 N), Space: O(1) auxiliary.

// ============================================================
// 6. QUICKSORT (LOMUTO & HOARE PARTITIONING)
// ============================================================

int lomutoPartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

int hoarePartition(vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int i = low - 1, j = high + 1;
    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

void quickSortLomuto(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = lomutoPartition(arr, low, high);
        quickSortLomuto(arr, low, pi - 1);
        quickSortLomuto(arr, pi + 1, high);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array using QuickSort with Lomuto and Hoare partitioning schemes.
// - Approach: Partition array around pivot so elements <= pivot are left and >= pivot are right.
// - Intuition: Lomuto uses single forward pointer with high pivot; Hoare uses two converging inward pointers with low pivot (doing ~3x fewer swaps than Lomuto on average).
// - Complexity: Time: O(N log N) average, O(N^2) worst case; Space: O(log N) stack.

// ============================================================
// 7. RANDOMIZED QUICKSORT (INTROSORT HYBRID CONCEPTS)
// ============================================================

int randomizedPartition(vector<int>& arr, int low, int high) {
    int randIdx = low + rand() % (high - low + 1);
    swap(arr[randIdx], arr[high]);
    return lomutoPartition(arr, low, high);
}

void randomizedQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}
// Interview Explanation:
// - Problem Statement: Mitigate O(N^2) worst-case adversary inputs on QuickSort using randomized pivot selection.
// - Approach: Swap randomly chosen pivot index with boundary prior to partitioning.
// - Intuition: Random pivot choice guarantees expected split balance regardless of input ordering, achieving O(N log N) with probability 1 - 1/N.
// - Complexity: Time: O(N log N) expected, Space: O(log N) recursion depth.

// ============================================================
// 8. 3-WAY QUICKSORT (BENTLEY-MCILROY / DUTCH NATIONAL FLAG)
// ============================================================

void quickSort3Way(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int pivot = arr[l];
    int lt = l, gt = r, i = l + 1;
    while (i <= gt) {
        if (arr[i] < pivot) swap(arr[lt++], arr[i++]);
        else if (arr[i] > pivot) swap(arr[i], arr[gt--]);
        else i++;
    }
    quickSort3Way(arr, l, lt - 1);
    quickSort3Way(arr, gt + 1, r);
}
// Interview Explanation:
// - Problem Statement: Sort array containing heavy duplicate elements in linear time O(N).
// - Approach: 3-way Dutch National Flag Partitioning into [< pivot], [== pivot], [> pivot].
// - Intuition: Bentley-McIlroy partition avoids recursing into duplicates subarray [lt...gt], solving duplicate-heavy arrays in O(N log K) time where K is unique keys.
// - Complexity: Time: O(N log N) avg, O(N) when all elements are identical; Space: O(log N).

// ============================================================
// 9. HEAPSORT (IN-PLACE MAX-HEAPIFY)
// ============================================================

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    // Build max heap in O(N)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // Extract max one by one in O(N log N)
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
// Interview Explanation:
// - Problem Statement: Sort array in-place with guaranteed O(N log N) runtime and O(1) auxiliary memory.
// - Approach: Build Max-Heap bottom-up in O(N), then repeatedly extract root to array end.
// - Intuition: In-place complete binary tree representation. Sift-down restores heap invariant in O(log N) after each root swap.
// - Complexity: Time: O(N log N) guaranteed in all cases, Space: O(1) auxiliary.

// ============================================================
// 10. SHELLSORT (KNUTH'S GAP SEQUENCE)
// ============================================================

void shellSort(vector<int>& arr) {
    int n = arr.size();
    // Knuth's gap sequence: h = 3*h + 1 (1, 4, 13, 40, 121...)
    int h = 1;
    while (h < n / 3) h = 3 * h + 1;

    while (h >= 1) {
        for (int i = h; i < n; i++) {
            int key = arr[i];
            int j = i;
            while (j >= h && arr[j - h] > key) {
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = key;
        }
        h /= 3;
    }
}
// Interview Explanation:
// - Problem Statement: Sort array by generalizing insertion sort to allow exchanges of distant elements.
// - Approach: Diminishing Gap Sequence Insertion Sort.
// - Intuition: By h-sorting for large h, array becomes nearly sorted rapidly, making final 1-sort run in nearly linear time.
// - Complexity: Time: O(N^(3/2)) with Knuth sequence, Space: O(1) auxiliary.

// ============================================================
// 11. TIMSORT CONCEPTS (RUN IDENTIFICATION + INSERTION MERGE)
// ============================================================

const int RUN_SIZE = 32;

void insertionSortRange(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void timSort(vector<int>& arr) {
    int n = arr.size();
    // Sort individual subarrays of size RUN_SIZE with insertion sort
    for (int i = 0; i < n; i += RUN_SIZE) {
        insertionSortRange(arr, i, min(i + RUN_SIZE - 1, n - 1));
    }
    // Merge sorted runs iteratively
    for (int size = RUN_SIZE; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}
// Interview Explanation:
// - Problem Statement: Implement core hybrid architecture of TimSort (the default sorting algorithm in Python, Java, and V8).
// - Approach: Chunk array into small contiguous runs (size 32/64), sort with Insertion Sort, and merge with balanced 2-way Merge.
// - Intuition: Takes advantage of natural pre-existing ordered sequences in real-world data; delivers O(N) on sorted data and stable O(N log N) worst-case.
// - Complexity: Time: O(N log N) worst/avg, O(N) best; Space: O(N) auxiliary.

// ============================================================
// 12. COUNTING SORT (STABLE FREQUENCY TABLE)
// ============================================================

vector<int> countingSort(const vector<int>& arr) {
    if (arr.empty()) return {};
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int k = maxVal - minVal + 1;

    vector<int> count(k, 0), output(arr.size());
    for (int x : arr) count[x - minVal]++;
    for (int i = 1; i < k; i++) count[i] += count[i - 1];
    // Traverse backwards to preserve stability
    for (int i = (int)arr.size() - 1; i >= 0; i--) {
        output[--count[arr[i] - minVal]] = arr[i];
    }
    return output;
}
// Interview Explanation:
// - Problem Statement: Sort integer array in linear time non-comparatively over bounded range [minVal, maxVal].
// - Approach: Frequency Count Accumulation + Prefix Sum Direct Slot Allocation.
// - Intuition: Prefix sums of counts give exact starting output indices for each element. Backwards iteration guarantees stable preservation of original order for identical keys.
// - Complexity: Time: O(N + K), Space: O(N + K) where K = maxVal - minVal + 1.

// ============================================================
// 13. RADIX SORT LSD (BASE-10 DIGIT-BY-DIGIT)
// ============================================================

void radixSortLSD(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());

    for (long long exp = 1; maxVal / exp > 0; exp *= 10) {
        vector<int> output(arr.size()), count(10, 0);
        for (int x : arr) count[(x / exp) % 10]++;
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
        for (int i = (int)arr.size() - 1; i >= 0; i--) {
            output[--count[(arr[i] / exp) % 10]] = arr[i];
        }
        arr = output;
    }
}
// Interview Explanation:
// - Problem Statement: Sort large integers without comparisons regardless of magnitude.
// - Approach: Least Significant Digit (LSD) Radix Sort with Counting Sort Subroutine.
// - Intuition: Sort stably by digits from right to left (1s, 10s, 100s...). By induction, sorting by higher-order digits while preserving stable lower-order ties yields a fully sorted array.
// - Complexity: Time: O(D * (N + B)), Space: O(N + B) where B = 10 (base) and D = digit count.

// ============================================================
// 14. RADIX SORT MSD (RECURSIVE BUCKET SORT)
// ============================================================

void msdRadixSortHelper(vector<string>& arr, int lo, int hi, int d, vector<string>& aux) {
    if (hi <= lo || d >= 100) return;
    const int R = 256;
    vector<int> count(R + 2, 0);

    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        count[c + 1]++;
    }
    for (int r = 0; r < R + 1; r++) count[r + 1] += count[r];
    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        aux[lo + count[c]++] = arr[i];
    }
    for (int i = lo; i <= hi; i++) arr[i] = aux[i];

    for (int r = 0; r < R; r++) {
        msdRadixSortHelper(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
    }
}
// Interview Explanation:
// - Problem Statement: Sort variable-length strings or high-precision integers lexicographically using Most Significant Digit (MSD) partitioning.
// - Approach: Recursive Trie-like MSD Bucket Partitioning.
// - Intuition: Partition into 256 character buckets based on the d-th character, then recursively sort each bucket independently.
// - Complexity: Time: O(Total Characters) = O(N * L), Space: O(N + R * Depth).

// ============================================================
// 15. BUCKET SORT (UNIFORM REAL RANGE [0, 1))
// ============================================================

void bucketSort(vector<float>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    vector<vector<float>> buckets(n);
    for (float x : arr) {
        int idx = min(n - 1, max(0, (int)(n * x)));
        buckets[idx].push_back(x);
    }
    int k = 0;
    for (int i = 0; i < n; i++) {
        sort(buckets[i].begin(), buckets[i].end());
        for (float x : buckets[i]) arr[k++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort elements uniformly distributed in range [0, 1) in expected linear time.
// - Approach: Scatter-Gather Bucket Sorting.
// - Intuition: Map elements to n uniform intervals. If distribution is uniform, each bucket has expected size O(1), making overall sorting O(N).
// - Complexity: Time: O(N) average, O(N^2) worst case; Space: O(N).

// ============================================================
// 16. CYCLIC SORT [1 TO N] (IN-PLACE SWAPS)
// ============================================================

void cyclicSort(vector<int>& nums) {
    int i = 0, n = nums.size();
    while (i < n) {
        int correctIdx = nums[i] - 1;
        if (nums[i] > 0 && nums[i] <= n && nums[i] != nums[correctIdx]) {
            swap(nums[i], nums[correctIdx]);
        } else {
            i++;
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort array containing integers in range [1, N] in-place in O(N) time with O(1) space.
// - Approach: Cyclic In-Place Index Mapping Swap.
// - Intuition: Number `x` belongs at index `x - 1`. Every swap moves at least one element to its permanent sorted index; at most N swaps occur across the entire loop.
// - Complexity: Time: O(N), Space: O(1) auxiliary.

// ============================================================
// 17. PIGEONHOLE SORT (DIRECT RANGE SLOTS)
// ============================================================

void pigeonholeSort(vector<int>& arr) {
    if (arr.empty()) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    vector<vector<int>> holes(range);
    for (int x : arr) holes[x - minVal].push_back(x);

    int idx = 0;
    for (int i = 0; i < range; i++) {
        for (int x : holes[i]) arr[idx++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort integer array where range of values R is close to the number of elements N.
// - Approach: Direct Pigeonhole / Slot Mapping.
// - Intuition: Direct array mapping from key to bucket index `val - minVal`, collecting items in order.
// - Complexity: Time: O(N + Range), Space: O(N + Range).

// ============================================================
// 18. PANCAKE SORTING — LeetCode 969
// ============================================================

vector<int> pancakeSort(vector<int>& arr) {
    vector<int> ans;
    for (int sz = arr.size(); sz > 1; sz--) {
        int maxIdx = max_element(arr.begin(), arr.begin() + sz) - arr.begin();
        if (maxIdx == sz - 1) continue;
        if (maxIdx > 0) {
            ans.push_back(maxIdx + 1);
            reverse(arr.begin(), arr.begin() + maxIdx + 1);
        }
        ans.push_back(sz);
        reverse(arr.begin(), arr.begin() + sz);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort array using prefix reversals (pancake flips) with <= 2*N flips (LeetCode 969).
// - Approach: Greedy Suffix Placement (At most 2 flips per element).
// - Intuition: For unsorted prefix of size `sz`, find max element. Flip to index 0, then flip whole range `sz` so max lands at its final position `sz - 1`.
// - Complexity: Time: O(N^2), Space: O(N) flips output.

// ============================================================
// 19. SORT COLORS / DUTCH NATIONAL FLAG — LeetCode 75
// ============================================================

void sortColors(vector<int>& nums) {
    int lo = 0, mid = 0, hi = (int)nums.size() - 1;
    while (mid <= hi) {
        if (nums[mid] == 0) swap(nums[lo++], nums[mid++]);
        else if (nums[mid] == 1) mid++;
        else swap(nums[mid], nums[hi--]);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array with 0s, 1s, and 2s in-place in a single pass (LeetCode 75).
// - Approach: Dijkstra's 3-Way Partitioning / Dutch National Flag.
// - Intuition: Maintain three boundary pointers: `lo` (0 boundary), `mid` (current), `hi` (2 boundary). Single pass partition puts 0s left, 1s mid, 2s right.
// - Complexity: Time: O(N), Space: O(1) in-place.

// ============================================================
// 20. SORT LIST (MERGE SORT ON SINGLY LINKED LIST) — LeetCode 148
// ============================================================

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) { tail->next = l1; l1 = l1->next; }
        else { tail->next = l2; l2 = l2->next; }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    // Find middle with fast/slow pointer
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* mid = slow->next;
    slow->next = nullptr; // Disconnect halves

    ListNode* left = sortList(head);
    ListNode* right = sortList(mid);
    return mergeTwoLists(left, right);
}
// Interview Explanation:
// - Problem Statement: Sort a singly linked list in O(N log N) time and O(log N) stack space (LeetCode 148).
// - Approach: Top-Down Divide-and-Conquer Merge Sort on Linked List.
// - Intuition: Split list using fast & slow pointers, disconnect halves, recursively sort, and merge with two pointers. Requires 0 array allocations.
// - Complexity: Time: O(N log N), Space: O(log N) recursion depth.

// ============================================================
// 21. LARGEST NUMBER FROM CONCATENATION — LeetCode 179
// ============================================================

string largestNumber(vector<int>& nums) {
    vector<string> arr;
    for (int x : nums) arr.push_back(to_string(x));

    sort(arr.begin(), arr.end(), [](const string& a, const string& b) {
        return a + b > b + a;
    });

    if (arr[0] == "0") return "0";
    string ans = "";
    for (const string& s : arr) ans += s;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Arrange list of non-negative integers such that their concatenation forms the largest number (LeetCode 179).
// - Approach: Custom Transitive Comparator Sorting (`a + b > b + a`).
// - Intuition: For strings A and B, if A+B > B+A, placing A before B always yields a larger concatenated value. Strict total order transitivity holds.
// - Complexity: Time: O(N log N * L), Space: O(N * L).

// ============================================================
// 22. WIGGLE SORT I & II — LeetCode 280 / 324
// ============================================================

void wiggleSortI(vector<int>& nums) {
    for (int i = 0; i < (int)nums.size() - 1; i++) {
        if ((i % 2 == 0 && nums[i] > nums[i + 1]) || (i % 2 == 1 && nums[i] < nums[i + 1])) {
            swap(nums[i], nums[i + 1]);
        }
    }
}

void wiggleSortII(vector<int>& nums) {
    int n = nums.size();
    auto midptr = nums.begin() + n / 2;
    nth_element(nums.begin(), midptr, nums.end());
    int mid = *midptr;

    // Virtual Indexing mapping: 1, 3, 5, ... 0, 2, 4 ...
    #define A(i) nums[(1 + 2 * (i)) % (n | 1)]

    int i = 0, j = 0, k = n - 1;
    while (j <= k) {
        if (A(j) > mid) swap(A(i++), A(j++));
        else if (A(j) < mid) swap(A(j), A(k--));
        else j++;
    }
    #undef A
}
// Interview Explanation:
// - Problem Statement: Reorder array into nums[0] < nums[1] > nums[2] < nums[3]... (Wiggle Sort II).
// - Approach: Quickselect Median + Virtual Indexing 3-Way DNF Partitioning.
// - Intuition: Elements > median go to odd indices (1, 3, 5...); elements < median go to even indices (0, 2, 4...). Virtual indexing allows in-place DNF partition.
// - Complexity: Time: O(N), Space: O(1) in-place.

// ============================================================
// 23. TOPOLOGICAL SORT VS COMPARISON SORT
// ============================================================

vector<int> topologicalSortKahn(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);
    for (auto& p : prerequisites) {
        adj[p[1]].push_back(p[0]);
        inDegree[p[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    return (int)order.size() == numCourses ? order : vector<int>();
}
// Interview Explanation:
// - Problem Statement: Order vertices of Directed Acyclic Graph (DAG) such that for every edge u -> v, u appears before v.
// - Approach: Kahn's BFS Algorithm with In-Degree Counting.
// - Intuition: Nodes with in-degree 0 have no prerequisites. Peeling them off reduces downstream in-degrees, discovering complete linear topological ordering or detecting cycles.
// - Complexity: Time: O(V + E), Space: O(V + E).

// ============================================================
// 24. PATIENCE SORTING & LIS (GREENE'S THEOREM)
// ============================================================

int patienceSortLISLength(const vector<int>& arr) {
    vector<int> piles;
    for (int x : arr) {
        auto it = lower_bound(piles.begin(), piles.end(), x);
        if (it == piles.end()) piles.push_back(x);
        else *it = x;
    }
    return piles.size();
}
// Interview Explanation:
// - Problem Statement: Determine the Longest Increasing Subsequence length using Patience Sorting card pile game.
// - Approach: Greedy Card Pile Placement + Binary Search (`std::lower_bound`).
// - Intuition: Place incoming card on the leftmost pile whose top card is >= current card. By Greene's Theorem, the minimum number of piles formed equals the LIS length.
// - Complexity: Time: O(N log N), Space: O(N).

// ============================================================
// 25. SORT CHARACTERS BY FREQUENCY — LeetCode 451
// ============================================================

string frequencySort(string s) {
    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    int maxFreq = s.size();
    vector<vector<char>> buckets(maxFreq + 1);
    for (auto& [c, count] : freq) {
        buckets[count].push_back(c);
    }

    string ans = "";
    for (int count = maxFreq; count >= 1; count--) {
        for (char c : buckets[count]) {
            ans.append(count, c);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort characters in string in decreasing order based on frequency of occurrence (LeetCode 451).
// - Approach: Bucket Sort on Frequency Array.
// - Intuition: Character frequencies range from 1 to N. Bucket sort by frequency avoids O(U log U) comparison sort and runs in pure O(N) linear time.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 26. SORT MATRIX DIAGONALLY — LeetCode 1329
// ============================================================

vector<vector<int>> diagonalSort(vector<vector<int>>& mat) {
    int m = mat.size(), n = mat[0].size();
    unordered_map<int, vector<int>> diagonals; // key = i - j

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            diagonals[i - j].push_back(mat[i][j]);
        }
    }
    for (auto& [key, vec] : diagonals) {
        sort(vec.rbegin(), vec.rend()); // Sort descending so we can pop_back in O(1)
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = diagonals[i - j].back();
            diagonals[i - j].pop_back();
        }
    }
    return mat;
}
// Interview Explanation:
// - Problem Statement: Sort each diagonal of an m x n matrix ascending from top-left to bottom-right (LeetCode 1329).
// - Approach: Hash Map Diagonal Grouping with Key `i - j`.
// - Intuition: All cells on the same diagonal share invariant `i - j`. Collect elements per diagonal, sort, and write back in order.
// - Complexity: Time: O(M * N * log(min(M, N))), Space: O(M * N).

// ============================================================
// 27. MULTI-CRITERIA CUSTOM SORTING LAMBDA
// ============================================================

struct Student {
    string name;
    int score;
    int age;
};

void sortStudentsCustom(vector<Student>& students) {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score; // Primary: Score descending
        if (a.age != b.age) return a.age < b.age;         // Secondary: Age ascending
        return a.name < b.name;                           // Tertiary: Name lexicographical
    });
}
// Interview Explanation:
// - Problem Statement: Sort records under complex multi-key criteria with priority tie-breakers.
// - Approach: C++ Lambda Comparator with Strict Weak Ordering.
// - Intuition: Cascade comparison checks through hierarchy of keys. Preserves strict weak ordering (irreflexive, asymmetric, transitive).
// - Complexity: Time: O(N log N), Space: O(1) auxiliary.

// ============================================================
// 28. MIN SWAPS TO SORT PERMUTATION (CYCLE DECOMPOSITION)
// ============================================================

int minSwapsToSort(vector<int>& arr) {
    int n = arr.size();
    vector<pair<int, int>> pos(n);
    for (int i = 0; i < n; i++) pos[i] = {arr[i], i};
    sort(pos.begin(), pos.end());

    vector<bool> visited(n, false);
    int totalSwaps = 0;

    for (int i = 0; i < n; i++) {
        if (visited[i] || pos[i].second == i) continue;

        int cycleSize = 0;
        int j = i;
        while (!visited[j]) {
            visited[j] = true;
            j = pos[j].second;
            cycleSize++;
        }
        if (cycleSize > 1) {
            totalSwaps += (cycleSize - 1);
        }
    }
    return totalSwaps;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of arbitrary element swaps required to sort an array.
// - Approach: Permutation Graph Disjoint Cycle Decomposition.
// - Intuition: Build directed graph where each element points to its sorted index. Any disjoint cycle of length L requires exactly L - 1 swaps. Total swaps = Sum(L_i - 1) = N - Number of Cycles.
// - Complexity: Time: O(N log N), Space: O(N).

// ============================================================
// 29. COUNT INVERSIONS & REVERSE PAIRS — LeetCode 493
// ============================================================

int mergeAndCountReversePairs(vector<int>& nums, int l, int mid, int r) {
    int count = 0;
    int j = mid + 1;
    for (int i = l; i <= mid; i++) {
        while (j <= r && (long long)nums[i] > 2LL * nums[j]) {
            j++;
        }
        count += (j - (mid + 1));
    }
    inplace_merge(nums.begin() + l, nums.begin() + mid + 1, nums.begin() + r + 1);
    return count;
}

int reversePairsHelper(vector<int>& nums, int l, int r) {
    if (l >= r) return 0;
    int mid = l + (r - l) / 2;
    int count = reversePairsHelper(nums, l, mid) + reversePairsHelper(nums, mid + 1, r);
    count += mergeAndCountReversePairs(nums, l, mid, r);
    return count;
}

int reversePairs(vector<int>& nums) {
    return reversePairsHelper(nums, 0, (int)nums.size() - 1);
}
// Interview Explanation:
// - Problem Statement: Count the number of reverse pairs where i < j and nums[i] > 2 * nums[j] (LeetCode 493).
// - Approach: Modified Merge Sort Counting Subroutine.
// - Intuition: Since left and right halves are sorted, two pointers can count all valid pairs (i, j) in linear O(N) time before merging.
// - Complexity: Time: O(N log N), Space: O(N) merge buffer.

// ============================================================
// 30. SORT TRANSFORMED ARRAY — LeetCode 360
// ============================================================

vector<int> sortTransformedArray(vector<int>& nums, int a, int b, int c) {
    auto f = [&](int x) { return a * x * x + b * x + c; };
    int n = nums.size();
    vector<int> res(n);
    int i = 0, j = n - 1;

    if (a >= 0) {
        // Parabola opens upwards: maximums are at extremities
        int k = n - 1;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 >= v2) { res[k--] = v1; i++; }
            else { res[k--] = v2; j--; }
        }
    } else {
        // Parabola opens downwards: minimums are at extremities
        int k = 0;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 <= v2) { res[k++] = v1; i++; }
            else { res[k++] = v2; j--; }
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Given sorted array and coefficients a, b, c, apply f(x) = ax^2 + bx + c and return sorted array in O(N) time (LeetCode 360).
// - Approach: Parabola Convexity / Concavity + Two Pointers.
// - Intuition: If a >= 0 (convex U-shape), maximum values lie at outer endpoints; fill from back. If a < 0 (concave inverted U-shape), minimum values lie at outer endpoints; fill from front.
// - Complexity: Time: O(N), Space: O(N) output array.
