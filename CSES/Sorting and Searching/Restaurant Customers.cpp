// Link: https://cses.fi/problemset/task/1619

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> events;

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;

        events.push_back({a, 1});   // arrival
        events.push_back({b, -1});  // leaving
    }

    sort(events.begin(), events.end());

    int current = 0;
    int answer = 0;

    for (auto [time, change] : events) {
        current += change;
        answer = max(answer, current);
    }

    cout << answer << '\n';
    return 0;
}