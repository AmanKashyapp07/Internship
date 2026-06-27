/**
 * @file SlidingWindowMasterTemplate.cpp
 * @brief Ultimate Sliding Window & Two-Pointer Blueprint for Interviews & Advanced CP.
 * @competitions LeetCode (Striver SDE Sheet), Codeforces, CodeChef Range Queries
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <climits>

using namespace std;

// =========================================================================
// SECTION 1: INTERVIEW & OA PATTERNS (STRIVER'S SDE SHEET CORE)
// =========================================================================

class StriverSlidingWindow {
public:
    /**
     * @brief 3. Longest Substring Without Repeating Characters
     * @note Core Strategy: Variable Window (Dynamic Shrink via Hashing Map)
     * Time Complexity: O(N) | Space Complexity: O(128) -> O(1) hash array
     */
    int lengthOfLongestSubstring(const string &s) {
        // Tracks the absolute last seen index of every ASCII character. Initialized to -1.
        vector<int> lastIdx(128, -1);
        int left = 0, maxLength = 0;

        for (int right = 0; right < s.length(); right++) {
            // CRITICAL STEP: If the current character was seen AFTER or AT the current left boundary,
            // it means a duplicate exists in our current window. Warp 'left' directly past the old instance.
            if (lastIdx[s[right]] >= left) {
                left = lastIdx[s[right]] + 1; 
            }
            
            // Record/Update the current character's latest position mapping
            lastIdx[s[right]] = right;
            
            // Calculate current valid window size: (right - left + 1)
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * @brief 1004. Max Consecutive Ones III
     * @note Core Strategy: Variable Window (Counting Constraints)
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    int longestOnes(const vector<int> &nums, int k) {
        int left = 0, zeroes = 0, maxLength = 0;

        for (int right = 0; right < nums.size(); right++) {
            // Expand Window: track total invalid elements (0s) introduced
            if (nums[right] == 0) zeroes++;
            
            // Shrink Window: If invalid count exceeds our allowance 'k', contract from left
            while (zeroes > k) {
                if (nums[left] == 0) zeroes--;
                left++; // Gradually slide the left boundary forward
            }
            
            // Window is guaranteed valid here because the while loop resolved any violation
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * @brief 904. Fruit Into Baskets (Longest Subarray with <= 2 Distinct Elements)
     * @note Core Strategy: Variable Window (Stateful Frequency Map tracking types)
     * Time Complexity: O(N) | Space Complexity: O(1) because map max size is 3 elements
     */
    int totalFruit(const vector<int> &fruits) {
        unordered_map<int, int> count; // Map to track {Fruit_Type -> Frequency_In_Window}
        int left = 0, maxFruits = 0;

        for (int right = 0; right < fruits.size(); right++) {
            count[fruits[right]]++; // Insert current item into window
            
            // If unique element types cross the threshold of 2, shrink the window
            while (count.size() > 2) {
                count[fruits[left]]--;
                if (count[fruits[left]] == 0) {
                    count.erase(fruits[left]); // Entirely wipe element out when count drops to 0
                }
                left++; // Shift left pointer up
            }
            maxFruits = max(maxFruits, right - left + 1);
        }
        return maxFruits;
    }

    /**
     * @brief 424. Longest Repeating Character Replacement
     * @note Core Strategy: Variable Window optimization via Max Frequency Preservation
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    int characterReplacement(const string &s, int k) {
        vector<int> count(26, 0);
        int left = 0, maxFreq = 0, maxLength = 0;

        for (int right = 0; right < s.length(); right++) {
            count[s[right] - 'A']++;
            // Track historical peak frequency of any single character inside the window.
            // Optimization: We don't need to recalculate maxFreq downward when shrinking left!
            maxFreq = max(maxFreq, count[s[right] - 'A']);

            // Invalid condition: (Total characters in window) - (Dominant character count) > k allowed swaps
            if ((right - left + 1) - maxFreq > k) {
                count[s[left] - 'A']--;
                left++; // Discard one element from left
            }
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * @brief Helper for 930 & 1248: Counts subarrays with AT MOST 'k' tracking metrics
     * @note Structural Pattern: Exact Count K = (At Most K) - (At Most K-1)
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    int numSubarraysAtMostK(const vector<int> &nums, int k) {
        if (k < 0) return 0;
        int left = 0, count = 0, currentSum = 0;

        for (int right = 0; right < nums.size(); right++) {
            currentSum += nums[right];
            
            // Shrink window while it breaks the upper bound sum 'k'
            while (currentSum > k) {
                currentSum -= nums[left++];
            }
            
            // KEY INSIGHT: The number of valid subarrays ending exactly at 'right' 
            // is equivalent to the total length of the current window.
            count += (right - left + 1);
        }
        return count;
    }

    int numSubarraysWithSum(const vector<int> &nums, int goal) {
        // Returns the exact combinations matching target 'goal'
        return numSubarraysAtMostK(nums, goal) - numSubarraysAtMostK(nums, goal - 1);
    }

    /**
     * @brief 1358. Number of Substrings Containing All Three Characters ('a', 'b', 'c')
     * @note Core Strategy: Last Seen Anchoring (Right-to-Left Accumulation lookup)
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    int numberOfSubstrings(const string &s) {
        vector<int> lastSeen(3, -1); // Index tracker for 'a', 'b', and 'c'
        int totalCount = 0;

        for (int i = 0; i < s.length(); i++) {
            lastSeen[s[i] - 'a'] = i; // Register current position
            
            // If all 3 characters have appeared at least once somewhere behind us
            if (lastSeen[0] != -1 && lastSeen[1] != -1 && lastSeen[2] != -1) {
                // KEY CONCEPT: The element among the 3 that appeared furthest to the left controls 
                // the minimum boundary. Every index from 0 up to that min element can serve as a 
                // valid start index for a substring ending at 'i'.
                totalCount += min({lastSeen[0], lastSeen[1], lastSeen[2]}) + 1;
            }
        }
        return totalCount;
    }

    /**
     * @brief 1423. Maximum Points You Can Obtain from Cards
     * @note Core Strategy: Inverted Sliding Window (Complementary Subarray Minimum Sum)
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    int maxScore(const vector<int> &cardPoints, int k) {
        int n = cardPoints.size(), windowSize = n - k;
        int totalSum = 0, currentWindowSum = 0;

        for (int x : cardPoints) totalSum += x;
        if (k == n) return totalSum; // Taking all elements

        // Initialize the first sliding window of size (n - k)
        for (int i = 0; i < windowSize; i++) {
            currentWindowSum += cardPoints[i];
        }
        int minWindowSum = currentWindowSum;

        // Slide the fixed window of size (n - k) across the array
        for (int i = windowSize; i < n; i++) {
            currentWindowSum += cardPoints[i] - cardPoints[i - windowSize]; // Slide action
            minWindowSum = min(minWindowSum, currentWindowSum);
        }
        
        // Max score from outer ends = Total Array Sum - Minimum Interior Window Sum
        return totalSum - minWindowSum;
    }

    /**
     * @brief 76. Minimum Window Substring
     * @note Core Strategy: Two Frequency vectors validation loop
     * Time Complexity: O(N) | Space Complexity: O(128)
     */
    bool isValid(const vector<int> &freq1, const vector<int> &freq2) {
        for (int i = 0; i < 128; i++) {
            if (freq2[i] < freq1[i]) return false; // Window missing needed characters
        }
        return true;
    }

    string minWindow(const string &s, const string &t) {
        vector<int> freq1(128, 0), freq2(128, 0);
        for (char ch : t) freq1[ch]++; // Seed target requirements

        int left = 0, start = -1, minLen = INT_MAX;

        for (int right = 0; right < s.size(); right++) {
            freq2[s[right]]++; // Include character in active map
            
            // Shrink window while all character counts from 't' are fully satisfied
            while (isValid(freq1, freq2)) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    start = left;
                }
                freq2[s[left]]--; // Eject character from left
                left++;
            }
        }
        return start == -1 ? "" : s.substr(start, minLen);
    }

    int subarraysWithAtMostKDistinct(const vector<int> &nums, int k) {
        unordered_map<int, int> freq;
        int left = 0, count = 0;

        for (int right = 0; right < nums.size(); right++) {
            freq[nums[right]]++;
            while (freq.size() > k) {
                freq[nums[left]]--;
                if (freq[nums[left]] == 0) {
                    freq.erase(nums[left]);
                }
                left++;
            }
            count += (right - left + 1);
        }
        return count;
    }

    /**
     * @brief 727. Minimum Window Subsequence (Sliding Window + Reverse Scan Alignment)
     * @note Core Strategy: Two-Pointer Forward Matching followed by Right-to-Left Minimization Scan
     * Time Complexity: O(N * M) worst-case | Space Complexity: O(1)
     */
    string minWindowSubsequence(const string &s, const string &t) {
        int n = s.size(), m = t.size();
        int bestStart = -1, bestLen = INT_MAX;
        int i = 0, j = 0;

        while (i < n) {
            if (s[i] == t[j]) j++; // Character match found, move string t pointer forward

            // If string t is completely matched as a subsequence within window
            if (j == m) {
                int end = i; // Lock the ending index of our valid window
                j--;
                
                // CRITICAL REVERSE SCAN: Backtrack from right to left to find optimal start index
                // This eliminates unnecessary matching prefixes from the left side of the window
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                int start = i + 1; // Compute compressed start index

                if (end - start + 1 < bestLen) {
                    bestLen = end - start + 1;
                    bestStart = start;
                }
                // Reset standard search variables just past the start pointer to find alternative solutions
                i = start + 1; 
                j = 0;
                continue;
            }
            i++;
        }
        return bestStart == -1 ? "" : s.substr(bestStart, bestLen);
    }
};

// =========================================================================
// SECTION 2: ADVANCED SYSTEM PROBLEMS (FIXED & DYNAMIC METRICS OVER RANGES)
// =========================================================================

class AdvancedSlidingWindow {
public:
    /**
     * @brief Pattern A: Sliding Window Minimum / Maximum (Monotonic Queue Pattern)
     * @note Core Strategy: Deque containing indices maintaining strict value ordering
     * Time Complexity: O(N) | Space Complexity: O(K)
     */
    vector<int> sliding_window_min(const vector<int> &a, int k) {
        deque<int> dq; // Double-ended queue storing element indices
        vector<int> res;

        for (int i = 0; i < (int)a.size(); i++) {
            // Step 1: Evict indices that have slid completely outside the trailing window bounds
            if (!dq.empty() && dq.front() <= i - k) dq.pop_front();

            // Step 2: Maintain monotonic increasing property. 
            // If the incoming value is smaller than current back element, the back element can never be the minimum! Drop it.
            while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
            dq.push_back(i);

            // Step 3: Once window size 'k' is initialized, the front of the queue is always our current min
            if (i >= k - 1) res.push_back(a[dq.front()]);
        }
        return res;
    }

    vector<int> sliding_window_max(const vector<int> &a, int k) {
        deque<int> dq; // Double-ended queue storing element indices
        vector<int> res;

        for (int i = 0; i < (int)a.size(); i++) {
            // Step 1: Evict indices outside current window
            if (!dq.empty() && dq.front() <= i - k) dq.pop_front();

            // Step 2: Maintain monotonic decreasing property.
            // Pop out smaller values since the new incoming element is larger and will outlive them.
            while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();
            dq.push_back(i);

            // Step 3: Front of queue is always our max element
            if (i >= k - 1) res.push_back(a[dq.front()]);
        }
        return res;
    }

    /**
     * @brief Pattern B: Sliding Window Bitwise XOR Aggregates
     * @note Core Strategy: Standard Fixed Window Invalidation Optimization
     * Time Complexity: O(N) | Space Complexity: O(1)
     */
    vector<int> sliding_window_xor(const vector<int> &a, int k) {
        vector<int> res;
        int xr = 0;

        // Process initial window frame
        for (int i = 0; i < k; i++) xr ^= a[i];
        res.push_back(xr);

        // Slide the window: Apply inverse property. (A ^ B ^ A = B)
        for (int i = k; i < (int)a.size(); i++) {
            xr ^= a[i - k]; // Undo/Eject oldest element out of current evaluation
            xr ^= a[i];     // Apply/Inject new incoming element
            res.push_back(xr);
        }
        return res;
    }

    /**
     * @brief Pattern C: Sliding Window Distinct Values Count
     * Time Complexity: O(N log K) via sorted tree tracking
     */
    vector<int> sliding_window_distinct(const vector<int> &a, int k) {
        vector<int> res;
        map<int, int> freq;

        // Seed initial frame
        for (int i = 0; i < k; i++) freq[a[i]]++;
        res.push_back(freq.size());

        // Process remaining sliding segments
        for (int i = k; i < (int)a.size(); i++) {
            if (--freq[a[i - k]] == 0) {
                freq.erase(a[i - k]); // Wipe key entirely to keep map size strictly mapped to uniques
            }
            freq[a[i]]++;
            res.push_back(freq.size());
        }
        return res;
    }

    /**
     * @brief Pattern D: Sliding Window Median (Two Balanced Multisets Strategy)
     * @note Core Strategy: Low Max-Heap simulation + High Min-Heap simulation via multisets
     * Time Complexity: O(N log K) | Space Complexity: O(K)
     */
    vector<double> slidingWindowMedian(const vector<int> &nums, int k) {
        multiset<int> low, high; // 'low' keeps smaller half (max element at rbegin), 'high' keeps larger half (min element at begin)
        vector<double> medians;

        // Lambda rule to keep the size balanced or skewed by 1 towards 'low' container
        auto balance = [&]() {
            if (low.size() > high.size() + 1) {
                high.insert(*low.rbegin());
                low.erase(prev(low.end()));
            } else if (low.size() < high.size()) {
                low.insert(*high.begin());
                high.erase(high.begin());
            }
        };

        // Add element to correct structural multiset
        auto add = [&](int num) {
            if (low.empty() || num <= *low.rbegin()) low.insert(num);
            else high.insert(num);
            balance();
        };

        // Remove old item out of window scope safely using iterator instance
        auto removeNode = [&](int num) {
            auto it = low.find(num); // find returns a single matching node pointer
            if (it != low.end()) low.erase(it);
            else high.erase(high.find(num));
            balance();
        };

        for (int i = 0; i < nums.size(); i++) {
            add(nums[i]);
            if (i >= k) removeNode(nums[i - k]); // Erase expired node out of our window bounds
            
            // Check if full window is generated
            if (i >= k - 1) {
                if (k % 2 != 0) {
                    medians.push_back(*low.rbegin()); // Odd window size: single middle element
                } else {
                    medians.push_back(((double)*low.rbegin() + *high.begin()) / 2.0); // Even window size: average of middles
                }
            }
        }
        return medians;
    }

    /**
     * @brief Pattern E: Sliding Window Transformation Cost to Match Median
     * @note Mathematical Rule: Total Cost to bring all numbers to median value is:
     * Cost = (median * count(low) - sum(low)) + (sum(high) - median * count(high))
     * Time Complexity: O(N log K) | Space Complexity: O(K)
     */
    vector<long long> slidingWindowCost(const vector<int> &nums, int k) {
        multiset<int> low, high;
        long long sumLow = 0, sumHigh = 0; // Running arithmetic aggregations
        vector<long long> costs;

        auto balance = [&]() {
            if (low.size() > high.size() + 1) {
                int val = *low.rbegin();
                sumLow -= val; sumHigh += val;
                high.insert(val);
                low.erase(prev(low.end()));
            } else if (low.size() < high.size()) {
                int val = *high.begin();
                sumHigh -= val; sumLow += val;
                low.insert(val);
                high.erase(high.begin());
            }
        };

        auto add = [&](int num) {
            if (low.empty() || num <= *low.rbegin()) {
                low.insert(num);
                sumLow += num; // Add to running prefix tracker
            } else {
                high.insert(num);
                sumHigh += num; // Add to running suffix tracker
            }
            balance();
        };

        auto removeNode = [&](int num) {
            auto it = low.find(num);
            if (it != low.end()) {
                sumLow -= num;
                low.erase(it);
            } else {
                sumHigh -= num;
                high.erase(high.find(num));
            }
            balance();
        };

        for (int i = 0; i < nums.size(); i++) {
            add(nums[i]);
            if (i >= k) removeNode(nums[i - k]);
            
            if (i >= k - 1) {
                long long median = *low.rbegin(); // Extract current accurate window median
                
                // Mathematical formulation execution to yield minimal cost variance
                long long cost = (median * low.size() - sumLow) + (sumHigh - median * high.size());
                costs.push_back(cost);
            }
        }
        return costs;
    }
};

// =========================================================================
// PIPELINE RUNNER
// =========================================================================

int main() {
    // Optimize standard I/O streams for Competitive Programming performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    return 0;
}