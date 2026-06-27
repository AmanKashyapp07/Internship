/**
 * ==========================================================
 *                 C++ Deque Cheat Sheet (OAs)
 * ==========================================================
 *
 * deque
 * -----
 * • Double-ended queue
 * • Dynamic sizing
 * • Non-contiguous block-based memory
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * deque
 * -----
 * push_back   O(1) amortized
 * push_front  O(1) amortized
 * pop_back    O(1)
 * pop_front   O(1)
 * operator[]  O(1)
 * insert      O(n)
 * erase       O(n)
 *
 * ==========================================================
 */

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

/*==========================================================
=            1. BASIC OPERATIONS
==========================================================*/

void basicOperations() {

    deque<int> dq;

    dq.push_back(10);
    dq.push_front(20);
    dq.push_back(5);

    // Order: 20 10 5

    cout << dq.front() << '\n';  // 20
    cout << dq.back() << '\n';   // 5
    cout << dq[1] << '\n';       // 10

    dq.pop_front();
    dq.pop_back();

    cout << dq.size() << '\n';   // 1
}

/*==========================================================
=            2. ITERATION & SEARCHING
==========================================================*/

void iterationAndSearching() {

    deque<int> dq = {10, 20, 30, 40};

    // Forward
    for (int x : dq)
        cout << x << " ";
    cout << '\n';

    // Reverse
    for (auto it = dq.rbegin(); it != dq.rend(); it++)
        cout << *it << " ";
    cout << '\n';

    // Search
    auto it = find(dq.begin(), dq.end(), 30);
    if (it != dq.end())
        cout << "Found at index " << distance(dq.begin(), it) << '\n';
}

/*==========================================================
=            3. MONOTONIC QUEUE
==========================================================*/

struct MonotonicQueue {
    deque<int> dq;

    void push(int val) {
        while (!dq.empty() && dq.back() < val) {
            dq.pop_back();
        }
        dq.push_back(val);
    }

    void pop(int val) {
        if (!dq.empty() && dq.front() == val) {
            dq.pop_front();
        }
    }

    int get_max() {
        return dq.front();
    }
};

void monotonicQueueDemo() {

    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    MonotonicQueue mq;

    for (int i = 0; i < k; ++i)
        mq.push(arr[i]);

    cout << mq.get_max() << " ";

    for (size_t i = k; i < arr.size(); ++i) {
        mq.pop(arr[i - k]);
        mq.push(arr[i]);
        cout << mq.get_max() << " ";
    }
    cout << '\n';
}

/*==========================================================
=            4. 0-1 BFS
==========================================================*/

void zeroOneBFS() {

    int n = 5;
    vector<vector<pair<int, int>>> adj(n); // {neighbor, weight 0 or 1}
    vector<int> dist(n, 1e9);
    deque<int> dq;

    dist[0] = 0;
    dq.push_back(0);

    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) {
                    dq.push_front(v);
                } else {
                    dq.push_back(v);
                }
            }
        }
    }
}

/*==========================================================
=            5. COMMON OPERATIONS
==========================================================*/

void operations() {

    deque<int> dq;

    dq.push_back(10);
    dq.push_front(20);

    dq.pop_back();
    dq.pop_front();

    cout << dq.size() << '\n';
    cout << dq.empty() << '\n';
    dq.clear();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Monotonic Deque Sliding Window Max
// deque<int> dq;
// while(!dq.empty() && arr[dq.back()] < val) dq.pop_back();
// dq.push_back(val);

// 0-1 BFS Priority Push
// if(w == 0) dq.push_front(v);
// else dq.push_back(v);

/*
==========================================================
Remember These
==========================================================

// Initialization
deque<int> dq;

// Front & Back operations
dq.push_front(val);
dq.push_back(val);
dq.pop_front();
dq.pop_back();

// Access
dq.front();
dq.back();
dq[index];

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // iterationAndSearching();
    // monotonicQueueDemo();
    // zeroOneBFS();
    // operations();

    return 0;
}
