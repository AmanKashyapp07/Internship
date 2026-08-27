// Link: https://cses.fi/problemset/task/1629
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pair<int, int>> movies(n);
    for (int i = 0; i < n; i++) cin >> movies[i].second >> movies[i].first; // {end, start}
    sort(movies.begin(), movies.end());

    int count = 0, lastEnd = 0;
    for (auto &[end, start] : movies) {
        if (start >= lastEnd) {
            count++;
            lastEnd = end;
        }
    }
    cout << count << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum number of movies one person can watch (CSES 1629).
// - Approach: Classic Greedy Interval Scheduling by End Time.
// - Intuition: Always picking the movie that finishes earliest leaves maximum remaining time for subsequent movies.
// - Complexity: Time: O(N \log N), Space: O(N).
