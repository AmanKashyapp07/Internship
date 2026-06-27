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
 * Note: Many competitive programming platforms exploit the default
 * hash function, which can lead to O(n) performance due to collisions.
 * In cases where keys are complex (like pairs), std::map is preferred
 * to avoid writing complex custom hash functions.
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
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/*==========================================================
=            1. BASIC OPERATIONS
==========================================================*/

void basicOperations() {

    unordered_map<int, string> hashmap;

    hashmap[10] = "Ten";
    hashmap[20] = "Twenty";
    hashmap.emplace(30, "Thirty");

    cout << hashmap.size() << '\n';
    cout << hashmap[10] << '\n';

    if (hashmap.count(20))
        cout << "20 exists\n";

    auto it = hashmap.find(40);
    if (it == hashmap.end()) cout << "40 not found\n";
    else cout << "40 found with value: " << it->second << '\n'; 

    hashmap.erase(20);
}

/*==========================================================
=            2. PAIR KEY MAP (USING MAP TO AVOID CUSTOM HASH)
==========================================================*/

void pairMapExample() {

    // Using std::map instead of std::unordered_map because key is a pair
    map<pair<int, int>, string> grid_map;

    grid_map[{0, 0}] = "Origin";
    grid_map[{3, 4}] = "Target";

    cout << grid_map[{3, 4}] << '\n'; // Target
}

/*==========================================================
=            3. STATE MEMOIZATION PATTERN
==========================================================*/

// Using std::map instead of std::unordered_map because key is a pair
map<pair<int, int>, int> memo;

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
=            4. COMMON OPERATIONS
==========================================================*/

void operations() {

    unordered_map<int, int> hashmap;

    hashmap[1] = 100;
    hashmap.erase(1); // removes key 1 from the map

    cout << hashmap.size() << '\n'; // output : 0
    cout << hashmap.empty() << '\n'; // output : 1 (true)
    hashmap.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Pair Key Map (visited coordinates / grid DP cache using std::map)
// map<pair<int,int>, int> cache;

/*
==========================================================
Remember These
==========================================================

// Initialization
unordered_map<int, int> ump;

// Pair Key Map
map<pair<int,int>, int> ump;

// Operations
ump[key] = val;
ump.erase(key);
ump.count(key);

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // pairMapExample();
    // memoizationDemo();
    // operations();

    return 0;
}

/*==========================================================
=            LOOKUP OPERATIONS
==========================================================*/

void lookupExample() {

    map<int, string> mp;
    // unordered_map<int, string> mp;

    mp[1] = "One";
    mp[2] = "Two";
    mp[3] = "Three";

    // Find
    auto it = mp.find(2);

    if (it != mp.end()) {
        cout << it->first << " " << it->second << '\n';
    }

    // Count (returns 0 or 1)
    if (mp.count(3)) {
        cout << "3 exists\n";
    }

    // Access
    cout << mp.at(1) << '\n'; 

    // operator[] creates key if absent
    if(mp[100] == "") {
        cout << "100 was not present, now created with default value\n";
    }
    else {
        cout << "100 exists with value: " << mp[100] << '\n';
    }
    
}