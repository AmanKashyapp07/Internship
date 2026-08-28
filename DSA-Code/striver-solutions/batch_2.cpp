#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 2 (Problems 01 to 17)
// ============================================================================

int findPeakElement(vi& nums) {
    int l = 0, r = nums.size() - 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] > nums[mid + 1]) {
            r = mid; // peak is at mid or to the left (downward slope)
        } else {
            l = mid + 1; // peak is strictly to the right (upward slope)
        }
    }
    return l;
}
// Interview Explanation:
// - Problem Statement: Return the index of any peak element (strictly greater than its neighbors) in O(log N) time.
// - Approach: Binary Search on array slope gradients.
// - Intuition: If nums[mid] > nums[mid + 1], we are on a downward slope; since boundaries are -infinity, a peak is guaranteed in [l, mid]. Otherwise, we are ascending, guaranteeing a peak in [mid + 1, r].
// - Complexity: Time: O(log N) strict halving, Space: O(1) auxiliary space.

bool canEatAll(const vi& piles, int h, int k) {
    ll hours = 0;
    for (int p : piles) {
        hours += (p + k - 1) / k; // ceil(p / k)
    }
    return hours <= h;
}

int minEatingSpeed(vi& piles, int h) {
    int l = 1, r = *max_element(piles.begin(), piles.end());
    int ans = r;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (canEatAll(piles, h, mid)) {
            ans = mid;
            r = mid - 1; // try smaller speed
        } else {
            l = mid + 1; // need faster speed
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the minimum integer eating speed k (bananas/hour) to finish all banana piles within h hours.
// - Approach: Binary Search on Answer in the search range [1, max(piles)].
// - Intuition: The time required monotonically decreases as speed k increases. If Koko can finish at speed k, all speeds > k are also valid; binary search for the first valid speed.
// - Complexity: Time: O(N * log(max(piles))), Space: O(1) auxiliary space.

bool canPlaceCows(const vi& stalls, int k, int dist) {
    int count = 1, last_pos = stalls[0];
    for (int i = 1; i < (int)stalls.size(); i++) {
        if (stalls[i] - last_pos >= dist) {
            count++;
            last_pos = stalls[i];
            if (count >= k) return true;
        }
    }
    return count >= k;
}

int aggressiveCows(vi& stalls, int k) {
    sort(stalls.begin(), stalls.end());
    int l = 1, r = stalls.back() - stalls.front();
    int ans = 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (canPlaceCows(stalls, k, mid)) {
            ans = mid;
            l = mid + 1; // try for larger minimum distance
        } else {
            r = mid - 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Place k cows in given stall positions such that the minimum distance between any two cows is maximized.
// - Approach: Sorting + Binary Search on Answer across distance range [1, stalls[n-1] - stalls[0]].
// - Intuition: Monotonic predicate: if placing cows with min distance >= d is possible, any distance < d is also possible. Greedily place the next cow at the first stall >= d away from the previous.
// - Complexity: Time: O(N log N + N log(max_dist)), Space: O(1) auxiliary space.

bool canAllocate(const vi& arr, int m, int max_pages) {
    int students = 1, current_pages = 0;
    for (int p : arr) {
        if (p > max_pages) return false;
        if (current_pages + p > max_pages) {
            students++;
            current_pages = p;
        } else {
            current_pages += p;
        }
    }
    return students <= m;
}

int findPages(vi& arr, int m) {
    int n = arr.size();
    if (m > n) return -1;
    int l = *max_element(arr.begin(), arr.end());
    int r = accumulate(arr.begin(), arr.end(), 0);
    int ans = -1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (canAllocate(arr, m, mid)) {
            ans = mid;
            r = mid - 1; // try smaller maximum page allocation
        } else {
            l = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Allocate contiguous books among m students such that the maximum pages allocated to any student is minimized.
// - Approach: Binary Search on Answer in range [max(arr), sum(arr)].
// - Intuition: If a page limit P is feasible for <= m students, any higher limit is also feasible. Greedily allocate contiguous books to a student until the page limit is exceeded, then move to the next student.
// - Complexity: Time: O(N * log(sum(arr) - max(arr))), Space: O(1) auxiliary space.

double findMedianSortedArrays(vi& a, vi& b) {
    if (a.size() > b.size()) return findMedianSortedArrays(b, a); // ensure binary search on smaller array
    int n1 = a.size(), n2 = b.size();
    int l = 0, r = n1;
    int left_total = (n1 + n2 + 1) / 2;

    while (l <= r) {
        int mid1 = l + (r - l) / 2;
        int mid2 = left_total - mid1;

        int l1 = (mid1 > 0) ? a[mid1 - 1] : INT_MIN;
        int l2 = (mid2 > 0) ? b[mid2 - 1] : INT_MIN;
        int r1 = (mid1 < n1) ? a[mid1] : INT_MAX;
        int r2 = (mid2 < n2) ? b[mid2] : INT_MAX;

        if (l1 <= r2 && l2 <= r1) {
            if ((n1 + n2) % 2 == 1) return max(l1, l2);
            return (max(l1, l2) + min(r1, r2)) / 2.0;
        } else if (l1 > r2) {
            r = mid1 - 1; // cut too far right in array 'a'
        } else {
            l = mid1 + 1; // cut too far left in array 'a'
        }
    }
    return 0.0;
}
// Interview Explanation:
// - Problem Statement: Find the median of two sorted arrays in O(log(min(n1, n2))) time.
// - Approach: Binary search on partition cut of the smaller array.
// - Intuition: We want to divide the merged set into two equal halves such that max(left_half) <= min(right_half) (i.e. l1 <= r2 and l2 <= r1). Binary search on cut1 automatically determines cut2 = (n1 + n2 + 1)/2 - cut1.
// - Complexity: Time: O(log(min(N1, N2))), Space: O(1) auxiliary space.

bool isGasStationDistPossible(const vi& stations, int k, double max_dist) {
    int needed = 0;
    for (int i = 1; i < (int)stations.size(); i++) {
        double diff = stations[i] - stations[i - 1];
        needed += (int)(diff / max_dist);
    }
    return needed <= k;
}

double minMaxGasStationDistance(vi& stations, int k) {
    double l = 0, r = 0;
    for (int i = 1; i < (int)stations.size(); i++) {
        r = max(r, (double)(stations[i] - stations[i - 1]));
    }
    double eps = 1e-6;
    while (r - l > eps) {
        double mid = l + (r - l) / 2.0;
        if (isGasStationDistPossible(stations, k, mid)) {
            r = mid; // try smaller distance
        } else {
            l = mid;
        }
    }
    return r;
}
// Interview Explanation:
// - Problem Statement: Add k new gas stations such that the maximum distance between adjacent gas stations is minimized.
// - Approach: Binary Search on Answer over continuous range [0, max_gap] with epsilon precision 1e-6.
// - Intuition: For a chosen max distance D, the number of stations required between stations[i] and stations[i-1] is floor((stations[i] - stations[i-1]) / D). If total needed <= k, distance D is achievable.
// - Complexity: Time: O(N * log(max_dist / eps)), Space: O(1) auxiliary space.

ListNode* middleNode(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
// Interview Explanation:
// - Problem Statement: Return the middle node of a singly linked list (second middle node if even length).
// - Approach: Fast and Slow pointer (Tortoise and Hare).
// - Intuition: The fast pointer moves at twice the speed of slow pointer; when fast pointer reaches the list end, slow pointer is stationed at the exact middle.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

bool hasCycle(ListNode *head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Detect if there is a cycle in a singly linked list.
// - Approach: Floyd's Cycle-Finding Algorithm (Tortoise and Hare).
// - Intuition: If a cycle exists, the fast pointer loops and closes the gap to the slow pointer by 1 node per step, guaranteeing they will meet. If fast reaches nullptr, no cycle exists.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode *fast = &dummy, *slow = &dummy;
    for (int i = 0; i <= n; i++) fast = fast->next;
    while (fast) {
        slow = slow->next;
        fast = fast->next;
    }
    slow->next = slow->next->next;
    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Remove the n-th node from the end of a singly linked list in a single pass and return the head.
// - Approach: Two pointers with a dummy sentinel node offset by n + 1 positions.
// - Intuition: Advancing fast by n + 1 nodes and then advancing both pointers simultaneously until fast is nullptr puts slow right before the node that needs to be deleted. Dummy node cleanly handles head removal.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

ListNode* getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (!headA || !headB) return nullptr;
    ListNode *p1 = headA, *p2 = headB;
    while (p1 != p2) {
        p1 = (p1 == nullptr) ? headB : p1->next;
        p2 = (p2 == nullptr) ? headA : p2->next;
    }
    return p1;
}
// Interview Explanation:
// - Problem Statement: Find the node at which the intersection of two singly linked lists begins.
// - Approach: Two pointers with alternate list redirection.
// - Intuition: Moving each pointer across both lists (L1 + L2 distance) equalizes path lengths and eliminates offset differences, aligning both pointers to hit the intersection node at the exact same step.
// - Complexity: Time: O(L1 + L2), Space: O(1) auxiliary space.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

int missingNumber(vi& nums) {
    int n = nums.size(), xr = n;
    for (int i = 0; i < n; i++) {
        xr ^= i ^ nums[i];
    }
    return xr;
}
// Interview Explanation:
// - Problem Statement: Given array nums containing n distinct numbers in the range [0, n], return the only number in the range that is missing.
// - Approach: Bit Manipulation (XOR cancellation).
// - Intuition: XOR of all indices 0..n with all numbers in array cancels duplicate values (`x ^ x = 0`), leaving only the single missing number.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

uint32_t reverseBits(uint32_t n) {
    uint32_t ans = 0;
    for (int i = 0; i < 32; i++) {
        ans = (ans << 1) | (n & 1);
        n >>= 1;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Reverse bits of a given 32-bit unsigned integer.
// - Approach: Bitwise shift and bit extraction over 32 iterations.
// - Intuition: Extract the least significant bit of n (`n & 1`), append it to `ans` shifted left, and shift `n` right.
// - Complexity: Time: O(32) = O(1), Space: O(1) auxiliary space.

int maxArea(vi& height) {
    int l = 0, r = height.size() - 1, max_water = 0;
    while (l < r) {
        max_water = max(max_water, min(height[l], height[r]) * (r - l));
        if (height[l] < height[r]) l++;
        else r--;
    }
    return max_water;
}
// Interview Explanation:
// - Problem Statement: Find two lines that together with the x-axis form a container that contains the most water.
// - Approach: Two Pointers from outermost boundaries inward.
// - Intuition: Container area is limited by the shorter line (`min(h[l], h[r]) * (r - l)`). Shrinking width can only increase area if height increases, so greedily move the shorter line pointer.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int longestConsecutive(vi& nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int longest_streak = 0;

    for (int x : st) {
        if (!st.count(x - 1)) {
            int curr = x, streak = 1;
            while (st.count(curr + 1)) {
                curr++;
                streak++;
            }
            longest_streak = max(longest_streak, streak);
        }
    }

    return longest_streak;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest consecutive elements sequence in an unsorted array in O(N) time.
// - Approach: Hash Set lookup for sequence beginnings.
// - Intuition: Insert all elements into an `unordered_set`. Only start extending a streak from `x` if `x - 1` does NOT exist in the set (ensuring `x` is the true start of a streak). Ensures every number is visited at most twice.
// - Complexity: Time: O(N) average time, Space: O(N) hash set.

int lengthOfLongestSubstring(string s) {
    vi last_pos(256, -1);
    int max_len = 0, left = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        if (last_pos[s[right]] >= left) {
            left = last_pos[s[right]] + 1;
        }
        last_pos[s[right]] = right;
        max_len = max(max_len, right - left + 1);
    }

    return max_len;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring without repeating characters.
// - Approach: Sliding Window with direct index jump map.
// - Intuition: Maintain window `[left, right]`. When character `s[right]` was previously seen at `last_pos >= left`, jump `left` directly to `last_pos + 1` to skip duplicate character in O(1).
// - Complexity: Time: O(N) single pass, Space: O(1) 256-character ASCII array.

int characterReplacement(string s, int k) {
    vi freq(26, 0);
    int l = 0, max_f = 0, max_len = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        freq[s[r] - 'A']++;
        max_f = max(max_f, freq[s[r] - 'A']);

        if ((r - l + 1) - max_f > k) {
            freq[s[l] - 'A']--;
            l++;
        }

        max_len = max(max_len, r - l + 1);
    }

    return max_len;
}
// Interview Explanation:
// - Problem Statement: Return the length of the longest substring containing the same letter after replacing at most k characters.
// - Approach: Sliding Window tracking the most frequent character in the current window.
// - Intuition: In a valid window of size `L`, characters to replace is `L - max_f`. If `L - max_f > k`, shrink window by advancing `l`. `max_f` does not need to decrease during shrinking because only a larger `max_f` can create a larger valid window.
// - Complexity: Time: O(N) single pass, Space: O(1) 26-size frequency array.

string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";
    vi target_freq(128, 0);
    for (char c : t) target_freq[c]++;

    int required = t.size(), l = 0, min_len = INT_MAX, start_idx = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        if (target_freq[s[r]] > 0) required--;
        target_freq[s[r]]--;

        while (required == 0) {
            if (r - l + 1 < min_len) {
                min_len = r - l + 1;
                start_idx = l;
            }
            target_freq[s[l]]++;
            if (target_freq[s[l]] > 0) required++;
            l++;
        }
    }

    return min_len == INT_MAX ? "" : s.substr(start_idx, min_len);
}
// Interview Explanation:
// - Problem Statement: Find the minimum window substring of s such that every character in t (including duplicates) is included in the window.
// - Approach: Sliding Window with character debt counter.
// - Intuition: Decrement frequency for `s[r]`; if `target_freq > 0`, decrement remaining `required` count. Once `required == 0` (window satisfies all characters), contract from left to minimize window while maintaining condition.
// - Complexity: Time: O(N + M), Space: O(1) 128-element ASCII array.
