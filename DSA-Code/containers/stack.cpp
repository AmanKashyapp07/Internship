/**
 * ==========================================================
 *                 C++ Stack Cheat Sheet (OAs)
 * ==========================================================
 *
 * stack
 * -----
 * • Last-In First-Out (LIFO)
 * • Container adaptor (default deque-backed)
 * • Restricts access to top only
 *
 * ==========================================================
 *
 * Time Complexities
 * -----------------
 *
 * stack
 * -----
 * push        O(1)
 * pop         O(1)
 * top         O(1)
 * size        O(1)
 * empty       O(1)
 *
 * ==========================================================
 */

#include <iostream>
#include <stack>
#include <vector>
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

    stack<int> s;

    // Vector backed stack for slightly better performance
    stack<int, vector<int>> s_vec;

    s.push(10);
    s.push(20);
    s.emplace(30);

    cout << s.top() << '\n';     // 30
    s.pop();
    cout << s.top() << '\n';     // 20
}

/*==========================================================
=            2. MONOTONIC STACK (NGE)
==========================================================*/

void monotonicStackExample() {

    vector<int> nums = {4, 5, 2, 25, 7, 8};
    int n = nums.size();
    vector<int> nge(n, -1);
    stack<int> s;

    for (int i = 0; i < n; i++) {
        while (!s.empty() && nums[i] > nums[s.top()]) {
            nge[s.top()] = nums[i];
            s.pop();
        }
        s.push(i);
    }

    for (int i = 0; i < n; i++) {
        cout << nums[i] << " -> " << nge[i] << '\n';
    }
}


/*==========================================================
=            4. COMMON OPERATIONS
==========================================================*/

void operations() {

    stack<int> s;

    s.push(10);
    s.pop();

    cout << s.size() << '\n';
    cout << s.empty() << '\n';
    
    // Clear
    s = stack<int>();
}

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// Monotonic Stack (Next Greater Element)
// stack<int> s;
// for(int i=0; i<n; i++) {
//     while(!s.empty() && arr[s.top()] < arr[i]) {
//         nge[s.top()] = arr[i];
//         s.pop();
//     }
//     s.push(i);
// }

// Vector Backed Stack
// stack<int, vector<int>> s;

/*
==========================================================
Remember These
==========================================================

// Initialization
stack<int> s;

// Operations
s.push(val);
s.pop();
s.top();

// Safe top and pop
if (!s.empty()) {
    int val = s.top();
    s.pop();
}
// if we pop on empty stack it will throw an exception, so always check if the stack is empty before popping or accessing the top element.
==========================================================
*/

int main() {

    // Uncomment to test

    // basicOperations();
    // monotonicStackExample();
    // iterativeDFSDemo();
    // operations();

    return 0;
}
