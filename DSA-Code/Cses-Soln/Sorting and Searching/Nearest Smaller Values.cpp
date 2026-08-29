// Link: https://cses.fi/problemset/task/1645
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.top()] >= a[i]) st.pop();
        cout << (st.empty() ? 0 : st.top() + 1) << (i + 1 == n ? '\n' : ' ');
        st.push(i);
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find index of nearest smaller element to left for each element (CSES 1645).
// - Approach: Monotonic Stack ($O(N)$).
// - Intuition: Pop stack elements $\ge a[i]$ as they can never be nearest smaller elements for current or future indices.
// - Complexity: Time: O(N), Space: O(N).