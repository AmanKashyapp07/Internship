/**
 * C++ STL Stack Cheat Sheet for Online Assessments
 *
 * Description:
 *  A container adaptor that provides a LIFO (Last-In, First-Out) data structure.
 *  It restricts insertion and deletion to only one end (the top).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - push()        : O(1)
 * - pop()         : O(1)
 * - top()         : O(1)
 * - size()        : O(1)
 * - empty()       : O(1)
 * - Space         : O(N)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Monotonic Stack Pattern
 *    - Used to find the next greater / next smaller element in O(N) time.
 *    - Keeps elements in a sorted order (monotonically increasing or decreasing).
 * 2. Nested/Matching Structures
 *    - Valid Parentheses, XML tag matching, calculator parsing (shunting-yard algorithm).
 * 3. Iterative DFS / Backtracking
 *    - Simulating recursive call stack explicitly to avoid stack overflow (though recursion is usually fine, iterative DFS can be useful).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - CRITICAL: Always check `s.empty()` before calling `s.top()` or `s.pop()`.
 *   Calling `top()` or `pop()` on an empty stack results in undefined behavior.
 * - Performance Tuning (Underlying Container):
 *   - By default, `std::stack` uses `std::deque` as its underlying container.
 *   - If you want contiguous memory and slightly better performance, you can use `std::vector` or `std::list`:
 *     `std::stack<int, std::vector<int>> s;`
 *   - Note that `std::vector` does not support `pop_front` (which stack doesn't need anyway).
 * - Clear a Stack:
 *   - There is no `.clear()` method. To clear a stack:
 *     `s = stack<int>();` or `while (!s.empty()) s.pop();`
 */

#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

// Node structure for iterative DFS demo
struct Node {
    int id;
    vector<int> neighbors;
};

// 1. Monotonic Stack Example: Next Greater Element (NGE)
// Problem: Given an array, find the next greater element for each element.
// If none exists, output -1.
vector<int> nextGreaterElement(const vector<int>& nums) {
    int n = nums.size();
    vector<int> nge(n, -1);
    stack<int> s; // Stores indices of elements

    for (int i = 0; i < n; i++) {
        // While current element is greater than the element at the index on top of stack,
        // we found the next greater element for that index.
        while (!s.empty() && nums[i] > nums[s.top()]) {
            nge[s.top()] = nums[i];
            s.pop();
        }
        s.push(i);
    }
    return nge;
}

// 2. Iterative DFS Demo using Stack
void iterativeDFS(const vector<Node>& graph, int startNode, int numNodes) {
    vector<bool> visited(numNodes, false);
    stack<int> s;

    s.push(startNode);
    cout << "Iterative DFS Traversal path: ";

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        if (!visited[curr]) {
            visited[curr] = true;
            cout << curr << " ";

            // Push neighbors to stack (pushing in reverse to visit in order)
            for (auto it = graph[curr].neighbors.rbegin(); it != graph[curr].neighbors.rend(); ++it) {
                if (!visited[*it]) {
                    s.push(*it);
                }
            }
        }
    }
    cout << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. BASIC STACK OPERATIONS
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    cout << "Stack size: " << s.size() << "\n"; // Outputs 3
    cout << "Top element: " << s.top() << "\n";  // Outputs 30

    s.pop();
    cout << "Top after pop: " << s.top() << "\n"; // Outputs 20

    // 2. PERFORMANCE OPTIMIZED STACK (VECTOR UNDERLYING)
    stack<int, vector<int>> s_vec;
    s_vec.push(100);
    s_vec.push(200);
    cout << "Vector-backed stack top: " << s_vec.top() << "\n"; // Outputs 200

    // 3. MONOTONIC STACK DEMO
    vector<int> nums = {4, 5, 2, 25, 7, 8};
    vector<int> nge = nextGreaterElement(nums);
    cout << "\nNext Greater Elements:\n";
    for (size_t i = 0; i < nums.size(); i++) {
        cout << "  " << nums[i] << " -> " << nge[i] << "\n";
    }

    // 4. ITERATIVE DFS DEMO
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
    
    iterativeDFS(graph, 0, 4);

    return 0;
}
