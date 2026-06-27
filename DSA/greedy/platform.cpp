#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arrival(n), departure(n);

    for (int i = 0; i < n; i++) cin >> arrival[i];
    for (int i = 0; i < n; i++) cin >> departure[i];

    vector<pair<int, int>> events;

    for (int i = 0; i < n; i++) {
        events.push_back({arrival[i], +1});
        events.push_back({departure[i], -1});
    }

    sort(events.begin(), events.end(), [](auto &a, auto &b) {
        if (a.first == b.first)
            return a.second > b.second; // arrival before departure
        return a.first < b.first;
    });

    int cur = 0, ans = 0;

    for (auto &[time, delta] : events) {
        cur += delta;
        ans = max(ans, cur);
    }

    cout << ans << '\n';

    return 0;
}