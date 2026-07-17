#include <bits/stdc++.h>
using namespace std;

struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        // Higher frequency comes first
        if (a.second != b.second)
            return a.second > b.second;

        // If frequency is same, smaller value comes first
        return a.first < b.first;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    unordered_map<int, int> freq;
    set<pair<int, int>, Compare> st;   // (value, frequency)

    auto add = [&](int x) {
        if (freq[x] > 0)
            st.erase({x, freq[x]});

        freq[x]++;
        st.insert({x, freq[x]});
    };

    auto remove = [&](int x) {
        st.erase({x, freq[x]});

        freq[x]--;

        if (freq[x] > 0)
            st.insert({x, freq[x]});
        else
            freq.erase(x);
    };

    for (int i = 0; i < n; i++) {
        cin >> a[i];

        add(a[i]);

        if (i >= k)
            remove(a[i - k]);

        if (i >= k - 1)
            cout << st.begin()->first << " ";
    }

    return 0;
}