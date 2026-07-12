/*
What: Coordinate Compression (Offline Coordinate Mapping)

Time:
- Compression : O(N log N)
- Query Rank  : O(log N)

Use When:
- Coordinate values are extremely large (e.g., coordinates <= 10^9 or 10^12)
- Total elements and queries are small (e.g., N, Q <= 10^5)
- Standard Segment Tree or Fenwick Tree would trigger MLE/Segfault on uncompressed boundaries

Core Idea:
Collects all possible coordinates, sorts them, removes duplicates, and maps each value to its relative sorted rank (0 to M-1).

Usage:
1. Push all coordinates (both initial values and query bounds!) to `coords`.
2. Compress the vector to remove duplicates.
3. Query coordinates using `get_compressed(val)`.
============================================================
*/
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Coordinate compression coordinator class
struct CoordinateCompressor {
    vector<int> coords;

    CoordinateCompressor() {}

    // Add a coordinate to be mapped
    void add(int val) {
        coords.push_back(val);
    }

    // Sorts and removes duplicate entries
    void build() {
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());
    }

    // Returns the compressed index (0-based rank) of a value in O(log M)
    int get(int val) const {
        return lower_bound(coords.begin(), coords.end(), val) - coords.begin();
    }

    // Returns the original value from a compressed rank in O(1)
    int getOriginal(int rank) const {
        return coords[rank];
    }

    // Returns count of unique coordinates
    int size() const {
        return coords.size();
    }
};

/*
============================================================
                    FULL INSTRUCTIONS
============================================================

1. Standard OA Offline Processing Loop:

   int n, q;
   cin >> n >> q;

   CoordinateCompressor comp;
   vector<int> a(n);
   for (int i = 0; i < n; i++) {
       cin >> a[i];
       comp.add(a[i]); // Add initial coordinates
   }

   struct Query { int type, l, r; };
   vector<Query> queries(q);
   for (int i = 0; i < q; i++) {
       cin >> queries[i].type >> queries[i].l >> queries[i].r;
       comp.add(queries[i].l); // Add query boundaries!
       comp.add(queries[i].r);
   }

   // Lock and build the mapping
   comp.build();

   // Now construct your Segment Tree / Fenwick Tree of size = comp.size()
   // SegmentTree st(comp.size());

   // Execute Updates and Queries using comp.get(val):
   // For updates: st.update(comp.get(a[idx]), new_val);
   // For queries: st.query(comp.get(l), comp.get(r));

============================================================
*/
