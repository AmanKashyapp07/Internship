// Link: https://cses.fi/problemset/task/1632
#include <bits/stdc++.h>
using namespace std;

struct Movie {
    int start, end;
    bool operator<(const Movie& o) const { return end < o.end || (end == o.end && start < o.start); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<Movie> movies(n);
    for (int i = 0; i < n; i++) cin >> movies[i].start >> movies[i].end;
    sort(movies.begin(), movies.end());

    multiset<int> freeTimes;
    for (int i = 0; i < k; i++) freeTimes.insert(0);

    int count = 0;
    for (const auto& m : movies) {
        auto it = freeTimes.upper_bound(m.start);
        if (it != freeTimes.begin()) {
            --it;
            freeTimes.erase(it);
            freeTimes.insert(m.end);
            count++;
        }
    }
    cout << count << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum total movies watched by k members (CSES 1632).
// - Approach: Greedy Interval Scheduling by End Time + Multiset Free Times (`upper_bound(start) - 1`).
// - Intuition: Assign movie to member whose available free time is latest among those $\le \text{movie.start}$ to leave larger gaps for future movies.
// - Complexity: Time: O(N \log N + N \log K), Space: O(N + K).