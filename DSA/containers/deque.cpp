/**
 * C++ STL Deque (Double-Ended Queue) Cheat Sheet for Online Assessments
 *
 * Description:
 *  A sequence container that allows fast insertion and deletion at both its beginning and its end.
 *  Unlike vectors, deques are not guaranteed to store elements in contiguous memory locations,
 *  but they support direct random access O(1) time.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - push_back()   : O(1) amortized
 * - pop_back()    : O(1)
 * - push_front()  : O(1) amortized
 * - pop_front()   : O(1)
 * - operator[]    : O(1)
 * - Space         : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Sliding Window Maximum / Minimum (Monotonic Deque Pattern)
 *    - Maintaining a deque of indices where the values are stored in monotonically
 *      decreasing/increasing order. Allows query of min/max in O(1) and amortized
 *      O(1) updates.
 * 2. 0-1 BFS (Shortest Path on graphs with edge weights 0 and 1)
 *    - When relaxing an edge: if weight = 0, push_front (higher priority); if weight = 1, push_back.
 *      Maintains queue sorted by distance in O(V + E) total time.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - Vector vs Deque Front Operations:
 *   - Inserting at the beginning of a `std::vector` takes O(N) time because all elements must shift.
 *   - Inserting at the beginning of a `std::deque` takes O(1) time because it allocates new chunks.
 * - Memory Overhead:
 *   - `std::deque` is implemented as an array of pointers to fixed-size memory blocks (chunks).
 *     It has slightly higher constant factor overhead than `std::vector` due to double indirection.
 *     If you only insert at the back and do not need front operations, prefer `std::vector`.
 */

#include <iostream>
#include <deque>
#include <vector>

using namespace std;

// Monotonic Queue helper class for Sliding Window Maximum
struct MonotonicQueue {
    deque<int> dq; // Stores elements (or indices) in decreasing order

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

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. BASIC DOUBLE-ENDED OPERATIONS
    deque<int> dq;
    dq.push_back(10);
    dq.push_front(20);
    dq.push_back(5);

    // Order: {20, 10, 5}
    cout << "Front element: " << dq.front() << "\n"; // Outputs 20
    cout << "Back element: " << dq.back() << "\n";   // Outputs 5
    cout << "Element at index 1: " << dq[1] << "\n"; // Outputs 10

    dq.pop_front(); // Removes 20
    dq.pop_back();  // Removes 5
    cout << "Size after pops: " << dq.size() << " (Front: " << dq.front() << ")\n"; // Outputs Size: 1, Front: 10

    // 2. MONOTONIC QUEUE SLIDING WINDOW DEMO
    // Problem: Find sliding window maximum of size K = 3
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
    for (int i = k; i < arr.size(); ++i) {
        mq.pop(arr[i - k]); // Remove element leaving the window
        mq.push(arr[i]);     // Add element entering the window
        max_results.push_back(mq.get_max());
    }

    // Expected output: {3, 3, 5, 5, 6, 7}
    cout << "Sliding window max results:\n";
    for (int val : max_results) {
        cout << val << " ";
    }
    cout << "\n";

    // 3. 0-1 BFS TEMPLATE SKELETON
    int n = 5;
    vector<vector<pair<int, int>>> adj(n); // {neighbor, weight 0 or 1}
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
                    bfs_dq.push_front(v); // weight 0: high priority, push front
                } else {
                    bfs_dq.push_back(v);  // weight 1: normal priority, push back
                }
            }
        }
    }

    return 0;
}
