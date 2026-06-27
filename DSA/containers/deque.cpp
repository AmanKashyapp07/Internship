/**
 * C++ STL Deque (Double-Ended Queue) Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  A sequence container that allows fast insertion and deletion at both its beginning and its end.
 *  Unlike vectors, deques are not guaranteed to store elements in contiguous memory locations,
 *  but they support direct random access O(1) time.
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ push_back()      │ O(1) amortized   │ Inserts element at the end               │
 * │ push_front()     │ O(1) amortized   │ Inserts element at the front             │
 * │ pop_back()       │ O(1)             │ Removes element from the end             │
 * │ pop_front()      │ O(1)             │ Removes element from the front           │
 * │ operator[]       │ O(1)             │ Random access (no bounds checking)       │
 * │ at()             │ O(1)             │ Random access (throws out_of_range)      │
 * │ insert() / erase()│ O(N)             │ Linear in distance to front/back         │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Sliding Window Maximum / Minimum (Monotonic Deque Pattern)
 *  - 0-1 BFS (Shortest Path on graphs with edge weights 0 and 1)
 *
 * Interview Tricks & Pitfalls:
 *  - Vector vs Deque Front Operations: Inserting at the beginning of a `std::vector` takes O(N) time because
 *    all elements must shift. `std::deque` does this in O(1) amortized because it allocates new chunks.
 *  - Memory Overhead: `std::deque` is implemented as an array of pointers to fixed-size memory blocks (chunks).
 *    It has slightly higher constant factor overhead than `std::vector` due to double indirection.
 *    If you only insert at the back and do not need front operations, prefer `std::vector`.
 *  - Iterator Invalidation:
 *    - Inserting at the front or back invalidates references to elements, and all iterators.
 *    - Erasing at the front or back invalidates only iterators/references to the erased elements.
 *    - Inserting/erasing in the middle invalidates all iterators and references.
 */

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";

    // Different ways to construct a deque
    deque<int> dq1;                              // Empty deque
    deque<int> dq2(5, 10);                       // {10, 10, 10, 10, 10}
    deque<int> dq3 = {1, 2, 3, 4};               // Initializer list (C++11)
    deque<int> dq4(dq3.begin(), dq3.end());      // Range-based construction

    // Basic insertion and deletion at both ends
    deque<int> dq;
    dq.push_back(10);        // {10}
    dq.push_front(20);       // {20, 10}
    dq.emplace_back(30);     // {20, 10, 30}
    dq.emplace_front(40);    // {40, 20, 10, 30}

    // Access elements
    cout << "Front element: " << dq.front() << "\n"; // 40
    cout << "Back element: " << dq.back() << "\n";   // 30
    cout << "Element at index 2: " << dq[2] << "\n"; // 10

    // Removal
    dq.pop_front();          // Removes 40 -> {20, 10, 30}
    dq.pop_back();           // Removes 30 -> {20, 10}

    cout << "Size after pops: " << dq.size() << "\n";
    for (int x : dq) {
        cout << x << " ";
    }
    cout << "\n\n";
}

/*==========================================================
=            2. ITERATION & SEARCHING
==========================================================*/

