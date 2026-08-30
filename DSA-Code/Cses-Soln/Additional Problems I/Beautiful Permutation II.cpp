// Link: https://cses.fi/problemset/task/3175
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    if (n == 2 || n == 3) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    // Output all even numbers followed by odd numbers
    for (int i = 2; i <= n; i += 2) cout << i << " ";
    for (int i = 1; i <= n; i += 2) cout << i << (i + 2 > n ? "" : " ");
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Construct a permutation of 1..N where no two adjacent elements differ by 1 (CSES 3175).
// - Approach: Even-Odd Interleaving Construction.
// - Intuition: Printing all even numbers [2, 4, 6, ...] followed by all odd numbers [1, 3, 5, ...] guarantees adjacent differences >= 2 except at the boundary where |N - 1| >= 2 for N >= 4.
// - Complexity: Time: O(N), Space: O(1).
