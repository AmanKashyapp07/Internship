/**
 * ==========================================================
 *                 C++ Queue Cheat Sheet (OAs)
 * ==========================================================
 *
 * queue
 * -----
 * • First-In First-Out (FIFO)
 * • Container adaptor (default deque-backed)
 * • Elements insert at back, pop from front
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * queue
 * -----
 * push        O(1)
 * pop         O(1)
 * front       O(1)
 * back        O(1)
 * size        O(1)
 * empty       O(1)
 *
 * ==========================================================
 */

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

struct Node {
    int id;
    vector<int> neighbors;
};

/*==========================================================
=            1. BASIC OPERATIONS
==========================================================*/

void basicOperations() {

    queue<int> q;

    q.push(10);
    q.push(20);
    q.push(30);

    cout << q.front() << '\n';   // 10
    cout << q.back() << '\n';    // 30

    q.pop();
    cout << q.front() << '\n';   // 20
}

/*==========================================================
=            2. LIST BACKED QUEUE
==========================================================*/

void listBackedQueue() {

    queue<int, list<int>> q_list;

    q_list.push(500);
    q_list.push(600);

    cout << q_list.front() << '\n'; // 500
}

/*==========================================================
=            3. BFS GRAPH DEMO
==========================================================*/

vector<int> bfsShortestPath(const vector<Node>& graph, int startNode, int numNodes) {
    vector<int> dist(numNodes, -1);
    queue<int> q;

    dist[startNode] = 0;
    q.push(startNode);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int neighbor : graph[curr].neighbors) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[curr] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}

void bfsDemo() {

    vector<Node> graph(4);
    graph[0] = {0, {1, 2}};
    graph[1] = {1, {3}};
    graph[2] = {2, {3}};
    graph[3] = {3, {}};

    vector<int> dists = bfsShortestPath(graph, 0, 4);

    for (int i = 0; i < 4; i++) {
        cout << "Node " << i << ": " << dists[i] << '\n';
    }
}

/*==========================================================
=            4. COMMON OPERATIONS
==========================================================*/

void operations() {

    queue<int> q;

    q.push(10);
    q.pop();

    cout << q.size() << '\n';
    cout << q.empty() << '\n';

    // Clear
    q = queue<int>();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Standard BFS Graph traversal
// queue<int> q;
// q.push(start);
// while(!q.empty()) {
//     int curr = q.front(); q.pop();
//     for(int next : graph[curr]) {
//         if(!visited[next]) {
//             visited[next] = true;
//             q.push(next);
//         }
//     }
// }

// Queue Re-initialization
// q = queue<int>();

/*
==========================================================
Remember These
==========================================================

// Initialization
queue<int> q;

// Backed by list
queue<int, list<int>> q;

// Operations
q.push(val);
q.pop();
q.front();
q.back();

==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // listBackedQueue();
    // bfsDemo();
    // operations();

    return 0;
}
