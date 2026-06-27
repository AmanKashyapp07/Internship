/**
 * C++ STL Unordered Set Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  An associative container that stores unique elements in no particular order.
 *  Uses a hash table internally, providing average-case constant time for standard operations.
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────┬──────────────────────┐
 * │ Operation        │ Average          │ Worst Case       │ Notes                │
 * ├──────────────────┼──────────────────┼──────────────────┼──────────────────────┤
 * │ insert()         │ O(1)             │ O(N)             │ Collision / rehash   │
 * │ emplace()        │ O(1)             │ O(N)             │ Collision / rehash   │
 * │ erase()          │ O(1)             │ O(N)             │ Collision / rehash   │
 * │ find()           │ O(1)             │ O(N)             │ Collision / rehash   │
 * │ count()          │ O(1)             │ O(N)             │ Returns 0 or 1       │
 * └──────────────────┴──────────────────┴──────────────────┴──────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Fast deduplication / tracking seen values
 *  - Cycle detection in DFS/BFS
 *  - O(1) membership lookup (e.g., checking if element has been processed)
 *
 * Interview Tricks & Pitfalls:
 *  - THE ANTI-HASH OA HACK: GCC's default hash function for integers in `std::unordered_set` is the
 *    identity function (`hash(x) = x`). Test cases in competitive programming and advanced OAs can be
 *    specifically crafted to cause severe collisions (anti-hash tests). This degrades operations from O(1)
 *    to O(N), yielding O(N^2) complexity and TLE.
 *    - FIX: Provide a custom time-seeded hash function using `splitmix64`.
 *  - Dynamic Reallocation Overhead: Unordered containers rehash when size exceeds capacity. To prevent TLE
 *    due to reallocations:
 *    - Use `s.reserve(N)` if you know the maximum size beforehand.
 *    - Set `s.max_load_factor(0.25)` to minimize collision likelihood at the expense of memory.
 *  - Pair Key Compilation: `std::unordered_set<pair<int, int>>` does NOT compile out of the box because C++
 *    does not define a default hash function for `std::pair`. You must supply a custom hash functor.
 */

#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";
    unordered_set<int> us;

    us.insert(10);
    us.insert(20);
    us.emplace(30);

    cout << "Size: " << us.size() << "\n";
    
    // Check existence
    if (us.count(20)) {
        cout << "20 exists in set\n";
    }

    auto it = us.find(40);
    if (it == us.end()) {
        cout << "40 does not exist in set\n";
    }

    // Erasing
    us.erase(20);
    cout << "Size after erasing 20: " << us.size() << "\n\n";
}

/*==========================================================
=            2. CUSTOM SAFE HASH TEMPLATE (PREVENTS TLE)
==========================================================*/

struct SafeHash {
    static uint64_t splitmix64(uint64_t x) {
        // High-quality 64-bit mixer
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        // High-precision clock-based random seed to prevent predictable collisions
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

void safeSetExample() {
    cout << "--- 2. SAFE SET DEMO ---\n";
    unordered_set<int, SafeHash> safe_set;

    // Apply optimization parameters
    safe_set.reserve(1024);
    safe_set.max_load_factor(0.25);

    safe_set.insert(100);
    safe_set.insert(200);

    cout << "Safe set count of 100: " << safe_set.count(100) << "\n\n";
}

/*==========================================================
=            3. CUSTOM PAIR HASH TEMPLATE
==========================================================*/

struct PairHash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h1 = SafeHash::splitmix64(p.first + FIXED_RANDOM);
        uint64_t h2 = SafeHash::splitmix64(p.second + FIXED_RANDOM);
        
        // Combine hashes (standard combination rule)
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

void pairSetExample() {
    cout << "--- 3. PAIR SET DEMO ---\n";
    // Crucial for grid coordinate visitation sets
    unordered_set<pair<int, int>, PairHash> visited;

    visited.insert({0, 1});
    visited.insert({2, 3});

    cout << "Has visited {0, 1}? " << (visited.count({0, 1}) ? "Yes" : "No") << "\n";
    cout << "Has visited {1, 1}? " << (visited.count({1, 1}) ? "Yes" : "No") << "\n\n";
}

/*==========================================================
=            4. GRID BFS VISITED CELLS PATTERN
==========================================================*/

void gridVisitedDemo() {
    cout << "--- 4. GRID BFS VISITED DEMO ---\n";
    // Simulation: grid DFS traversal visited set
    unordered_set<pair<int, int>, PairHash> visited;
    
    vector<pair<int, int>> path = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};
    
    cout << "Path traversal status:\n";
    for (auto cell : path) {
        if (visited.count(cell)) {
            cout << "  Cell (" << cell.first << ", " << cell.second << ") already visited!\n";
        } else {
            visited.insert(cell);
            cout << "  Visiting Cell (" << cell.first << ", " << cell.second << ")\n";
        }
    }
    cout << "\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ us.insert(x)     │ Insert element             │ O(1) average / O(N) worst   │
 * │ us.erase(x)      │ Remove element             │ O(1) average / O(N) worst   │
 * │ us.find(x)       │ Iterator to element        │ O(1) average / O(N) worst   │
 * │ us.count(x)      │ Returns 1 if present       │ O(1) average / O(N) worst   │
 * │ us.reserve(n)    │ Preallocate bucket space   │ O(N)                        │
 * │ us.size()        │ Get element count          │ O(1)                        │
 * │ us.empty()       │ Check if empty             │ O(1)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Safe Unordered Set Template
// struct SafeHash { ... } (refer section 2)
// unordered_set<int, SafeHash> us;
// us.reserve(2048);
// us.max_load_factor(0.25);

// 2. Unordered Set of Pairs
// struct PairHash { ... } (refer section 3)
// unordered_set<pair<int, int>, PairHash> visited;

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Standard Initialization
unordered_set<int> us;

// Optimized Initialization
unordered_set<int, SafeHash> safe_us;
safe_us.reserve(4096);

// Pair Grid Visited Set
unordered_set<pair<int, int>, PairHash> visited;

// Operations
us.insert(val);
us.erase(val);
if (us.count(val)) { ... }
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // safeSetExample();
    // pairSetExample();
    // gridVisitedDemo();

    return 0;
}
