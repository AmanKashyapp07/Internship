// Link: https://cses.fi/problemset/task/1147
#include <bits/stdc++.h>
using namespace std;

long long maxHistogram(const vector<int>& h) {
    int m = h.size();
    stack<int> st;
    long long maxArea = 0;
    for (int i = 0; i <= m; i++) {
        int cur = (i == m ? 0 : h[i]);
        while (!st.empty() && h[st.top()] >= cur) {
            long long height = h[st.top()]; st.pop();
            long long width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    return maxArea;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<int> heights(m, 0);
    long long maxBuilding = 0;

    for (int r = 0; r < n; r++) {
        string s; cin >> s;
        for (int c = 0; c < m; c++) {
            heights[c] = (s[c] == '.') ? heights[c] + 1 : 0;
        }
        maxBuilding = max(maxBuilding, maxHistogram(heights));
    }
    cout << maxBuilding << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the maximum area of a rectangular building plot on an n x m grid (CSES 1147).
// - Approach: Row-by-row histogram height accumulation + Monotonic Stack max rectangle calculation.
// - Intuition: Build height array for '.' characters and compute largest histogram rectangle per row in linear time.
// - Complexity: Time: O(N \cdot M), Space: O(M).