/**
 * ==========================================================
 *             C++ Unordered Map Cheat Sheet (OAs)
 * ==========================================================
 *
 * unordered_map
 * -------------
 * • Unordered key-value pairs
 * • Hash Table implementation
 * • Average O(1) complexity
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * unordered_map
 * -------------
 * insert      O(1) average / O(n) worst
 * erase       O(1) average / O(n) worst
 * find        O(1) average / O(n) worst
 * []          O(1) average / O(n) worst
 *
 * ==========================================================
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
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

    unordered_map<int, string> um;

    um[10] = "Ten";
    um[20] = "Twenty";
    um.emplace(30, "Thirty");

    cout << um.size() << '\n';
    cout << um[10] << '\n';

    if (um.count(20))
        cout << "20 exists\n";

    auto it = um.find(40);
    if (it == um.end())
        cout << "40 not found\n";

    um.erase(20);
}

/*==========================================================
=            2. SAFE HASH UNORDERED MAP (Anti-TLE)
==========================================================*/

void safeMapExample() {

    unordered_map<int, int, SafeHash> safe_map;

    // Sizing optimizations
    safe_map.reserve(1024);
    safe_map.max_load_factor(0.25);

    safe_map[42] = 100;
    safe_map[13] = 200;

    cout << safe_map[42] << '\n';
}

/*==========================================================
=            3. PAIR HASH UNORDERED MAP
==========================================================*/

void pairMapExample() {

    unordered_map<pair<int, int>, string, PairHash> grid_map;

    grid_map[{0, 0}] = "Origin";
    grid_map[{3, 4}] = "Target";

    cout << grid_map[{3, 4}] << '\n'; // Target
}

/*==========================================================
=            4. STATE MEMOIZATION PATTERN
==========================================================*/

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

    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };
    memo.clear();
    int max_profit = solveMemoizedDP(0, 0, grid);
    cout << "Max Profit: " << max_profit << '\n';
}

/*==========================================================
=            5. COMMON OPERATIONS
==========================================================*/

void operations() {

    unordered_map<int, int> um;

    um[1] = 100;
    um.erase(1);

    cout << um.size() << '\n';
    cout << um.empty() << '\n';
    um.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Safe Unordered Map declaration
// unordered_map<int, int, SafeHash> umap;
// umap.reserve(2048);
// umap.max_load_factor(0.25);

// Pair Key Unordered Map (visited coordinates / grid DP cache)
// unordered_map<pair<int,int>, int, PairHash> cache;

/*
==========================================================
Remember These
==========================================================

// Initialization
unordered_map<int, int> ump;

// Safe Custom Hash Map
unordered_map<int, int, SafeHash> ump;

// Pair Key Map
unordered_map<pair<int,int>, int, PairHash> ump;

// Operations
ump[key] = val;
ump.erase(key);
ump.count(key);

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // safeMapExample();
    // pairMapExample();
    // memoizationDemo();
    // operations();

    return 0;
}
