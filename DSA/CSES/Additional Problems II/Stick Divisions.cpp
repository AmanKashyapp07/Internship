// Link: https://cses.fi/problemset/task/1161

#include <bits/stdc++.h>
using namespace std;

int main() {
    int x, n;
    cin >> x >> n;

    priority_queue<long long, vector<long long>, greater<long long>> pq;

    while (n--) {
        long long d;
        cin >> d;
        pq.push(d);
    }

    long long ans = 0;

    while (pq.size() > 1) {
        long long a = pq.top(); pq.pop();
        long long b = pq.top(); pq.pop();

        ans += a + b;
        pq.push(a + b);
    }

    cout << ans;
}