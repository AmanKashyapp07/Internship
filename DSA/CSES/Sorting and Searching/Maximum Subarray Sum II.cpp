// Link: https://cses.fi/problemset/task/1644

#include <bits/stdc++.h>
using namespace std;

// Maximum Subarray Sum with Length in [A, B]

int main() {
    int n, A, B;
    cin >> n >> A >> B;

    vector<long long> arr(n + 1);
    vector<long long> pref(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        pref[i] = pref[i - 1] + arr[i];
    }

    deque<int> dq;
    long long ans = LLONG_MIN;

    for (int i = A; i <= n; i++) {

        // Valid range of j:
        // i - B <= j <= i - A
        int L = i - B;
        int R = i - A;
         // Remove indices outside the left boundary
        while (!dq.empty() && dq.front() < L) {
            dq.pop_front();
        }
        // Insert the new right boundary index
        while (!dq.empty() && pref[dq.back()] >= pref[R]) {
            dq.pop_back();
        }
        dq.push_back(R);


        // Minimum prefix sum in [L, R] is at the front
        ans = max(ans, pref[i] - pref[dq.front()]);
    }

    cout << ans << '\n';
}