// Link: https://cses.fi/problemset/task/3220

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    vector<long long> window(k);

    long long windowSum = 0;
    long long answer = 0;

    // Generate first k elements
    long long current = x;

    for (int i = 0; i < k; i++) {
        window[i] = current; // Store the current value in the circular buffer
        windowSum += current; // Add to the current window sum

        current = (a * current + b) % c; // Generate the next value in the sequence
    }

    answer ^= windowSum; // Initialize the answer with the sum of the first window

    int pos = 0; // oldest element in circular buffer

    // Process remaining elements
    for (int i = k; i < n; i++) {
        long long incoming = current; // The new incoming element to the window, which is coming from the generated sequence

        windowSum -= window[pos]; // Remove the oldest element from the window sum
        windowSum += incoming; // Add the new incoming element to the window sum

        window[pos] = incoming; // Update the circular buffer with the new incoming element
        pos = (pos + 1) % k;

        answer ^= windowSum;

        current = (a * current + b) % c;
    }

    cout << answer << '\n';

    return 0;
}