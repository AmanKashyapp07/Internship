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
 | 1  | Assign Cookies                              | Two Pointers on Sorted Arrays     | O(N logN)| O(1)     |
 | 2  | Best Time to Buy and Sell Stock II          | Greedy Valley-to-Peak Accumulation| O(N)     | O(1)     |
 | 3  | Jump Game                                   | Greedy Farthest Reachable Index   | O(N)     | O(1)     |
 | 4  | Jump Game II                                | Implicit BFS Window Extension      | O(N)     | O(1)     |
 | 5  | Gas Station                                 | Greedy Net Balance Reset          | O(N)     | O(1)     |
 | 6  | Candy                                       | Two-Pass Left & Right Sweeps      | O(N)     | O(N)     |
 | 7  | Lemonade Change                             | Greedy Change Denomination Cache  | O(N)     | O(1)     |
 | 8  | Can Place Flowers                           | Adjacent Zero Boundary Check      | O(N)     | O(1)     |
 | 9  | Maximum Units on a Truck                    | Value Density Sorting + Greedy Fill| O(N logN)| O(1)     |
 | 10 | Maximum 69 Number                           | Most Significant Digit Flip       | O(D)     | O(D)     |
 | 11 | Minimum Sum of Four Digit Number            | Ascending Digit Interleaving      | O(1)     | O(1)     |
 | 12 | Largest Odd Number in String                | Reverse Scan Suffix Truncation    | O(N)     | O(1)     |
 | 13 | Min Partitions Deci-Binary Numbers          | Maximum Character Digit Reduction | O(N)     | O(1)     |
 | 14 | Max Product Difference Between Two Pairs    | Single-Pass 2-Extremes Tracking    | O(N)     | O(1)     |
 | 15 | Buy Two Chocolates                          | Two Smallest Values Accumulation  | O(N)     | O(1)     |
 | 16 | Merge Intervals                             | Interval Sorting + Greedy Merge   | O(N logN)| O(1)     |
 | 17 | Insert Interval                             | 3-Stage Linear Sweep              | O(N)     | O(1)     |
 | 18 | Non-overlapping Intervals                   | Interval Scheduling by End Time   | O(N logN)| O(1)     |
 | 19 | Meeting Rooms II                            | Chronological Event Line-Sweep    | O(N logN)| O(N)     |
 | 20 | Meeting Rooms                               | Interval Start-End Overlap Check  | O(N logN)| O(1)     |
 ====================================================================================================
*/

// =========================================================
// 1. ASSIGN COOKIES
// =========================================================

int findContentChildren(vector<int>& g, vector<int>& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());

    int i = 0, j = 0;
    int n = g.size(), m = s.size();

    while (i < n && j < m) {
        if (s[j] >= g[i]) {
            i++;
        }
        j++;
    }
    return i;
}
// Interview Explanation:
// - Problem Statement: Maximize number of children satisfied with cookies where child i needs cookie >= g[i].
// - Approach: Sort both arrays and apply two pointers greedily.
// - Intuition:
//   * A larger cookie can satisfy a child with smaller greed, but wasting large cookies hurts future optimal assignments.
//   * Sort children greed factors and cookie sizes in ascending order.
//   * Greedily match the smallest available cookie that can satisfy the child with the smallest greed factor.
//   * If cookie s[j] satisfies child g[i], advance both child and cookie pointers; otherwise, discard cookie s[j].
// - Complexity: Time: O(N log N + M log M), Space: O(1) auxiliary space.


// =========================================================
// 2. BEST TIME TO BUY AND SELL STOCK II
// =========================================================

