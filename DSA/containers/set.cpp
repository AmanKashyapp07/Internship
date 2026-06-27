/**
 * C++ STL Set (Ordered Set) Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  An associative container that stores unique elements in sorted order.
 *  Implemented internally as a self-balancing Binary Search Tree (typically a Red-Black Tree).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ insert()         │ O(log N)         │ Inserts element if unique                │
 * │ emplace()        │ O(log N)         │ Inserts element in-place                 │
 * │ erase(val)       │ O(log N)         │ Removes element by value                 │
 * │ erase(iterator)  │ O(1) amortized   │ Removes element by position              │
 * │ find()           │ O(log N)         │ Searches for element                     │
 * │ count()          │ O(log N)         │ Returns 1 if present, 0 otherwise        │
 * │ lower_bound()    │ O(log N)         │ Iterator to first element >= target      │
 * │ upper_bound()    │ O(log N)         │ Iterator to first element > target       │
 * │ size() / empty() │ O(1)             │ Container sizing checks                  │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Maintaining unique sorted elements (e.g. sweep-line algorithms, active ranges)
 *  - Predecessor and successor queries (finding nearest smaller/larger elements dynamically)
 *
 * Interview Tricks & Pitfalls:
 *  - CRITICAL PERFORMANCE WARNING: Never use the global `std::lower_bound(s.begin(), s.end(), val)`
 *    on a set. It runs in O(N) because bidirectionality makes pointer-hopping linear.
 *    ALWAYS use the member function: `s.lower_bound(val)` which runs in O(log N).
 *  - Safe Predecessor / Successor Traversal:
 *    - Predecessor of element iterator `it`: `std::prev(it)`. Ensure `it != s.begin()`.
 *    - Successor of element iterator `it`: `std::next(it)`. Ensure `it != s.end()` and `std::next(it) != s.end()`.
 *  - Insert Return Type: `s.insert(val)` returns `std::pair<iterator, bool>`.
 *    The `bool` is true if the element was successfully inserted (didn't exist before) and
 *    false if it was a duplicate and insertion did not occur.
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

// Custom Comparator for set (sorting in descending order)
struct DescendingCmp {
    bool operator()(const int& a, const int& b) const {
        return a > b;
    }
};

/*==========================================================
=            1. CONSTRUCTION, CUSTOM COMPARATORS & BASIC OPS
==========================================================*/

void basicOperations() {
    cout << "--- 1. CONSTRUCTION & BASIC OPERATIONS ---\n";

    // 1. Default ascending set
    set<int> unique_set;

    // 2. Custom comparator set (descending order)
    set<int, DescendingCmp> desc_set;

    // Check insertion status via pair return
    auto [it_inserted, inserted] = unique_set.insert(10); // inserted is true
    auto [it_dup, dup_inserted] = unique_set.insert(10);     // dup_inserted is false

    cout << "Insertion of 10 succeeded: " << (inserted ? "Yes" : "No") << "\n";
    cout << "Insertion of duplicate 10 succeeded: " << (dup_inserted ? "Yes" : "No") << "\n";

    unique_set.insert(30);
    unique_set.insert(20);
    
    // Order of elements: 10, 20, 30
    cout << "Ascending Set size: " << unique_set.size() << "\n";
    cout << "Ascending Set elements: ";
    for (int x : unique_set) {
        cout << x << " ";
    }
    cout << "\n";

    desc_set.insert(10);
    desc_set.insert(30);
    desc_set.insert(20);

    // Order will be: 30, 20, 10
    cout << "Descending Set elements: ";
    for (int x : desc_set) {
        cout << x << " ";
    }
    cout << "\n\n";
}

/*==========================================================
=            2. SEARCHING & MEMBER BOUNDS (O(log N))
==========================================================*/

