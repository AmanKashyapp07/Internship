// Link: https://cses.fi/problemset/task/2132
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    priority_queue<long long> medianHeap;
    long long ans = 0;

    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        medianHeap.push(x);
        if (medianHeap.top() > x) {
            ans += medianHeap.top() - x;
            medianHeap.pop();
            medianHeap.push(x);
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum total cost (increments/decrements) to make an array non-decreasing (CSES 2132).
// - Approach: Slope Trick via Max-Heap.
// - Intuition: The optimal dynamic programming cost function is piecewise linear; maintaining change-points in a max-heap allows updating cost in $O(\log N)$.
// - Complexity: Time: O(N \log N), Space: O(N).