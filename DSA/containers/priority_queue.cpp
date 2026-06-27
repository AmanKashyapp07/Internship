/**
 * C++ STL Priority Queue (Heap) Cheat Sheet for Online Assessments
 *
 * Description:
 *  A container adapter that provides constant time lookup of the largest (by default) element,
 *  at the expense of logarithmic insertion and extraction.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - push(x)       : O(log N)
 * - pop()         : O(log N)
 * - top()         : O(1)
 * - size(), empty(): O(1)
 * - Build heap from vector: O(N) — using constructor pq(v.begin(), v.end())
 * - Space         : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. "K-th Largest / Smallest Element"
 *    - To keep K largest elements: maintain a Min-Heap of size K. When size > K, pop.
 *      Time complexity: O(N log K), Space: O(K).
 * 2. "Running Median" / "Continuous Median"
 *    - Maintain two heaps: Max-Heap (left half) and Min-Heap (right half).
 *      Ensure sizes differ by at most 1, and max of left <= min of right.
 * 3. Shortest Paths (Dijkstra) & Minimum Spanning Tree (Prim)
 *    - Always pop the vertex with the minimum distance or edge weight.
 * 4. Merging K Sorted Lists (K-Way Merge)
 *    - Push the first element of each list into a Min-Heap.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - Min-Heap Shortcut: Instead of using greater<T>, you can store negative values (-val)
 *   in a standard Max-Heap. Just remember to negate them back upon popping!
 * - Custom Comparator: For std::priority_queue, the comparator returns true if the
 *   first argument has LOWER priority than the second (i.e. should be placed towards the bottom).
 *   It behaves opposite to std::sort!
 * - Lazy Deletion Pattern: Since std::priority_queue does not support deleting an arbitrary
 *   element or updating keys (decrease-key), we perform "lazy deletion". Push new states
 *   into the heap, and when popping, discard stale/already-processed states.
 */

#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

// Struct-based Custom Comparator for custom objects / coordinates
struct Point {
    int x, y;
};

// We want points closer to the origin (smaller x^2 + y^2) to have HIGHER priority (min-heap behavior)
struct ComparePoints {
    bool operator()(const Point& a, const Point& b) {
        int distA = a.x * a.x + a.y * a.y;
        int distB = b.x * b.x + b.y * b.y;
        // Returns true if 'a' should be placed deeper than 'b' (has lower priority)
        // Hence, if distA > distB, 'a' goes down. This builds a Min-Heap of distances.
        return distA > distB;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. STANDARD MAX-HEAP
    priority_queue<int> max_heap;
    max_heap.push(10);
    max_heap.push(30);
    max_heap.push(20);
    
    // Elements: top is 30
    cout << "Max-heap top: " << max_heap.top() << "\n"; // Outputs 30
    max_heap.pop();
    cout << "Max-heap top after pop: " << max_heap.top() << "\n"; // Outputs 20

    // 2. STANDARD MIN-HEAP
    priority_queue<int, vector<int>, greater<int>> min_heap;
    min_heap.push(10);
    min_heap.push(30);
    min_heap.push(20);
    
    cout << "Min-heap top: " << min_heap.top() << "\n"; // Outputs 10
    min_heap.pop();
    cout << "Min-heap top after pop: " << min_heap.top() << "\n"; // Outputs 20

    // 3. O(N) HEAPIFY CONSTRUCTION (Very useful in OAs to save time!)
    vector<int> nums = {4, 1, 3, 5, 2};
    // Creating heap directly using range constructor runs in O(N) time
    priority_queue<int> pq_from_vec(nums.begin(), nums.end());
    cout << "Heapify top: " << pq_from_vec.top() << "\n"; // Outputs 5

    // 4. CUSTOM COMPARATOR USING STRUCT
    priority_queue<Point, vector<Point>, ComparePoints> point_pq;
    point_pq.push({3, 4}); // dist = 25
    point_pq.push({1, 1}); // dist = 2
    point_pq.push({0, 2}); // dist = 4
    
    Point closest = point_pq.top();
    cout << "Closest point: (" << closest.x << ", " << closest.y << ")\n"; // Outputs (1, 1)

    // 5. CUSTOM COMPARATOR USING LAMBDA & decltype (Clean & inline for OAs)
    auto lambda_cmp = [](const pair<int, int>& a, const pair<int, int>& b) {
        // Compare by second value descending, then first value ascending
        if (a.second != b.second) {
            return a.second < b.second; // smaller second value has lower priority
        }
        return a.first > b.first; // larger first value has lower priority
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(lambda_cmp)> lambda_pq(lambda_cmp);
    lambda_pq.push({5, 10});
    lambda_pq.push({6, 10});
    lambda_pq.push({1, 20});
    
    // Expected order of popping: {1, 20}, {5, 10}, {6, 10}
    pair<int, int> best = lambda_pq.top();
    cout << "Lambda PQ top: {" << best.first << ", " << best.second << "}\n"; // Outputs {1, 20}

    // 6. LAZY DELETION / DIJKSTRA SKELETON
    int n = 5;
    vector<int> dist(n, 1e9);
    vector<bool> visited(n, false);
    
    // min-heap storing {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> dijkstra_pq;
    
    dist[0] = 0;
    dijkstra_pq.push({0, 0});
    
    while (!dijkstra_pq.empty()) {
        auto [d, u] = dijkstra_pq.top();
        dijkstra_pq.pop();
        
        // LAZY DELETION check: if we already found a shorter path to 'u', discard this state.
        if (d > dist[u]) continue; 
        if (visited[u]) continue;
        visited[u] = true;
        
        // Node 'u' is now finalized. Relax its neighbors...
    }

    return 0;
}
