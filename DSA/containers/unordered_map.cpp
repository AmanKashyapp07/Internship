/**
 * C++ STL Unordered Map Cheat Sheet for Online Assessments
 *
 * Description:
 *  An associative container that stores key-value pairs using a hash table.
 *  Provides average-case constant time complexity for insertions, deletions, and lookups.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - insert / emplace : Average: O(1) | Worst-case: O(N) (due to collisions or rehashing)
 * - erase(key)       : Average: O(1) | Worst-case: O(N)
 * - find(key)        : Average: O(1) | Worst-case: O(N)
 * - count(key)       : Average: O(1) | Worst-case: O(N)
 * - operator[key]    : Average: O(1) | Worst-case: O(N)
 * - Space            : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Fast Key-Value Queries (without sorted order requirements)
 *    - E.g. Frequency mapping, checking if an element's complement exists (Two Sum).
 * 2. Memoization Cache in DP
 *    - Storing states of a sparse dynamic programming recursion.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS — THE "ANTI-HASH HACK" PREVENTION (CRITICAL!)
 * ────────────────────────────────────────────────────────────────────────────────
 * - GCC's default hash function for integers in std::unordered_map is simply the identity
 *   function: hash(x) = x. 
 * - Competitive programming platforms and advanced test suites on OA platforms have
 *   "anti-hash" test cases specifically designed to force massive hash collisions.
 *   This degrades std::unordered_map lookup times from O(1) to O(N), resulting in O(N^2)
 *   overall complexity and TLE.
 *
 * - FIX: Provide a custom hash function that uses a high-quality mixing algorithm (e.g. splitmix64)
 *   and a randomized time-based seed.
 *
 * - OPTIMIZATION: Call `reserve(N)` and `max_load_factor(0.25)` to prevent overhead
 *   from dynamic rehashing.
 */

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <vector>

using namespace std;

// ────────────────────────────────────────────────────────────────────────────────
// 1. CUSTOM SAFE HASH TEMPLATE (Must-have in OAs using unordered_map/set)
// ────────────────────────────────────────────────────────────────────────────────
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

// ────────────────────────────────────────────────────────────────────────────────
// 2. PAIR HASH TEMPLATE
// ────────────────────────────────────────────────────────────────────────────────
// std::unordered_map does not compile with std::pair as key out of the box.
// We must provide a custom hash functor for pairs.
struct PairHash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h1 = SafeHash().splitmix64(p.first + FIXED_RANDOM);
        uint64_t h2 = SafeHash().splitmix64(p.second + FIXED_RANDOM);
        
        // Combine hashes (standard combination rule)
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ────────────────────────────────────────────────────────────────────────────────
    // DEMO: SAFE UNORDERED MAP
    // ────────────────────────────────────────────────────────────────────────────────
    unordered_map<int, int, SafeHash> safe_map;
    
    // OPTIMIZATION: Reserve buckets to prevent rehashing overhead
    safe_map.reserve(1024);
    // Setting max load factor lower reduces collisions at the expense of a bit more memory
    safe_map.max_load_factor(0.25);

    safe_map[42] = 100;
    safe_map[13] = 200;

    cout << "safe_map[42]: " << safe_map[42] << "\n"; // Outputs 100
    cout << "Count of 13: " << safe_map.count(13) << "\n"; // Outputs 1
    
    if (safe_map.find(999) == safe_map.end()) {
        cout << "999 not found in safe_map\n";
    }

    // ────────────────────────────────────────────────────────────────────────────────
    // DEMO: UNORDERED MAP WITH PAIR KEY
    // ────────────────────────────────────────────────────────────────────────────────
    // Essential for grid coordinates, ranges, states representation.
    unordered_map<pair<int, int>, string, PairHash> grid_map;
    
    grid_map[{0, 0}] = "Origin";
    grid_map[{3, 4}] = "Target";

    cout << "grid_map[{3, 4}]: " << grid_map[{3, 4}] << "\n"; // Outputs "Target"

    return 0;
}
