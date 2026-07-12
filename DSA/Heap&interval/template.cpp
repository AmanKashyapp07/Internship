/**
 * Problem: Priority Queue / Heap Master Template
 * Category: Heap Data Structure
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

// Standard Heap Types
using MaxHeap     = priority_queue<int>;
using MinHeap     = priority_queue<int, vector<int>, greater<int>>;
using MaxPairHeap = priority_queue<pii>;
using MinPairHeap = priority_queue<pii, vector<pii>, greater<pii>>;

// =========================================================================
// 1. K-TH VARIANTS
// =========================================================================
int kthLargest(const vector<int>& nums, int k) {
    MinHeap pq;
    for (int x : nums) {
        pq.push(x);
        if (pq.size() > k) pq.pop();
    }
    return pq.top();
}

int kthSmallest(const vector<int>& nums, int k) {
    MaxHeap pq;
    for (int x : nums) {
        pq.push(x);
        if (pq.size() > k) pq.pop();
    }
    return pq.top();
}

// =========================================================================
// 2. TOP K FREQUENT
// =========================================================================
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    MinPairHeap pq;
    for (auto& [num, f] : freq) {
        pq.push({f, num});
        if (pq.size() > k) pq.pop();
    }

    vector<int> res;
    while (!pq.empty()) {
        res.push_back(pq.top().second);
        pq.pop();
    }
    return res;
}

// =========================================================================
// 3. K CLOSEST POINTS
// =========================================================================
vector<vector<int>> kClosestPoints(const vector<vector<int>>& points, int k) {
    priority_queue<pair<int, int>> pq; // {dist, index}
    for (int i = 0; i < (int)points.size(); ++i) {
        int d = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        pq.push({d, i});
        if (pq.size() > k) pq.pop();
    }

    vector<vector<int>> res;
    while (!pq.empty()) {
        res.push_back(points[pq.top().second]);
        pq.pop();
    }
    return res;
}

// =========================================================================
// 4. STREAM & MEDIAN
// =========================================================================
class MedianFinder {
private:
    MaxHeap left;
    MinHeap right;
public:
    void addNum(int num) {
        left.push(num);
        right.push(left.top());
        left.pop();
        if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }
    }

    double findMedian() {
        if (left.size() > right.size()) return left.top();
        return (left.top() + right.top()) / 2.0;
    }
};

class KthLargestStream {
private:
    MinHeap pq;
    int k;
public:
    KthLargestStream(int k, const vector<int>& nums) : k(k) {
        for (int x : nums) add(x);
    }
    
    int add(int val) {
        pq.push(val);
        if ((int)pq.size() > k) pq.pop();
        return pq.top();
    }
};

// =========================================================================
// 5. K-WAY MERGE
// =========================================================================
vector<int> mergeKsortedArrays(const vector<vector<int>>& arrays) {
    using T = tuple<int, int, int>; // value, arrIdx, elemIdx
    priority_queue<T, vector<T>, greater<T>> pq;

    for (int i = 0; i < (int)arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            pq.push({arrays[i][0], i, 0});
        }
    }

    vector<int> res;
    while (!pq.empty()) {
        auto [val, arrIdx, elemIdx] = pq.top(); pq.pop();
        res.push_back(val);
        if (elemIdx + 1 < (int)arrays[arrIdx].size()) {
            pq.push({arrays[arrIdx][elemIdx + 1], arrIdx, elemIdx + 1});
        }
    }
    return res;
}

// =========================================================================
// 6. K SMALLEST PAIRS
// =========================================================================
vector<pair<int, int>> kSmallestPairs(const vector<int>& nums1, const vector<int>& nums2, int k) {
    vector<pair<int, int>> result;
    if (nums1.empty() || nums2.empty() || k <= 0) return result;

    using T = pair<int, pair<int, int>>;
    priority_queue<T, vector<T>, greater<T>> pq;

    for (int i = 0; i < min((int)nums1.size(), k); ++i) {
        pq.push({nums1[i] + nums2[0], {i, 0}});
    }

    while (k-- && !pq.empty()) {
        auto [sum, idx] = pq.top(); pq.pop();
        auto [i, j] = idx;
        result.emplace_back(nums1[i], nums2[j]);
        if (j + 1 < (int)nums2.size()) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
        }
    }
    return result;
} 

// =========================================================================
// 7. MAX K SUM COMBINATIONS
// =========================================================================
vector<int> maxKsumCombination(vector<int>& nums1, vector<int>& nums2, int k) {
    sort(nums1.rbegin(), nums1.rend());
    sort(nums2.rbegin(), nums2.rend());

    using T = pair<int, pair<int, int>>;
    priority_queue<T> pq;
    set<pair<int, int>> visited;

    pq.push({nums1[0] + nums2[0], {0, 0}});
    visited.insert({0, 0});
    int n1= nums1.size(), n2=nums2.size();
    vector<int> result;
    while (k-- && !pq.empty()) {
        auto [sum, pos] = pq.top(); pq.pop();
        auto [i, j] = pos;
        result.push_back(sum);

        if (i + 1 < n1 && !visited.count({i + 1, j})) {
            pq.push({nums1[i + 1] + nums2[j], {i + 1, j}});
            visited.insert({i + 1, j});
        }
        if (j + 1 < n2 && !visited.count({i, j + 1})) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
            visited.insert({i, j + 1});
        }
    }
    return result;
}

// =========================================================================
// 8. GREEDY HEAP ALGORITHMS
// =========================================================================
int minCostToConnectSticks(const vector<int>& sticks) {
    MinHeap pq(sticks.begin(), sticks.end());
    int cost = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        int merged = a + b;
        cost += merged;
        pq.push(merged);
    }
    return cost;
}

// arranging characters of string in such a way that no two adjacent characters are same
string reorganizeString(string s) {
    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    priority_queue<pair<int, char>> pq;
    for (auto& [c, cnt] : freq) {
        if (cnt > (int(s.length()) + 1) / 2) return "";
        pq.push({cnt, c});
    }

    string result;
    pair<int, char> prev = {-1, '#'};
    while (!pq.empty()) {
        auto [cnt, c] = pq.top(); pq.pop();
        result += c;
        if (prev.first > 0) pq.push(prev);
        prev = {cnt - 1, c};
    }
    return result.length() == s.length() ? result : "";
}

// =========================================================================
// 9. RANK TRANSFORM
// =========================================================================
vector<int> replaceElementsByRank(const vector<int>& arr) {
    vector<int> sorted = arr;
    sort(sorted.begin(), sorted.end());

    unordered_map<int, int> rankMap;
    int rank = 1;
    for (int x : sorted) {
        if (rankMap.find(x) == rankMap.end()) {
            rankMap[x] = rank++;
        }
    }

    vector<int> result;
    for (int x : arr) result.push_back(rankMap[x]);
    return result;
}