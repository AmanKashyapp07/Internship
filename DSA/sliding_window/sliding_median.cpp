#include <bits/stdc++.h>
using namespace std;

/**
 * TWO-MULTISET STRATEGY FOR RUNNING MEDIAN
 * * We split the numbers inside the current window into two halves:
 * 1. 'lo' (Lower Half): Stores the smaller half of the elements. 
 * The largest element here (*lo.rbegin()) is the candidate median.
 * 2. 'hi' (Upper Half): Stores the larger half of the elements. 
 * The smallest element here (*hi.begin()) is right above the median.
 * * Invariant Maintained: 
 * lo.size() == hi.size()      (for even window sizes) OR
 * lo.size() == hi.size() + 1  (for odd window sizes)
 */
multiset<int> lo, hi;

/**
 * @brief Restores the size invariant between the two multisets.
 * Ensures 'lo' contains exactly the same number of elements as 'hi', 
 * or at most 1 extra element.
 */
void balance() {
    // Case 1: 'lo' has accumulated too many elements. 
    // Shift the largest element of 'lo' over to 'hi'.
    if (lo.size() > hi.size() + 1) {
        hi.insert(*lo.rbegin());
        lo.erase(prev(lo.end())); // prev(lo.end()) points to the largest element
    }
    // Case 2: 'hi' has more elements than 'lo'.
    // Shift the smallest element of 'hi' over to 'lo'.
    else if (lo.size() < hi.size()) {
        lo.insert(*hi.begin());
        hi.erase(high.begin());
    }
}

/**
 * @brief Inserts a new element into the sliding window data structure.
 * @param x The element to inject.
 */
void add(int x) {
    // Ternary Routing: If 'lo' is empty or x is smaller than/equal to the 
    // maximum element in 'lo', it belongs in the lower half. Otherwise, upper half.
    (lo.empty() || x <= *lo.rbegin() ? lo : hi).insert(x);
    
    // Insertion may break the size invariant; re-balance containers
    balance();
}

/**
 * @brief Removes an expired element that is sliding out of the window.
 * @param x The element to eject.
 */
void remove(int x) {
    // CRITICAL: Use lo.find(x) instead of lo.erase(x). 
    // lo.erase(x) deletes ALL duplicate instances of x. 
    // lo.find(x) returns an iterator to just ONE instance, allowing targeted removal.
    auto it = lo.find(x);
    
    if (it != lo.end()) {
        lo.erase(it); // Element was tracking inside the lower half
    } else {
        hi.erase(hi.find(x)); // Element must exist inside the upper half
    }
    
    // Removal may break the size invariant; re-balance containers
    balance();
}

int main() {
    // Fast I/O optimize
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int &x : a) cin >> x;

    // Step 1: Initialize the very first window of size 'k'
    for (int i = 0; i < k; i++) {
        add(a[i]);
    }
    
    // The largest element in the lower half is always the median for 
    // odd 'k', and the lower-bound median for even 'k'.
    cout << *lo.rbegin();

    // Step 2: Slide the window across the rest of the array
    for (int i = k; i < n; i++) {
        remove(a[i - k]); // Eject the element leaving the window trailing edge
        add(a[i]);       // Inject the new element entering the window leading edge
        
        cout << ' ' << *lo.rbegin(); // Print the updated window's median
    }
    cout << '\n';
    
    return 0;
}