/**
 * ==========================================================
 *                 C++ Map Cheat Sheet (OAs)
 * ==========================================================
 *
 * map
 * ----
 * • Ordered (sorted by key)
 * • Implemented using Red-Black Tree
 * • Unique keys
 *
 * unordered_map
 * -------------
 * • Unordered
 * • Hash Table
 * • Average O(1)
 *
 * multimap
 * --------
 * • Ordered
 * • Duplicate keys allowed
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * map
 * ----
 * insert      O(log n)
 * erase       O(log n)
 * find        O(log n)
 * count       O(log n)
 * lower_bound O(log n)
 * upper_bound O(log n)
 * []          O(log n)
 *
 * unordered_map
 * -------------
 * insert      O(1) average
 * erase       O(1) average
 * find        O(1) average
 * []          O(1) average
 *
 * ==========================================================
 */

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/*==========================================================
=            1. ORDERED MAP
==========================================================*/

void orderedMapExample() {

    map<int, string> mp;

    mp[3] = "Three";
    mp[1] = "One";
    mp[2] = "Two";

    // Keys are sorted automatically

    for (auto [key, value] : mp)
        cout << key << " " << value << '\n';

    /*
    1 One
    2 Two
    3 Three
    */
}

/*==========================================================
=            2. UNORDERED MAP
==========================================================*/

void unorderedMapExample() {

    unordered_map<int, string> mp;

    mp[3] = "Three";
    mp[1] = "One";
    mp[2] = "Two";

    // Order is NOT guaranteed

    for (auto [key, value] : mp)
        cout << key << " " << value << '\n';
}

/*==========================================================
=            3. INSERT
==========================================================*/

void insertExample() {

    map<int,int> mp;

    mp[5] = 100;

    mp.insert({2,50});

    mp.emplace(7,200);

}

/*==========================================================
=            4. ACCESS
==========================================================*/

void accessExample() {

    map<int,int> mp;

    mp[1] = 10;

    cout << mp[1] << '\n';

    // Creates key if absent

    cout << mp[5] << '\n';   // 0
}

/*==========================================================
=            5. FIND
==========================================================*/

void findExample() {

    map<int,int> mp;

    mp[2] = 20;

    auto it = mp.find(2);

    if (it != mp.end())
        cout << it->first << " "
             << it->second << '\n';
}

/*==========================================================
=            6. COUNT
==========================================================*/

void countExample() {

    map<int,int> mp;

    mp[5] = 10;

    cout << mp.count(5) << '\n'; // 1

    cout << mp.count(8) << '\n'; // 0
}
// find returns an iterator to the element if found, or mp.end() if not found.
// count returns 1 if the key exists, or 0 if it does not.
// for majoity of cases, find is preferred when you need to access the value, while count is used for existence checks.

/*==========================================================
=            7. ERASE
==========================================================*/

void eraseExample() {

    map<int,int> mp;

    mp[1] = 10;
    mp[2] = 20;
    mp[3] = 30;

    mp.erase(2);

    auto it = mp.find(3);

    mp.erase(it);
}

/*==========================================================
=            8. ITERATION
==========================================================*/

void iterateExample() {

    map<int,int> mp;

    mp[1]=10;
    mp[2]=20;
    mp[3]=30;

    for (auto [key,value] : mp)
        cout << key << " " << value << '\n';

    cout << '\n';

    for (auto it = mp.begin(); it != mp.end(); it++)
        cout << it->first << " "
             << it->second << '\n';
}

/*==========================================================
=            9. REVERSE ITERATION
==========================================================*/

void reverseIteration() {

    map<int,int> mp;

    mp[1]=10;
    mp[2]=20;
    mp[3]=30;

    for (auto it = mp.rbegin(); it != mp.rend(); it++)
        cout << it->first << " "
             << it->second << '\n';
}

/*==========================================================
=            10. LOWER BOUND
==========================================================*/

void lowerBoundExample() {

    map<int,int> mp;

    mp[2]=20;
    mp[5]=50;
    mp[8]=80;

    auto it = mp.lower_bound(4);
    // lower_bound only works on ordered containers like map, multimap, set, multiset. It returns an iterator to the first element that is not less than the given key.

    if (it != mp.end())
        cout << it->first << '\n';

    // Output : 5
}

/*==========================================================
=            11. UPPER BOUND
==========================================================*/

void upperBoundExample() {

    map<int,int> mp;

    mp[2]=20;
    mp[5]=50;
    mp[8]=80;

    auto it = mp.upper_bound(5);

    if (it != mp.end())
        cout << it->first << '\n';

    // Output : 8
}

/*==========================================================
=            12. MULTIMAP
==========================================================*/

void multimapExample() {

    multimap<int,string> mp;

    mp.insert({1,"A"});
    mp.insert({1,"B"});
    mp.insert({1,"C"});

    for (auto [key,val] : mp)
        cout << key << " " << val << '\n';
}

/*==========================================================
=            13. FREQUENCY COUNT
==========================================================*/

void frequencyCount() {

    vector<int> nums = {1,2,2,3,3,3};

    map<int,int> freq;

    for (int x : nums)
        freq[x]++;

    for (auto [num,cnt] : freq)
        cout << num << " -> " << cnt << '\n';
}

/*==========================================================
=            14. COMMON OPERATIONS
==========================================================*/

void operations() {

    map<int,int> mp;

    mp[1] = 100;

    mp.insert({2,200});

    mp.erase(1);

    cout << mp.count(2) << '\n';

    cout << mp.size() << '\n';

    cout << mp.empty() << '\n';

    mp.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Ordered Map
map<int,int> mp;

// Unordered Map
unordered_map<int,int> ump;

// Multimap
multimap<int,int> mmp;

// Map of Vectors
map<int, vector<int>> graph;

// Frequency Map
map<int,int> freq;

// String Frequency
unordered_map<string,int> wordFreq;

// Pair Key Map
map<pair<int,int>, int> pairMap;

/*
==========================================================
Remember These
==========================================================

// Ordered Map
map<int,int> mp;

// Unordered Map
unordered_map<int,int> ump;

// Frequency
freq[x]++;

// Exists?
if (mp.count(x))

// Find
auto it = mp.find(x);

// Erase
mp.erase(x);

// Iterate
for (auto [k,v] : mp)

// Lower Bound
auto it = mp.lower_bound(x);

// Upper Bound
auto it = mp.upper_bound(x);

==========================================================

Interview Rule

Need sorted keys?
    -> map

Need fastest lookup?
    -> unordered_map

Need duplicate keys?
    -> multimap

==========================================================
*/

int main() {

    // Uncomment to test

    // orderedMapExample();
    // unorderedMapExample();
    // insertExample();
    // accessExample();
    // findExample();
    // countExample();
    // eraseExample();
    // iterateExample();
    // reverseIteration();
    // lowerBoundExample();
    // upperBoundExample();
    // multimapExample();
    // frequencyCount();
    // operations();

    return 0;
}