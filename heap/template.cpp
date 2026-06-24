/**
 * Problem: Priority Queue / Heap Master Template
 * Category: Heap Data Structure
 * * Description:
 * A production-grade competitive programming and interview template covering 
 * essential Priority Queue patterns, time complexities, and algorithms.
 * * Standard Complexities:
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
#include <algorithm>

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

// Custom Comparator Setup (Sorts pairs ascending by their SECOND element)
struct CompareSecond {
    bool operator()(const pii& a, const pii& b) const {
        return a.second > b.second; 
    }
};
using CustomHeap = priority_queue<pii, vector<pii>, CompareSecond>;

// =========================================================================
// 2. K-TH VARIANT CLASSIC PROBLEMS
// =========================================================================

/**
 * Strategy: Maintain a Min Heap of size K. 
 * The top element represents the lower bound threshold of our top-K largest elements.
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

// =========================================================================
// 3. ADVANCED INTERVIEW PATTERNS
// =========================================================================

/**
 * LeetCode 295: Find Median from Data Stream
 * Strategy: Balance elements across two halves. 
 * - Max Heap (left side) holds the smaller numbers.
 * - Min Heap (right side) holds the larger numbers.
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
 * Strategy: Pointer tracking across K arrays using a tuple state structure.
 * Corrects original implementation pointer deletion performance bottleneck.
 */
vector<int> mergeKsortedArrays(const vector<vector<int>>& arrays) {
    // Priority queue elements structured as: {value, {array_index, element_index}}
    using ElementState = pair<int, pair<int, int>>;
    priority_queue<ElementState, vector<ElementState>, greater<ElementState>> pq;

    for (int i = 0; i < arrays.size(); i++) {
        if (!arrays[i].empty()) {
            pq.push({arrays[i][0], {i, 0}});
        }
    }

    vector<int> result;
    while (!pq.empty()) {
        auto [val, coord] = pq.top(); pq.pop();
        auto [arrIdx, elemIdx] = coord;
        
        result.push_back(val);

        // Instead of removing elements from the array front, advance index tracking forward
        if (elemIdx + 1 < arrays[arrIdx].size()) {
            pq.push({arrays[arrIdx][elemIdx + 1], {arrIdx, elemIdx + 1}});
        }
    }
    return result;
}

/**
 * Strategy: Sort values to map continuous numerical bounds to direct rank identifiers.
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
} // this assigns rank to elements on basis of their sorted order, with ties receiving the same rank

/**
 * Strategy: Decreasing order sort + BFS search on combination matrices.
 */
vector<int> maxKsumCombination(vector<int>& nums1, vector<int>& nums2, int k) {
    sort(nums1.rbegin(), nums1.rend());
    sort(nums2.rbegin(), nums2.rend());

    using ElementNode = pair<int, pair<int, int>>; // {sum, {idx1, idx2}}
    priority_queue<ElementNode> pq;
    set<pair<int, int>> visited;

    pq.push({nums1[0] + nums2[0], {0, 0}});
    visited.insert({0, 0});

    vector<int> result;
    while (k-- && !pq.empty()) {
        auto [sum, position] = pq.top(); pq.pop();
        auto [i, j] = position;
        
        result.push_back(sum);

        // Branch right step
        if (i + 1 < nums1.size() && !visited.count({i + 1, j})) {
            pq.push({nums1[i + 1] + nums2[j], {i + 1, j}});
            visited.insert({i + 1, j});
        }
        // Branch down step
        if (j + 1 < nums2.size() && !visited.count({i, j + 1})) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
            visited.insert({i, j + 1});
        }
    }
    return result;
}

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
        if (pq.size() > maxCapacity) {
            pq.pop();
        }
        return pq.top();
    }
};

/**
 * Strategy: Greedy combination (Huffman coding principle). Always combine 
 * the two shortest remaining sticks first.
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

int numSubarraywithSumatmostK(const vector<int>& nums, int k) {
    int left = 0, right = 0, currentSum = 0, count = 0;
    for(int right = 0; right < nums.size(); right++) {
        currentSum += nums[right];
        while(currentSum > k && left <= right) {
            currentSum -= nums[left];
            left++;
        }
        count += (right - left + 1); // all subarrays starting from left to right are valid, subarrays will be [left, right], [left+1, right], ..., [right, right]
    }
    return count;
}
int numSubarrayswithSumK(const vector<int>& nums, int k) {
    return numSubarraywithSumatmostK(nums, k) - numSubarraywithSumatmostK(nums, k - 1);
}
// =========================================================================
// EXECUTIVE EXECUTION BLOCK
// =========================================================================
int main() {
    // Fast Input/Output Pipeline
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    return 0;
}