int maxProfit(vector<int>& p) {
    int ans = 0;
    for (int i = 1; i < (int)p.size(); ++i) {
        if (p[i] > p[i - 1]) {
            ans += p[i] - p[i - 1];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum total profit from buying and selling stock multiple times with at most 1 share held at once.
// - Approach: Greedy Valley-to-Peak Accumulation.
// - Intuition:
//   * Any multi-day price increase (p[k] - p[i]) decomposes into consecutive single-day increases: (p[i+1]-p[i]) + ... + (p[k]-p[k-1]).
//   * Capturing every positive single-day slope guarantees collecting the maximum profit across all upward trends.
//   * Skip negative slopes entirely by holding no position across declining days.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 3. JUMP GAME
// =========================================================

bool canJump(vector<int>& a) {
    int n = a.size();
    int mx = 0;

    for (int i = 0; i < n; ++i) {
        if (i > mx) return false;
        mx = max(mx, i + a[i]);
        if (mx >= n - 1) return true; // if the farthest reachable index is beyond or at the last index, we can reach the end
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if index n - 1 is reachable from index 0 where a[i] is max jump length.
// - Approach: Greedy Farthest Reachable Index Tracking.
// - Intuition:
//   * At any index i, index i is reachable if and only if i <= mx (the farthest reach discovered so far).
//   * If i > mx, we hit an impassable barrier; return false immediately.
//   * Update farthest reachable boundary: mx = max(mx, i + a[i]).
//   * If mx extends to or beyond n - 1, destination is reachable; return true early.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 4. JUMP GAME II
// =========================================================

int jump(vector<int>& a) {
    int n = a.size();
    if (n <= 1) return 0;

    int jumps = 0, curEnd = 0, curFarthest = 0;

    for (int i = 0; i < n - 1; ++i) {
        curFarthest = max(curFarthest, i + a[i]);
        if (i == curEnd) {
            jumps++;
            curEnd = curFarthest;
            if (curEnd >= n - 1) break;
        }
    }
    return jumps;
}
// Interview Explanation:
// - Problem Statement: Return minimum number of jumps required to reach index n - 1.
// - Approach: Implicit BFS Level Window Extension.
// - Intuition:
//   * Each jump transition expands the current reach from [prevEnd, curEnd] to curFarthest.
//   * Iterate up to n - 2, continuously updating the maximum index reachable from any node in the current window.
//   * When the index pointer reaches curEnd, a jump must be taken; set curEnd = curFarthest and increment jumps.
//   * Terminates in a single linear sweep without constructing an explicit BFS queue.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 5. GAS STATION
// =========================================================

int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
    int n = gas.size();
    int tot = 0, cur = 0, start = 0;

    for (int i = 0; i < n; ++i) {
        int diff = gas[i] - cost[i];
        tot += diff;
        cur += diff;
        if (cur < 0) {
            start = i + 1;
            cur = 0;
        }
    }
    return (tot >= 0 ? start : -1);
}
// Interview Explanation:
// - Problem Statement: Find starting gas station index to complete a circular circuit clockwise once.
// - Approach: Net Balance Invariant with Greedy Failure Reset.
// - Intuition:
//   * If total gas sum is strictly less than total cost sum, completion is mathematically impossible; return -1.
//   * If starting from station A, running fuel balance drops below zero at station B, no station between A and B can reach B either.
//   * Reset candidate start station greedily to B + 1 and reset current fuel tank to 0.
//   * When tot >= 0, the remaining candidate start index is uniquely guaranteed to complete the circuit.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 6. CANDY
// =========================================================

int candy(vector<int>& r) {
    int n = r.size();
    vector<int> c(n, 1);

    for (int i = 1; i < n; ++i) {
        if (r[i] > r[i - 1]) {
            c[i] = c[i - 1] + 1;
        }
    }

    for (int i = n - 2; i >= 0; --i) {
        if (r[i] > r[i + 1]) {
            c[i] = max(c[i], c[i + 1] + 1);
        }
    }

    return accumulate(c.begin(), c.end(), 0);
}
// Interview Explanation:
// - Problem Statement: Distribute minimum candies such that each child has at least 1, and higher-rated children get more than neighbors.
// - Approach: Two-Pass Greedy Left & Right Sweeps.
// - Intuition:
//   * Decompose two-sided neighbor constraints into two independent directional passes.
//   * Pass 1 (Left to Right): If rating r[i] > r[i-1], child i gets c[i-1] + 1 candies; ensures left neighbor constraint.
//   * Pass 2 (Right to Left): If rating r[i] > r[i+1], child i needs at least c[i+1] + 1 candies; take max(c[i], c[i+1] + 1).
//   * Taking the pointwise maximum satisfies both left and right constraints simultaneously with minimal total candies.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 7. LEMONADE CHANGE
// =========================================================

bool lemonadeChange(vector<int>& bills) {
    int five = 0, ten = 0;

    for (int b : bills) {
        if (b == 5) {
            five++;
        } else if (b == 10) {
            if (five == 0) return false;
            five--;
            ten++;
        } else {
            // Bill is 20: prefer giving 10 + 5 over 5 + 5 + 5
            if (ten > 0 && five > 0) {
                ten--;
                five--;
            } else if (five >= 3) {
                five -= 3;
            } else {
                return false;
            }
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if correct change can be provided to every customer paying with $5, $10, or $20 bills.
// - Approach: Greedy Cash Register Balance Tracking.
// - Intuition:
//   * $5 bills are strictly more versatile than $10 bills because $5 can serve both $10 and $20 customers.
//   * On receiving $10: must give one $5 bill.
//   * On receiving $20: change needed is $15. Greedily prioritize giving (1x $10 + 1x $5) over (3x $5) to preserve precious $5 bills.
//   * If required denominations are unavailable at any transaction, return false immediately.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 8. CAN PLACE FLOWERS
// =========================================================

bool canPlaceFlowers(vector<int>& fb, int k) {
    int n = fb.size();
    for (int i = 0; i < n && k > 0; ++i) {
        if (fb[i] == 0) {
            bool emptyLeft  = (i == 0 || fb[i - 1] == 0);
            bool emptyRight = (i == n - 1 || fb[i + 1] == 0);
            if (emptyLeft && emptyRight) {
                fb[i] = 1;
                k--;
            }
        }
    }
    return k <= 0;
}
// Interview Explanation:
// - Problem Statement: Determine if k new flowers can be planted without violating no-adjacent-flowers rule.
// - Approach: Greedy Linear Scan with Virtual Boundary Checks.
// - Intuition:
//   * Planting as early as possible never worsens subsequent planting opportunities.
//   * A plot at index i can host a flower iff plot i is empty, and both neighbor plots (i-1 and i+1) are empty.
//   * Array boundaries are treated as empty plots: index 0 needs only right plot empty, and index n-1 needs only left plot empty.
//   * Greedily plant upon finding a valid plot and decrement k until k reaches 0.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 9. MAXIMUM UNITS ON A TRUCK
// =========================================================

bool compareBoxTypes(const vector<int>& a, const vector<int>& b) {
    return a[1] > b[1];
}

int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
    sort(boxTypes.begin(), boxTypes.end(), compareBoxTypes);

    int ans = 0;
    for (const auto& box : boxTypes) {
        int take = min(box[0], truckSize);
        ans += take * box[1];
        truckSize -= take;
        if (truckSize == 0) break;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Maximize units loaded onto a truck with capacity truckSize boxes.
// - Approach: Greedy Fractional Knapsack by Unit Density.
// - Intuition:
//   * Each box takes identical capacity (1 unit of truck space).
//   * Greedily prioritize boxes providing the highest units-per-box value.
//   * Sort boxTypes in descending order of units per box.
//   * Take min(available_boxes, remaining_truck_capacity) for each box type until the truck is full.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 10. MAXIMUM 69 NUMBER
// =========================================================

int maximum69Number(int num) {
    string s = to_string(num);
    for (char& c : s) {
        if (c == '6') {
            c = '9';
            break;
        }
    }
    return stoi(s);
}
// Interview Explanation:
// - Problem Statement: Maximize number composed of digits 6 and 9 by flipping at most one digit from 6 to 9.
// - Approach: Greedy Most-Significant-Digit Modification.
// - Intuition:
//   * Flipping a digit at power 10^k contributes an increase of 3 * 10^k to the total value.
//   * Larger powers of 10 dominate all smaller powers combined (3000 > 300 + 30 + 3).
//   * Greedily flip the leftmost (most significant) occurrence of digit '6' to '9'.
// - Complexity: Time: O(D) where D is number of digits (<= 5), Space: O(D).


// =========================================================
// 11. MINIMUM SUM OF FOUR DIGIT NUMBER AFTER SPLITTING DIGITS
// =========================================================

int minimumSum(int num) {
    vector<int> d;
    while (num > 0) {
        d.push_back(num % 10);
        num /= 10;
    }
    sort(d.begin(), d.end());

    // Pair smallest two digits as tens digits
    return (d[0] * 10 + d[2]) + (d[1] * 10 + d[3]);
}
// Interview Explanation:
// - Problem Statement: Split 4-digit number into two numbers new1 and new2 minimizing their sum.
// - Approach: Greedy Ascending Digit Interleaving.
// - Intuition:
//   * Minimizing the sum requires placing the smallest available digits into the most significant (tens) places.
//   * Sort the 4 digits: d[0] <= d[1] <= d[2] <= d[3].
//   * Assign d[0] and d[1] as the tens digits: new1 = d[0]*10 + d[2], new2 = d[1]*10 + d[3].
//   * Total sum equals 10 * (d[0] + d[1]) + (d[2] + d[3]).
// - Complexity: Time: O(1), Space: O(1).


// =========================================================
// 12. LARGEST ODD NUMBER IN STRING
// =========================================================

string largestOddNumber(string s) {
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        if ((s[i] - '0') % 2 != 0) {
            return s.substr(0, i + 1);
        }
    }
    return "";
}
// Interview Explanation:
// - Problem Statement: Find the largest-valued odd integer substring from a large numeric string.
// - Approach: Reverse Scan for Last Odd Digit.
// - Intuition:
//   * Any integer's parity is entirely dictated by its final digit.
//   * A longer prefix starting at index 0 produces an exponentially larger numerical value.
//   * Scan backwards from the rightmost character until encountering the first odd digit.
//   * Prefix s[0..i] forms the largest possible odd integer substring.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 13. PARTITIONING INTO MINIMUM NUMBER OF DECI-BINARY NUMBERS
// =========================================================

int minPartitions(string s) {
    char mx = '0';
    for (char c : s) {
        mx = max(mx, c);
    }
    return mx - '0';
}
// Interview Explanation:
// - Problem Statement: Find minimum number of positive deci-binary numbers (digits only 0 or 1) that sum to s.
// - Approach: Maximum Digit Value Reduction.
// - Intuition:
//   * A deci-binary number contributes at most 1 to any decimal column per term.
//   * To form a column with digit d, we must sum at least d individual 1s in that column.
//   * The bottleneck column is the one with the maximum decimal digit across the entire string.
//   * Hence, exactly max(s[i] - '0') deci-binary numbers are necessary and sufficient.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 14. MAXIMUM PRODUCT DIFFERENCE BETWEEN TWO PAIRS
// =========================================================

int maxProductDifference(vector<int>& a) {
    int mx1 = INT_MIN, mx2 = INT_MIN;
    int mn1 = INT_MAX, mn2 = INT_MAX;

    for (int x : a) {
        if (x > mx1) {
            mx2 = mx1;
            mx1 = x;
        } else if (x > mx2) {
            mx2 = x;
        }

        if (x < mn1) {
            mn2 = mn1;
            mn1 = x;
        } else if (x < mn2) {
            mn2 = x;
        }
    }

    return (mx1 * mx2) - (mn1 * mn2);
}
// Interview Explanation:
// - Problem Statement: Maximize (a[w] * a[x]) - (a[y] * a[z]) using four distinct indices.
// - Approach: Single-Pass Extremes Tracking.
// - Intuition:
//   * To maximize product difference, maximize the positive minuend and minimize the subtrahend.
//   * Since all values are positive, the two largest numbers yield the maximum product.
//   * The two smallest numbers yield the minimum product.
//   * Maintain the two largest and two smallest elements in a single O(N) pass without sorting.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 15. BUY TWO CHOCOLATES
// =========================================================

int buyChoco(vector<int>& p, int money) {
    int mn1 = INT_MAX, mn2 = INT_MAX;

    for (int x : p) {
        if (x < mn1) {
            mn2 = mn1;
            mn1 = x;
        } else if (x < mn2) {
            mn2 = x;
        }
    }

    int cost = mn1 + mn2;
    return (cost <= money ? money - cost : money);
}
// Interview Explanation:
// - Problem Statement: Buy exactly two chocolates minimizing remaining debt; return leftover money or original money if unaffordable.
// - Approach: Greedy Two Smallest Values Tracking.
// - Intuition:
//   * To minimize cost and avoid non-negative balance, select the two cheapest available chocolates.
//   * Single pass tracks the first and second minimum prices.
//   * If mn1 + mn2 <= money, spend money - (mn1 + mn2); otherwise return money unchanged.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 16. MERGE INTERVALS
// =========================================================

vector<vector<int>> merge(vector<vector<int>>& a) {
    if (a.empty()) return {};
    sort(a.begin(), a.end());

    vector<vector<int>> ans;
    ans.push_back(a[0]);

    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] <= ans.back()[1]) {
            ans.back()[1] = max(ans.back()[1], a[i][1]);
        } else {
            ans.push_back(a[i]);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals.
// - Approach: Interval Sorting + Greedy Sequential Merge.
// - Intuition:
//   * Sort intervals by start time. Overlapping intervals become contiguous in sorted order.
//   * If current interval starts before or at the end of the previous merged interval (a[i][0] <= ans.back()[1]), they overlap.
//   * Extend merged end: ans.back()[1] = max(ans.back()[1], a[i][1]).
//   * Otherwise, push current interval as a new disjoint entry in ans.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 17. INSERT INTERVAL
// =========================================================

vector<vector<int>> insert(vector<vector<int>>& a, vector<int>& nw) {
    vector<vector<int>> ans;
    int i = 0, n = a.size();

    // 1. Add all intervals ending before nw starts
    while (i < n && a[i][1] < nw[0]) {
        ans.push_back(a[i++]);
    }

    // 2. Merge all overlapping intervals with nw
    while (i < n && a[i][0] <= nw[1]) {
        nw[0] = min(nw[0], a[i][0]);
        nw[1] = max(nw[1], a[i][1]);
        i++;
    }
    ans.push_back(nw);

    // 3. Add all remaining intervals
    while (i < n) {
        ans.push_back(a[i++]);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into sorted non-overlapping intervals, merging if necessary.
// - Approach: 3-Stage Linear Sweep without re-sorting.
// - Intuition:
//   * Stage 1: Collect all intervals strictly preceding nw (a[i][1] < nw[0]).
//   * Stage 2: Merge overlapping intervals (a[i][0] <= nw[1]) by updating nw[0] = min and nw[1] = max.
//   * Push the merged interval nw.
//   * Stage 3: Collect all intervals strictly following nw.
//   * Achieves O(N) time by exploiting existing sorted order.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 18. NON-OVERLAPPING INTERVALS
// =========================================================

bool compareIntervalEnds(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1];
}

int eraseOverlapIntervals(vector<vector<int>>& a) {
    if (a.empty()) return 0;
    sort(a.begin(), a.end(), compareIntervalEnds);

    int kept = 1;
    int prevEnd = a[0][1];

    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] >= prevEnd) {
            kept++;
            prevEnd = a[i][1];
        }
    }
    return (int)a.size() - kept;
}
// Interview Explanation:
// - Problem Statement: Find minimum intervals to remove to make remaining intervals non-overlapping.
// - Approach: Greedy Interval Scheduling by Earliest End Time.
// - Intuition:
//   * Minimizing removals is equivalent to maximizing the count of mutually compatible intervals.
//   * Classic activity selection: sort intervals by end time in ascending order.
//   * Greedily retain the interval ending earliest to leave maximal space for subsequent intervals.
//   * If next interval starts after or at prevEnd, keep it and advance prevEnd.
//   * Result is total_intervals - kept.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 19. MEETING ROOMS II
// =========================================================
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;

        sort(intervals.begin(), intervals.end()); // sort by start time, then by end time

        priority_queue<int, vector<int>, greater<int>> pq;

        for (auto& meeting : intervals) {
            // Reuse the room whose meeting ends earliest
            if (!pq.empty() && pq.top() <= meeting[0])
                pq.pop();

            // Assign current meeting to a room
            pq.push(meeting[1]);
        }

        return pq.size();
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum meeting rooms required to accommodate all intervals.
// - Approach: Chronological Two-Pointer Sweep on Separate Starts and Ends.
// - Intuition:
//   * A room is needed whenever a meeting starts before the earliest finishing meeting concludes.
//   * Decouple start and end timestamps into two sorted arrays.
//   * When starts[i] < ends[j], a new meeting begins before any existing room empties; increment rooms and advance i.
//   * When starts[i] >= ends[j], an active meeting ends; decrement rooms and advance j.
//   * The maximum concurrency across time gives the minimum rooms required.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 20. MEETING ROOMS
// =========================================================

bool canAttendMeetings(vector<vector<int>>& a) {
    sort(a.begin(), a.end());
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] < a[i - 1][1]) {
            return false;
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if person can attend all meetings without overlap.
// - Approach: Sort by Start Time and Check Adjacencies.
// - Intuition:
//   * Sort meetings in chronological order by start time.
//   * If any meeting starts strictly before the previous meeting finishes (a[i][0] < a[i-1][1]), a conflict exists.
//   * If no adjacent overlaps occur throughout the sorted sequence, all meetings can be attended.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.
