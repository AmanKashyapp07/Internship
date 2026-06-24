/**
 * Aggressive Cows
 *
 * Problem:
 * Given positions of stalls and k cows, place the cows in stalls
 * such that the minimum distance between any two cows is maximized.
 *
 * Approach:
 * Binary Search on Answer.
 * For a distance 'mid', greedily check if we can place all k cows
 * while maintaining at least 'mid' distance between consecutive cows.
 *
 * Time Complexity: O(n log(max_position))
 * Space Complexity: O(1)
 */

#include <bits/stdc++.h>
using namespace std;

bool canPlace(vector<int>& stalls, int k, int dist) {
    int cows = 1;
    int lastPos = stalls[0];

    for (int i = 1; i < stalls.size(); i++) {
        if (stalls[i] - lastPos >= dist) {
            cows++;
            lastPos = stalls[i];

            if (cows >= k)
                return true;
        }
    }

    return false;
}

int aggressiveCows(vector<int>& stalls, int k) {
    sort(stalls.begin(), stalls.end());

    int low = 1;
    int high = stalls.back() - stalls.front();
    int ans = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (canPlace(stalls, k, mid)) {
            ans = mid;
            low = mid + 1;      // try larger distance
        } else {
            high = mid - 1;     // reduce distance
        }
    }

    return ans;
}

int main() {
    vector<int> stalls = {1, 2, 4, 8, 9};
    int k = 3;

    cout << aggressiveCows(stalls, k) << "\n";
    return 0;
}