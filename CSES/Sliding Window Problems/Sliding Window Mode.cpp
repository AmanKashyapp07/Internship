// Link: https://cses.fi/problemset/task/3224

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    unordered_map<int, int> freq;
    set<pair<int, int>> st;   // (-frequency, value)

    auto add = [&](int x) {
        if (freq[x] > 0)
            st.erase({-freq[x], x});

        freq[x]++;
        st.insert({-freq[x], x});
    };

    auto remove = [&](int x) {
        st.erase({-freq[x], x});
        freq[x]--;

        if (freq[x] > 0)
            st.insert({-freq[x], x});
        else
            freq.erase(x);
    };

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        add(a[i]);

        if (i >= k)
            remove(a[i - k]);

        if (i >= k - 1)
            cout << st.begin()->second << " ";
    }

    return 0;
}