#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
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

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                    PROBLEM SUMMARY & COMPLEXITY TABLE: G2.CPP (Problems 23 - 44)                    
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 23 | Minimum Cost to Hire K Workers              | Greedy Ratio Sort + Max-Heap Qual | O(N logN) | O(N + K) |
 | 24 | Meeting Rooms III                           | Dual Priority Queue Scheduling    | O(M logM) | O(N)     |
 | 25 | Maximum Profit in Job Scheduling            | End-Time Sort + DP + Binary Search | O(N logN) | O(N)     |
 | 26 | Minimum Difficulty of a Job Schedule        | Dynamic Programming (Day Chunks)  | O(D * N^2 | O(D * N) |
 | 27 | Boats to Save People                        | Two Pointers Greedy Pairing       | O(N logN) | O(1)     |
 | 28 | Queue Reconstruction by Height              | Descending Height List Insertion  | O(N^2)   | O(N)     |
 | 29 | Advantage Shuffle                           | Greedy Sorted Tian Ji Strategy    | O(N logN) | O(N)     |
 | 30 | Bag of Tokens                               | Two Pointers Face-Up/Down Greed   | O(N logN) | O(1)     |
 | 31 | Reduce Array Size to The Half               | Frequency Map + Greedy Sort       | O(N logN) | O(N)     |
 | 32 | Minimum Increment to Make Array Unique      | In-Place Running Floor Propagation | O(N logN) | O(1)     |
 | 33 | Min Moves Make Array Complementary          | Difference Array Event Sweep      | O(N + L) | O(L)     |
 | 34 | Divide Array Into Arrays With Max Difference | Triplet Contiguous Window Check   | O(N logN) | O(N)     |
 | 35 | Put Boxes Into the Warehouse I              | Preprocessed Ceiling + Greedy Fill | O(N logN) | O(1)     |
 | 36 | Put Boxes Into the Warehouse II             | Two-Ended Inward Warehouse Pointers | O(N logN) | O(1)     |
 | 37 | Remove Duplicate Letters                    | Monotonic Stack + Last Seen Index | O(N)     | O(1)     |
 | 38 | Remove K Digits                             | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 39 | Create Maximum Number                       | Monotonic Stack + Vector Merge    | O(K*(N+M)) | O(K)     |
 | 40 | Reorganize String                           | Max-Heap Frequency Pairing        | O(N logA) | O(A)     |
 | 41 | Rearrange String k Distance Apart           | Max-Heap + Wait-Queue Cooldown    | O(N logA) | O(A)     |
 | 42 | Minimum Add to Make Parentheses Valid       | Single-Pass Balance Counter       | O(N)     | O(1)     |
 | 43 | Min Remove to Make Valid Parentheses        | Two-Pass Invalidation Filter      | O(N)     | O(N)     |
 | 44 | Valid Parenthesis String                    | Range of Open Parentheses [lo, hi] | O(N)     | O(1)     |
 ====================================================================================================
*/

// =========================================================
// 23. MINIMUM COST TO HIRE K WORKERS [G-23]
// =========================================================

double mincostToHireWorkers(vector<int>& qual, vector<int>& wage, int k) {
    int n = qual.size();
    vector<pair<double, int>> w(n);
    for (int i = 0; i < n; i++) w[i] = {(double)wage[i] / qual[i], qual[i]};
    sort(w.begin(), w.end());

    priority_queue<int> pq; // max-heap of qualities
    int sumQ = 0;
    double ans = 1e18;

    for (auto [ratio, q] : w) {
        pq.push(q);
        sumQ += q;
        if ((int)pq.size() > k) {
            sumQ -= pq.top();
            pq.pop();
        }
        if ((int)pq.size() == k)
            ans = min(ans, sumQ * ratio);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Hire k workers minimizing total cost while paying each worker in proportion to their quality.
// - Approach: Greedy Wage/Quality Ratio Sort + Max-Heap of Qualities.
// - Intuition:
//   * Each worker receives max_ratio * quality[i] to satisfy wage constraints across all hired workers.
//   * Sort workers by wage/quality ratio ascending.
//   * When considering worker i, their ratio is guaranteed to be the maximum ratio among all workers seen so far.
//   * To minimize total pay (ratio * sum_quality), maintain a max-heap of qualities of size k.
//   * Evict the worker with the highest quality to keep sumQ minimized.
// - Complexity: Time: O(N log N + N log K), Space: O(N + K).


// =========================================================
// 24. MEETING ROOMS III [G-24]
// =========================================================

int mostBooked(int n, vector<vector<int>>& meetings) {
    sort(meetings.begin(), meetings.end());

    priority_queue<int, vector<int>, greater<int>> free;
    for (int i = 0; i < n; i++) free.push(i);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> busy;
    vector<int> cnt(n);

    for (auto& m : meetings) {
        long long start = m[0], end = m[1];
        while (!busy.empty() && busy.top().first <= start) {
            free.push(busy.top().second);
            busy.pop();
        }
        if (!free.empty()) {
            int room = free.top(); free.pop();
            cnt[room]++;
            busy.push({end, room});
        } else {
            auto [freeTime, room] = busy.top(); busy.pop();
            cnt[room]++;
            busy.push({freeTime + (end - start), room});
        }
    }
    return max_element(cnt.begin(), cnt.end()) - cnt.begin();
}
// Interview Explanation:
// - Problem Statement: Find room hosting the most meetings under lowest-index-first and delayed-allocation rules.
// - Approach: Dual Priority Queue Scheduling (Free Rooms & Busy Rooms).
// - Intuition:
//   * Free rooms min-heap orders rooms by lowest index.
//   * Busy rooms min-heap orders active meetings by {freeTime, room_index}.
//   * For each incoming meeting, release all rooms whose meetings finished on or before current start time.
//   * If a room is free, assign immediately.
//   * If all rooms are busy, fast-forward to earliest finished meeting (busyRooms.top()), reuse that room, and delay the meeting.
// - Complexity: Time: O(M log M + M log N), Space: O(N).


// =========================================================
// 25. MAXIMUM PROFIT IN JOB SCHEDULING [G-25]
// =========================================================

int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
    int n = startTime.size();
    vector<array<int, 3>> jobs(n);
    for (int i = 0; i < n; i++) jobs[i] = {endTime[i], startTime[i], profit[i]};
    sort(jobs.begin(), jobs.end());

    vector<pair<int, int>> dp = {{0, 0}}; // {end, max_profit}
    for (auto& [e, s, p] : jobs) {
        auto it = prev(upper_bound(dp.begin(), dp.end(), make_pair(s, INT_MAX)));
        int cur = it->second + p;
        if (cur > dp.back().second)
            dp.push_back({e, cur});
    }
    return dp.back().second;
}
// Interview Explanation:
// - Problem Statement: Select non-overlapping jobs to maximize total profit.
// - Approach: End-Time Sorting + 1D DP with Binary Search.
// - Intuition:
//   * Sort jobs ascending by end time.
//   * For each job, either skip it (keep previous maximum profit) or schedule it.
//   * If scheduled, add its profit to the max profit achievable from jobs finishing on or before its start time.
//   * Use binary search on previously recorded {end_time, profit} entries to find the compatible predecessor in O(log N).
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 26. MINIMUM DIFFICULTY OF A JOB SCHEDULE [G-26]
// =========================================================

int minDifficulty(vector<int>& job, int d) {
    int n = job.size();
    if (n < d) return -1;
    vector<int> dp(n + 1, 1e9);
    dp[0] = 0;

    for (int day = 1; day <= d; day++) {
        vector<int> next(n + 1, 1e9);
        for (int i = day; i <= n; i++) {
            int mx = 0;
            for (int j = i; j >= day; j--) {
                mx = max(mx, job[j - 1]);
                if (dp[j - 1] != 1e9)
                    next[i] = min(next[i], dp[j - 1] + mx);
            }
        }
        dp = std::move(next);
    }
    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Schedule n jobs over d days with at least 1 job per day minimizing sum of daily maximum difficulties.
// - Approach: Dynamic Programming on Days and Subarrays.
// - Intuition:
//   * If jobs count n < days d, scheduling is impossible; return -1.
//   * Let dp[day][i] be the minimum difficulty scheduling the first i jobs across day days.
//   * Transition: iterate partition boundary j for the current day's chunk jobs[j-1..i-1].
//   * Track max difficulty in the chunk, updating dp[day][i] = min(dp[day-1][j-1] + chunkMax).
// - Complexity: Time: O(D * N^2), Space: O(D * N).


// =========================================================
// 27. BOATS TO SAVE PEOPLE [G-27]
// =========================================================

int numRescueBoats(vector<int>& people, int limit) {
    sort(people.begin(), people.end());
    int l = 0, r = (int)people.size() - 1, boats = 0;
    while (l <= r) {
        if (people[l] + people[r] <= limit) l++;
        r--;
        boats++;
    }
    return boats;
}
// Interview Explanation:
// - Problem Statement: Minimize rescue boats where each boat carries at most 2 people whose sum weight <= limit.
// - Approach: Greedy Two Pointers on Sorted Weights.
// - Intuition:
//   * The heaviest person (people[r]) must board a boat.
//   * To optimize boat capacity, check if the lightest remaining person (people[l]) can share this boat.
//   * If people[l] + people[r] <= limit, pair both onto the boat (l++, r--).
//   * Otherwise, the heaviest person must travel alone (r--).
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 28. QUEUE RECONSTRUCTION BY HEIGHT [G-28]
// =========================================================

vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
    sort(people.begin(), people.end(), [](auto& a, auto& b) {
        return a[0] == b[0] ? a[1] < b[1] : a[0] > b[0];
    });
    vector<vector<int>> ans;
    for (auto& p : people)
        ans.insert(ans.begin() + p[1], p);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Reconstruct queue from [height, k] where k is count of people in front with height >= h.
// - Approach: Descending Height Sort + Index Insertion.
// - Intuition:
//   * Taller people do not perceive shorter people in front of them.
//   * Sort people primarily by height descending; tie-break by k ascending.
//   * Insert each person directly at index k of the output list.
//   * Since all previously inserted people are taller or equal, placing at index k guarantees exactly k taller people are in front.
// - Complexity: Time: O(N^2), Space: O(N).


// =========================================================
// 29. ADVANTAGE SHUFFLE [G-29]
// =========================================================

vector<int> advantageCount(vector<int>& a, vector<int>& b) {
    int n = a.size();
    sort(a.begin(), a.end());
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int i, int j) { return b[i] < b[j]; });

    vector<int> ans(n);
    int lo = 0, hi = n - 1;
    for (int x : a) {
        if (x > b[idx[lo]]) ans[idx[lo++]] = x;
        else ans[idx[hi--]] = x;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Permute array a to maximize count of indices where a[i] > b[i].
// - Approach: Greedy Tian Ji Horse Racing Strategy.
// - Intuition:
//   * Sort both a and b (tracking original indices for b).
//   * If the smallest available element in a can beat the smallest element in b, greedily take that win!
//   * If it cannot beat the smallest element in b, it cannot beat any element in b; sacrifice this weakest horse against b's strongest horse (sortedB[hi]).
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 30. BAG OF TOKENS [G-30]
// =========================================================

int bagOfTokensScore(vector<int>& tokens, int power) {
    sort(tokens.begin(), tokens.end());
    int l = 0, r = (int)tokens.size() - 1, score = 0, ans = 0;
    while (l <= r) {
        if (power >= tokens[l]) {
            power -= tokens[l++];
            ans = max(ans, ++score);
        } else if (score > 0 && l < r) {
            power += tokens[r--];
            score--;
        } else break;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Maximize score using tokens to gain score (face-up, costs power) or gain power (face-down, costs 1 score).
// - Approach: Greedy Two Pointers on Sorted Tokens.
// - Intuition:
//   * To gain score, spend power on the cheapest available tokens (tokens[l]).
//   * When power is exhausted, trade 1 score to purchase the largest available power source (tokens[r]).
//   * Never trade score if no cheaper tokens remain to be bought (l < r).
//   * Maintain maxScore across all intermediate states.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 31. REDUCE ARRAY SIZE TO THE HALF [G-31]
// =========================================================

int minSetSize(vector<int>& arr) {
    unordered_map<int, int> freq;
    for (int x : arr) freq[x]++;
    vector<int> counts;
    for (auto& [_, c] : freq) counts.push_back(c);
    sort(counts.rbegin(), counts.rend());

    int removed = 0, ans = 0, half = arr.size() / 2;
    for (int c : counts) {
        removed += c;
        ans++;
        if (removed >= half) break;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum size of integer set to remove so at least half of the array is removed.
// - Approach: Frequency Hash Map + Greedy Count Sorting.
// - Intuition:
//   * Removing numbers with the highest frequencies deletes the most elements per unique choice.
//   * Compute frequencies using a hash map and sort them in descending order.
//   * Greedily pick the largest frequencies until the cumulative sum reaches at least n / 2.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 32. MINIMUM INCREMENT TO MAKE ARRAY UNIQUE [G-32]
// =========================================================

int minIncrementForUnique(vector<int>& a) {
    sort(a.begin(), a.end());
    int ans = 0;
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i] <= a[i - 1]) {
            ans += a[i - 1] + 1 - a[i];
            a[i] = a[i - 1] + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum increment operations so that every array element is unique.
// - Approach: In-Place Running Floor Propagation on Sorted Array.
// - Intuition:
//   * Sort the array ascending.
//   * If element a[i] <= a[i-1], a[i] must be incremented to at least a[i-1] + 1 to maintain uniqueness.
//   * Accumulate the delta (a[i-1] + 1 - a[i]) into ans and update a[i] in-place.
//   * A single forward pass resolves all duplicates optimally.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 33. MINIMUM NUMBER OF MOVES TO MAKE ARRAY COMPLEMENTARY [G-33]
// =========================================================

int minMoves(vector<int>& a, int limit) {
    int n = a.size();
    vector<int> diff(2 * limit + 2, 0);

    for (int i = 0; i < n / 2; ++i) {
        int x = min(a[i], a[n - 1 - i]);
        int y = max(a[i], a[n - 1 - i]);

        // [2, 2 * limit] default: 2 moves
        diff[2] += 2;
        diff[2 * limit + 1] -= 2;

        // [1 + x, limit + y]: 1 move
        diff[1 + x] -= 1;
        diff[limit + y + 1] += 1;

        // [x + y]: 0 moves
        diff[x + y] -= 1;
        diff[x + y + 1] += 1;
    }

    int ans = n, cur = 0;
    for (int s = 2; s <= 2 * limit; ++s) {
        cur += diff[s];
        ans = min(ans, cur);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find min moves to make all complementary pairs (a[i] + a[n-1-i]) equal across an array.
// - Approach: Difference Array Interval Range Overlap.
// - Intuition:
//   * For pair (x, y) with x <= y:
//     - Sum in [2, 2*limit]: 2 replacements needed.
//     - Sum in [1 + x, limit + y]: only 1 replacement needed.
//     - Sum == x + y: exactly 0 replacements needed.
//   * Record delta changes for each pair into a difference array.
//   * Prefix sum across all possible target sums 2..2*limit finds the minimum moves.
// - Complexity: Time: O(N + Limit), Space: O(Limit).


// =========================================================
// 34. DIVIDE ARRAY INTO ARRAYS WITH MAX DIFFERENCE [G-34]
// =========================================================

vector<vector<int>> divideArray(vector<int>& a, int k) {
    sort(a.begin(), a.end());
    vector<vector<int>> ans;
    for (int i = 0; i < (int)a.size(); i += 3) {
        if (a[i + 2] - a[i] > k) return {};
        ans.push_back({a[i], a[i + 1], a[i + 2]});
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Divide array into triplets such that max difference in each triplet <= k.
// - Approach: Sorted Triplet Contiguous Window Check.
// - Intuition:
//   * In a sorted array, adjacent elements have the smallest pairwise differences.
//   * If a valid division exists, contiguous triplets [a[i], a[i+1], a[i+2]] offer the tightest spreads.
//   * Check if a[i+2] - a[i] <= k. If violated for any triplet, no valid grouping is possible; return empty.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 35. PUT BOXES INTO THE WAREHOUSE I [G-35]
// =========================================================

int maxBoxesInWarehouse(vector<int>& boxes, vector<int>& warehouse) {
    int m = warehouse.size();
    for (int i = 1; i < m; ++i)
        warehouse[i] = min(warehouse[i], warehouse[i - 1]);
    sort(boxes.begin(), boxes.end());

    int ans = 0;
    for (int i = m - 1; i >= 0 && ans < (int)boxes.size(); --i)
        if (boxes[ans] <= warehouse[i]) ans++;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Push boxes into a warehouse from left to right maximizing boxes stored.
// - Approach: Preprocessed Effective Ceiling + Right-to-Left Greedy Sweep.
// - Intuition:
//   * A box cannot pass through any doorway smaller than its height.
//   * Preprocess effective height from left to right: warehouse[i] = min(warehouse[i], warehouse[i-1]).
//   * Fill from the deepest room (rightmost) back toward room 0.
//   * Match the smallest available box into the deepest room possible.
// - Complexity: Time: O(N log N + M), Space: O(1) auxiliary space.


// =========================================================
// 36. PUT BOXES INTO THE WAREHOUSE II [G-36]
// =========================================================

int maxBoxesInWarehouse2(vector<int>& boxes, vector<int>& warehouse) {
    sort(boxes.rbegin(), boxes.rend());
    int l = 0, r = (int)warehouse.size() - 1, ans = 0;
    for (int b : boxes) {
        if (l > r) break;
        if (warehouse[l] >= warehouse[r]) {
            if (warehouse[l] >= b) ans++, l++;
        } else {
            if (warehouse[r] >= b) ans++, r--;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Push boxes from either left or right entrance maximizing boxes stored.
// - Approach: Two-Ended Inward Warehouse Pointers + Largest-Box Matching.
// - Intuition:
//   * Boxes can enter from either end.
//   * Sort boxes in descending order to place the hardest-to-fit (largest) boxes first.
//   * Compare the entrance capacities at left (l) and right (r); push into whichever side has greater clearance.
// - Complexity: Time: O(N log N + M), Space: O(1) auxiliary space.


// =========================================================
// 37. REMOVE DUPLICATE LETTERS [G-37]
// =========================================================

string removeDuplicateLetters(string s) {
    vector<int> last(26);
    vector<bool> seen(26);
    for (int i = 0; i < (int)s.size(); ++i) last[s[i] - 'a'] = i;

    string ans = "";
    for (int i = 0; i < (int)s.size(); ++i) {
        int c = s[i] - 'a';
        if (seen[c]) continue;
        while (!ans.empty() && ans.back() > s[i] && last[ans.back() - 'a'] > i) {
            seen[ans.back() - 'a'] = false;
            ans.pop_back();
        }
        ans.push_back(s[i]);
        seen[c] = true;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Remove duplicate letters so every letter appears once and result is smallest in lexicographical order.
// - Approach: Monotonic Stack with Last Occurrence Lookup and Visited Array.
// - Intuition:
//   * To minimize lexicographical order, earlier characters should be as small as possible.
//   * When encountering character s[i], if it is already retained in our candidate stack, skip it.
//   * Otherwise, pop characters from stack top that are lexicographically larger than s[i] IF they appear again later (last[c] > i).
//   * Pushing s[i] onto stack maintains monotonic increasing order whenever future duplicates allow.
// - Complexity: Time: O(N), Space: O(1) (alphabet of 26).


// =========================================================
// 38. REMOVE K DIGITS [G-38]
// =========================================================

string removeKdigits(string num, int k) {
    string ans = "";
    for (char c : num) {
        while (!ans.empty() && k > 0 && ans.back() > c) {
            ans.pop_back();
            k--;
        }
        ans.push_back(c);
    }
    while (k-- > 0 && !ans.empty()) ans.pop_back();

    int start = 0;
    while (start < (int)ans.size() && ans[start] == '0') start++;
    ans = ans.substr(start);
    return ans.empty() ? "0" : ans;
}
// Interview Explanation:
// - Problem Statement: Remove k digits from non-negative integer string num to make the remaining number as small as possible.
// - Approach: Monotonic Increasing Stack.
// - Intuition:
//   * In decimal numbers, higher significant digits (left) dominate the total magnitude.
//   * Whenever a digit is followed by a smaller digit, deleting the larger preceding digit produces a smaller overall value.
//   * Maintain a monotonically increasing character stack; pop when st.back() > current digit while k > 0.
//   * Strip remaining k from the right and trim leading zeros.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 39. CREATE MAXIMUM NUMBER [G-39]
// =========================================================

vector<int> maxSubsequence(const vector<int>& a, int k) {
    vector<int> st;
    int drop = (int)a.size() - k;
    for (int x : a) {
        while (!st.empty() && drop > 0 && st.back() < x) {
            st.pop_back();
            drop--;
        }
        st.push_back(x);
    }
    st.resize(k);
    return st;
}

vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
    int n = nums1.size(), m = nums2.size();
    vector<int> best;
    for (int i = max(0, k - m); i <= min(k, n); ++i) {
        vector<int> seq1 = maxSubsequence(nums1, i);
        vector<int> seq2 = maxSubsequence(nums2, k - i);
        vector<int> cand;
        auto itA = seq1.begin(), itB = seq2.begin();
        while (itA != seq1.end() || itB != seq2.end()) {
            if (lexicographical_compare(itA, seq1.end(), itB, seq2.end()))
                cand.push_back(*itB++);
            else
                cand.push_back(*itA++);
        }
        best = max(best, cand);
    }
    return best;
}
// Interview Explanation:
// - Problem Statement: Create maximum number of length k from digits of two arrays preserving relative order.
// - Approach: Monotonic Stack Subsequence Extraction + Lexicographical Vector Merge.
// - Intuition:
//   * Decompose into two subproblems:
//     1. Pick largest subsequence of length i from nums1 and length (k - i) from nums2 using a monotonic stack.
//     2. Merge two candidate sequences greedily: compare suffixes lexicographically to decide which vector to pull from.
//   * Iterate over all valid split lengths i in [max(0, k-m), min(k, n)] and retain the globally largest candidate.
// - Complexity: Time: O(K * (N + M)), Space: O(K).


// =========================================================
// 40. REORGANIZE STRING [G-40]
// =========================================================

string reorganizeString(string s) {
    vector<int> cnt(26);
    for (char c : s) cnt[c - 'a']++;

    priority_queue<pair<int, char>> pq;
    for (int i = 0; i < 26; ++i) {
        if (cnt[i] > ((int)s.size() + 1) / 2) return "";
        if (cnt[i]) pq.push({cnt[i], (char)('a' + i)});
    }

    string ans = "";
    while (pq.size() >= 2) {
        auto [c1, ch1] = pq.top(); pq.pop();
        auto [c2, ch2] = pq.top(); pq.pop();
        ans.push_back(ch1);
        ans.push_back(ch2);
        if (--c1) pq.push({c1, ch1});
        if (--c2) pq.push({c2, ch2});
    }
    if (!pq.empty()) ans.push_back(pq.top().second);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Rearrange characters such that no two adjacent characters are identical.
// - Approach: Max-Heap Frequency Pairing.
// - Intuition:
//   * Pigeonhole principle: if any character has count > (n + 1) / 2, valid arrangement is impossible; return "".
//   * Maintain a max-heap of character frequencies.
//   * Greedily pull the two most frequent characters simultaneously and append them in sequence.
//   * Decrement their counts and push back to the heap if remaining count > 0.
//   * Guarantees adjacent elements are distinct.
// - Complexity: Time: O(N log A) where A = 26, Space: O(A) = O(1).


// =========================================================
// 41. REARRANGE STRING K DISTANCE APART [G-41]
// =========================================================

string rearrangeString(string s, int k) {
    if (k <= 1) return s;
    int freq[26] = {0};
    for (char c : s) freq[c - 'a']++;

    priority_queue<pair<int, char>> pq;
    for (int i = 0; i < 26; ++i)
        if (freq[i]) pq.push({freq[i], (char)('a' + i)});

    queue<pair<int, char>> waitQ;
    string ans = "";

    while (!pq.empty()) {
        auto [count, c] = pq.top(); pq.pop();
        ans.push_back(c);
        waitQ.push({count - 1, c});

        if ((int)waitQ.size() >= k) {
            auto [cnt, ch] = waitQ.front(); waitQ.pop();
            if (cnt > 0) pq.push({cnt, ch});
        }
    }
    return ans.size() == s.size() ? ans : "";
}
// Interview Explanation:
// - Problem Statement: Rearrange string such that identical characters are at least distance k apart.
// - Approach: Max-Heap + Cooldown Wait-Queue of Size k.
// - Intuition:
//   * Always prioritize scheduling the most frequent characters first.
//   * When a character is placed, it cannot be reused for the next (k - 1) slots.
//   * Buffer used characters in a cooldown queue of size k.
//   * When the cooldown queue reaches size k, release its front element back into the priority queue if occurrences remain.
//   * If the priority queue empties before all characters are placed, valid placement is impossible.
// - Complexity: Time: O(N log A), Space: O(A).


// =========================================================
// 42. MINIMUM ADD TO MAKE PARENTHESES VALID [G-42]
// =========================================================

int minAddToMakeValid(string s) {
    int open = 0, add = 0;
    for (char c : s) {
        if (c == '(') open++;
        else if (open > 0) open--;
        else add++;
    }
    return add + open;
}
// Interview Explanation:
// - Problem Statement: Find minimum parentheses added to make string valid.
// - Approach: Single-Pass Balance Tracking Counter.
// - Intuition:
//   * On '(': increment available open bracket balance open++.
//   * On ')': if open > 0, match with an available '(' (open--).
//   * If open == 0, the ')' is unmatched and requires adding an opening parenthesis (add++).
//   * At the end, any remaining unclosed '(' count open also requires matching closing brackets.
//   * Total insertions needed is add + open.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 43. MINIMUM REMOVE TO MAKE VALID PARENTHESES [G-43]
// =========================================================

string minRemoveToMakeValid(string s) {
    int open = 0;
    string tmp = "";
    for (char c : s) {
        if (c == '(') open++, tmp += c;
        else if (c == ')') {
            if (open > 0) open--, tmp += c;
        } else tmp += c;
    }
    string ans = "";
    for (int i = (int)tmp.size() - 1; i >= 0; --i) {
        if (tmp[i] == '(' && open > 0) open--;
        else ans += tmp[i];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Remove minimum parentheses to make resulting parentheses valid.
// - Approach: Two-Pass Invalidation Filter with Index Stack.
// - Intuition:
//   * A ')' is invalid if encountered when no unmatched '(' exists; mark its index for removal.
//   * Push indices of '(' onto stack; pop on matching ')'.
//   * Any '(' remaining on stack after full traversal has no matching ')' and must be removed.
//   * Filter out marked indices in a second pass.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 44. VALID PARENTHESIS STRING [G-44]
// =========================================================

bool checkValidString(string s) {
    int lo = 0, hi = 0;
    for (char c : s) {
        lo += (c == '(') ? 1 : -1;
        hi += (c != ')') ? 1 : -1;
        if (hi < 0) return false;
        lo = max(lo, 0);
    }
    return lo == 0;
}
// Interview Explanation:
// - Problem Statement: Validate parenthesis string containing '(', ')', and '*' (which can act as '(', ')', or empty).
// - Approach: Dynamic Open-Parenthesis Range Tracking [lo, hi].
// - Intuition:
//   * Maintain bounds [lo, hi] representing the minimum and maximum possible count of open '(' brackets.
//   * On '(': both lo and hi increment.
//   * On ')': both lo and hi decrement.
//   * On '*': treating '*' as ')' decreases lo, treating as '(' increases hi.
//   * If hi < 0, even converting every '*' to '(' cannot match excess ')'; return false.
//   * Clamping lo = max(lo, 0) handles cases where '*' acts as empty rather than invalid negative open brackets.
//   * Valid iff 0 falls within the final range (lo == 0).
// - Complexity: Time: O(N), Space: O(1).

