// Link: https://cses.fi/problemset/task/1142
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> h(n + 1, 0);
    for (int i = 0; i < n; i++) cin >> h[i];

    stack<int> st;
    long long maxArea = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && h[st.top()] >= h[i]) {
            long long height = h[st.top()]; st.pop();
            long long width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    cout << maxArea << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the maximum area of a rectangle in a histogram given n bar heights (CSES 1142).
// - Approach: Monotonic Increasing Stack storing bar indices with a zero height sentinel at index n.
// - Intuition: Popping taller bars when a shorter bar incoming determines the maximal width span [left + 1, i - 1] for popped bar height.
// - Complexity: Time: O(N) single pass, Space: O(N) stack.