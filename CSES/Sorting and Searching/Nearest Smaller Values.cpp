#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.top()] >= a[i]) {
            st.pop();
        }

        if (st.empty()) {
            cout << 0;
        } else {
            cout << st.top() + 1;
        }

        cout << (i + 1 == n ? '\n' : ' ');

        st.push(i);
    }

    return 0;
}