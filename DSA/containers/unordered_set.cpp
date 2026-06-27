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
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

// Custom Safe Hash to prevent anti-hash tests (O(N^2) TLE)
struct SafeHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

// Custom Hash for std::pair
struct PairHash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h1 = SafeHash::splitmix64(p.first + FIXED_RANDOM);
        uint64_t h2 = SafeHash::splitmix64(p.second + FIXED_RANDOM);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

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
=            2. SAFE HASH UNORDERED SET (Anti-TLE)
==========================================================*/

void safeSetExample() {

    unordered_set<int, SafeHash> safe_set;

    // Sizing optimizations to prevent rehash overhead
    safe_set.reserve(1024);
    safe_set.max_load_factor(0.25);

    safe_set.insert(100);
    safe_set.insert(200);

    cout << safe_set.count(100) << '\n';
}

/*==========================================================
=            3. PAIR HASH UNORDERED SET
==========================================================*/

void pairSetExample() {

    unordered_set<pair<int, int>, PairHash> visited;

    visited.insert({0, 1});
    visited.insert({2, 3});

    cout << visited.count({0, 1}) << '\n'; // 1
}

/*==========================================================
=            4. GRID BFS VISITED PATTERN
==========================================================*/

void gridVisitedDemo() {

    unordered_set<pair<int, int>, PairHash> visited;
    vector<pair<int, int>> path = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};

    for (auto cell : path) {
        if (!visited.count(cell)) {
            visited.insert(cell);
            cout << "Visited: " << cell.first << "," << cell.second << '\n';
        }
    }
}

/*==========================================================
=            5. COMMON OPERATIONS
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

// Safe set instantiation
// unordered_set<int, SafeHash> us;
// us.reserve(2048);
// us.max_load_factor(0.25);

// Grid visited set
// unordered_set<pair<int,int>, PairHash> visited;

/*
==========================================================
Remember These
==========================================================

// Initialization
unordered_set<int> us;

// Safe custom hash set
unordered_set<int, SafeHash> us;

// Pair visited set
unordered_set<pair<int,int>, PairHash> visited;

// Operations
us.insert(val);
us.erase(val);
us.count(val);

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // safeSetExample();
    // pairSetExample();
    // gridVisitedDemo();
    // operations();

    return 0;
}
