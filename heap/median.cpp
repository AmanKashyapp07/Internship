/**
 * CSES Problem Set
 * 
 * Problem: Running Median / Find Median from Data Stream
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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

class MedianFinder {
private:

    // left half (max heap)
    priority_queue<int> left;

    // right half (min heap)
    priority_queue<int, vector<int>, greater<int>> right;

public:

    MedianFinder() {}

    void addNum(int num) {

        left.push(num); // Add to max heap first

        right.push(left.top()); // Move the largest from left to right, beacuse we want to maintain the property that all elements in left are <= all elements in right
        left.pop(); // Remove the largest from left

        if (right.size() > left.size()) { // If right has more elements, move the smallest from right to left to maintain size property
            left.push(right.top()); // Move the smallest from right to left
            right.pop(); // Remove the smallest from right
        }
    }

    double findMedian() {

        if (left.size() > right.size()) {
            return left.top();
        }

        return (left.top() + right.top()) / 2.0;
    }
};

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    MedianFinder mf;

    mf.addNum(1);
    mf.addNum(2);

    cout << mf.findMedian() << '\n'; // 1.5

    mf.addNum(3);

    cout << mf.findMedian() << '\n'; // 2

    return 0;
}