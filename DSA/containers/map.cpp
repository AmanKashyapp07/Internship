/**
 * C++ STL Map (Ordered Map) Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  An associative container that stores key-value pairs sorted by key.
 *  Implemented internally as a self-balancing Binary Search Tree (Red-Black Tree).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ insert()         │ O(log N)         │ Inserts a key-value pair                 │
 * │ emplace()        │ O(log N)         │ Inserts in-place                         │
 * │ erase(key)       │ O(log N)         │ Removes key-value pair                   │
 * │ find(key)        │ O(log N)         │ Search for key                           │
 * │ count(key)       │ O(log N)         │ Returns 1 if present, 0 otherwise        │
 * │ lower_bound()    │ O(log N)         │ First key-value pair with key >= target  │
 * │ upper_bound()    │ O(log N)         │ First key-value pair with key > target   │
 * │ operator[key]    │ O(log N)         │ Access/insert key                        │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Frequency mapping in sorted order
 *  - Coordinate compression (assigning ranks to sparse values)
 *  - Sweep-line algorithms where events are processed at sorted event coordinates
 *
 * Interview Tricks & Pitfalls:
 *  - The Autocreation Trap: Checking if a key exists via `if (mp[key] == 0)` will *automatically insert*
 *    the key with a default value of `0` if it does not exist. This inflates map size and memory consumption.
 *    To search without modifying the map, ALWAYS use `mp.count(key)` or `mp.find(key)`.
 *  - Member Bounds: Like `std::set`, never use the global `std::lower_bound` on maps. Always use
 *    `mp.lower_bound(key)` for O(log N) performance.
 */

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & INSERTION
==========================================================*/

