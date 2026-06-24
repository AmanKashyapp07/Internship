/**
 * C++ STL Map (Ordered Map) Cheat Sheet for Online Assessments
 *
 * Description:
 *  An associative container that stores key-value pairs sorted by keys.
 *  Implemented internally as a self-balancing Binary Search Tree (typically a Red-Black Tree).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - insert / emplace : O(log N)
 * - erase(key)       : O(log N)
 * - find(key)        : O(log N)
 * - count(key)       : O(log N)
 * - operator[key]    : O(log N)
 * - lower_bound(key) : O(log N) — returns iterator to first key >= target
 * - upper_bound(key) : O(log N) — returns iterator to first key > target
 * - traversal        : O(N) in sorted order of keys
 * - Space            : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Coordinate Compression
 *    - Maps large coordinate values (e.g. -10^9 to 10^9) to indices (0 to M-1)
 *      while preserving their sorted ordering.
 * 2. Range Queries / Nearest Key Lookups
 *    - When you need to answer queries like "find the smallest active timestamp >= T".
 * 3. Dynamic Sorted State
 *    - To maintain keys dynamically (insert/delete) while needing access to minimum
 *      key (`mp.begin()`) or maximum key (`mp.rbegin()`).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - CRITICAL PERFORMANCE TRAP: Never use the global std::lower_bound(mp.begin(), mp.end(), k)
 *   on a map. Because map iterators are Bidirectional (not RandomAccess), it runs in O(N).
 *   ALWAYS use the member function: mp.lower_bound(k) which runs in O(log N).
 * - Default Insertion Trap: Referencing a key using `mp[key]` inserts it into the map
 *   with its default value (e.g. 0 for integers) if it doesn't already exist.
 *   Use `mp.count(key)` or `mp.find(key)` to check existence safely without adding elements.
 * - Custom Sort Keys: Pass a custom comparator as a template argument to sort keys differently,
 *   e.g. `map<int, string, greater<int>>` to keep keys in descending order.
 */

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Custom Key Comparator for custom sorting of keys
struct CompareLength {
    bool operator()(const string& a, const string& b) const {
        if (a.length() != b.length()) {
            return a.length() < b.length(); // Sort shorter strings first
        }
        return a < b; // Alphabetical tie-breaker
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. DECLARATION & INSERTION
    map<string, int> age_map;
    age_map["Alice"] = 25;                  // operator[] syntax (inserts if not present)
    age_map.insert({"Bob", 30});            // insert pair
    age_map.emplace("Charlie", 35);         // emplace (more efficient)

    // 2. CHECK EXISTENCE WITHOUT INSERTION
    // Trap warning: if we did age_map["Diana"] == 0, "Diana" gets added with value 0!
    if (age_map.count("Diana") == 0) {
        // "Diana" is confirmed not in the map
    }
    
    auto it_find = age_map.find("Bob");
    if (it_find != age_map.end()) {
        cout << "Bob's age: " << it_find->second << "\n"; // Outputs 30
    }

    // 3. MEMBER BOUNDS VS GLOBAL BOUNDS (Crucial OA Speed Tip)
    map<int, string> event_map;
    event_map[10] = "Start";
    event_map[20] = "Processing";
    event_map[30] = "End";

    // Fast: O(log N)
    auto it_fast = event_map.lower_bound(15); 
    cout << "Lower bound of 15: {" << it_fast->first << ", " << it_fast->second << "}\n"; // Outputs {20, Processing}

    // SLOW: O(N) — DO NOT USE IN OAs!
    // auto it_slow = lower_bound(event_map.begin(), event_map.end(), 15);

    // 4. COORDINATE COMPRESSION PATTERN
    // Given arbitrary large values, compress them to range [0, unique_count - 1]
    vector<int> large_coords = {1000000000, -500, 200, 1000000000, 200};
    
    // Step a: Add elements to map to unique-ify and sort them
    map<int, int> compress_map;
    for (int val : large_coords) {
        compress_map[val] = 0; // Temp value
    }
    
    // Step b: Assign rank index
    int rank = 0;
    for (auto& pair : compress_map) {
        pair.second = rank++;
    }
    
    // Step c: Retrieve compressed coordinates
    // -500 -> rank 0, 200 -> rank 1, 1000000000 -> rank 2
    cout << "Compressed -500: " << compress_map[-500] << "\n"; // Outputs 0
    cout << "Compressed 200: " << compress_map[200] << "\n";   // Outputs 1

    // 5. REVERSE ITERATION & MIN/MAX KEYS
    // Min Key: map.begin()
    cout << "Min key in event_map: " << event_map.begin()->first << "\n"; // Outputs 10
    // Max Key: map.rbegin()
    cout << "Max key in event_map: " << event_map.rbegin()->first << "\n"; // Outputs 30

    // Iterating in reverse order
    for (auto rit = event_map.rbegin(); rit != event_map.rend(); ++rit) {
        cout << rit->first << " -> " << rit->second << "\n";
    }

    // 6. CUSTOM KEY SORTING
    map<string, int, CompareLength> len_map;
    len_map["aaa"] = 3;
    len_map["z"] = 1;
    len_map["bb"] = 2;
    // Keys will be sorted as: "z" (len 1), "bb" (len 2), "aaa" (len 3)
    cout << "First key in length-sorted map: " << len_map.begin()->first << "\n"; // Outputs "z"

    return 0;
}
