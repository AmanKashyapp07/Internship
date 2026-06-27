/*
==================== MULTISET ====================

Properties:
- Sorted order
- Allows duplicates
- Red-Black Tree

Complexities:
insert            O(log n)
erase(iterator)   O(log n)
erase(value)      O(log n + freq)
find              O(log n)
count             O(log n + freq)
lower_bound       O(log n)
upper_bound       O(log n)
size              O(1)

Use:
- Need sorting      -> set / multiset
- Need duplicates   -> multiset
- Need lower_bound  -> set / multiset
==================================================
*/

multiset<int> ms;

// Insert
ms.insert(x);

// Find
auto it = ms.find(x);

// Count occurrences
ms.count(x);

// Erase one occurrence
if ((it = ms.find(x)) != ms.end())
    ms.erase(it);

// Erase all occurrences
ms.erase(x);

// Minimum / Maximum
*ms.begin();
*ms.rbegin();          // or *prev(ms.end())

// First element >= x
it = ms.lower_bound(x);

// First element > x
it = ms.upper_bound(x);

// Largest element <= x
it = ms.upper_bound(x);
if (it != ms.begin()) {
    --it;
    cout << *it;
}

// Largest element < x
it = ms.lower_bound(x);
if (it != ms.begin()) {
    --it;
    cout << *it;
}

// Traverse
for (auto x : ms) cout << x << ' ';

// Size / Empty
ms.size();
ms.empty();

// Clear
ms.clear();