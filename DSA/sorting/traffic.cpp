/**
 * CSES Problem Set
 * 
 * Problem: Traffic Lights
 * Link: https://cses.fi/problemset/task/1163
 * Category: Sorting and Searching
 * 
 * Description:
 * Find the longest street segment between traffic lights after each new light is added.
 * 
 * Logic/Approach:
 * Track light positions in a set and segment lengths in a multiset.
 */

#include <iostream>
#include <set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, n;
    cin >> x >> n;

    set<int> lights;
    multiset<int> lengths;
    lights.insert(0); 
    lights.insert(x);
    lengths.insert(x);

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        auto it = lights.upper_bound(p);

        int r = *it;
        int l = *prev(it); // prev(it) gives us an iterator to the element just before it, which is the left boundary of the segment. The time complexity of prev is O(1) because it just moves the iterator one step back. 

        lengths.erase(lengths.find(r - l));
        lengths.insert(p - l);
        lengths.insert(r - p);
        lights.insert(p);
        cout << *lengths.rbegin() << ' '; 
    }

    cout << '\n';
    return 0;
}