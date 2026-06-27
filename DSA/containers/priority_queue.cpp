/**
 * C++ STL Priority Queue (Heap) Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  A container adaptor that provides constant time lookup of the largest (by default)
 *  element, at the expense of logarithmic insertion and extraction.
 *  Implemented internally using a binary heap (usually stored in a std::vector).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ push()           │ O(log N)         │ Inserts element into the heap            │
 * │ pop()            │ O(log N)         │ Removes the top element                  │
 * │ top()            │ O(1)             │ Accesses the top element                 │
 * │ size()           │ O(1)             │ Returns number of elements               │
 * │ empty()          │ O(1)             │ Checks if the container is empty         │
 * │ construction     │ O(N)             │ If initialized from a vector/range       │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Find Top K elements (Min-heap of size K)
 *  - K-way merge (Merge K sorted lists)
 *  - Dijkstra's shortest path algorithm
 *  - Prim's Minimum Spanning Tree algorithm
 *
 * Interview Tricks & Pitfalls:
 *  - The Min-Heap Trick: By default, `priority_queue<T>` is a Max-Heap. To make it a Min-Heap,
 *    you must pass three arguments: `priority_queue<T, vector<T>, greater<T>> pq;`.
 *  - Inverse Comparator Logic: In `std::sort`, returning `true` means `a` goes before `b`.
 *    In `std::priority_queue`, the custom comparator's `operator()` returning `true` means `a` has
 *    LOWER priority than `b` (meaning `a` will sink towards the bottom of the heap).
 *    Therefore, `a > b` creates a Min-Heap (smaller elements float to top).
 *  - No Iteration: Unlike sets/vectors, you CANNOT iterate through a priority queue. To view elements,
 *    you must pop them one by one.
 *  - Undefined Behavior: Always check `!pq.empty()` before calling `pq.top()` or `pq.pop()`.
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

/*==========================================================
=            1. BASIC OPERATIONS & MAX HEAP
==========================================================*/

void maxHeapExample() {
    cout << "--- 1. MAX HEAP BASIC OPERATIONS ---\n";
    
    // Default priority queue is a Max-Heap
    priority_queue<int> pq;

    pq.push(5);
    pq.push(2);
    pq.emplace(8); // In-place construction

    cout << "Size: " << pq.size() << "\n"; // 3
    cout << "Top element: " << pq.top() << "\n"; // 8

    pq.pop(); // Removes 8
    cout << "Top element after pop: " << pq.top() << "\n"; // 5
    
    cout << "All elements popped in order: ";
    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << "\n\n";
}

/*==========================================================
=            2. MIN HEAP & PAIR HEAPS
==========================================================*/

void minHeapExample() {
    cout << "--- 2. MIN HEAP & PAIR HEAPS ---\n";

    // Min-Heap declaration syntax
    priority_queue<int, vector<int>, greater<int>> min_pq;

    min_pq.push(5);
    min_pq.push(2);
    min_pq.push(8);

    cout << "Min Heap Top (expected 2): " << min_pq.top() << "\n";

    // Max-Heap of Pairs: Lexicographically largest pair floats to the top
    priority_queue<pair<int, int>> pair_max_pq;
    pair_max_pq.push({5, 10});
    pair_max_pq.push({3, 20});
    pair_max_pq.push({5, 7});
    
    auto [f_val, s_val] = pair_max_pq.top();
    cout << "Pair Max Heap Top: (" << f_val << ", " << s_val << ")\n"; // (5, 10)

    // Min-Heap of Pairs (Commonly used in Dijkstra's)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pair_min_pq;
    pair_min_pq.push({4, 2});
    pair_min_pq.push({1, 5});
    pair_min_pq.push({3, 7});

    auto [dist, node] = pair_min_pq.top();
    cout << "Pair Min Heap Top (Dijkstra): Dist=" << dist << ", Node=" << node << "\n\n"; // Dist=1, Node=5
}

/*==========================================================
=            3. CUSTOM COMPARATORS (STRUCT & LAMBDA)
==========================================================*/

// Struct Custom Comparator (For int)
struct cmpInt {
    bool operator()(int a, int b) {
        return a > b; // Returns true if a > b, putting larger values at the bottom (Min-Heap)
    }
};

