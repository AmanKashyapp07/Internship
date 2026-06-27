/**
 * C++ STL Unordered Set Cheat Sheet for Online Assessments
 *
 * Description:
 *  A container that stores unique elements in no particular order.
 *  Uses a hash table internally, providing average-case constant time for standard operations.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - insert / emplace : Average: O(1) | Worst-case: O(N)
 * - erase(val)       : Average: O(1) | Worst-case: O(N)
 * - find(val)        : Average: O(1) | Worst-case: O(N)
 * - count(val)       : Average: O(1) | Worst-case: O(N) (returns 0 or 1)
 * - Space            : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Fast Duplicate Detection / Unique Tracking
 *    - Checking if an element has already been visited or processed (e.g. standard cycle detection, DFS/BFS).
 * 2. Lookup Tables (Membership Queries)
 *    - To store allowed states/elements and check inclusion in O(1) time.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - Same Anti-Hash Vulnerability as unordered_map:
 *   GCC's default hash function for integers in std::unordered_set is the identity function.
 *   Avoid TLE from anti-hash tests by using a `SafeHash` custom hash structure.
 * - Sizing Optimizations:
 *   Use `reserve(N)` and `max_load_factor(0.25)` to prevent dynamic re-allocation overhead.
 * - Custom Pair Set:
 *   Provide custom hash functors to compile `unordered_set<pair<int, int>>`.
 */

#include <iostream>
#include <unordered_set>
#include <chrono>
#include <vector>

using namespace std;

// Custom Safe Hash to prevent predictable collisions and O(N^2) hacks
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
        uint64_t h1 = SafeHash().splitmix64(p.first + FIXED_RANDOM);
        uint64_t h2 = SafeHash().splitmix64(p.second + FIXED_RANDOM);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. SAFE UNORDERED SET
    unordered_set<int, SafeHash> seen_set;
    
    // Performance setup
    seen_set.reserve(2048);
    seen_set.max_load_factor(0.25);

    seen_set.insert(100);
    seen_set.insert(200);

    // Verification
    cout << "Count of 100: " << seen_set.count(100) << "\n"; // Outputs 1
    
    if (seen_set.find(300) == seen_set.end()) {
        cout << "300 not found\n";
    }

    // Erasing elements
    seen_set.erase(100);
    cout << "Count of 100 after erase: " << seen_set.count(100) << "\n"; // Outputs 0

    // 2. PAIR UNORDERED SET (Extremely common in 2D grid algorithms)
    unordered_set<pair<int, int>, PairHash> visited_cells;
    
    visited_cells.insert({0, 1});
    visited_cells.insert({2, 3});

    cout << "Count of {0, 1}: " << visited_cells.count({0, 1}) << "\n"; // Outputs 1
    cout << "Count of {1, 0}: " << visited_cells.count({1, 0}) << "\n"; // Outputs 0

    return 0;
}
