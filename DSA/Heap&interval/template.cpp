/**
 * Problem: Priority Queue / Heap Master Template
 * Category: Heap Data Structure
 *
 * Description:
 * A production-grade competitive programming and interview template covering 
 * essential Priority Queue patterns, time complexities, and algorithms.
 *
 * Standard Complexities:
 * - Push / Insert : O(log N)
 * - Pop / Delete  : O(log N)
 * - Top / Peek    : O(1)
 * - Heapify Init  : O(N)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

// Standard Type Aliases
using pii = pair<int, int>;

// =========================================================================
// 1. STANDARD HEAP TYPE DEFINITIONS
// =========================================================================

// Max Heaps (Largest element on top)
using MaxHeap     = priority_queue<int>;
using MaxPairHeap = priority_queue<pii>;

// Min Heaps (Smallest element on top)
using MinHeap     = priority_queue<int, vector<int>, greater<int>>;
using MinPairHeap = priority_queue<pii, vector<pii>, greater<pii>>;

bool cmp(const pii& a, const pii& b) {
    return a.second > b.second;
}

priority_queue<pii, vector<pii>, decltype(&cmp)> pq(cmp);

// =========================================================================
// 2. K-TH VARIANT CLASSIC PROBLEMS
// =========================================================================

/**
 * Strategy: Maintain a Min Heap of size K. 
 * The top element represents the lower bound threshold of our top-K largest elements.
 * Time: O(N log K) | Space: O(K)
 */
int kthLargest(const vector<int>& nums, int k) {
    MinHeap pq;
    for (int x : nums) {
        pq.push(x);
        if (pq.size() > k) pq.pop();
    }
    return pq.top();
}

/**
 * Strategy: Maintain a Max Heap of size K. 
 * The top element represents the upper bound threshold of our top-K smallest elements.
 * Time: O(N log K) | Space: O(K)
 */
int kthSmallest(const vector<int>& nums, int k) {
    MaxHeap pq;
    for (int x : nums) {
        pq.push(x);
        if (pq.size() > k) pq.pop();
    }
    return pq.top();
}

/**
 * Strategy: Count frequencies using a hash map, then push frequencies into a Min Heap.
 * Time: O(N log K) | Space: O(N)
 */
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freqMap;
    for (int x : nums) freqMap[x]++;

    MinPairHeap pq; // Stores {frequency, element}
    for (auto& [num, freq] : freqMap) {
        pq.push({freq, num});
        if (pq.size() > k) pq.pop(); // Evict elements with the lowest frequency
    }

    vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().second);
        pq.pop();
    }
    return result;
}

/**
 * LeetCode 973: K Closest Points to Origin
 * Strategy: Use a Max Heap to keep track of the K closest points.
 * Time: O(N log K) | Space: O(K)
 */
vector<vector<int>> kClosestPoints(const vector<vector<int>>& points, int k) {
    // Max heap storing {distance_squared, index_of_point}
    priority_queue<pair<int, int>> pq;
    for (int i = 0; i < (int)points.size(); ++i) {
        int dist = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        pq.push({dist, i});
        if (pq.size() > k) {
            pq.pop();
        }
    }
    vector<vector<int>> result;
    while (!pq.empty()) {
        result.push_back(points[pq.top().second]);
        pq.pop();
    }
    return result;
}

// =========================================================================
// 3. STREAM & SLIDING WINDOW PATTERNS
// =========================================================================

/**
 * LeetCode 295: Find Median from Data Stream
 * Strategy: Balance elements across two halves. 
 * - Max Heap (left side) holds the smaller numbers.
 * - Min Heap (right side) holds the larger numbers.
 * Time: O(log N) per insert | Space: O(N)
 */
class MedianFinder {
private:
    MaxHeap left;  // Stores smaller half
    MinHeap right; // Stores larger half

public:
    void addNum(int num) {
        left.push(num);
        right.push(left.top());
        left.pop();

        // Rebalance to preserve the invariant: size(left) >= size(right)
        if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }
    }

    double findMedian() {
        if (left.size() > right.size()) {
            return left.top();
        }
        return (left.top() + right.top()) / 2.0;
    }
};

/**
 * LeetCode 703: Kth Largest Element in a Stream
 */
class KthLargestStream {
private:
    MinHeap pq;
    int maxCapacity;

public:
    KthLargestStream(int k, const vector<int>& nums) : maxCapacity(k) {
        for (int x : nums) {
            add(x);
        }
    }
    
    int add(int val) {
        pq.push(val);
        if ((int)pq.size() > maxCapacity) {
            pq.pop();
        }
        return pq.top();
    }
};

// =========================================================================
// 4. K-WAY MERGE & ARRAY MATCHING PATTERNS
// =========================================================================

/**
 * Strategy: Pointer tracking across K arrays using a tuple state structure.
 * Time: O(N log K) where N is total elements | Space: O(K)
 */
vector<int> mergeKsortedArrays(const vector<vector<int>>& arrays) {
    // Priority queue elements structured as: {value, {array_index, element_index}}
    using T = pair<int, pair<int, int>>;
    priority_queue<T, vector<T>, greater<T>> pq;

    for (int i = 0; i < (int)arrays.size(); i++) {
        if (!arrays[i].empty()) {
            pq.push({arrays[i][0], {i, 0}});
        }
    }

    vector<int> result;
    while (!pq.empty()) {
        auto [val, coord] = pq.top(); pq.pop();
        auto [arrIdx, elemIdx] = coord;
        
        result.push_back(val);

        if (elemIdx + 1 < (int)arrays[arrIdx].size()) {
            pq.push({arrays[arrIdx][elemIdx + 1], {arrIdx, elemIdx + 1}});
        }
    }
    return result;
}

