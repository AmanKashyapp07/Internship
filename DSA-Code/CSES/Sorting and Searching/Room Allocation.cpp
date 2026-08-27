// Link: https://cses.fi/problemset/task/1164
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<array<int, 3>> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i][0] >> a[i][1]; a[i][2] = i; }
    sort(a.begin(), a.end());

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> ans(n);
    int rooms = 0;

    for (auto &[l, r, i] : a) {
        int room;
        if (!pq.empty() && pq.top().first < l) {
            room = pq.top().second; pq.pop();
        } else {
            rooms++; room = rooms;
        }
        ans[i] = room;
        pq.push({r, room});
    }

    cout << rooms << '\n';
    for (int x : ans) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of rooms needed for n customer stays and assign room numbers (CSES 1164).
// - Approach: Sorting Interval Start Times + Min-Heap Priority Queue of Free Times `{departure_time, room_id}`.
// - Intuition: Reusing the room with the earliest departure time minimizes total required rooms.
// - Complexity: Time: O(N \log N), Space: O(N).