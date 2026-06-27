/**
 * C++ STL Multiset Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  An associative container that stores elements in sorted order, allowing duplicate values.
 *  Implemented internally as a self-balancing Binary Search Tree (typically a Red-Black Tree).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ insert()         │ O(log N)         │ Inserts element                          │
 * │ erase(val)       │ O(log N + K)     │ Erases ALL K copies of val (DANGER!)     │
 * │ erase(iterator)  │ O(1) amortized   │ Erases ONLY the single pointed element   │
 * │ find(val)        │ O(log N)         │ Returns iterator to the FIRST instance   │
 * │ count(val)       │ O(log N + K)     │ Can be LINEAR in duplicates (avoid!)     │
 * │ lower_bound()    │ O(log N)         │ Iterator to first element >= target      │
 * │ upper_bound()    │ O(log N)         │ Iterator to first element > target       │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Sliding Window Median (using iterator tracking or double heaps)
 *  - Range query problems with duplicates (e.g. interval booking/matching)
 *
 * Interview Tricks & Pitfalls:
 *  - THE ERASE BUG (EXTREMELY IMPORTANT): Calling `ms.erase(val)` removes *all* occurrences of `val`.
 *    To remove only a single occurrence, find its iterator first and erase that iterator:
 *    `auto it = ms.find(val); if (it != ms.end()) ms.erase(it);`
 *  - THE COUNT BUG: Calling `ms.count(val)` is NOT O(log N). It is O(log N + K) where K is the number
 *    of duplicate occurrences. If all elements in the multiset are equal, this degrades to O(N).
 *    To check if `val` exists, always use `ms.find(val) != ms.end()` which is strictly O(log N).
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";
    multiset<int> ms = {5, 2, 8, 5, 2, 5}; // Initializer list

    cout << "Multiset size: " << ms.size() << "\n"; // 6
    cout << "Sorted elements: ";
    for (int x : ms) {
        cout << x << " ";
    }
    // Output: 2 2 5 5 5 8
    cout << "\n\n";
}

/*==========================================================
=            2. SAFE SEARCH & THE ERASE PITFALL
==========================================================*/

void safeSearchAndErase() {
    cout << "--- 2. SAFE SEARCH & THE ERASE PITFALL ---\n";
    multiset<int> ms = {2, 5, 5, 5, 8};

    // 1. SAFE SEARCH (Avoid ms.count(x))
    auto find_it = ms.find(5);
    if (find_it != ms.end()) {
        cout << "Found 5 in multiset (first occurrence pointer)\n";
    }

    // 2. THE ERASE PITFALL
    multiset<int> ms_copy1 = ms;
    multiset<int> ms_copy2 = ms;

    // Erase by value: Erases ALL copies of 5!
    ms_copy1.erase(5);
    cout << "After ms_copy1.erase(5) (by value), size: " << ms_copy1.size() << "\n"; // size is 2 (contains 2, 8)

    // Erase by iterator: Erases ONLY ONE copy of 5!
    auto it = ms_copy2.find(5);
    if (it != ms_copy2.end()) {
        ms_copy2.erase(it);
    }
    cout << "After iterator erase on ms_copy2, size: " << ms_copy2.size() << "\n"; // size is 4 (contains 2, 5, 5, 8)
    cout << "\n";
}

/*==========================================================
=            3. RANGE QUERIES & BOUNDS
==========================================================*/

void rangeAndBounds() {
    cout << "--- 3. RANGE QUERIES & BOUNDS ---\n";
    multiset<int> ms = {2, 5, 5, 5, 8};

    // equal_range returns pair of iterators {lower_bound, upper_bound}
    auto [lower, upper] = ms.equal_range(5);
    int count_fives = 0;
    for (auto it = lower; it != upper; ++it) {
        count_fives++;
    }
    cout << "Number of 5s using equal_range: " << count_fives << "\n"; // 3

    // bounds
    auto lb = ms.lower_bound(5); // points to first 5
    auto ub = ms.upper_bound(5); // points to 8 (first element > 5)
    cout << "First 5: " << *lb << "\n";
    cout << "Element after last 5: " << *ub << "\n";

    // Min and Max elements in O(1)
    cout << "Min element: " << *ms.begin() << "\n";
    cout << "Max element: " << *ms.rbegin() << "\n\n";
}

/*==========================================================
=            4. SLIDING WINDOW MEDIAN PATTERN
==========================================================*/

// Demonstration of maintaining a median iterator dynamically in a multiset
vector<double> slidingWindowMedian(const vector<int>& nums, int k) {
    vector<double> medians;
    multiset<int> window;

    for (int i = 0; i < (int)nums.size(); i++) {
        // Insert new element
        window.insert(nums[i]);

        // Remove element that slid out of window
        if (i >= k) {
            // Find iterator of the element to erase (using find to erase ONLY ONE)
            window.erase(window.find(nums[i - k]));
        }

        // Output median once window is full
        if (i >= k - 1) {
            auto it = window.begin();
            // Advance to the middle
            advance(it, (k - 1) / 2);
            if (k % 2 != 0) {
                medians.push_back((double)*it);
            } else {
                auto next_it = next(it);
                medians.push_back((*it + *next_it) / 2.0);
            }
        }
    }
    return medians;
}

void slidingWindowMedianDemo() {
    cout << "--- 4. SLIDING WINDOW MEDIAN DEMO ---\n";
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    vector<double> medians = slidingWindowMedian(nums, k);

    cout << "Sliding window medians (k=3):\n";
    for (double val : medians) {
        cout << val << " ";
    }
    cout << "\n\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ ms.insert(x)     │ Insert element             │ O(log N)                    │
 * │ ms.erase(x)      │ Erases ALL copies of x     │ O(log N + K)                │
 * │ ms.erase(iterator)│ Erases single copy at it   │ O(1) amortized              │
 * │ ms.find(x)       │ Iterator to first copy     │ O(log N)                    │
 * │ ms.equal_range(x)│ Pair of {first, last + 1}  │ O(log N)                    │
 * │ ms.lower_bound(x)│ Iterator to first >= x     │ O(log N)                    │
 * │ ms.upper_bound(x)│ Iterator to first > x      │ O(log N)                    │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Safe Single Element Deletion Template
// auto it = ms.find(val);
// if (it != ms.end()) {
//     ms.erase(it);
// }

// 2. Safe Element Existence Check (Strictly O(log N))
// bool exists = (ms.find(val) != ms.end());

// 3. Count occurrences safely (O(log N + K)) without ms.count()
// auto [lower, upper] = ms.equal_range(val);
// int count = distance(lower, upper);

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
multiset<int> ms;

// Erase Single Occurrence (Crucial!)
ms.erase(ms.find(val));

// Check existence
if (ms.find(val) != ms.end())

// Min & Max
int min_val = *ms.begin();
int max_val = *ms.rbegin();
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // safeSearchAndErase();
    // rangeAndBounds();
    // slidingWindowMedianDemo();

    return 0;
}
