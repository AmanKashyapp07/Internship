/**
 * Problem: Course Schedule III (LeetCode 630)
 * 
 * Given n courses with duration and deadline, return the maximum number
 * of courses you can complete without taking more than one at a time.
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        // Sort by deadline (earliest first)
        sort(courses.begin(), courses.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        priority_queue<int> pq; // max-heap of durations taken
        int time = 0;
        int count = 0;

        for (auto& c : courses) {
            int duration = c[0];
            int deadline = c[1];

            if (time + duration <= deadline) {
                // Can take this course
                time += duration;
                pq.push(duration);
                count++;
            } 
            else if (!pq.empty() && pq.top() > duration) {
                // Replace the longest course with this one
                time += duration - pq.top();
                pq.pop();
                pq.push(duration);
            }
        }

        return count;
    }
};