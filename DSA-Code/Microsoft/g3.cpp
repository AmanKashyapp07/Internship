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
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 41 | Minimum Cost to Hire K Workers              | Greedy Ratio Sort + Max-Heap Qual | O(N logN)| O(N + K) |
 | 42 | Maximum Number of Events Attended           | Day Sweep + Min-Heap End Day      | O(N logN)| O(N)     |
 | 43 | Meeting Rooms III                           | Dual Priority Queue Scheduling    | O(M logM)| O(N)     |
 | 44 | Maximum Profit in Job Scheduling            | End-Time Sort + DP + Binary Search| O(N logN)| O(N)     |
 | 45 | Minimum Difficulty of a Job Schedule        | Dynamic Programming (Day Chunks)  | O(D * N^2| O(D * N) |
 | 46 | Boats to Save People                        | Two Pointers Greedy Pairing       | O(N logN)| O(1)     |
 | 47 | Queue Reconstruction by Height              | Descending Height List Insertion  | O(N^2)   | O(N)     |
 | 48 | Advantage Shuffle                           | Greedy Sorted Tian Ji Strategy    | O(N logN)| O(N)     |
 | 49 | Bag of Tokens                               | Two Pointers Face-Up/Down Greed   | O(N logN)| O(1)     |
 | 50 | Reduce Array Size to The Half               | Frequency Map + Greedy Sort       | O(N logN)| O(N)     |
 | 51 | Minimum Increment to Make Array Unique      | In-Place Running Floor Propagation| O(N logN)| O(1)     |
 | 52 | Min Moves Make Array Complementary          | Difference Array Event Sweep      | O(N + L) | O(L)     |
 | 53 | Divide Array Into Arrays With Max Difference| Triplet Contiguous Window Check   | O(N logN)| O(N)     |
 | 54 | Maximum Ice Cream Bars                      | Greedy Price-Ascending Purchase   | O(N logN)| O(1)     |
 | 55 | Put Boxes Into the Warehouse I              | Preprocessed Ceiling + Greedy Fill| O(N logN)| O(1)     |
 | 56 | Put Boxes Into the Warehouse II             | Two-Ended Inward Warehouse Pointers| O(N logN)| O(1)    |
 | 57 | Maximum Bags With Full Capacity of Rocks    | Remaining Deficit Sorting         | O(N logN)| O(N)     |
 | 58 | Max Element Decreasing & Rearranging        | In-Place Neighbor Gap Clamping    | O(N logN)| O(1)     |
 | 59 | Min Difference Highest & Lowest of K Scores | Fixed-Size Sliding Window Sort    | O(N logN)| O(1)     |
 | 60 | Maximum Number of Coins You Can Get         | Sorted Stride-2 Second-Largest Sum| O(N logN)| O(1)     |
 ====================================================================================================
*/

// =========================================================
// 41. MINIMUM COST TO HIRE K WORKERS
// =========================================================

struct Worker {
    double ratio;
    int qual;
};

bool compareWorkerRatio(const Worker& a, const Worker& b) {
    return a.ratio < b.ratio;
}

double mincostToHireWorkers(vector<int>& qual, vector<int>& wage, int k) {
    int n = qual.size();
    vector<Worker> w(n);
    for (int i = 0; i < n; ++i) {
        w[i] = {(double)wage[i] / qual[i], qual[i]};
    }
    sort(w.begin(), w.end(), compareWorkerRatio);

    priority_queue<int> pq; // max-heap of qualities
    int sumQ = 0;
    double ans = 1e18;

    for (const auto& worker : w) {
        pq.push(worker.qual);
        sumQ += worker.qual;

        if ((int)pq.size() > k) {
            sumQ -= pq.top();
            pq.pop();
        }

        if ((int)pq.size() == k) {
            ans = min(ans, sumQ * worker.ratio);
        }
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
// 42. MAXIMUM NUMBER OF EVENTS THAT CAN BE ATTENDED
// =========================================================

bool compareEventStart(const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
}

int maxEvents(vector<vector<int>>& events) {
    sort(events.begin(), events.end(), compareEventStart);

    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of end days
    int ans = 0, i = 0, n = events.size();

    for (int d = 1; d <= 100000; ++d) {
        while (i < n && events[i][0] <= d) {
            pq.push(events[i][1]);
            i++;
        }

        while (!pq.empty() && pq.top() < d) {
            pq.pop();
        }

        if (!pq.empty()) {
            pq.pop();
            ans++;
        }

        if (i >= n && pq.empty()) break;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Attend the maximum number of events where you can attend at most one event per day.
// - Approach: Day-by-Day Chronological Sweep with Min-Heap of End Days.
// - Intuition:
//   * On any day d, add all events that have started (events[i][0] <= d) into a min-heap tracking their closing day.
//   * Discard any events from the heap that have already expired (closing day < d).
//   * Greedily attend the event that closes earliest (pq.top()); this preserves future days for events with later deadlines.
// - Complexity: Time: O(N log N + D log N), Space: O(N).


// =========================================================
// 43. MEETING ROOMS III
// =========================================================

bool compareMeetingStart(const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
}

struct BusyRoomCompare {
    bool operator()(const pair<long long, int>& a, const pair<long long, int>& b) const {
        if (a.first != b.first) return a.first > b.first; // freeTime
        return a.second > b.second;                       // room index
    }
};

int mostBooked(int n, vector<vector<int>>& meetings) {
    sort(meetings.begin(), meetings.end(), compareMeetingStart);

    priority_queue<int, vector<int>, greater<int>> freeRooms;
    for (int i = 0; i < n; ++i) freeRooms.push(i);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, BusyRoomCompare> busyRooms;
    vector<int> cnt(n, 0);

    for (const auto& m : meetings) {
        long long start = m[0], end = m[1];
        long long dur = end - start;

        while (!busyRooms.empty() && busyRooms.top().first <= start) {
            freeRooms.push(busyRooms.top().second);
            busyRooms.pop();
        }

        if (!freeRooms.empty()) {
            int room = freeRooms.top();
            freeRooms.pop();
            cnt[room]++;
            busyRooms.push({end, room});
        } else {
            auto [freeTime, room] = busyRooms.top();
            busyRooms.pop();
            cnt[room]++;
            busyRooms.push({freeTime + dur, room});
        }
    }

    int bestRoom = 0;
    for (int i = 1; i < n; ++i) {
        if (cnt[i] > cnt[bestRoom]) {
            bestRoom = i;
        }
    }
    return bestRoom;
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
// 44. MAXIMUM PROFIT IN JOB SCHEDULING
// =========================================================

struct JobItem {
    int start, end, profit;
};

bool compareJobItemEnd(const JobItem& a, const JobItem& b) {
    return a.end < b.end;
}

int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
    int n = startTime.size();
    vector<JobItem> jobs(n);
    for (int i = 0; i < n; ++i) {
        jobs[i] = {startTime[i], endTime[i], profit[i]};
    }
    sort(jobs.begin(), jobs.end(), compareJobItemEnd);

    // dp[i] = {end_time, max_profit}
    vector<pair<int, int>> dp;
    dp.push_back({0, 0});

    for (const auto& j : jobs) {
        // Binary search for latest non-overlapping job
        int lo = 0, hi = (int)dp.size() - 1, best = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (dp[mid].first <= j.start) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        int curProfit = dp[best].second + j.profit;
        if (curProfit > dp.back().second) {
            dp.push_back({j.end, curProfit});
        }
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
// 45. MINIMUM DIFFICULTY OF A JOB SCHEDULE
// =========================================================

int minDifficulty(vector<int>& jobDifficulty, int d) {
    int n = jobDifficulty.size();
    if (n < d) return -1;

    vector<vector<int>> dp(d + 1, vector<int>(n + 1, 1e9));
    dp[0][0] = 0;

    for (int day = 1; day <= d; ++day) {
        for (int i = day; i <= n; ++i) {
            int maxDiff = 0;
            for (int j = i; j >= day; --j) {
                maxDiff = max(maxDiff, jobDifficulty[j - 1]);
                if (dp[day - 1][j - 1] != 1e9) {
                    dp[day][i] = min(dp[day][i], dp[day - 1][j - 1] + maxDiff);
                }
            }
        }
    }
    return dp[d][n];
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
// 46. BOATS TO SAVE PEOPLE
// =========================================================

int numRescueBoats(vector<int>& people, int limit) {
    sort(people.begin(), people.end());

    int l = 0, r = (int)people.size() - 1;
    int boats = 0;

    while (l <= r) {
        if (people[l] + people[r] <= limit) {
            l++;
        }
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
// 47. QUEUE RECONSTRUCTION BY HEIGHT
// =========================================================

bool compareQueuePeople(const vector<int>& a, const vector<int>& b) {
    if (a[0] != b[0]) return a[0] > b[0]; // height descending
    return a[1] < b[1];                   // k ascending
}

vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
    sort(people.begin(), people.end(), compareQueuePeople);

    vector<vector<int>> ans;
    for (const auto& p : people) {
        ans.insert(ans.begin() + p[1], p);
    }
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
// 48. ADVANTAGE SHUFFLE
// =========================================================

vector<int> advantageCount(vector<int>& a, vector<int>& b) {
    int n = a.size();
    sort(a.begin(), a.end());

    vector<pair<int, int>> sortedB(n);
    for (int i = 0; i < n; ++i) sortedB[i] = {b[i], i};
    sort(sortedB.begin(), sortedB.end());

    vector<int> ans(n);
    int lo = 0, hi = n - 1;

    for (int x : a) {
        if (x > sortedB[lo].first) {
            ans[sortedB[lo].second] = x;
            lo++;
        } else {
            ans[sortedB[hi].second] = x;
            hi--;
        }
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
// 49. BAG OF TOKENS
// =========================================================

int bagOfTokensScore(vector<int>& tokens, int power) {
    sort(tokens.begin(), tokens.end());

    int l = 0, r = (int)tokens.size() - 1;
    int score = 0, maxScore = 0;

    while (l <= r) {
        if (power >= tokens[l]) {
            power -= tokens[l++];
            score++;
            maxScore = max(maxScore, score);
        } else if (score > 0 && l < r) {
            power += tokens[r--];
            score--;
        } else {
            break;
        }
    }
    return maxScore;
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
// 50. REDUCE ARRAY SIZE TO THE HALF
// =========================================================

int minSetSize(vector<int>& arr) {
    unordered_map<int, int> freq;
    for (int x : arr) freq[x]++;

    vector<int> counts;
    for (const auto& [val, count] : freq) counts.push_back(count);
    sort(counts.rbegin(), counts.rend());

    int removed = 0, ans = 0;
    int half = arr.size() / 2;

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
// 51. MINIMUM INCREMENT TO MAKE ARRAY UNIQUE
// =========================================================

int minIncrementForUnique(vector<int>& a) {
    sort(a.begin(), a.end());

    int ans = 0;
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i] <= a[i - 1]) {
            int needed = a[i - 1] + 1;
            ans += needed - a[i];
            a[i] = needed;
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
// 52. MINIMUM NUMBER OF MOVES TO MAKE ARRAY COMPLEMENTARY
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
// 53. DIVIDE ARRAY INTO ARRAYS WITH MAX DIFFERENCE
// =========================================================

vector<vector<int>> divideArray(vector<int>& a, int k) {
    sort(a.begin(), a.end());
    vector<vector<int>> ans;

    for (int i = 0; i < (int)a.size(); i += 3) {
        if (a[i + 2] - a[i] > k) {
            return {};
        }
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
// 54. MAXIMUM ICE CREAM BARS
// =========================================================

int maxIceCream(vector<int>& costs, int coins) {
    sort(costs.begin(), costs.end());

    int ans = 0;
    for (int c : costs) {
        if (coins >= c) {
            coins -= c;
            ans++;
        } else {
            break;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Maximize number of ice cream bars bought with initial coins.
// - Approach: Greedy Price-Ascending Purchase.
// - Intuition:
//   * Each ice cream bar counts equally as 1 bar toward the objective.
//   * Buying cheaper bars leaves the maximal amount of coins to purchase subsequent bars.
//   * Sort prices ascending and purchase greedily until coins are depleted.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 55. PUT BOXES INTO THE WAREHOUSE I
// =========================================================

int maxBoxesInWarehouse(vector<int>& boxes, vector<int>& warehouse) {
    int m = warehouse.size();
    for (int i = 1; i < m; ++i) {
        warehouse[i] = min(warehouse[i], warehouse[i - 1]);
    }
    sort(boxes.begin(), boxes.end());

    int ans = 0;
    int bIdx = 0;

    for (int i = m - 1; i >= 0 && bIdx < (int)boxes.size(); --i) {
        if (boxes[bIdx] <= warehouse[i]) {
            ans++;
            bIdx++;
        }
    }
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
// 56. PUT BOXES INTO THE WAREHOUSE II
// =========================================================

int maxBoxesInWarehouse2(vector<int>& boxes, vector<int>& warehouse) {
    sort(boxes.rbegin(), boxes.rend());

    int l = 0, r = (int)warehouse.size() - 1;
    int ans = 0;

    for (int b : boxes) {
        if (l > r) break;
        if (warehouse[l] >= warehouse[r]) {
            if (warehouse[l] >= b) {
                ans++;
                l++;
            }
        } else {
            if (warehouse[r] >= b) {
                ans++;
                r--;
            }
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
// 57. MAXIMUM BAGS WITH FULL CAPACITY OF ROCKS
// =========================================================

int maximumBags(vector<int>& capacity, vector<int>& rocks, int additionalRocks) {
    int n = capacity.size();
    vector<int> diff(n);
    for (int i = 0; i < n; ++i) {
        diff[i] = capacity[i] - rocks[i];
    }
    sort(diff.begin(), diff.end());

    int ans = 0;
    for (int d : diff) {
        if (additionalRocks >= d) {
            additionalRocks -= d;
            ans++;
        } else {
            break;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum bags filled to capacity using additional rocks.
// - Approach: Remaining Deficit Sorting + Greedy Placement.
// - Intuition:
//   * Each bag needs diff[i] = capacity[i] - rocks[i] additional rocks to become full.
//   * Filling bags with smaller deficits maximizes total full bags for a given rock budget.
//   * Sort diff ascending and fill bags greedily.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 58. MAXIMUM ELEMENT AFTER DECREASING AND REARRANGING
// =========================================================

int maximumElementAfterDecrementingAndRearranging(vector<int>& a) {
    sort(a.begin(), a.end());
    a[0] = 1;

    for (int i = 1; i < (int)a.size(); ++i) {
        a[i] = min(a[i], a[i - 1] + 1);
    }
    return a.back();
}
// Interview Explanation:
// - Problem Statement: Rearrange and decrement elements so a[0] == 1 and adjacent differences <= 1, maximizing final value.
// - Approach: Sorting + In-Place Neighbor Gap Clamping.
// - Intuition:
//   * Sorting organizes numbers into optimal ascending order.
//   * Anchor a[0] = 1.
//   * For each subsequent element, it can increase by at most 1 over its predecessor: a[i] = min(a[i], a[i-1] + 1).
//   * Returning the last element gives the maximum possible terminal value.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 59. MINIMUM DIFFERENCE BETWEEN HIGHEST AND LOWEST OF K SCORES
// =========================================================

int minimumDifference(vector<int>& a, int k) {
    if (k <= 1) return 0;
    sort(a.begin(), a.end());

    int ans = INT_MAX;
    for (int i = 0; i + k - 1 < (int)a.size(); ++i) {
        ans = min(ans, a[i + k - 1] - a[i]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Choose k scores minimizing the difference between highest and lowest chosen scores.
// - Approach: Fixed-Size Sliding Window on Sorted Array.
// - Intuition:
//   * Any optimal subset of size k corresponds to a contiguous subsegment in the sorted array.
//   * Sort scores ascending.
//   * Slide a window of length k: diff = a[i + k - 1] - a[i].
//   * Track the minimum difference across all windows.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 60. MAXIMUM NUMBER OF COINS YOU CAN GET
// =========================================================

int maxCoins(vector<int>& piles) {
    sort(piles.begin(), piles.end());

    int n = piles.size();
    int rounds = n / 3;
    int ans = 0;

    for (int i = 0; i < rounds; ++i) {
        ans += piles[n - 2 - 2 * i];
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: In each step, pick 3 piles where Alice takes the largest, you take second largest, Bob takes smallest. Maximize your coins.
// - Approach: Sorted Stride-2 Second-Largest Accumulation.
// - Intuition:
//   * In each round of 3, Bob gets the absolute smallest piles available (the first n/3 elements).
//   * From the remaining 2n/3 elements, Alice takes the largest and you take the second largest.
//   * Sort ascending and pick every second element from the right: piles[n - 2 - 2*i].
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.