/**
 * LeetCode 373: Find K Pairs with Smallest Sums
 *
 * Problem Statement:
 * Given two integer arrays nums1 and nums2 sorted in ascending order,
 * and an integer k, return the k pairs (u, v), where u belongs to nums1
 * and v belongs to nums2, with the smallest sums.
 *
 * Approach:
 * Multi-pointer advancement using a Min Heap (K-way Merge).
 *
 * Time Complexity: O(k log min(n, k))
 * Space Complexity: O(min(n, k))
 */
vector<pair<int, int>> kSmallestPairs(const vector<int>& nums1, const vector<int>& nums2, int k) {
    vector<pair<int, int>> result;
    if (nums1.empty() || nums2.empty() || k <= 0) return result;
    int n=nums1.size(), m=nums2.size();
    // Min heap storing: {sum, {idx1, idx2}}
    using T = pair<int, pair<int, int>>;
    priority_queue<T, vector<T>, greater<T>> pq;
    
    // Push nums1[i] + nums2[0] for up to min(nums1.size(), k)
    for (int i = 0; i < min(n, k); ++i) {
        pq.push({nums1[i] + nums2[0], {i, 0}});
    }
    
    while (k > 0 && !pq.empty()) {
        auto [sum, indices] = pq.top(); pq.pop();
        auto [i, j] = indices;
        result.push_back({nums1[i], nums2[j]});
        
        // Push the next candidate pair nums1[i] + nums2[j + 1]
        if (j + 1 < m) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
        }
        k--;
    }
    return result;
}


/**
 * Maximum Sum Combinations (Top K Sum Combinations)
 *
 * Problem Statement:
 * Given two integer arrays nums1 and nums2 of size N, find the top k
 * maximum sum combinations, where each combination consists of one
 * element from nums1 and one element from nums2. Return the k largest
 * possible sums.
 *
 * Approach:
 * Sort both arrays in decreasing order and perform a Best-First Search
 * (BFS) on the virtual sum matrix using a Max Heap and a Visited Set.
 *
 * Time Complexity: O(n log n + k log k)
 * Space Complexity: O(k)
 */
vector<int> maxKsumCombination(vector<int>& nums1, vector<int>& nums2, int k) {
    sort(nums1.rbegin(), nums1.rend());
    sort(nums2.rbegin(), nums2.rend());

    using T = pair<int, pair<int, int>>; // {sum, {idx1, idx2}}
    priority_queue<T> pq;
    set<pair<int, int>> visited;

    pq.push({nums1[0] + nums2[0], {0, 0}});
    visited.insert({0, 0});

    vector<int> result;
    while (k-- && !pq.empty()) {
        auto [sum, position] = pq.top(); pq.pop();
        auto [i, j] = position;
        
        result.push_back(sum);

        // Branch right step
        if (i + 1 < (int)nums1.size() && !visited.count({i + 1, j})) {
            pq.push({nums1[i + 1] + nums2[j], {i + 1, j}});
            visited.insert({i + 1, j});
        }
        // Branch down step
        if (j + 1 < (int)nums2.size() && !visited.count({i, j + 1})) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
            visited.insert({i, j + 1});
        }
    }
    return result;
}

// =========================================================================
// 5. GREEDY HEAP ALGORITHMS
// =========================================================================

/**
 * Strategy: Greedy combination (Huffman coding principle). Always combine 
 * the two shortest remaining sticks first.
 * Time: O(N log N) | Space: O(N)
 */
int minCostToConnectSticks(const vector<int>& sticks) {
    MinHeap pq(sticks.begin(), sticks.end()); // O(N) heap construction optimization
    int dynamicTotalCost = 0;

    while (pq.size() > 1) {
        int shortest = pq.top(); pq.pop();
        int secondShortest = pq.top(); pq.pop();

        int combinedLength = shortest + secondShortest;
        dynamicTotalCost += combinedLength;

        pq.push(combinedLength);
    }
    return dynamicTotalCost;
}

/**
 * LeetCode 767: Reorganize String
 * Strategy: Use a Max Heap to arrange characters by highest frequency first, 
 * keeping track of the previous character to prevent adjacency duplicates.
 * Time: O(N log A) where A is alphabet size (26) | Space: O(A)
 */
string reorganizeString(string s) {
    unordered_map<char, int> freqMap;
    for (char c : s) freqMap[c]++;
    
    // Max heap storing: {frequency, char}
    priority_queue<pair<int, char>> pq;
    for (auto& [c, count] : freqMap) {
        if (count > ((int)s.length() + 1) / 2) return "";
        pq.push({count, c});
    }
    
    string result = "";
    pair<int, char> prev = {-1, '#'};
    
    while (!pq.empty()) {
        auto [count, c] = pq.top(); pq.pop();
        result += c;
        
        if (prev.first > 0) {
            pq.push(prev);
        }
        
        prev = {count - 1, c};
    }
    return result.length() == s.length() ? result : "";
}

/**
 * Strategy: Sort values to map continuous numerical bounds to direct rank identifiers.
 * Time: O(N log N) | Space: O(N)
 */
vector<int> replaceElementsByRank(const vector<int>& arr) {
    vector<int> sortedCopy = arr;
    sort(sortedCopy.begin(), sortedCopy.end());

    unordered_map<int, int> ranks;
    int currentRank = 1;

    for (int x : sortedCopy) {
        if (ranks.find(x) == ranks.end()) {
            ranks[x] = currentRank++;
        }
    }

    vector<int> result;
    result.reserve(arr.size());
    for (int x : arr) {
        result.push_back(ranks[x]);
    }
    return result;
}