// Struct Custom Comparator (For pair: sort by second value ascending)
struct cmpPair {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second; // Smaller second value gets higher priority (floats to top)
    }
};

void customComparatorExample() {
    cout << "--- 3. CUSTOM COMPARATOR DEMO ---\n";

    // 1. Struct Comparator for int
    priority_queue<int, vector<int>, cmpInt> custom_int_pq;
    custom_int_pq.push(5);
    custom_int_pq.push(2);
    custom_int_pq.push(8);
    cout << "Custom Int PQ Top (Min-Heap expected 2): " << custom_int_pq.top() << "\n";

    // 2. Struct Comparator for pair
    priority_queue<pair<int, int>, vector<pair<int, int>>, cmpPair> custom_pair_pq;
    custom_pair_pq.push({10, 5});
    custom_pair_pq.push({2, 1});
    custom_pair_pq.push({7, 3});
    auto [x, y] = custom_pair_pq.top();
    cout << "Custom Pair PQ Top (expected 2 1): " << x << " " << y << "\n";

    // 3. Lambda Comparator (C++11/17 style)
    auto lambda_cmp = [](int a, int b) {
        return a > b; // Min-Heap
    };
    // Must pass the lambda variable to the constructor
    priority_queue<int, vector<int>, decltype(lambda_cmp)> lambda_pq(lambda_cmp);
    lambda_pq.push(5);
    lambda_pq.push(2);
    lambda_pq.push(8);
    cout << "Lambda PQ Top (expected 2): " << lambda_pq.top() << "\n\n";
}

/*==========================================================
=            4. COMMON INTERVIEW PATTERNS
==========================================================*/

// Pattern A: Find Top K Elements of a vector using Min-Heap
vector<int> findTopK(const vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> min_heap;
    for (int x : nums) {
        min_heap.push(x);
        if (min_heap.size() > (size_t)k) {
            min_heap.pop();
        }
    }
    vector<int> result;
    while (!min_heap.empty()) {
        result.push_back(min_heap.top());
        min_heap.pop();
    }
    reverse(result.begin(), result.end()); // Top largest first
    return result;
}

// Pattern B: Dijkstra shortest path skeleton
void dijkstraSkeleton(int n, const vector<vector<pair<int, int>>>& adj, int start) {
    vector<int> dist(n, 1e9);
    // Min-heap storing: {distance, node_id}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Very important optimization: skip if we found a better path already
        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

void interviewPatternsDemo() {
    cout << "--- 4. COMMON INTERVIEW PATTERNS ---\n";
    vector<int> nums = {3, 2, 1, 5, 6, 4};
    vector<int> top3 = findTopK(nums, 3);
    cout << "Top 3 elements: ";
    for (int x : top3) {
        cout << x << " ";
    }
    cout << "\n\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ pq.push(x)       │ Push element into heap     │ O(log N)                    │
 * │ pq.emplace(args) │ Construct element in heap  │ O(log N)                    │
 * │ pq.pop()         │ Remove top element         │ O(log N)                    │
 * │ pq.top()         │ Access top element         │ O(1)                        │
 * │ pq.size()        │ Get number of elements     │ O(1)                        │
 * │ pq.empty()       │ Check if empty             │ O(1)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Max Heap
// priority_queue<int> maxHeap;

// 2. Min Heap
// priority_queue<int, vector<int>, greater<int>> minHeap;

// 3. Min Heap of Pairs (Dijkstra)
// priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

// 4. Custom Comparator Struct
// struct CustomCmp {
//     bool operator()(const Type& a, const Type& b) {
//         return a.priority_val > b.priority_val; // true if a should go BELOW b (Min-Heap behavior)
//     }
// };
// priority_queue<Type, vector<Type>, CustomCmp> pq;

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Default Max Heap
priority_queue<int> pq;

// Standard Min Heap
priority_queue<int, vector<int>, greater<int>> min_pq;

// Dijkstra Queue
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

// Access and Pop
if (!pq.empty()) {
    int val = pq.top();
    pq.pop();
}
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // maxHeapExample();
    // minHeapExample();
    // customComparatorExample();
    // interviewPatternsDemo();

    return 0;
}