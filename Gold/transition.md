# HackerRank Environment Transition & Strategy Guide

This guide highlights the key structural differences between HackerRank and LeetCode/CodeSignal, and outlines essential tips to make you comfortable with the environment during the Goldman Sachs OA.

---

## 1. Do You Write the Full Code or Just a Function?

HackerRank supports two modes. You must identify which one is active as soon as you open the problem:

### Mode A: Function-Stub Mode (Most Common)
*   **What it looks like:** You see a class or a single function template (e.g., `int getMinMoves(vector<int> arr)`).
*   **Your task:** Only write your logic *inside* this function.
*   **Under the hood:** The platform has a hidden or locked `main()` function at the bottom that reads inputs from the system, parses them into variables, calls your function, and prints the result.
*   **Rule:** **Do not change the function signature or name.** Modifying parameter types will cause compilation errors in the hidden driver code.

### Mode B: Full-Program Mode (Competitive Programming style)
*   **What it looks like:** The editor is completely blank.
*   **Your task:** You must write the entire program from scratch.
*   **Requirements:**
    *   Include standard headers (`#include <iostream>`, `#include <vector>`, etc.).
    *   Write the `int main()` function.
    *   Manually read inputs using `std::cin` and print outputs using `std::cout`.
    *   Format outputs *exactly* as requested (pay attention to spaces, newlines, and capitalization).

---

## 2. Key Differences: HackerRank vs. LeetCode

| Feature | LeetCode | HackerRank |
| :--- | :--- | :--- |
| **Input Format** | JSON arrays/objects: `[1, 2, 3]` | Raw text lines: `3 \n 1 2 3` |
| **I/O Errors** | Rare | Common (if parsing fails) |
| **Compilation Errors** | High-level summaries | Raw command-line compiler logs |
| **Headers/Imports** | Pre-imported | Must import yourself if using blank editor |
| **Fast I/O** | Automatic | Must write manually for $10^5$ constraints |

---

## 3. General Tips for Mastering the HackerRank UI

### 1. Use `std::cerr` for Debugging (The Ultimate Hack)
*   If you print debug statements using `std::cout` in a full-program challenge, the automated matcher will fail your test cases because your output doesn't match the expected output.
*   **Solution:** Write debug prints using `std::cerr` (Standard Error). HackerRank captures `std::cerr` and shows it under the "Debug Output" tab, but the test runner **ignores** it when verifying correctness.
    *   `std::cerr << "Value of x: " << x << "\n";`

### 2. The "Run Code" Button is Free
*   There is **no penalty** for clicking "Run Code" to compile and run your solution against the sample test cases. Use it frequently to test incremental progress.
*   Only "Submit Code" counts as your final solution attempt.

### 3. How to Structure Custom Input
If you want to test your code with a custom test case, you must enter it in raw text format matching the problem's input specification:
*   *For a 1D vector of size N:*
    ```text
    5
    1 2 3 4 5
    ```
*   *For a 2D grid of size R x C:*
    ```text
    3 3
    1 2 3
    4 5 6
    7 8 9
    ```
*   If you enter custom test cases with wrong dimensions, you will trigger runtime segfaults or `OutOfRange` index errors.

### 4. Fast I/O is Critical
If a problem's constraints show $N \ge 10^5$ and you are in **Full-Program Mode**, place this template at the beginning of your `main()` function to prevent Time Limit Exceeded (TLE) errors:
```cpp
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Your code here
    return 0;
}
```

### 5. Always Use `\n` Instead of `std::endl`
`std::endl` forces a physical disk buffer flush on every single call. In challenges with large outputs (e.g., printing $10^5$ lines), `std::endl` can make your solution up to 10x slower, causing a false TLE.

---

## 4. Blank-Editor Boilerplate Template

If you open a problem and the editor is completely blank, copy and paste this standard skeleton:

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <climits>

using namespace std;

void solve() {
    // Read your inputs here
    // Implement your logic
}

int main() {
    // Optimizes input/output operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    // cin >> t; // Uncomment if there are multiple test cases
    while (t--) {
        solve();
    }
    
    return 0;
}
```
