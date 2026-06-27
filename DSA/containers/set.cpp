/**
 * C++ STL Set (Ordered Set) Cheat Sheet for Online Assessments
 *
 * Description:
 *  A container that stores unique elements in sorted order.
 *  Implemented internally as a self-balancing Binary Search Tree (typically a Red-Black Tree).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - insert / emplace : O(log N)
 * - erase(val)       : O(log N) — deletes the value if it exists
 * - erase(iterator)  : O(1) amortized
 * - find(val)        : O(log N)
 * - count(val)       : O(log N) (returns either 0 or 1)
 * - lower_bound(val) : O(log N) — returns iterator to first element >= target
 * - upper_bound(val) : O(log N) — returns iterator to first element > target
 * - traversal        : O(N) in sorted order
 * - Space            : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Maintaining Unique Sorted Elements
 *    - E.g. Keeping track of active points/lines in a Sweep-Line algorithm.
 * 2. Predecessor and Successor Queries
 *    - When you need to frequently query "what is the next larger active element?"
 *      or "what is the largest element less than X?".
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - CRITICAL PERFORMANCE TRAP: Never use the global std::lower_bound(s.begin(), s.end(), val)
 *   on a set. It runs in O(N) because bidirectionality makes pointer-hopping linear.
 *   ALWAYS use the member function: s.lower_bound(val) which runs in O(log N).
 * - Safe Predecessor / Successor Traversal:
 *   - Predecessor of element iterator `it`: `std::prev(it)`. Ensure `it != s.begin()`.
 *   - Successor of element iterator `it`: `std::next(it)`. Ensure `it != s.end()` and `std::next(it) != s.end()`.
 * - Insert Return Type: `s.insert(val)` returns `std::pair<iterator, bool>`.
 *   The `bool` is true if the element was successfully inserted (didn't exist before) and
 *   false if it was a duplicate and insertion did not occur.
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

// Custom Comparator for set
struct DescendingCmp {
    bool operator()(const int& a, const int& b) const {
        return a > b;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. STANDARD DECLARATION & INSERTION
    set<int> unique_set;
    
    // Check insertion status
    auto [it_inserted, inserted] = unique_set.insert(10); // inserted is true
    auto [it_dup, dup_inserted] = unique_set.insert(10);     // dup_inserted is false

    unique_set.insert(30);
    unique_set.insert(20);
    // Order of elements: 10, 20, 30

    // 2. CHECK MEMBERSHIP
    if (unique_set.count(10)) {
        cout << "10 exists in the set\n";
    }
    
    auto find_it = unique_set.find(20);
    if (find_it != unique_set.end()) {
        cout << "Found: " << *find_it << "\n"; // Outputs 20
    }

    // 3. MEMBER BOUNDS VS GLOBAL BOUNDS (Crucial OA Speed Tip)
    // O(log N) member call
    auto lb_it = unique_set.lower_bound(15); 
    cout << "Lower bound of 15: " << *lb_it << "\n"; // Outputs 20

    auto ub_it = unique_set.upper_bound(20);
    cout << "Upper bound of 20: " << *ub_it << "\n"; // Outputs 30

    // 4. FIND PREDECESSOR & SUCCESSOR (Sweep-line pattern)
    // Goal: Find predecessor and successor of '20' in the set
    auto target_it = unique_set.find(20);
    
    // Predecessor (largest element < 20)
    if (target_it != unique_set.begin()) {
        auto pred_it = prev(target_it);
        cout << "Predecessor of 20: " << *pred_it << "\n"; // Outputs 10
    }
    
    // Successor (smallest element > 20)
    if (target_it != unique_set.end()) {
        auto succ_it = next(target_it);
        if (succ_it != unique_set.end()) {
            cout << "Successor of 20: " << *succ_it << "\n"; // Outputs 30
        }
    }

    // 5. CUSTOM COMPARATOR SORTING (Descending)
    set<int, DescendingCmp> desc_set;
    desc_set.insert(10);
    desc_set.insert(30);
    desc_set.insert(20);
    // Order will be: 30, 20, 10
    cout << "Largest element: " << *desc_set.begin() << "\n"; // Outputs 30

    // 6. CLEARING & ERASING
    // Erase by value
    int erased_count = unique_set.erase(20); // Returns number of elements erased (1 if exists, 0 otherwise)
    cout << "Erased count: " << erased_count << "\n"; // Outputs 1

    // Erase by iterator
    auto it_to_erase = unique_set.find(10);
    if (it_to_erase != unique_set.end()) {
        unique_set.erase(it_to_erase); // Runs in O(1) amortized
    }

    return 0;
}
