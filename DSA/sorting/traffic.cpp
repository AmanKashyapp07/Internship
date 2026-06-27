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

    set<int> lights; // normal set to store the positions of the lights, we use a set because it automatically keeps the elements sorted and allows for efficient insertion and searching.
    multiset<int> lengths; // multiset to store the lengths of the segments between the lights, we use a multiset because we can have multiple segments of the same length and we need to be able to store duplicates, and it also keeps the elements sorted which allows us to easily find the longest segment by looking at the last element.

    lights.insert(0); // time complexity is O(log n) for each insertion, and we have 2 insertions here, so O(2 log n) which simplifies to O(log n).
    lights.insert(x);
    lengths.insert(x);

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        auto it = lights.upper_bound(p); // this gives us an iterator to the first element in the set that is greater than p, which will be the right boundary of the segment that p falls into. The left boundary will be the element just before this iterator, which we can get using prev(it). The time complexity of upper_bound is O(log n) because it uses binary search on the sorted set.

        int r = *it;
        int l = *prev(it); // prev(it) gives us an iterator to the element just before it, which is the left boundary of the segment. The time complexity of prev is O(1) because it just moves the iterator one step back.

        lengths.erase(lengths.find(r - l)); // we need to erase the length of the segment that is being split by the new light. We use lengths.find(r - l) to get an iterator to one occurrence of that length in the multiset, and then we erase it. The time complexity of find is O(log n) and the time complexity of erase is O(1) for an iterator, so this step is O(log n).

        lengths.insert(p - l);
        lengths.insert(r - p);

        lights.insert(p);

        cout << *lengths.rbegin() << ' '; // lengths.rbegin() gives us an iterator to the last element in the multiset, which is the largest length, and we dereference it to get the value. The time complexity of rbegin is O(1) and dereferencing is also O(1), so this step is O(1).
    }

    cout << '\n';
    return 0;
}