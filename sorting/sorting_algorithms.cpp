/**
 * Essential Sorting Algorithms for OAs and Interviews
 *
 * This file contains implementations of core sorting methods, along with
 * common variations (Count Inversions, Quick Select) and custom comparators.
 *
 * Algorithms implemented:
 * 1. Bubble Sort, Selection Sort, Insertion Sort (O(N^2) basics)
 * 2. Merge Sort & Count Inversions (O(N log N) stable, divide-and-conquer)
 * 3. Quick Sort (Randomized Pivot) & Quick Select (O(N log N) / O(N) average)
 * 4. Heap Sort (O(N log N) in-place, unstable)
 * 5. Cycle Sort (O(N) for range [1, N] or [0, N]; very important for duplicates/missing numbers)
 * 6. Counting Sort (O(N + K) non-comparison based stable sort)
 * 7. Custom Comparators (Standard lambda, functors, and std::greater examples)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
#include <queue>

using namespace std;

// Helper function to print a vector
void printArray(const string &label, const vector<int> &arr)
{
    cout << label << ": [";
    for (size_t i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << (i == arr.size() - 1 ? "" : ", ");
    }
    cout << "]\n";
}

// =========================================================================
// 1. BASIC O(N^2) SORTS
// =========================================================================

/**
 * Bubble Sort:
 * - Repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order.
 * - Time: O(N^2) worst/average, O(N) best (using optimized swapped flag).
 * - Space: O(1) in-place.
 * - Stable: Yes.
 */
void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped by inner loop, then array is sorted.
        if (!swapped)
            break;
    }
}

/**
 * Selection Sort:
 * - Divides the input list into two parts: the sublist of items already sorted, and the sublist of items remaining to be sorted.
 * - Finds the minimum element from the unsorted part and puts it at the beginning.
 * - Time: O(N^2) all cases.
 * - Space: O(1) in-place.
 * - Stable: No (standard array implementation swaps elements across distance, breaking stability).
 */
void selectionSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIdx])
            {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

/**
 * Insertion Sort:
 * - Places an unsorted element at its suitable place in each iteration (like sorting playing cards).
 * - Highly efficient for small datasets or nearly sorted arrays.
 * - Time: O(N^2) worst/average, O(N) best case (already sorted).
 * - Space: O(1) in-place.
 * - Stable: Yes.
 */
void insertionSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        // Move elements of arr[0..i-1], that are greater than key,
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// =========================================================================
// 2. MERGE SORT & COUNT INVERSIONS
// =========================================================================

/**
 * Merge Sort (Standard Out-of-Place):
 * - A divide-and-conquer algorithm that divides the input array into two halves, calls itself for the two halves, and then merges the two sorted halves.
 * - Time: O(N log N) all cases.
 * - Space: O(N) auxiliary space.
 * - Stable: Yes.
 */
void merge(vector<int> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void mergeSortHelper(vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(vector<int> &arr)
{
    if (arr.empty())
        return;
    mergeSortHelper(arr, 0, arr.size() - 1);
}

/**
 * Count Inversions (Merge Sort Variation):
 * - Two elements a[i] and a[j] form an inversion if a[i] > a[j] and i < j.
 * - Crucial interview problem (e.g. measuring how close the array is to being sorted).
 * - Time: O(N log N)
 * - Space: O(N)
 */
long long mergeAndCount(vector<int> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    long long cnt = 0;

    // Count pairs L[i] > 2 * R[j]
    int j = 0;
    for (int i = 0; i < n1; i++)
    {
        while (j < n2 && (long long)L[i] > 2LL * R[j]) // if we want to count inversions instead of reverse pairs, change the condition to L[i] > R[j]
        {
            j++;
        }
        {
            j++;
        }
        cnt += j;
    }

    // Standard merge step
    int i = 0;
    j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    return cnt;
}
long long mergeSortAndCount(vector<int> &arr, int left, int right)
{
    long long count = 0;
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        count += mergeSortAndCount(arr, left, mid);
        count += mergeSortAndCount(arr, mid + 1, right);
        count += mergeAndCount(arr, left, mid, right);
    }
    return count;
}

long long countInversions(vector<int> arr)
{ // Pass by value to avoid modifying original array
    if (arr.empty())
        return 0;
    return mergeSortAndCount(arr, 0, arr.size() - 1);
}

// =========================================================================
// 3. QUICK SORT (RANDOMIZED) & QUICK SELECT
// =========================================================================

/**
 * Quick Sort (Randomized Pivot):
 * - Picks an element as pivot and partitions the array around it.
 * - Randomizing the pivot selection avoids worst-case O(N^2) performance on already sorted/reverse-sorted inputs.
 * - Time: O(N log N) average/best, O(N^2) worst case.
 * - Space: O(log N) recursive call stack.
 * - Stable: No.
 */
int partitionLomuto(vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

int randomizedPartition(vector<int> &arr, int low, int high)
{
    // Pick random index between low and high
    int randomIdx = low + rand() % (high - low + 1);
    swap(arr[randomIdx], arr[high]);
    return partitionLomuto(arr, low, high);
}

void quickSortHelper(vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int p = randomizedPartition(arr, low, high);
        quickSortHelper(arr, low, p - 1);
        quickSortHelper(arr, p + 1, high);
    }
}

void quickSort(vector<int> &arr)
{
    if (arr.empty())
        return;
    quickSortHelper(arr, 0, arr.size() - 1);
}

/**
 * Quick Select:
 * - Finds the k-th smallest element in an unordered list. (0-indexed, meaning k=0 is the minimum element).
 * - Time: O(N) average case, O(N^2) worst case.
 * - Space: O(1) auxiliary space (using tail recursion or iterative form).
 */
int quickSelectHelper(vector<int> &arr, int low, int high, int k)
{
    if (low == high)
        return arr[low];
    int p = randomizedPartition(arr, low, high);
    if (p == k)
    {
        return arr[p];
    }
    else if (p < k)
    {
        return quickSelectHelper(arr, p + 1, high, k);
    }
    else
    {
        return quickSelectHelper(arr, low, p - 1, k);
    }
}

int quickSelect(vector<int> arr, int k)
{ // Pass by value to preserve original array
    assert(k >= 0 && k < (int)arr.size());
    return quickSelectHelper(arr, 0, arr.size() - 1, k);
}

// =========================================================================
// 4. HEAP SORT
// =========================================================================

/**
 * Heap Sort:
 * - Comparison-based sorting technique based on Binary Heap data structure.
 * - Time: O(N log N) in all cases.
 * - Space: O(1) in-place.
 * - Stable: No.
 */
void heapify(vector<int> &arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int> &arr)
{
    int n = arr.size();
    // Build max heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }
    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        swap(arr[0], arr[i]);
        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// =========================================================================
// 5. CYCLE SORT (INTERVIEW FAVORITE PATTERN)
// =========================================================================

/**
 * Cycle Sort Pattern:
 * - Crucial for interview problems where numbers are in the range [1, N] or [0, N].
 * - Finds duplicates, missing numbers, or misplaced numbers in O(N) time and O(1) space.
 * - Places each element in its correct index by swapping.
 * - Time: O(N)
 * - Space: O(1)
 * - Stable: No.
 */
void cycleSort(vector<int> &arr)
{
    int i = 0;
    int n = arr.size();
    while (i < n)
    {
        // Assuming values are in range [1, N]
        // The correct index for value x is x - 1
        int correctIdx = arr[i] - 1;
        if (correctIdx >= 0 && correctIdx < n && arr[i] != arr[correctIdx])
        {
            swap(arr[i], arr[correctIdx]);
        }
        else
        {
            i++;
        }
    }
}

// =========================================================================
// 6. COUNTING SORT
// =========================================================================

/**
 * Counting Sort:
 * - Non-comparison sorting technique based on keys between a specific range.
 * - Very efficient when range of elements is small.
 * - Time: O(N + K), where K is the range of input values (maxVal - minVal + 1).
 * - Space: O(N + K).
 * - Stable: Yes (in the implemented output-array based form).
 */
void countingSort(vector<int> &arr)
{
    if (arr.empty())
        return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    vector<int> count(range, 0);
    for (int num : arr)
    {
        count[num - minVal]++;
    }

    // Cumulative sum to preserve stability
    for (int i = 1; i < range; i++)
    {
        count[i] += count[i - 1];
    }

    vector<int> output(arr.size());
    // Build output array in reverse order to maintain stability
    for (int i = arr.size() - 1; i >= 0; i--)
    {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }

    arr = output;
}

// =========================================================================
// 7. CUSTOM COMPARATORS
// =========================================================================

struct Item
{
    int value;
    int weight;
};

// 1. Comparator function for standard sorting
bool compareItems(const Item &a, const Item &b)
{
    // Primary: sort descending by value/weight ratio
    double ratioA = (double)a.value / a.weight;
    double ratioB = (double)b.value / b.weight;
    if (abs(ratioA - ratioB) > 1e-9)
    {
        return ratioA > ratioB;
    }
    // Secondary: sort ascending by weight
    return a.weight < b.weight;
}

// 2. Functor comparator (useful for std::set, std::map, std::priority_queue)
struct ItemFunctor
{
    bool operator()(const Item &a, const Item &b) const
    {
        // For a min-heap based on value (smallest value on top of priority_queue):
        // priority_queue elements are order-inverted compared to std::sort.
        // Return true if `a` should be placed after `b` (meaning `a` is less priority than `b`).
        return a.value > b.value;
    }
};

void demonstrateCustomComparators()
{
    cout << "\n--- Custom Comparators Demonstration ---\n";
    vector<Item> items = {{60, 10}, {100, 20}, {120, 30}, {60, 5}};

    // Using custom function comparator
    sort(items.begin(), items.end(), compareItems);
    cout << "Sorted by value/weight ratio (desc), then weight (asc):\n";
    for (const auto &item : items)
    {
        cout << "  Val: " << item.value << ", Wt: " << item.weight
             << " (Ratio: " << (double)item.value / item.weight << ")\n";
    }

    // Using Lambda expression
    vector<int> nums = {4, 2, 7, 1, 9};
    sort(nums.begin(), nums.end(), [](int a, int b)
         {
             return a > b; // Descending order
         });
    printArray("Lambda Descending Sort", nums);

    // Using std::greater
    sort(nums.begin(), nums.end(), greater<int>());
    printArray("std::greater Sort", nums);

    // Using Custom Functor in Priority Queue
    priority_queue<Item, vector<Item>, ItemFunctor> pq;
    pq.push({100, 20});
    pq.push({50, 10});
    pq.push({150, 30});
    cout << "Priority Queue (Custom Functor - Min Value Heap):\n";
    while (!pq.empty())
    {
        cout << "  Top Item Value: " << pq.top().value << "\n";
        pq.pop();
    }
}

// =========================================================================
// TEST RUNNER
// =========================================================================

int main()
{
    // Initialize random seed
    srand(time(NULL));

    cout << "=== RUNNING SORTING ALGORITHM TESTS ===\n\n";

    vector<int> original = {38, 27, 43, 3, 9, 82, 10};
    printArray("Original Input", original);

    // 1. Bubble Sort
    vector<int> test = original;
    bubbleSort(test);
    printArray("Bubble Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 2. Selection Sort
    test = original;
    selectionSort(test);
    printArray("Selection Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 3. Insertion Sort
    test = original;
    insertionSort(test);
    printArray("Insertion Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 4. Merge Sort
    test = original;
    mergeSort(test);
    printArray("Merge Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 5. Inversion Counting
    long long invs = countInversions(original);
    cout << "Inversion Count of Original Array: " << invs << "\n";

    // 6. Quick Sort
    test = original;
    quickSort(test);
    printArray("Quick Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 7. Quick Select (K-th smallest)
    // Sorted order: 3, 9, 10, 27, 38, 43, 82
    int k = 3; // 4th smallest element (index 3) -> should be 27
    int val = quickSelect(original, k);
    cout << "Quick Select (" << k << "-th smallest): " << val << " (Expected: 27)\n";
    assert(val == 27);

    // 8. Heap Sort
    test = original;
    heapSort(test);
    printArray("Heap Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // 9. Cycle Sort (Input range [1, N])
    vector<int> cycleInput = {3, 5, 2, 1, 4};
    printArray("Cycle Sort Input", cycleInput);
    cycleSort(cycleInput);
    printArray("Cycle Sort Result", cycleInput);
    assert(is_sorted(cycleInput.begin(), cycleInput.end()));

    // 10. Counting Sort
    test = original;
    countingSort(test);
    printArray("Counting Sort Result", test);
    assert(is_sorted(test.begin(), test.end()));

    // Custom Comparators Demo
    demonstrateCustomComparators();

    cout << "\nALL TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
