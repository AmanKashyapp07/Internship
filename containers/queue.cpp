/**
 * C++ STL Queue Cheat Sheet for Online Assessments
 *
 * Description:
 *  A container adaptor that provides a FIFO (First-In, First-Out) data structure.
 *  It inserts elements at the back and removes elements from the front.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - push()        : O(1)
 * - pop()         : O(1)
 * - front()       : O(1)
 * - back()        : O(1)
 * - size()        : O(1)
 * - empty()       : O(1)
 * - Space         : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Breadth-First Search (BFS) / Level Order Traversal
 *    - Shortest path in unweighted graphs, level-by-level tree traversal, multi-source BFS.
 * 2. Task / Job Scheduling
 *    - Simulating tasks processed in FIFO order.
 * 3. Sliding Window (sometimes)
 *    - If window size changes dynamically and you only need insertion at the back and removal from the front.
 *    - Note: For sliding window maximum/minimum, use `std::deque` because `std::queue` does not support `pop_back()`.
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - CRITICAL: Always check `q.empty()` before calling `q.front()`, `q.back()`, or `q.pop()`.
 *   Calling these on an empty queue results in undefined behavior.
 * - Performance Tuning (Underlying Container):
 *   - By default, `std::queue` uses `std::deque` as its underlying container.
 *   - You can also use `std::list` if needed: `std::queue<int, std::list<int>> q;`
 *   - WARNING: Unlike `std::stack`, you CANNOT use `std::vector` as the underlying container for `std::queue` because `std::vector` does not support `pop_front()`.
 * - Clear a Queue:
 *   - There is no `.clear()` method. To clear a queue:
 *     `q = queue<int>();` or `while (!q.empty()) q.pop();`
 */

#include <iostream>
#include <queue>
#include <vector>
#include <list>

using namespace std;

// Node structure for BFS graph demo
struct Node {
    int id;
    vector<int> neighbors;
};

// 1. Graph BFS Demo
// Finds shortest path distances from a startNode in an unweighted graph
vector<int> bfsShortestPath(const vector<Node>& graph, int startNode, int numNodes) {
    vector<int> dist(numNodes, -1);
    queue<int> q;

    dist[startNode] = 0;
    q.push(startNode);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int neighbor : graph[curr].neighbors) {
            if (dist[neighbor] == -1) { // Not visited
                dist[neighbor] = dist[curr] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. BASIC QUEUE OPERATIONS
    queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    cout << "Queue size: " << q.size() << "\n";   // Outputs 3
    cout << "Front element: " << q.front() << "\n"; // Outputs 10
    cout << "Back element: " << q.back() << "\n";   // Outputs 30

    q.pop();
    cout << "Front after pop: " << q.front() << "\n"; // Outputs 20

    // 2. LIST-BACKED QUEUE
    queue<int, list<int>> q_list;
    q_list.push(500);
    q_list.push(600);
    cout << "List-backed queue front: " << q_list.front() << "\n"; // Outputs 500

    // 3. BFS GRAPH DEMO
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

    cout << "\nBFS Shortest Path Distances from Node 0:\n";
    for (int i = 0; i < 4; i++) {
        cout << "  Node " << i << ": Distance = " << dists[i] << "\n";
    }

    return 0;
}
