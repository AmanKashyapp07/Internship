// Link: https://cses.fi/problemset/task/1085

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool canDivide(const vector<long long>& a, int k, long long maxSum) {
    int parts = 1;
    long long currentSum = 0;

    for (long long x : a) {
        if (currentSum + x <= maxSum) {
            currentSum += x;
        } else {
            parts++;
            currentSum = x;
        }
    }

    return parts <= k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> a(n);

    long long left = 0;
    long long right = 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        left = max(left, a[i]);
        right += a[i];
    }

    long long answer = right;

    while (left <= right) {
        long long mid = left + (right - left) / 2;

        if (canDivide(a, k, mid)) {
            answer = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    cout << answer << '\n';

    return 0;
}