#include <iostream>
#include <deque>

using namespace std;

struct Node {
    int idx;
    long long val;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    deque<Node> dq;
    long long answer = 0;

    long long current = x;

    for (int i = 0; i < n; i++) {

        long long value = current;

        // Remove larger elements from the back
        while (!dq.empty() && dq.back().val >= value) {
            dq.pop_back();
        }

        dq.push_back({i, value});

        // Remove elements outside the window
        while (!dq.empty() && dq.front().idx <= i - k) {
            dq.pop_front();
        } // we can do this step before the previous step because we are only interested in the minimum value in the current window, and if the front element is outside the window, it cannot be the minimum.

        // First complete window
        if (i >= k - 1) {
            answer ^= dq.front().val;
        }

        current = (a * current + b) % c;
    }

    cout << answer << '\n';
    return 0;
}