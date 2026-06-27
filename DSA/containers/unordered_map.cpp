/**
 * C++ STL Unordered Map Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  An associative container that stores key-value pairs using a hash table.
 *  Provides average-case constant time complexity for insertions, deletions, and lookups.
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
 * │ operator[key]    │ O(1)             │ O(N)             │ Access/insert key    │
 * └──────────────────┴──────────────────┴──────────────────┴──────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Fast key-value mapping (frequency counts, index tracking)
 *  - Memoization cache for sparse Dynamic Programming (top-down DP)
 *  - Complement checking (e.g. Two Sum)
 *
 * Interview Tricks & Pitfalls:
 *  - THE ANTI-HASH OA HACK: GCC's default hash function for integers in `std::unordered_map` is the
 *    identity function (`hash(x) = x`). Test cases in competitive programming and advanced OAs can be
 *    specifically crafted to cause severe collisions (anti-hash tests). This degrades operations from O(1)
 *    to O(N), yielding O(N^2) complexity and TLE.
 *    - FIX: Provide a custom time-seeded hash function using `splitmix64`.
 *  - Dynamic Reallocation Overhead: Unordered containers rehash when size exceeds capacity. To prevent TLE
 *    due to reallocations:
 *    - Use `mp.reserve(N)` if you know the maximum size beforehand.
 *    - Set `mp.max_load_factor(0.25)` to minimize collision likelihood at the expense of memory.
 *  - Pair Key Compilation: `std::unordered_map<pair<int, int>, Value>` does NOT compile out of the box because
 *    C++ does not define a default hash function for `std::pair`. You must supply a custom hash functor.
 *  - Autocreation Trap: Avoid checking key existence via `if (mp[key] == 0)`. This automatically inserts the key
 *    with a default value of 0, causing map bloat. Use `mp.count(key)` or `mp.find(key)` instead.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";
    unordered_map<int, string> um;

    // Insertion
    um[10] = "Ten";
    um[20] = "Twenty";
    um.emplace(30, "Thirty");

    cout << "Size: " << um.size() << "\n";
    cout << "Value at 10: " << um[10] << "\n";

    // Safe existence check
    if (um.count(20)) {
        cout << "Key 20 exists in map\n";
    }

    auto it = um.find(40);
    if (it == um.end()) {
        cout << "Key 40 does not exist in map\n";
    }

    // Erasing
    um.erase(20);
    cout << "Size after erasing key 20: " << um.size() << "\n\n";
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

void safeMapExample() {
    cout << "--- 2. SAFE MAP DEMO ---\n";
    unordered_map<int, int, SafeHash> safe_map;

    // Apply optimization parameters
    safe_map.reserve(1024);
    safe_map.max_load_factor(0.25);

    safe_map[42] = 100;
    safe_map[13] = 200;

    cout << "Value at 42: " << safe_map[42] << "\n\n";
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

void pairMapExample() {
    cout << "--- 3. PAIR MAP DEMO ---\n";
    // Crucial for grid coordinate / interval state tracking
    unordered_map<pair<int, int>, string, PairHash> grid_map;

    grid_map[{0, 0}] = "Origin";
    grid_map[{3, 4}] = "Target";

    cout << "Value at {3, 4}: " << grid_map[{3, 4}] << "\n\n";
}

/*==========================================================
=            4. STATE MEMOIZATION PATTERN
==========================================================*/

// Example: DP Memoization state caching for grid traversal/game states
// Storing coordinates {row, col} -> max_profit
unordered_map<pair<int, int>, int, PairHash> memo;

int solveMemoizedDP(int r, int c, const vector<vector<int>>& grid) {
    if (r >= (int)grid.size() || c >= (int)grid[0].size()) return 0;
    if (r == (int)grid.size() - 1 && c == (int)grid[0].size() - 1) return grid[r][c];

    pair<int, int> state = {r, c};
    if (memo.count(state)) {
        return memo[state];
    }

    int right = solveMemoizedDP(r, c + 1, grid);
    int down = solveMemoizedDP(r + 1, c, grid);

    memo[state] = grid[r][c] + max(right, down);
    return memo[state];
}

void memoizationDemo() {
    cout << "--- 4. STATE MEMOIZATION DEMO ---\n";
    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };
    memo.clear();
    int max_profit = solveMemoizedDP(0, 0, grid);
    cout << "Max profit traversing grid: " << max_profit << "\n\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ mp[key] = value  │ Insert or update key       │ O(1) average / O(N) worst   │
 * │ mp.insert(pair)  │ Insert pair if key absent  │ O(1) average / O(N) worst   │
 * │ mp.erase(key)    │ Erases key-value pair      │ O(1) average / O(N) worst   │
 * │ mp.find(key)     │ Iterator to key if exists  │ O(1) average / O(N) worst   │
 * │ mp.count(key)    │ Returns 1 if key exists    │ O(1) average / O(N) worst   │
 * │ mp.reserve(n)    │ Preallocate bucket space   │ O(N)                        │
 * │ mp.size()        │ Get element count          │ O(1)                        │
 * │ mp.empty()       │ Check if map is empty      │ O(1)                        │
 * │ mp.clear()       │ Remove all elements        │ O(N)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Safe Unordered Map Template
// struct SafeHash { ... } (refer section 2)
// unordered_map<int, int, SafeHash> umap;
// umap.reserve(2048);
// umap.max_load_factor(0.25);

// 2. Pair Unordered Map Template
// struct PairHash { ... } (refer section 3)
// unordered_map<pair<int, int>, int, PairHash> grid_cache;

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Optimized Initialization
unordered_map<int, int, SafeHash> safe_ump;
safe_ump.reserve(4096);

// Pair State Cache
unordered_map<pair<int, int>, int, PairHash> memo;

// Access safely (Avoid [] autocreation)
if (safe_ump.count(key)) {
    int val = safe_ump[key];
}

// Structured Binding Iteration
for (auto [key, val] : safe_ump) { ... }
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // safeMapExample();
    // pairMapExample();
    // memoizationDemo();

    return 0;
}
