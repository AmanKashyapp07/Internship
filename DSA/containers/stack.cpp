/**
 * C++ STL Stack Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  A container adaptor that provides LIFO (Last-In, First-Out) access.
 *  It restricts insertion and deletion to only one end (the top).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ push()           │ O(1)             │ Inserts element at the top               │
 * │ pop()            │ O(1)             │ Removes element from the top             │
 * │ top()            │ O(1)             │ Accesses the top element                 │
 * │ size()           │ O(1)             │ Returns number of elements               │
 * │ empty()          │ O(1)             │ Checks if the container is empty         │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: O(N)
 *
 * Typical Interview Usage:
 *  - Monotonic Stack (to find next greater/smaller element in O(N))
 *  - Nested structures (valid parentheses, parsing expressions, Shunting-Yard)
 *  - Iterative DFS (simulating the recursion stack to avoid depth limits)
 *
 * Interview Tricks & Pitfalls:
 *  - Undefined Behavior: Always check `!s.empty()` before calling `s.top()` or `s.pop()`.
 *    Calling these on an empty stack is a common cause of segmentation faults.
 *  - Performance Tuning: By default, `std::stack` is backed by `std::deque`. You can back it with
 *    a `std::vector` for contiguous memory and better cache performance:
 *    `std::stack<int, std::vector<int>> s;`
 *  - No Clear Method: `std::stack` does not have a `.clear()` member function. To clear it:
 *    `s = std::stack<int>();` or `while (!s.empty()) s.pop();`
 */

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

/*==========================================================
=            1. CONSTRUCTION & BASIC OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";

    // 1. Default stack (backed by deque)
    stack<int> s;

    // 2. Custom container stack (backed by vector)
    stack<int, vector<int>> s_vec;

    s.push(10);
    s.push(20);
    s.emplace(30); // Construct in-place

    cout << "Stack size: " << s.size() << "\n"; // 3
    cout << "Top element: " << s.top() << "\n";  // 30

    s.pop(); // Removes 30
    cout << "Top after pop: " << s.top() << "\n"; // 20

    // Clear stack
    s = stack<int>(); // Quick re-initialization
    cout << "Is empty after clearing: " << (s.empty() ? "Yes" : "No") << "\n\n";
}

/*==========================================================
=            2. MONOTONIC STACK (NEXT GREATER ELEMENT)
==========================================================*/

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

void monotonicStackDemo() {
    cout << "--- 2. MONOTONIC STACK DEMO ---\n";
    vector<int> nums = {4, 5, 2, 25, 7, 8};
    vector<int> nge = nextGreaterElement(nums);

    cout << "Next Greater Elements:\n";
    for (size_t i = 0; i < nums.size(); i++) {
        cout << nums[i] << " -> " << nge[i] << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            3. ITERATIVE DFS
==========================================================*/

struct Node {
    int id;
    vector<int> neighbors;
};

// Simulates recursive DFS using an explicit stack
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

            // Push neighbors to stack in reverse to visit in order
            for (auto it = graph[curr].neighbors.rbegin(); it != graph[curr].neighbors.rend(); ++it) {
                if (!visited[*it]) {
                    s.push(*it);
                }
            }
        }
    }
    cout << "\n\n";
}

void iterativeDFSDemo() {
    cout << "--- 3. ITERATIVE DFS DEMO ---\n";
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
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ s.push(x)        │ Push element to top        │ O(1)                        │
 * │ s.emplace(args)  │ Construct element on top   │ O(1)                        │
 * │ s.pop()          │ Remove top element         │ O(1)                        │
 * │ s.top()          │ Access top element         │ O(1)                        │
 * │ s.size()         │ Get number of elements     │ O(1)                        │
 * │ s.empty()        │ Check if stack is empty    │ O(1)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Monotonic Stack (Next Greater Element Template)
// stack<int> s; // stores indices
// for(int i=0; i<n; ++i) {
//     while(!s.empty() && arr[s.top()] < arr[i]) {
//         nge[s.top()] = arr[i];
//         s.pop();
//     }
//     s.push(i);
// }

// 2. Performance Optimized Stack
// stack<int, vector<int>> s;

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization
stack<int> s;

// Basic Operations
s.push(val);
s.pop();     // void return, does not return the top value!
s.top();     // Returns reference to top element

// Safe access pattern
if (!s.empty()) {
    int val = s.top();
    s.pop();
}

// Clearing stack
s = stack<int>();
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // monotonicStackDemo();
    // iterativeDFSDemo();

    return 0;
}
