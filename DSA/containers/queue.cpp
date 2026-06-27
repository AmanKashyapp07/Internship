/**
 * C++ STL Queue Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  A container adaptor that provides FIFO (First-In, First-Out) access.
 *  Elements are inserted at the back and removed from the front.
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ push()           │ O(1)             │ Inserts element at the back              │
 * │ pop()            │ O(1)             │ Removes element from the front           │
 * │ front()          │ O(1)             │ Accesses the front element               │
 * │ back()           │ O(1)             │ Accesses the back element                │
 * │ size()           │ O(1)             │ Returns number of elements               │
 * │ empty()          │ O(1)             │ Checks if the container is empty         │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Breadth-First Search (BFS) / Level Order Traversal on trees/graphs
 *  - Shortest path in unweighted graphs
 *  - Simulating FIFO queue-based workflows (e.g. task schedulers, message queues)
 *
 * Interview Tricks & Pitfalls:
 *  - Undefined Behavior: Always check `!q.empty()` before calling `q.front()`, `q.back()`, or `q.pop()`.
 *    Calling these on an empty queue causes undefined behavior (often crash).
 *  - Performance Tuning: By default, `std::queue` is backed by `std::deque`. You can back it with
 *    a `std::list` if list performance characteristics are desired:
 *    `std::queue<int, std::list<int>> q_list;`
 *    WARNING: Unlike `std::stack`, you CANNOT use `std::vector` as the underlying container for `std::queue`
 *    because `std::vector` does not support `pop_front()`.
 *  - No Clear Method: `std::queue` does not have a `.clear()` member function. To clear it:
 *    `q = std::queue<int>();` or `while (!q.empty()) q.pop();`
 */

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";

    // 1. Default queue (backed by deque)
    queue<int> q;

    // 2. Custom container queue (backed by list)
    queue<int, list<int>> q_list;

    q.push(10);
    q.push(20);
    q.emplace(30); // In-place construction

    cout << "Queue size: " << q.size() << "\n";   // 3
    cout << "Front element: " << q.front() << "\n"; // 10
    cout << "Back element: " << q.back() << "\n";   // 30

    q.pop(); // Removes 10
    cout << "Front after pop: " << q.front() << "\n"; // 20

    // Clear queue
    q = queue<int>();
    cout << "Is empty after clear: " << (q.empty() ? "Yes" : "No") << "\n\n";
}

/*==========================================================
=            2. GRAPH BFS (SHORTEST PATH SKELETON)
==========================================================*/

struct Node {
    int id;
    vector<int> neighbors;
};

// Finds shortest path distances from a startNode in an unweighted graph using BFS
vector<int> bfsShortestPath(const vector<Node>& graph, int startNode, int numNodes) {
    vector<int> dist(numNodes, -1);
    queue<int> q;

    dist[startNode] = 0;
    q.push(startNode);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int neighbor : graph[curr].neighbors) {
            if (dist[neighbor] == -1) { // Not visited yet
                dist[neighbor] = dist[curr] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}

void bfsDemo() {
    cout << "--- 2. BFS GRAPH DEMO ---\n";
    // Construct simple graph: 
    // 0 -> 1, 2
    // 1 -> 3
    // 2 -> 3
    // 3 -> (none)
    vector<Node> graph(4);
    graph[0] = {0, {1, 2}};
    graph[1] = {1, {3}};
    graph[2] = {2, {3}};
    graph[3] = {3, {}};

    vector<int> dists = bfsShortestPath(graph, 0, 4);

    cout << "BFS Shortest Path Distances from Node 0:\n";
    for (int i = 0; i < 4; i++) {
        cout << "Node " << i << ": Distance = " << dists[i] << "\n";
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
 * │ q.push(x)        │ Push element to back       │ O(1)                        │
 * │ q.emplace(args)  │ Construct element at back  │ O(1)                        │
 * │ q.pop()          │ Remove front element       │ O(1)                        │
 * │ q.front()        │ Access front element       │ O(1)                        │
 * │ q.back()         │ Access back element        │ O(1)                        │
 * │ q.size()         │ Get number of elements     │ O(1)                        │
 * │ q.empty()        │ Check if queue is empty    │ O(1)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Standard BFS Template
// queue<int> q;
// q.push(start);
// visited[start] = true;
// while(!q.empty()) {
//     int curr = q.front(); q.pop();
//     for(int next : adj[curr]) {
//         if(!visited[next]) {
//             visited[next] = true;
//             q.push(next);
//         }
//     }
// }

// 2. Queue clearing template
// q = queue<int>();

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
queue<int> q;

// Basic operations
q.push(val);
q.pop();     // void return, does not return the front element!
q.front();   // Reference to front element
q.back();    // Reference to back element

// Safe queue pop
if (!q.empty()) {
    int val = q.front();
    q.pop();
}
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // bfsDemo();

    return 0;
}