void searchAndBounds() {
    cout << "--- 2. SEARCHING & MEMBER BOUNDS ---\n";
    set<int> s = {10, 20, 30, 40, 50};

    // Safe membership checks
    if (s.count(30)) {
        cout << "30 is present in the set\n";
    }

    auto find_it = s.find(40);
    if (find_it != s.end()) {
        cout << "Found element: " << *find_it << "\n";
    }

    // Member lower_bound: first element >= target
    auto lb_it = s.lower_bound(25); // Points to 30
    if (lb_it != s.end()) {
        cout << "Lower bound of 25: " << *lb_it << "\n";
    }

    // Member upper_bound: first element > target
    auto ub_it = s.upper_bound(20); // Points to 30
    if (ub_it != s.end()) {
        cout << "Upper bound of 20: " << *ub_it << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            3. SWEEP-LINE PREDECESSOR & SUCCESSOR PATTERN
==========================================================*/

void sweepLineDemo() {
    cout << "--- 3. SWEEP-LINE PREDECESSOR & SUCCESSOR PATTERN ---\n";
    set<int> active_points = {10, 25, 40, 55};
    int current_point = 25;

    auto target_it = active_points.find(current_point);
    if (target_it == active_points.end()) return;

    // Predecessor (largest element < target)
    if (target_it != active_points.begin()) {
        auto pred_it = prev(target_it);
        cout << "Predecessor of 25: " << *pred_it << "\n"; // 10
    } else {
        cout << "No predecessor (at begin)\n";
    }

    // Successor (smallest element > target)
    auto succ_it = next(target_it);
    if (succ_it != active_points.end()) {
        cout << "Successor of 25: " << *succ_it << "\n"; // 40
    } else {
        cout << "No successor (at end)\n";
    }
    cout << "\n";
}

/*==========================================================
=            4. ERASING & CLEARING
==========================================================*/

void eraseAndClear() {
    cout << "--- 4. ERASING & CLEARING ---\n";
    set<int> s = {10, 20, 30, 40};

    // Erase by value
    int erased_count = s.erase(20); // Returns 1 if erased, 0 if not found
    cout << "Erased value 20? Count: " << erased_count << "\n";

    // Erase by iterator (highly efficient, O(1) amortized)
    auto it = s.find(30);
    if (it != s.end()) {
        s.erase(it);
    }

    cout << "Set size after erasures: " << s.size() << "\n";
    s.clear();
    cout << "Set size after clear: " << s.size() << "\n\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ s.insert(x)      │ Insert unique element      │ O(log N)                    │
 * │ s.erase(x)       │ Remove element by value    │ O(log N)                    │
 * │ s.erase(iterator)│ Remove element by position │ O(1) amortized              │
 * │ s.find(x)        │ Get iterator to element    │ O(log N)                    │
 * │ s.count(x)       │ Check if element exists    │ O(log N) (returns 0 or 1)   │
 * │ s.lower_bound(x) │ First iterator >= x        │ O(log N)                    │
 * │ s.upper_bound(x) │ First iterator > x         │ O(log N)                    │
 * │ s.size()         │ Get element count          │ O(1)                        │
 * │ s.empty()        │ Check if set is empty      │ O(1)                        │
 * │ s.clear()        │ Erase all elements         │ O(N)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Safe Predecessor / Successor Lookup (e.g. Sweep-Line)
// auto it = s.lower_bound(val);
// if (it != s.begin()) {
//     auto pred = prev(it); // predecessor
// }
// if (it != s.end()) {
//     auto succ = it;       // successor
// }

// 2. Custom struct comparator
// struct PointCmp {
//     bool operator()(const pair<int,int>& a, const pair<int,int>& b) const {
//         if (a.first != b.first) return a.first < b.first;
//         return a.second > b.second;
//     }
// };
// set<pair<int,int>, PointCmp> sweep_line;

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
set<int> s;
set<int, greater<int>> desc_s;

// Insertion Check
auto [it, success] = s.insert(val);

// Member Bounds
auto it_lb = s.lower_bound(val); // ALWAYS USE MEMBER!
auto it_ub = s.upper_bound(val);

// Predecessor/Successor
auto pred = prev(it);
auto succ = next(it);
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // searchAndBounds();
    // sweepLineDemo();
    // eraseAndClear();

    return 0;
}
