#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int64> h(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }
    h[n] = 0; // Sentinel

    stack<int> st;
    int64 ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && h[st.top()] > h[i]) {
            int64 height = h[st.top()];
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int64 width = i - left - 1;

            ans = max(ans, height * width);
        }
        st.push(i);
    }

    cout << ans << '\n';

    return 0;
}