void iterationAndSearch() {
    cout << "--- 2. ITERATION & SEARCHING ---\n";
    deque<int> dq = {10, 20, 30, 40};

    // Forward iteration
    cout << "Forward iteration: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";

    // Reverse iteration
    cout << "Reverse iteration: ";
    for (auto rit = dq.rbegin(); rit != dq.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << "\n";

    // C++17 Structured Bindings / Range-based for loop
    cout << "Range-based for loop: ";
    for (const auto& val : dq) {
        cout << val << " ";
    }
    cout << "\n";

    // Searching using std::find (O(N) for deque)
    auto it = find(dq.begin(), dq.end(), 30);
    if (it != dq.end()) {
        cout << "Found 30 at index: " << distance(dq.begin(), it) << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            3. MONOTONIC QUEUE SLIDING WINDOW MAXIMUM
==========================================================*/

// Monotonic Queue helper class: Maintains elements in decreasing order
struct MonotonicQueue {
    deque<int> dq; // Stores values (or indices) in decreasing order

    void push(int val) {
        // Maintain decreasing order: remove all elements smaller than val from the back
        while (!dq.empty() && dq.back() < val) {
            dq.pop_back();
        }
        dq.push_back(val);
    }

    void pop(int val) {
        // If the element leaving the window is the maximum, pop it from the front
        if (!dq.empty() && dq.front() == val) {
            dq.pop_front();
        }
    }

    int get_max() {
        return dq.front();
    }
};

void monotonicQueueDemo() {
    cout << "--- 3. MONOTONIC QUEUE SLIDING WINDOW MAX ---\n";
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    vector<int> max_results;

    MonotonicQueue mq;
    
    // Initialize first window
    for (int i = 0; i < k; ++i) {
        mq.push(arr[i]);
    }
    max_results.push_back(mq.get_max()); // First max

    // Slide window
    for (size_t i = k; i < arr.size(); ++i) {
        mq.pop(arr[i - k]); // Remove element leaving the window
        mq.push(arr[i]);     // Add element entering the window
        max_results.push_back(mq.get_max());
    }

    // Expected output: {3, 3, 5, 5, 6, 7}
    cout << "Sliding window max results:\n";
    for (int val : max_results) {
        cout << val << " ";
    }
    cout << "\n\n";
}

/*==========================================================
=            4. 0-1 BFS SKELETON
==========================================================*/

void zeroOneBFSDemo() {
    cout << "--- 4. 0-1 BFS DEMO ---\n";
    int n = 5;
    // Graph adj list: {neighbor, weight 0 or 1}
    vector<vector<pair<int, int>>> adj(n);
    adj[0].push_back({1, 0});
    adj[0].push_back({2, 1});
    adj[1].push_back({3, 1});
    adj[2].push_back({3, 0});
    adj[3].push_back({4, 0});

    vector<int> dist(n, 1e9);
    deque<int> bfs_dq;

    dist[0] = 0;
    bfs_dq.push_back(0);
    
    while (!bfs_dq.empty()) {
        int u = bfs_dq.front();
        bfs_dq.pop_front();
        
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) {
                    bfs_dq.push_front(v); // weight 0: higher priority, push front
                } else {
                    bfs_dq.push_back(v);  // weight 1: normal priority, push back
                }
            }
        }
    }

    cout << "Shortest path distances from node 0:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Node " << i << ": " << dist[i] << "\n";
    }
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
 * │ dq.push_back(x)  │ Insert element at back     │ O(1) amortized              │
 * │ dq.push_front(x) │ Insert element at front    │ O(1) amortized              │
 * │ dq.pop_back()    │ Remove element at back     │ O(1)                        │
 * │ dq.pop_front()   │ Remove element at front    │ O(1)                        │
 * │ dq.front()       │ Get front element          │ O(1)                        │
 * │ dq.back()        │ Get back element           │ O(1)                        │
 * │ dq.size()        │ Get size of deque          │ O(1)                        │
 * │ dq.empty()       │ Check if empty             │ O(1)                        │
 * │ dq.clear()       │ Remove all elements        │ O(N)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Monotonic Deque for Sliding Window Minimum
// deque<int> dq; // Stores indices of elements
// while(!dq.empty() && arr[dq.back()] >= arr[i]) dq.pop_back();
// dq.push_back(i);
// if(dq.front() < i - k + 1) dq.pop_front();

// 2. 0-1 BFS Template
// deque<int> dq;
// dq.push_front(start_node);
// if(w == 0) dq.push_front(v); else dq.push_back(v);

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
deque<int> dq;

// Fast insertion / deletion
dq.push_front(val);
dq.push_back(val);
dq.pop_front();
dq.pop_back();

// Element Access
dq.front();
dq.back();
dq[index];

// Monotonic Deque Window Slide
while (!dq.empty() && dq.back() < val) dq.pop_back();
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // iterationAndSearch();
    // monotonicQueueDemo();
    // zeroOneBFSDemo();

    return 0;
}
