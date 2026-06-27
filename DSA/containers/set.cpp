/**
 * ==========================================================
 *                 C++ Set Cheat Sheet (OAs)
 * ==========================================================
 *
 * set
 * ---
 * • Ordered (sorted ascending by default)
 * • Implemented using Red-Black Tree
 * • Unique elements
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * set
 * ---
 * insert      O(log n)
 * erase       O(log n)
 * find        O(log n)
 * count       O(log n)
 * lower_bound O(log n)
 * upper_bound O(log n)
 *
 * ==========================================================
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

/*==========================================================
=            1. STANDARD DECLARATION & INSERTION
==========================================================*/

void basicOperations() {

    set<int> unique_set;

    // Check insertion status
    auto [it_inserted, inserted] = unique_set.insert(10); // inserted is true
    auto [it_dup, dup_inserted] = unique_set.insert(10);     // dup_inserted is false

    unique_set.insert(30);
    unique_set.insert(20);

    // Elements: 10, 20, 30

    for (int x : unique_set)
        cout << x << " ";
    cout << '\n';
}

/*==========================================================
=            2. CHECK MEMBERSHIP
==========================================================*/

void checkMembership() {

    set<int> s = {10, 20, 30};

    if (s.count(10))
        cout << "10 exists\n";

    auto it = s.find(20);
    if (it != s.end())
        cout << "Found: " << *it << '\n';
}

/*==========================================================
=            3. MEMBER BOUNDS
==========================================================*/

void boundsExample() {

    set<int> s = {10, 20, 30, 40};

    // O(log n) member call
    auto lb = s.lower_bound(15); // Points to 20
    cout << *lb << '\n';

    auto ub = s.upper_bound(20); // Points to 30
    cout << *ub << '\n';
}

/*==========================================================
=            4. FIND PREDECESSOR & SUCCESSOR
==========================================================*/

void sweepLineDemo() {

    set<int> s = {10, 20, 30};
    auto target_it = s.find(20);

    // Predecessor (largest element < 20)
    if (target_it != s.begin()) {
        auto pred_it = prev(target_it);
        cout << "Predecessor: " << *pred_it << '\n'; // 10
    }

    // Successor (smallest element > 20)
    if (target_it != s.end()) {
        auto succ_it = next(target_it);
        if (succ_it != s.end()) {
            cout << "Successor: " << *succ_it << '\n'; // 30
        }
    }
}

/*==========================================================
=            5. CUSTOM COMPARATOR SORTING
==========================================================*/

void customComparatorDemo() {

    set<int, DescendingCmp> desc_set;

    desc_set.insert(10);
    desc_set.insert(30);
    desc_set.insert(20);

    // Order: 30, 20, 10
    cout << *desc_set.begin() << '\n'; // 30
}

/*==========================================================
=            6. CLEARING & ERASING
==========================================================*/

void eraseAndClear() {

    set<int> s = {10, 20, 30};

    // Erase by value
    int erased = s.erase(20); // returns 1 or 0

    // Erase by iterator
    auto it = s.find(10);
    if (it != s.end()) {
        s.erase(it); // O(1) amortized
    }
}

/*==========================================================
=            7. COMMON OPERATIONS
==========================================================*/

void operations() {

    set<int> s;

    s.insert(10);
    s.erase(10);

    cout << s.size() << '\n';
    cout << s.empty() << '\n';
    s.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Standard set lower bound
// auto it = s.lower_bound(val);

// Sweep-line active set tracking with predecessor
// auto it = s.lower_bound(val);
// if(it != s.begin()) {
//     auto pred = prev(it);
// }

/*
==========================================================
Remember These
==========================================================

// Initialization
set<int> s;

// Custom Descending
set<int, greater<int>> s;

// Bounds (Strictly use member functions!)
auto lb = s.lower_bound(x);
auto ub = s.upper_bound(x);

// Predecessor & Successor
auto pred = prev(it);
auto succ = next(it);

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // checkMembership();
    // boundsExample();
    // sweepLineDemo();
    // customComparatorDemo();
    // eraseAndClear();
    // operations();

    return 0;
}
