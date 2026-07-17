#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> songs(n);
    for (int i = 0; i < n; i++) {
        cin >> songs[i];
    }

    set<int> window;
    int left = 0;
    int longestUniqueSequence = 0;

    for (int right = 0; right < n; right++) {
        while (window.count(songs[right])) {
            window.erase(songs[left]);
            left++;
        }

        window.insert(songs[right]);
        longestUniqueSequence = max(longestUniqueSequence, right - left + 1);
    }

    cout << longestUniqueSequence << '\n';

    return 0;
}