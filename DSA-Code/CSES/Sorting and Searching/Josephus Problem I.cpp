// Link: https://cses.fi/problemset/task/2162
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    queue<int> q;
    for (int i = 1; i <= n; i++) q.push(i);

    while (!q.empty()) {
        q.push(q.front()); q.pop();
        cout << q.front() << ' '; q.pop();
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Output removal order of n children in a circle where every second child is removed (CSES 2162).
// - Approach: Queue Simulation.
// - Intuition: Rotate queue once (skip child), then pop and print the next child until queue is empty.
// - Complexity: Time: O(N), Space: O(N).