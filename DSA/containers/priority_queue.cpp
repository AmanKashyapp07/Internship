/**
 * ==========================================================
 *             C++ Priority Queue Cheat Sheet (OAs)
 * ==========================================================
 *
 * priority_queue
 * --------------
 * • Max-heap by default
 * • Binary heap implementation (vector backed)
 * • No iterator support
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * priority_queue
 * --------------
 * push        O(log n)
 * pop         O(log n)
 * top         O(1)
 * size        O(1)
 * empty       O(1)
 *
 * ==========================================================
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

/*==========================================================
=            1. MAX HEAP (Default)
==========================================================*/

void maxHeapExample() {

    priority_queue<int> pq;

    pq.push(5);
    pq.push(2);
    pq.push(8);

    cout << pq.top() << '\n';   // 8
    pq.pop();
    cout << pq.top() << '\n';   // 5
}

/*==========================================================
=            2. MIN HEAP
==========================================================*/

void minHeapExample() {

    priority_queue<int, vector<int>, greater<int>> pq;

    pq.push(5);
    pq.push(2);
    pq.push(8);

    cout << pq.top() << '\n';   // 2
}

/*==========================================================
=            3. MAX HEAP OF PAIRS
==========================================================*/

void pairMaxHeapExample() {

    priority_queue<pair<int,int>> pq;

    pq.push({5,10});
    pq.push({3,20});
    pq.push({5,7});

    cout << pq.top().first << " "
         << pq.top().second << '\n'; // 5 10
}

/*==========================================================
=            4. MIN HEAP OF PAIRS (Dijkstra)
==========================================================*/

void pairMinHeapExample() {

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    pq.push({4,2});
    pq.push({1,5});
    pq.push({3,7});

    auto [dist, node] = pq.top();
    cout << dist << " " << node << '\n'; // 1 5
}

/*==========================================================
=            5. CUSTOM COMPARATOR (INT)
==========================================================*/

struct cmpInt {
    bool operator()(int a, int b) {
        return a > b;       // Min Heap
    }
};

void customComparatorInt() {

    priority_queue<int, vector<int>, cmpInt> pq;

    pq.push(5);
    pq.push(2);
    pq.push(8);

    cout << pq.top() << '\n'; // 2
}

/*==========================================================
=            6. CUSTOM COMPARATOR (PAIR)
==========================================================*/

struct cmpPair {
    bool operator()(pair<int,int> a, pair<int,int> b) {
        // Smaller second value gets higher priority
        return a.second > b.second;
    }
};

void customComparatorPair() {

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        cmpPair
    > pq;

    pq.push({10,5});
    pq.push({2,1});
    pq.push({7,3});

    auto [x,y] = pq.top();
    cout << x << " " << y << '\n'; // 2 1
}

/*==========================================================
=            7. LAMBDA COMPARATOR
==========================================================*/

void lambdaComparator() {

    auto cmp = [](int a, int b) {
        return a > b; // Min-Heap
    };

    priority_queue<
        int,
        vector<int>,
        decltype(cmp)
    > pq(cmp);

    pq.push(5);
    pq.push(2);
    pq.push(8);

    cout << pq.top() << '\n'; // 2
}

/*==========================================================
=            8. TOP K ELEMENTS PATTERN
==========================================================*/

void topKDemo() {

    vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 3;
    priority_queue<int, vector<int>, greater<int>> min_heap;

    for (int x : nums) {
        min_heap.push(x);
        if (min_heap.size() > (size_t)k) {
            min_heap.pop();
        }
    }

    while (!min_heap.empty()) {
        cout << min_heap.top() << " ";
        min_heap.pop();
    }
    cout << '\n'; // 4 5 6
}

/*==========================================================
=            9. DIJKSTRA SKELETON
==========================================================*/

void dijkstraDemo() {

    int n = 5;
    vector<vector<pair<int, int>>> adj(n);
    vector<int> dist(n, 1e9);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    dist[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

/*==========================================================
=            10. COMMON OPERATIONS
==========================================================*/

void operations() {

    priority_queue<int> pq;

    pq.push(10);
    pq.push(5);

    cout << pq.top() << '\n';
    pq.pop();

    cout << pq.size() << '\n';
    cout << pq.empty() << '\n';
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Min Heap of pairs for Dijkstra
// priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

// Custom priority queue with struct cmp
// priority_queue<Type, vector<Type>, CustomCmp> pq;

/*
==========================================================
Remember These
==========================================================

// Max Heap (Default)
priority_queue<int> pq;

// Min Heap
priority_queue<int, vector<int>, greater<int>> pq;

// Max Heap of pairs
priority_queue<pair<int,int>> pq;

// Min Heap of pairs
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

==========================================================
*/

int main() {

    // Uncomment to test

    // maxHeapExample();
    // minHeapExample();
    // pairMaxHeapExample();
    // pairMinHeapExample();
    // customComparatorInt();
    // customComparatorPair();
    // lambdaComparator();
    // topKDemo();
    // dijkstraDemo();
    // operations();

    return 0;
}