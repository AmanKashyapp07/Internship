#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    map<int, int> freq;
    long long ans = 0;

    int left = 0;

    for (int right = 0; right < n; right++) {
        freq[a[right]]++;

        while (freq[a[right]] > 1) {
            freq[a[left]]--;
            left++;
        }

        ans += (right - left + 1);
    }

    cout << ans << '\n';
    return 0;
}