// Link: https://cses.fi/problemset/task/3108
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct State {
    ll sum;
    int idx;
    bool operator>(const State& other) const { return sum > other.sum; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<ll> a(n);
    ll base = 0;
    for (ll &x : a) {
        cin >> x;
        if (x < 0) base += x;
    }

    for (ll &x : a) x = abs(x);
    sort(a.begin(), a.end());

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0, 0});

    vector<ll> ans;
    ans.reserve(k);

    while (!pq.empty() && (int)ans.size() < k) {
        auto [sum, idx] = pq.top();
        pq.pop();

        ans.push_back(base + sum);
        if (idx == n) continue;

        pq.push({sum + a[idx], idx + 1});
        if (idx > 0) pq.push({sum - a[idx - 1] + a[idx], idx + 1});
    }

    for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find K smallest subset sums of an array in non-decreasing order (CSES 3108).
// - Approach: Positive Conversion + Priority Queue Branching.
// - Intuition: Shifting negative numbers into the baseline sum converts problem to finding K smallest subsets of positive values; expanding states (take a[idx], replace a[idx-1]) generates each subset in sorted order without duplicates.
// - Complexity: Time: O(N log N + K log K), Space: O(N + K).
