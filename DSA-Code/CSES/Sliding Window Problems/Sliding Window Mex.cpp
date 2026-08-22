// Link: https://cses.fi/problemset/task/3219

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    for (int &x : a)
        cin >> x;

    vector<int> freq(k + 1, 0);
    set<int> missing;

    for (int i = 0; i <= k; i++)
        missing.insert(i);

    // First window
    for (int i = 0; i < k; i++) {
        if (a[i] <= k) {
            if (++freq[a[i]] == 1)
                missing.erase(a[i]);
        }
    }

    cout << *missing.begin();

    // Remaining windows
    for (int i = k; i < n; i++) {
        int out = a[i - k];
        if (out <= k) {
            if (--freq[out] == 0)
                missing.insert(out);
        }

        int in = a[i];
        if (in <= k) {
            if (++freq[in] == 1)
                missing.erase(in);
        }

        cout << " " << *missing.begin();
    }

    cout << "\n";
}