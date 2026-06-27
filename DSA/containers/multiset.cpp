/**
 * C++ STL Multiset Cheat Sheet for Online Assessments
 *
 * Description:
 *  An associative container that stores elements in sorted order, allowing duplicate values.
 *  Implemented internally as a self-balancing Binary Search Tree (Red-Black Tree).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - insert / emplace : O(log N)
 * - erase(val)       : O(log N + K) — erases ALL K copies of val (CRITICAL WARNING!)
 * - erase(iterator)  : O(1) amortized — erases ONLY the single element at the iterator
 * - find(val)        : O(log N) — returns iterator to the FIRST instance of val
 * - count(val)       : O(log N + K) — can be LINEAR in the number of duplicates (avoid in OAs!)
 * - lower_bound(val) : O(log N) — iterator to first element >= target
 * - upper_bound(val) : O(log N) — iterator to first element > target
 * - Space            : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Sliding Window Median (without two heaps)
 *    - Insert and remove elements from a sliding window dynamically while keeping track
 *      of duplicates. Maintain a "median iterator" and shift it left/right as elements are added/removed.
 * 2. Range Queries with Duplicates
 *    - E.g. Matching buyers and sellers with specific price ranges where multiple buyers
 *      can have the same budget.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS — THE ERASE PITFALL (EXTREMELY IMPORTANT!)
 * ────────────────────────────────────────────────────────────────────────────────
 * - CRITICAL BUG: `ms.erase(x)` deletes ALL occurrences of `x`.
 *   If your multiset has {2, 5, 5, 5, 8} and you call `ms.erase(5)`, the set becomes {2, 8}.
 * - FIX: To delete ONLY ONE occurrence of `5`, find its iterator first and erase that:
 *   `ms.erase(ms.find(5));`
 *   Make sure `ms.find(5)` actually exists (is not `ms.end()`) before erasing, otherwise it will crash.
 *
 * - AVOID count(val): Calling `ms.count(x)` can take O(K) where K is the number of copies of x.
 *   If a test case has N copies of x, `count(x)` takes O(N) time! 
 *   To check if x exists, use `ms.find(x) != ms.end()` which is strictly O(log N).
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. DECLARATION & INSERTION
    multiset<int> ms;
    ms.insert(5);
    ms.insert(2);
    ms.insert(5);
    ms.insert(8);
    ms.insert(5);

    // Elements in multiset are sorted: {2, 5, 5, 5, 8}
    cout << "Multiset size: " << ms.size() << "\n"; // Outputs 5

    // 2. CHECK EXISTENCE SAFELY
    // O(log N) check
    if (ms.find(5) != ms.end()) {
        cout << "5 exists in multiset\n";
    }

    // 3. THE ERASE PITFALL DEMONSTRATION
    // Let's copy the multiset to demonstrate both behaviors
    multiset<int> ms_copy1 = ms;
    multiset<int> ms_copy2 = ms;

    // Behavior A: Erasing by VALUE (erases all copies)
    ms_copy1.erase(5); 
    // ms_copy1 is now: {2, 8}
    cout << "After ms_copy1.erase(5), size: " << ms_copy1.size() << "\n"; // Outputs 2

    // Behavior B: Erasing by ITERATOR (erases only ONE copy)
    auto it = ms_copy2.find(5);
    if (it != ms_copy2.end()) {
        ms_copy2.erase(it); // Erase only the single copy pointed by iterator
    }
    // ms_copy2 is now: {2, 5, 5, 8}
    cout << "After iterator erase on ms_copy2, size: " << ms_copy2.size() << "\n"; // Outputs 4

    // 4. FINDING RANGE OF DUPLICATES (equal_range)
    // Returns a pair of iterators {lower_bound, upper_bound}
    auto [lower, upper] = ms.equal_range(5);
    int count_fives = 0;
    for (auto it_range = lower; it_range != upper; ++it_range) {
        count_fives++;
    }
    cout << "Count of 5s: " << count_fives << "\n"; // Outputs 3

    // 5. BOUNDS ON MULTISET
    // lower_bound(5) points to the FIRST 5
    auto lb = ms.lower_bound(5);
    // upper_bound(5) points to the element AFTER the last 5, which is 8
    auto ub = ms.upper_bound(5);
    
    cout << "First 5: " << *lb << "\n"; // Outputs 5
    cout << "Element after last 5: " << *ub << "\n"; // Outputs 8

    // Get Min and Max
    cout << "Min: " << *ms.begin() << "\n"; // Outputs 2
    cout << "Max: " << *ms.rbegin() << "\n"; // Outputs 8

    return 0;
}