void insertionExample() {
    cout << "--- 1. CONSTRUCTION & INSERTION ---\n";
    
    // Ordered map: sorted by key ascending
    map<int, string> mp;

    // Insertion Method A: Operator[]
    mp[3] = "Three";
    mp[1] = "One";

    // Insertion Method B: std::insert
    mp.insert({2, "Two"});

    // Insertion Method C: std::emplace (In-place construction)
    mp.emplace(5, "Five");

    // Keys are sorted automatically: 1, 2, 3, 5
    cout << "Map elements after insertion:\n";
    for (auto [key, val] : mp) {
        cout << "  " << key << " -> " << val << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            2. ELEMENT ACCESS & SEARCH (THE TRAP)
==========================================================*/

void accessAndSearch() {
    cout << "--- 2. ELEMENT ACCESS & SEARCH TRAPS ---\n";
    map<int, int> mp;
    mp[10] = 100;

    // THE AUTOCREATION TRAP DEMO
    cout << "Map size before trap check: " << mp.size() << "\n"; // Size is 1
    if (mp[42] == 0) {
        cout << "  Key 42 accessed via operator[].\n";
    }
    cout << "Map size after trap check: " << mp.size() << "\n"; // Size is now 2! Key 42 was inserted.

    // Safe Search Method A: mp.count(key)
    if (mp.count(10)) {
        cout << "Key 10 exists (checked safely via count)\n";
    }

    // Safe Search Method B: mp.find(key)
    auto it = mp.find(10);
    if (it != mp.end()) {
        cout << "Key 10 exists, Value = " << it->second << " (checked safely via find)\n";
    }

    // Erase keys
    mp.erase(42); // Remove by key
    cout << "Map size after erasing key 42: " << mp.size() << "\n\n";
}

/*==========================================================
=            3. ITERATION (FORWARD & REVERSE)
==========================================================*/

void iterationExample() {
    cout << "--- 3. ITERATION (FORWARD & REVERSE) ---\n";
    map<int, int> mp = {{1, 10}, {2, 20}, {3, 30}};

    // C++17 Structured Bindings (Forward)
    cout << "Forward iteration: ";
    for (auto [key, val] : mp) {
        cout << "[" << key << ":" << val << "] ";
    }
    cout << "\n";

    // Reverse iteration
    cout << "Reverse iteration: ";
    for (auto it = mp.rbegin(); it != mp.rend(); ++it) {
        cout << "[" << it->first << ":" << it->second << "] ";
    }
    cout << "\n\n";
}

/*==========================================================
=            4. BOUNDS & MULTIMAP
==========================================================*/

void boundsAndMultimap() {
    cout << "--- 4. BOUNDS & MULTIMAP ---\n";
    map<int, int> mp = {{2, 20}, {5, 50}, {8, 80}};

    // Member lower_bound (first key >= target)
    auto lb = mp.lower_bound(4); // Points to {5, 50}
    if (lb != mp.end()) {
        cout << "Lower bound of 4: " << lb->first << " -> " << lb->second << "\n";
    }

    // Member upper_bound (first key > target)
    auto ub = mp.upper_bound(5); // Points to {8, 80}
    if (ub != mp.end()) {
        cout << "Upper bound of 5: " << ub->first << " -> " << ub->second << "\n";
    }

    // Multimap: allows duplicate keys
    multimap<int, string> mmp;
    mmp.insert({1, "A"});
    mmp.insert({1, "B"});
    mmp.insert({1, "C"});
    cout << "Multimap contents for key 1:\n";
    auto [range_start, range_end] = mmp.equal_range(1);
    for (auto it = range_start; it != range_end; ++it) {
        cout << "  " << it->first << " -> " << it->second << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            5. COMMON INTERVIEW PATTERNS
==========================================================*/

// Pattern A: Frequency count mapping
void frequencyCountDemo() {
    vector<int> nums = {1, 2, 2, 3, 3, 3};
    map<int, int> freq;
    for (int x : nums) {
        freq[x]++;
    }
    cout << "Frequency count in sorted order:\n";
    for (auto [num, cnt] : freq) {
        cout << "  " << num << " occurs " << cnt << " times\n";
    }
}

// Pattern B: Coordinate Compression (mapping sparse values to ranks 0..N-1)
void coordinateCompressionDemo() {
    vector<int> coords = {1000, 50, 1000, 200, 50};
    map<int, int> compressed;
    
    // Insert unique coordinates in sorted order
    for (int x : coords) {
        compressed[x] = 0;
    }

    // Assign ranks
    int rank = 0;
    for (auto& [val, rank_val] : compressed) {
        rank_val = rank++;
    }

    cout << "Coordinate compression mapping:\n";
    for (int x : coords) {
        cout << "  Original: " << x << " -> Compressed Rank: " << compressed[x] << "\n";
    }
}

void interviewPatterns() {
    cout << "--- 5. COMMON INTERVIEW PATTERNS ---\n";
    frequencyCountDemo();
    coordinateCompressionDemo();
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
 * │ mp[key] = value  │ Insert or update key       │ O(log N)                    │
 * │ mp.insert(pair)  │ Insert pair if key absent  │ O(log N)                    │
 * │ mp.erase(key)    │ Erases key-value pair      │ O(log N)                    │
 * │ mp.find(key)     │ Iterator to key if exists  │ O(log N)                    │
 * │ mp.count(key)    │ Returns 1 if key exists    │ O(log N)                    │
 * │ mp.lower_bound(k)│ First iterator with key>=k │ O(log N)                    │
 * │ mp.upper_bound(k)│ First iterator with key>k  │ O(log N)                    │
 * │ mp.size()        │ Get element count          │ O(1)                        │
 * │ mp.empty()       │ Check if map is empty      │ O(1)                        │
 * │ mp.clear()       │ Remove all elements        │ O(N)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Coordinate Compression Template
// map<int, int> ranks;
// for (int x : arr) ranks[x] = 0;
// int r = 0;
// for (auto& [val, rank_val] : ranks) rank_val = r++;

// 2. Map of Vectors (Adjacency List / Grouping)
// map<int, vector<int>> groups;
// groups[key].push_back(val);

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
map<int, int> mp;

// Access safely (Avoid [] autocreation)
if (mp.count(key)) {
    int val = mp[key];
}

// Searching
auto it = mp.find(key);
if (it != mp.end()) {
    int val = it->second;
}

// Insertion & Deletion
mp[key] = val;
mp.erase(key);

// Bounds
auto it_lb = mp.lower_bound(k);
auto it_ub = mp.upper_bound(k);

// Structured Binding Iteration
for (auto [k, v] : mp) { ... }
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // insertionExample();
    // accessAndSearch();
    // iterationExample();
    // boundsAndMultimap();
    // interviewPatterns();

    return 0;
}