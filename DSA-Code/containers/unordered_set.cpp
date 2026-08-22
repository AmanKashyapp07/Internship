/**
 * ==========================================================
 *             C++ Unordered Set Cheat Sheet (OAs)
 * ==========================================================
 *
 * unordered_set
 * -------------
 * • Unordered elements
 * • Hash Table implementation
 * • Average O(1) complexity
 * 
 * Note: Since std::unordered_set does not have a default hash
 * function for std::pair, std::set is preferred for coordinate/pair
 * tracking in OAs to avoid writing custom hash functions.
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * unordered_set
 * -------------
 * insert      O(1) average / O(n) worst
 * erase       O(1) average / O(n) worst
 * find        O(1) average / O(n) worst
 * count       O(1) average / O(n) worst
 *
 * ==========================================================
 */

#include <iostream>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

/*==========================================================
=            1. BASIC OPERATIONS
==========================================================*/

void basicOperations() {

    unordered_set<int> us;

    us.insert(10);
    us.insert(20);
    us.emplace(30);

    cout << us.size() << '\n';

    if (us.count(20))
        cout << "20 exists\n";

    auto it = us.find(40);
    if (it == us.end())
        cout << "40 not found\n";

    us.erase(20);
}

/*==========================================================
=            2. PAIR KEY SET (USING SET TO AVOID CUSTOM HASH)
==========================================================*/

void pairSetExample() {

    // Using std::set instead of std::unordered_set because key is a pair
    set<pair<int, int>> visited;

    visited.insert({0, 1});
    visited.insert({2, 3});

    cout << visited.count({0, 1}) << '\n'; // 1
}

/*==========================================================
=            3. GRID BFS VISITED PATTERN
==========================================================*/

void gridVisitedDemo() {

    // Using std::set instead of std::unordered_set because key is a pair
    set<pair<int, int>> visited;
    vector<pair<int, int>> path = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};

    for (auto cell : path) {
        if (!visited.count(cell)) {
            visited.insert(cell);
            cout << "Visited: " << cell.first << "," << cell.second << '\n';
        }
    }
}

/*==========================================================
=            4. COMMON OPERATIONS
==========================================================*/

void operations() {

    unordered_set<int> us;

    us.insert(10);
    us.erase(10);

    cout << us.size() << '\n';
    cout << us.empty() << '\n';
    us.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Pair Grid Visited Set (uses std::set to avoid writing a custom hash)
// set<pair<int,int>> visited;

/*
==========================================================
Remember These
==========================================================

// Initialization
unordered_set<int> us;

// Pair Visited Set
set<pair<int,int>> visited;

// Operations
us.insert(val);
us.erase(val);
us.count(val);

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // pairSetExample();
    // gridVisitedDemo();
    // operations();

    return 0;
}

// diff between unordered_set and set:
// 1. unordered_set is implemented using a hash table, while set is implemented using a balanced binary search tree (usually a red-black tree).
// 2. unordered_set provides average O(1) time complexity for insert, erase, and find operations, while set provides O(log n) time complexity for these operations.
// 3. unordered_set does not maintain any order of elements, while set maintains elements in sorted order.
// 4. unordered_set does not allow duplicate elements, while set also does not allow duplicates, but it maintains a strict order of elements.
// 5. unordered_set requires a hash function for custom types, while set requires a comparison function for custom types.
// For OAs, you should prefer unordered_set for faster average performance, but if you need ordered elements or are using pairs as keys, set is often more convenient to avoid writing custom hash functions.