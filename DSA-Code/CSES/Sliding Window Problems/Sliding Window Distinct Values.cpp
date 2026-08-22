// Link: https://cses.fi/problemset/task/3222

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    map<int, int> freq;
    int distinct = 0;

    // Build the first window
    for (int i = 0; i < k; i++) {
        if (++freq[a[i]] == 1) {
            distinct++;
        }
    }

    cout << distinct;

    // Slide the window
    for (int right = k; right < n; right++) {
        int outgoing = a[right - k];
        int incoming = a[right];

        // Remove outgoing element
        if (--freq[outgoing] == 0) {
            distinct--;
        }

        // Add incoming element
        if (++freq[incoming] == 1) {
            distinct++;
        }

        cout << ' ' << distinct;
    }

    cout << '\n';
    return 0;
}