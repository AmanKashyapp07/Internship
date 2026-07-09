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

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

bool canPlace(vector<int>& stalls, int k, int dist) {
    int cows = 1;
    int lastPos = stalls[0];

    for (int i = 1; i < stalls.size(); i++) {
        if (stalls[i] - lastPos >= dist) { // if diff between two stalls is greater than or equal to dist, we can place a cow in the current stall 
            cows++;
            lastPos = stalls[i];

            if (cows >= k)
                return true;
        }
    }

    return false;
} // dist is the max minimum distance we can have between any two cows, we are checking if we can place all k cows in the stalls such that the minimum distance between any two cows is at least dist, if we can place all k cows then we return true else false.

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