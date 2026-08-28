#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <utility>
using namespace std;

/**
 * ============================================================================
 * DESIGN 09: HEAP / PRIORITY QUEUE (Min-Heap & Max-Heap)
 * ============================================================================
 * 
 * A Binary Heap is a Complete Binary Tree stored compactly in a contiguous array:
 * For a node at 0-based index `i`:
 * - Parent index      : (i - 1) / 2
 * - Left Child index  : 2 * i + 1
 * - Right Child index : 2 * i + 2
 * 
 * Key Interview Concepts:
 * 1. Sift-Up (Heapify-Up): Used in push() to bubble newly inserted element up.
 * 2. Sift-Down (Heapify-Down): Used in pop() to sink root element down.
 * 3. Linear-Time Build Heap (O(N) Bottom-Up Heapify):
 *    - Starting from the last non-leaf node ((N / 2) - 1) down to index 0.
 *    - Mathematical proof: Sum of (h * N / 2^(h+1)) converges to O(N), NOT O(N log N)!
 * 
 * Time Complexities:
 * - top()        : O(1)
 * - push()       : O(log N)
 * - pop()        : O(log N)
 * - buildHeap()  : O(N)
 * Space Complexity: O(N)
 */

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    vector<T> heap;
    Compare comp;

    // Helper: Sift up element at index i: O(log N)
    void siftUp(size_t i) {
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (comp(heap[i], heap[parent])) {
                swap(heap[i], heap[parent]);
                i = parent;
            } else break;
        }
    }

    // Helper: Sift down element at index i: O(log N)
    void siftDown(size_t i) {
        size_t n = heap.size();
        while (true) {
            size_t left = 2 * i + 1, right = 2 * i + 2, best = i;
            if (left < n && comp(heap[left], heap[best])) best = left;
            if (right < n && comp(heap[right], heap[best])) best = right;
            if (best != i) {
                swap(heap[i], heap[best]);
                i = best;
            } else break;
        }
    }

public:
    PriorityQueue(const Compare& comparator = Compare()) : comp(comparator) {}

    // Constructor: O(N) Linear-Time Build Heap from raw vector
    PriorityQueue(const vector<T>& items, const Compare& comparator = Compare())
        : heap(items), comp(comparator) {
        if (heap.empty()) return;
        for (int i = (int)(heap.size() / 2) - 1; i >= 0; i--) siftDown(i);
    }

    // Insert new element: O(log N)
    void push(const T& val) {
        heap.push_back(val);
        siftUp(heap.size() - 1);
    }

    // Remove top priority element: O(log N)
    void pop() {
        if (empty()) throw runtime_error("[PriorityQueue Error] Cannot pop from empty heap!");
        heap[0] = std::move(heap.back());
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
    }

    // Access top priority element: O(1)
    const T& top() const {
        if (empty()) throw runtime_error("[PriorityQueue Error] Heap is empty!");
        return heap[0];
    }

    size_t size() const { return heap.size(); }
    bool empty() const { return heap.empty(); }
    void clear() { heap.clear(); }
};

// Convenient Type Aliases:
template <typename T>
using MinHeap = PriorityQueue<T, std::less<T>>;

template <typename T>
using MaxHeap = PriorityQueue<T, std::greater<T>>;
