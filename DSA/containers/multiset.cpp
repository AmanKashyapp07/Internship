/**
 * ==========================================================
 *               C++ Multiset Cheat Sheet (OAs)
 * ==========================================================
 *
 * multiset
 * --------
 * • Ordered (sorted ascending by default)
 * • Implemented using Red-Black Tree
 * • Duplicate elements allowed
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * multiset
 * --------
 * insert      O(log n)
 * erase(val)  O(log n + k)  // erases all k copies of val
 * erase(it)   O(1) amortized
 * find        O(log n)      // returns iterator to first copy
 * count       O(log n + k)  // can be linear, avoid!
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

/*==========================================================
=            1. DECLARATION & INSERTION
==========================================================*/

void declarationExample() {

    multiset<int> ms;

    ms.insert(5);
    ms.insert(2);
    ms.insert(5);
    ms.insert(8);
    ms.insert(5);

    // Elements are sorted: 2 5 5 5 8

    for (int x : ms)
        cout << x << " ";
    cout << '\n';
}

/*==========================================================
=            2. CHECK MEMBERSHIP SAFELY
==========================================================*/

void checkMembership() {

    multiset<int> ms = {2, 5, 5, 5, 8};

    // ALWAYS use find instead of count to check existence (O(log n))
    if (ms.find(5) != ms.end()) {
        cout << "5 exists\n";
    }
}

/*==========================================================
=            3. THE ERASE PITFALL (CRITICAL)
==========================================================*/

void erasePitfallDemo() {

    multiset<int> ms = {2, 5, 5, 5, 8};

    multiset<int> ms1 = ms;
    multiset<int> ms2 = ms;

    // A: Erase by VALUE (erases all copies!)
    ms1.erase(5); // ms1 becomes {2, 8}
    cout << "Erase by value size: " << ms1.size() << '\n'; // 2

    // B: Erase by ITERATOR (erases only one copy!)
    auto it = ms2.find(5);
    if (it != ms2.end()) {
        ms2.erase(it); // ms2 becomes {2, 5, 5, 8}
    }
    cout << "Erase by iterator size: " << ms2.size() << '\n'; // 4
}

/*==========================================================
=            4. RANGE & BOUND QUERIES
==========================================================*/

void rangeQueriesDemo() {

    multiset<int> ms = {2, 5, 5, 5, 8};

    // equal_range: returns pair of iterators {lower_bound, upper_bound}
    auto [lower, upper] = ms.equal_range(5);
    int cnt = 0;
    for (auto it = lower; it != upper; ++it) {
        cnt++;
    }
    cout << "Count of 5s: " << cnt << '\n'; // 3

    // bounds
    auto lb = ms.lower_bound(5); // Points to first 5
    auto ub = ms.upper_bound(5); // Points to 8 (first element > 5)

    cout << "Min: " << *ms.begin() << '\n';  // 2
    cout << "Max: " << *ms.rbegin() << '\n'; // 8
}

/*==========================================================
=            5. SLIDING WINDOW MEDIAN PATTERN
==========================================================*/

void slidingWindowMedianDemo() {

    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    multiset<int> window;

    for (int i = 0; i < (int)nums.size(); i++) {
        window.insert(nums[i]);
        if (i >= k) {
            window.erase(window.find(nums[i - k])); // Erase one copy!
        }
        if (i >= k - 1) {
            auto it = window.begin();
            advance(it, (k - 1) / 2);
            cout << *it << " "; // Medians: 1 3 -1 3 5 6
        }
    }
    cout << '\n';
}

/*==========================================================
=            6. COMMON OPERATIONS
==========================================================*/

void operations() {

    multiset<int> ms;

    ms.insert(10);
    ms.erase(ms.find(10)); // Safe single erase

    cout << ms.size() << '\n';
    cout << ms.empty() << '\n';
    ms.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Safe single occurrence erase
// auto it = ms.find(val);
// if(it != ms.end()) ms.erase(it);

// Exists check
// bool exists = (ms.find(val) != ms.end());

/*
==========================================================
Remember These
==========================================================

// Initialization
multiset<int> ms;

// Erase ALL copies
ms.erase(val);

// Erase ONE copy
ms.erase(ms.find(val));

// Check existence
if (ms.find(val) != ms.end())

==========================================================
*/

int main() {

    // Uncomment to test

    // declarationExample();
    // checkMembership();
    // erasePitfallDemo();
    // rangeQueriesDemo();
    // slidingWindowMedianDemo();
    // operations();

    return 0;
}
