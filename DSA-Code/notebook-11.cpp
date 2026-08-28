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

const ll MOD = 1e9 + 7;

// =========================================================
// 1. PASCAL'S TRIANGLE (LEETCODE 118)
// =========================================================

vvi generatePascalsTriangle(int numRows) {
    vvi triangle;
    for (int i = 0; i < numRows; ++i) {
        vi row(i + 1, 1);
        for (int j = 1; j < i; ++j) {
            row[j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
        triangle.push_back(row);
    }
    return triangle;
}

// Generate N-th row in O(N) time: nCr combinatorial step
vi getPascalsRow(int rowIndex) {
    vi row(rowIndex + 1, 1);
    ll val = 1;
    for (int col = 1; col <= rowIndex; ++col) {
        val = val * (rowIndex - col + 1) / col;
        row[col] = (int)val;
    }
    return row;
}
// Interview Explanation:
// - Problem Statement: Generate the first numRows of Pascal's triangle and any specific row in O(N) time.
// - Approach: Dynamic Programming level addition + Combinatorial nCr formula `val = val * (n - c + 1) / c`.
// - Intuition: Element at (r, c) equals nCr(r, c); multiplying by remaining terms and dividing eliminates factorial recomputation.
// - Complexity: Time: O(N^2) for full triangle, O(N) for single row, Space: O(1) auxiliary space beyond result.

// =========================================================
// 2. SORT COLORS / SORT ARRAY OF 0s, 1s, 2s (LEETCODE 75)
// =========================================================

void sortColors(vi &nums) {
    int low = 0, mid = 0, high = (int)nums.size() - 1;
    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low++], nums[mid++]);
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high--]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array with values 0, 1, and 2 in-place in a single pass without library sort.
// - Approach: Dutch National Flag algorithm with three pointers (low, mid, high).
// - Intuition: Invariant: [0...low-1] are 0s, [low...mid-1] are 1s, [mid...high] are unprocessed, [high+1...N-1] are 2s.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place auxiliary space.

// =========================================================
// 3. MERGE TWO SORTED ARRAYS IN O(1) EXTRA SPACE (GAP ALGORITHM)
// =========================================================

void mergeSortedArraysGap(vi &a, vi &b) {
    int n = a.size(), m = b.size();
    int gap = (n + m + 1) / 2;
    while (gap > 0) {
        int i = 0, j = gap;
        while (j < n + m) {
            // Case 1: both pointers in array a
            if (j < n) {
                if (a[i] > a[j]) swap(a[i], a[j]);
            }
            // Case 2: pointer i in a, pointer j in b
            else if (i < n && j >= n) {
                if (a[i] > b[j - n]) swap(a[i], b[j - n]);
            }
            // Case 3: both pointers in array b
            else {
                if (b[i - n] > b[j - n]) swap(b[i - n], b[j - n]);
            }
            i++;
            j++;
        }
        if (gap == 1) break;
        gap = (gap + 1) / 2;
    }
}
// Interview Explanation:
// - Problem Statement: Merge two sorted arrays `a` and `b` in-place such that elements in `a` are <= elements in `b`.
// - Approach: Shell sort's Gap Method ($O(1)$ space).
// - Intuition: Initial gap = ceil((N+M)/2); compare elements separated by gap across combined virtual array, halving gap until 1.
// - Complexity: Time: O((N + M) log(N + M)), Space: O(1) in-place extra memory.

// =========================================================
// 4. FIND THE DUPLICATE NUMBER IN N+1 INTEGERS (LEETCODE 287)
// =========================================================

int findDuplicate(const vi &nums) {
    int slow = nums[0];
    int fast = nums[0];

    // Phase 1: Detect cycle
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    // Phase 2: Find cycle entry (duplicate value)
    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}
// Interview Explanation:
// - Problem Statement: Given array of N+1 integers between 1 and N, find the duplicate number without modifying array in O(1) space.
// - Approach: Floyd's Tortoise and Hare Cycle Finding on Array Indices.
// - Intuition: Map index `i -> nums[i]`. A duplicate value has multiple incoming pointers, creating a directed functional graph cycle entry.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.

// =========================================================
// 5. MAJORITY ELEMENT-II (> N/3) (LEETCODE 229)
// =========================================================

vi majorityElementII(const vi &nums) {
    int cnt1 = 0, cnt2 = 0;
    int el1 = INT_MIN, el2 = INT_MIN;

    for (int x : nums) {
        if (cnt1 == 0 && el2 != x) {
            cnt1 = 1; el1 = x;
        } else if (cnt2 == 0 && el1 != x) {
            cnt2 = 1; el2 = x;
        } else if (x == el1) {
            cnt1++;
        } else if (x == el2) {
            cnt2++;
        } else {
            cnt1--;
            cnt2--;
        }
    }

    vi ans;
    cnt1 = 0; cnt2 = 0;
    for (int x : nums) {
        if (x == el1) cnt1++;
        else if (x == el2) cnt2++;
    }
    int n = nums.size();
    if (cnt1 > n / 3) ans.push_back(el1);
    if (cnt2 > n / 3) ans.push_back(el2);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all elements that appear more than floor(N / 3) times.
// - Approach: Extended Boyer-Moore Voting Algorithm tracking two candidates.
// - Intuition: At most two elements can appear > N/3 times. Cancelling triplet groups of distinct elements leaves dominant candidates.
// - Complexity: Time: O(N) two linear passes, Space: O(1) auxiliary space.

// =========================================================
// 6. COUNT SUBARRAYS WITH GIVEN XOR K
// =========================================================

ll countSubarraysWithXorK(const vi &nums, int k) {
    ll count = 0;
    int xr = 0;
    unordered_map<int, int> freq;
    freq[0] = 1;

    for (int x : nums) {
        xr ^= x;
        int target = xr ^ k;
        if (freq.find(target) != freq.end()) {
            count += freq[target];
        }
        freq[xr]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total number of subarrays having bitwise XOR equal to K.
// - Approach: Prefix XOR Frequency Map ($XR[i \dots j] = P[j] \oplus P[i-1]$).
// - Intuition: $P[j] \oplus P[i-1] = K \iff P[i-1] = P[j] \oplus K$. Looking up frequency of $P[j] \oplus K$ gives matching prefixes in O(1).
// - Complexity: Time: O(N) single pass with hash table, Space: O(N) for frequency map.

// =========================================================
// 7. FIND THE REPEATING AND MISSING NUMBER
// =========================================================

pii findRepeatingAndMissing(const vi &nums) {
    ll n = nums.size();
    ll S = 0, S2 = 0;
    ll SN = n * (n + 1) / 2;
    ll S2N = n * (n + 1) * (2 * n + 1) / 6;

    for (int x : nums) {
        S += x;
        S2 += (ll)x * (ll)x;
    }

    ll diff = S - SN;          // R - M
    ll sumSquaresDiff = S2 - S2N; // R^2 - M^2 = (R - M)(R + M)
    ll sumRM = sumSquaresDiff / diff; // R + M

    ll repeating = (diff + sumRM) / 2;
    ll missing = sumRM - repeating;

    return {(int)repeating, (int)missing};
}
// Interview Explanation:
// - Problem Statement: In an array of size N containing 1 to N, one number is missing and one is repeated. Find both in O(1) space.
// - Approach: Mathematical Equations on Sum and Sum of Squares.
// - Intuition: Compute $S - S_N = R - M$ and $S^2 - S_N^2 = R^2 - M^2$. Dividing gives $R + M$, allowing exact $O(1)$ space resolution.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary memory.

// =========================================================
// 8. REVERSE PAIRS (LEETCODE 493)
// =========================================================

int countAndMergeReversePairs(vi &nums, int low, int mid, int high) {
    int count = 0;
    int j = mid + 1;
    for (int i = low; i <= mid; ++i) {
        while (j <= high && (ll)nums[i] > 2LL * nums[j]) {
            j++;
        }
        count += (j - (mid + 1));
    }

    vi temp;
    int left = low, right = mid + 1;
    while (left <= mid && right <= high) {
        if (nums[left] <= nums[right]) temp.push_back(nums[left++]);
        else temp.push_back(nums[right++]);
    }
    while (left <= mid) temp.push_back(nums[left++]);
    while (right <= high) temp.push_back(nums[right++]);
    for (int i = low; i <= high; ++i) nums[i] = temp[i - low];

    return count;
}

int reversePairsHelper(vi &nums, int low, int high) {
    if (low >= high) return 0;
    int mid = low + (high - low) / 2;
    int count = reversePairsHelper(nums, low, mid) + reversePairsHelper(nums, mid + 1, high);
    count += countAndMergeReversePairs(nums, low, mid, high);
    return count;
}

int reversePairs(vi &nums) {
    return reversePairsHelper(nums, 0, (int)nums.size() - 1);
}
// Interview Explanation:
// - Problem Statement: Count number of reverse pairs $i < j$ such that $nums[i] > 2 \cdot nums[j]$.
// - Approach: Modified Merge Sort with linear counting before merge step.
// - Intuition: Both halves are sorted; for each element in left half, two-pointer scan counts elements in right half satisfying $nums[i] > 2 \cdot nums[j]$.
// - Complexity: Time: O(N log N), Space: O(N) auxiliary buffer for merging.

// =========================================================
// 9. 4-SUM (LEETCODE 18)
// =========================================================

vvi fourSum(vi &nums, int target) {
    vvi ans;
    int n = nums.size();
    sort(nums.begin(), nums.end());

    for (int i = 0; i < n; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1; j < n; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;

            int left = j + 1, right = n - 1;
            while (left < right) {
                ll sum = (ll)nums[i] + nums[j] + nums[left] + nums[right];
                if (sum == target) {
                    ans.push_back({nums[i], nums[j], nums[left], nums[right]});
                    left++; right--;
                    while (left < right && nums[left] == nums[left - 1]) left++;
                    while (left < right && nums[right] == nums[right + 1]) right--;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all unique quadruplets $[a, b, c, d]$ such that $a + b + c + d = target$.
// - Approach: Sorting + 2 Nested Loops + Two-Pointer Convergence with duplicate pruning.
// - Intuition: Fix first two indices $i, j$; apply sorted two-pointer sweep on remaining subarray; skip identical elements to prevent duplicates.
// - Complexity: Time: O(N^3), Space: O(1) auxiliary space.

// =========================================================
// 10. LARGEST SUBARRAY WITH 0 SUM
// =========================================================

int maxLenZeroSum(const vi &arr) {
    unordered_map<int, int> prefixIndex;
    int maxLen = 0, sum = 0;

    for (int i = 0; i < (int)arr.size(); ++i) {
        sum += arr[i];
        if (sum == 0) {
            maxLen = i + 1;
        } else if (prefixIndex.find(sum) != prefixIndex.end()) {
            maxLen = max(maxLen, i - prefixIndex[sum]);
        } else {
            prefixIndex[sum] = i; // record earliest occurrence
        }
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of the longest subarray with sum equal to 0.
// - Approach: Prefix Sum Hash Map storing earliest index of each prefix sum.
// - Intuition: If prefix sum $S$ repeats at index $j$ after index $i$, then subarray sum in $(i \dots j]$ is exactly 0.
// - Complexity: Time: O(N), Space: O(N) hash map memory.

// =========================================================
// 11. REMOVE DUPLICATES (LC 26) & MAX CONSECUTIVE ONES (LC 485)
// =========================================================

int removeDuplicatesSorted(vi &nums) {
    if (nums.empty()) return 0;
    int i = 0;
    for (int j = 1; j < (int)nums.size(); ++j) {
        if (nums[j] != nums[i]) {
            nums[++i] = nums[j];
        }
    }
    return i + 1;
}

int findMaxConsecutiveOnes(const vi &nums) {
    int maxCount = 0, currentCount = 0;
    for (int x : nums) {
        if (x == 1) {
            currentCount++;
            maxCount = max(maxCount, currentCount);
        } else {
            currentCount = 0;
        }
    }
    return maxCount;
}
// Interview Explanation:
// - Problem Statement: Remove duplicates in-place from sorted array, and find maximum consecutive 1s in binary array.
// - Approach: Two-Pointer slow/fast overwrite and single pass state resetting.
// - Complexity: Time: O(N), Space: O(1) in-place auxiliary memory.

// =========================================================
// 12. ROTATE MATRIX BY 90 DEGREES CLOCKWISE (LEETCODE 48)
// =========================================================

void rotateMatrix90(vvi &matrix) {
    int n = matrix.size();
    // Step 1: Transpose matrix
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    // Step 2: Reverse each row
    for (int i = 0; i < n; ++i) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}
// Interview Explanation:
// - Problem Statement: Rotate an N x N 2D matrix 90 degrees clockwise in-place.
// - Approach: Transpose + Horizontal Row Reversal.
// - Intuition: Transposing swaps rows and columns $(i, j) \leftrightarrow (j, i)$; reversing rows flips horizontally to produce 90° clockwise rotation.
// - Complexity: Time: O(N^2), Space: O(1) in-place.

// =========================================================
// 13. N MEETINGS IN ONE ROOM & MINIMUM PLATFORMS
// =========================================================

struct Meeting {
    int start, end, id;
};

int maxMeetings(vi &start, vi &end) {
    int n = start.size();
    vector<Meeting> meetings(n);
    for (int i = 0; i < n; ++i) meetings[i] = {start[i], end[i], i + 1};

    sort(meetings.begin(), meetings.end(), [](const Meeting &a, const Meeting &b) {
        return a.end < b.end;
    });

    int count = 1;
    int lastEnd = meetings[0].end;
    for (int i = 1; i < n; ++i) {
        if (meetings[i].start > lastEnd) {
            count++;
            lastEnd = meetings[i].end;
        }
    }
    return count;
}

int findMinPlatforms(vi &arr, vi &dep) {
    int n = arr.size();
    sort(arr.begin(), arr.end());
    sort(dep.begin(), dep.end());

    int platforms = 1, maxPlatforms = 1;
    int i = 1, j = 0;
    while (i < n && j < n) {
        if (arr[i] <= dep[j]) {
            platforms++;
            i++;
        } else {
            platforms--;
            j++;
        }
        maxPlatforms = max(maxPlatforms, platforms);
    }
    return maxPlatforms;
}
// Interview Explanation:
// - Problem Statement: Find maximum non-overlapping meetings, and minimum railway platforms required for scheduled trains.
// - Approach: Greedy Activity Selection by finish time + Chronological arrival/departure two-pointer scan.
// - Complexity: Time: O(N log N) sorting, Space: O(N) struct buffer.

// =========================================================
// 14. JOB SEQUENCING WITH DEADLINES & FRACTIONAL KNAPSACK
// =========================================================

struct Job {
    int id, dead, profit;
};

pii jobScheduling(vector<Job> &jobs) {
    sort(jobs.begin(), jobs.end(), [](const Job &a, const Job &b) {
        return a.profit > b.profit;
    });

    int maxDead = 0;
    for (auto &j : jobs) maxDead = max(maxDead, j.dead);

    vi slot(maxDead + 1, -1);
    int totalProfit = 0, countJobs = 0;

    for (auto &j : jobs) {
        for (int k = j.dead; k > 0; --k) {
            if (slot[k] == -1) {
                slot[k] = j.id;
                countJobs++;
                totalProfit += j.profit;
                break;
            }
        }
    }
    return {countJobs, totalProfit};
}

struct Item {
    int value, weight;
};

double fractionalKnapsack(int W, vector<Item> &items) {
    sort(items.begin(), items.end(), [](const Item &a, const Item &b) {
        double r1 = (double)a.value / a.weight;
        double r2 = (double)b.value / b.weight;
        return r1 > r2;
    });

    double totalValue = 0.0;
    int currentWeight = 0;

    for (auto &item : items) {
        if (currentWeight + item.weight <= W) {
            currentWeight += item.weight;
            totalValue += item.value;
        } else {
            int remain = W - currentWeight;
            totalValue += item.value * ((double)remain / item.weight);
            break;
        }
    }
    return totalValue;
}
// Interview Explanation:
// - Problem Statement: Maximize job profit under single-slot deadlines, and fractional knapsack maximum value.
// - Approach: Greedy sort by profit with latest-deadline slot placement + Value-to-weight ratio greedy selection.
// - Complexity: Time: O(N log N + N * maxDead) for jobs, O(N log N) for knapsack, Space: O(maxDead).

// =========================================================
// 15. SUBSET SUMS & SUBSETS II (LEETCODE 90)
// =========================================================

void subsetSumsHelper(int idx, int sum, const vi &arr, vi &ans) {
    if (idx == (int)arr.size()) {
        ans.push_back(sum);
        return;
    }
    subsetSumsHelper(idx + 1, sum + arr[idx], arr, ans); // pick
    subsetSumsHelper(idx + 1, sum, arr, ans);            // not pick
}

vi subsetSums(const vi &arr) {
    vi ans;
    subsetSumsHelper(0, 0, arr, ans);
    sort(ans.begin(), ans.end());
    return ans;
}

void subsetsWithDupHelper(int idx, vi &nums, vi &curr, vvi &ans) {
    ans.push_back(curr);
    for (int i = idx; i < (int)nums.size(); ++i) {
        if (i > idx && nums[i] == nums[i - 1]) continue; // skip duplicates at same tree level
        curr.push_back(nums[i]);
        subsetsWithDupHelper(i + 1, nums, curr, ans);
        curr.pop_back();
    }
}

vvi subsetsWithDup(vi &nums) {
    vvi ans;
    vi curr;
    sort(nums.begin(), nums.end());
    subsetsWithDupHelper(0, nums, curr, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate all subset sums in ascending order, and all unique subsets from array with duplicates.
// - Approach: Pick/Not-pick recursion and Level-wise duplicate skipping via `if (i > idx && nums[i] == nums[i-1]) continue`.
// - Complexity: Time: O(2^N), Space: O(N) recursion stack.

// =========================================================
// 16. COMBINATION SUM II (LC 40) & PALINDROME PARTITIONING (LC 131)
// =========================================================

void combSum2Helper(int idx, int target, vi &candidates, vi &curr, vvi &ans) {
    if (target == 0) {
        ans.push_back(curr);
        return;
    }
    for (int i = idx; i < (int)candidates.size(); ++i) {
        if (i > idx && candidates[i] == candidates[i - 1]) continue;
        if (candidates[i] > target) break;
        curr.push_back(candidates[i]);
        combSum2Helper(i + 1, target - candidates[i], candidates, curr, ans);
        curr.pop_back();
    }
}

vvi combinationSum2(vi &candidates, int target) {
    vvi ans;
    vi curr;
    sort(candidates.begin(), candidates.end());
    combSum2Helper(0, target, candidates, curr, ans);
    return ans;
}

bool isPalindromeStr(const string &s, int l, int r) {
    while (l < r) {
        if (s[l++] != s[r--]) return false;
    }
    return true;
}

void partitionHelper(int idx, const string &s, vector<string> &curr, vector<vector<string>> &ans) {
    if (idx == (int)s.size()) {
        ans.push_back(curr);
        return;
    }
    for (int i = idx; i < (int)s.size(); ++i) {
        if (isPalindromeStr(s, idx, i)) {
            curr.push_back(s.substr(idx, i - idx + 1));
            partitionHelper(i + 1, s, curr, ans);
            curr.pop_back();
        }
    }
}

vector<vector<string>> partitionPalindrome(string s) {
    vector<vector<string>> ans;
    vector<string> curr;
    partitionHelper(0, s, curr, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all unique combinations summing to target (each element used once), and partition string into palindrome substrings.
// - Approach: Backtracking with duplicate skipping + prefix palindrome substring validation.
// - Complexity: Time: O(2^N * N), Space: O(N) recursion stack.

// =========================================================
// 17. K-TH PERMUTATION (LC 60) & RAT IN A MAZE (GFG)
// =========================================================

string getPermutation(int n, int k) {
    int fact = 1;
    vi numbers;
    for (int i = 1; i < n; ++i) {
        fact *= i;
        numbers.push_back(i);
    }
    numbers.push_back(n);
    string ans = "";
    k = k - 1; // 0-indexed

    while (true) {
        ans += to_string(numbers[k / fact]);
        numbers.erase(numbers.begin() + (k / fact));
        if (numbers.empty()) break;
        k %= fact;
        fact /= numbers.size();
    }
    return ans;
}

void ratInMazeDFS(int r, int c, vvi &m, int n, string curr, vector<string> &ans, vvi &vis) {
    if (r == n - 1 && c == n - 1) {
        ans.push_back(curr);
        return;
    }
    string dir = "DLRU";
    int dr[] = {1, 0, 0, -1};
    int dc[] = {0, -1, 1, 0};

    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < n && nc >= 0 && nc < n && !vis[nr][nc] && m[nr][nc] == 1) {
            vis[r][c] = 1;
            ratInMazeDFS(nr, nc, m, n, curr + dir[i], ans, vis);
            vis[r][c] = 0;
        }
    }
}

vector<string> findPathRatInMaze(vvi &m, int n) {
    vector<string> ans;
    if (m[0][0] == 0 || m[n - 1][n - 1] == 0) return ans;
    vvi vis(n, vi(n, 0));
    ratInMazeDFS(0, 0, m, n, "", ans, vis);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find K-th permutation sequence in O(N^2) time without computing all permutations, and find all paths for Rat in a Maze.
// - Approach: Factorial bucket indexing $k / (n-1)!$ + 4-directional Backtracking DFS.
// - Complexity: Time: O(N^2) for permutation, O(4^(N^2)) for maze, Space: O(N) recursion stack.

// =========================================================
// 18. N-TH ROOT OF AN INTEGER & MATRIX MEDIAN
// =========================================================

int nthRoot(int n, int m) {
    int low = 1, high = m;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        ll val = 1;
        for (int i = 0; i < n; ++i) {
            val *= mid;
            if (val > m) break;
        }
        if (val == m) return mid;
        if (val < m) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int countSmallerEqualInMatrixRow(const vi &row, int x) {
    return upper_bound(row.begin(), row.end(), x) - row.begin();
}

int matrixMedian(const vvi &matrix) {
    int r = matrix.size(), c = matrix[0].size();
    int low = 1, high = 1e9;
    int required = (r * c) / 2;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int count = 0;
        for (int i = 0; i < r; ++i) {
            count += countSmallerEqualInMatrixRow(matrix[i], mid);
        }
        if (count <= required) low = mid + 1;
        else high = mid - 1;
    }
    return low;
}
// Interview Explanation:
// - Problem Statement: Find integer N-th root of M, and find median of row-wise sorted matrix in O(R * log C * log(MAX)).
// - Approach: Binary search on answer space + Binary search (`upper_bound`) across matrix rows.
// - Complexity: Time: O(log M) for Nth root, O(32 * R log C) for matrix median, Space: O(1).

// =========================================================
// 19. SINGLE ELEMENT IN SORTED ARRAY & K-TH OF 2 SORTED ARRAYS
// =========================================================

int singleNonDuplicate(const vi &nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    if (nums[0] != nums[1]) return nums[0];
    if (nums[n - 1] != nums[n - 2]) return nums[n - 1];

    int low = 1, high = n - 2;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] != nums[mid - 1] && nums[mid] != nums[mid + 1]) {
            return nums[mid];
        }
        // Even-Odd index parity check
        if ((mid % 2 == 1 && nums[mid] == nums[mid - 1]) ||
            (mid % 2 == 0 && nums[mid] == nums[mid + 1])) {
            low = mid + 1; // single element lies on right half
        } else {
            high = mid - 1; // single element lies on left half
        }
    }
    return -1;
}

int kthElementTwoSortedArrays(const vi &a, const vi &b, int k) {
    if (a.size() > b.size()) return kthElementTwoSortedArrays(b, a, k);
    int n = a.size(), m = b.size();
    int low = max(0, k - m), high = min(k, n);

    while (low <= high) {
        int cut1 = low + (high - low) / 2;
        int cut2 = k - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : a[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : b[cut2 - 1];
        int r1 = (cut1 == n) ? INT_MAX : a[cut1];
        int r2 = (cut2 == m) ? INT_MAX : b[cut2];

        if (l1 <= r2 && l2 <= r1) return max(l1, l2);
        if (l1 > r2) high = cut1 - 1;
        else low = cut1 + 1;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find single non-duplicate in sorted array in O(log N), and find k-th element of two sorted arrays in O(log(min(N, M))).
// - Approach: Index Parity Binary Search + Partition Cut Binary Search on smaller array.
// - Complexity: Time: O(log N) & O(log(min(N, M))), Space: O(1) auxiliary space.

// =========================================================
// 20. STACK VIA QUEUE, QUEUE VIA STACK & NEXT SMALLER ELEMENT
// =========================================================

class MyStackSingleQueue {
    queue<int> q;
public:
    void push(int x) {
        q.push(x);
        for (int i = 0; i < (int)q.size() - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }
    int pop() {
        int val = q.front(); q.pop(); return val;
    }
    int top() { return q.front(); }
    bool empty() { return q.empty(); }
};

class MyQueueTwoStacks {
    stack<int> s1, s2;
public:
    void push(int x) { s1.push(x); }
    int pop() {
        peek();
        int val = s2.top(); s2.pop();
        return val;
    }
    int peek() {
        if (s2.empty()) {
            while (!s1.empty()) {
                s2.push(s1.top());
                s1.pop();
            }
        }
        return s2.top();
    }
    bool empty() { return s1.empty() && s2.empty(); }
};

vi nextSmallerElement(const vi &arr) {
    int n = arr.size();
    vi nse(n, -1);
    stack<int> st;

    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && st.top() >= arr[i]) {
            st.pop();
        }
        if (!st.empty()) nse[i] = st.top();
        st.push(arr[i]);
    }
    return nse;
}
// Interview Explanation:
// - Problem Statement: Implement Stack with single Queue, Queue with 2 Stacks, and find Next Smaller Element.
// - Approach: Queue rotation on push, amortized O(1) transfer between stacks, and monotonic stack right-to-left.
// - Complexity: Time: Amortized O(1) for Queue/Stack operations, O(N) for NSE, Space: O(N).

// =========================================================
// 21. LFU CACHE (LEETCODE 460) & MIN STACK (LEETCODE 155)
// =========================================================

struct LFUNode {
    int key, val, freq;
    LFUNode *prev, *next;
    LFUNode(int k, int v) : key(k), val(v), freq(1), prev(nullptr), next(nullptr) {}
};

struct LFDoublyList {
    LFUNode *head, *tail;
    int size;
    LFDoublyList() {
        head = new LFUNode(0, 0);
        tail = new LFUNode(0, 0);
        head->next = tail;
        tail->prev = head;
        size = 0;
    }
    void addFront(LFUNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
        size++;
    }
    void removeNode(LFUNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        size--;
    }
    LFUNode* removeTail() {
        if (size == 0) return nullptr;
        LFUNode* node = tail->prev;
        removeNode(node);
        return node;
    }
};

class LFUCache {
    int cap, minFreq;
    unordered_map<int, LFUNode*> keyMap;
    unordered_map<int, LFDoublyList*> freqMap;

    void updateFreq(LFUNode* node) {
        freqMap[node->freq]->removeNode(node);
        if (node->freq == minFreq && freqMap[node->freq]->size == 0) {
            minFreq++;
        }
        node->freq++;
        if (freqMap.find(node->freq) == freqMap.end()) {
            freqMap[node->freq] = new LFDoublyList();
        }
        freqMap[node->freq]->addFront(node);
    }
public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        if (keyMap.find(key) == keyMap.end()) return -1;
        LFUNode* node = keyMap[key];
        updateFreq(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cap == 0) return;
        if (keyMap.find(key) != keyMap.end()) {
            LFUNode* node = keyMap[key];
            node->val = value;
            updateFreq(node);
            return;
        }
        if ((int)keyMap.size() >= cap) {
            LFUNode* evicted = freqMap[minFreq]->removeTail();
            keyMap.erase(evicted->key);
            delete evicted;
        }
        LFUNode* newNode = new LFUNode(key, value);
        minFreq = 1;
        if (freqMap.find(1) == freqMap.end()) freqMap[1] = new LFDoublyList();
        freqMap[1]->addFront(newNode);
        keyMap[key] = newNode;
    }
};

class MinStackO1Space {
    stack<ll> st;
    ll minVal;
public:
    MinStackO1Space() : minVal(LLONG_MAX) {}

    void push(int val) {
        if (st.empty()) {
            minVal = val;
            st.push(val);
        } else if (val >= minVal) {
            st.push(val);
        } else {
            st.push(2LL * val - minVal); // mathematical encoding
            minVal = val;
        }
    }
    void pop() {
        if (st.empty()) return;
        ll topVal = st.top(); st.pop();
        if (topVal < minVal) {
            minVal = 2 * minVal - topVal; // decode previous min
        }
    }
    int top() {
        ll topVal = st.top();
        if (topVal < minVal) return (int)minVal;
        return (int)topVal;
    }
    int getMin() { return (int)minVal; }
};
// Interview Explanation:
// - Problem Statement: Design O(1) LFU Cache and Min Stack with O(1) extra space.
// - Approach: Frequency Map + Doubly Linked Lists & Value Encoding $2v - 	ext{min}$.
// - Complexity: Time: O(1) all operations, Space: O(Capacity) & O(1) extra space.

// =========================================================
// 22. STOCK SPAN (LC 901) & THE CELEBRITY PROBLEM (LC 277)
// =========================================================

class StockSpanner {
    stack<pair<int, int>> st; // {price, span}
public:
    StockSpanner() {}
    int next(int price) {
        int span = 1;
        while (!st.empty() && st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }
        st.push({price, span});
        return span;
    }
};

int findCelebrity(const vvi &M, int n) {
    int a = 0, b = n - 1;
    while (a < b) {
        if (M[a][b] == 1) a++; // a knows b, so a cannot be celebrity
        else b--;              // a doesn't know b, so b cannot be celebrity
    }
    int candidate = a;
    for (int i = 0; i < n; ++i) {
        if (i != candidate) {
            if (M[candidate][i] == 1 || M[i][candidate] == 0) return -1;
        }
    }
    return candidate;
}
// Interview Explanation:
// - Problem Statement: Calculate stock span of consecutive <= prices, and find party celebrity who knows no one and is known by everyone.
// - Approach: Monotonic Stack span accumulation + Two-Pointer Elimination in O(N) time.
// - Complexity: Time: O(1) amortized for stock span, O(N) for celebrity, Space: O(N) stack & O(1) space.

// =========================================================
// 23. STRING MASTER (REVERSE WORDS, ROMAN, ATOI, LCP)
// =========================================================

string reverseWords(string s) {
    stringstream ss(s);
    string word, ans = "";
    vector<string> words;
    while (ss >> word) words.push_back(word);
    for (int i = (int)words.size() - 1; i >= 0; --i) {
        ans += words[i];
        if (i > 0) ans += " ";
    }
    return ans;
}

int romanToInt(string s) {
    unordered_map<char, int> m = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500}, {'M', 1000}
    };
    int ans = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (i + 1 < (int)s.size() && m[s[i]] < m[s[i + 1]]) {
            ans -= m[s[i]];
        } else {
            ans += m[s[i]];
        }
    }
    return ans;
}

int myAtoi(string s) {
    int i = 0, n = s.size();
    while (i < n && s[i] == ' ') i++;
    if (i == n) return 0;

    int sign = 1;
    if (s[i] == '+' || s[i] == '-') {
        if (s[i] == '-') sign = -1;
        i++;
    }

    ll result = 0;
    while (i < n && isdigit(s[i])) {
        result = result * 10 + (s[i] - '0');
        if (sign == 1 && result >= INT_MAX) return INT_MAX;
        if (sign == -1 && -result <= INT_MIN) return INT_MIN;
        i++;
    }
    return (int)(sign * result);
}

string longestCommonPrefix(vector<string> &strs) {
    if (strs.empty()) return "";
    sort(strs.begin(), strs.end());
    string first = strs.front(), last = strs.back();
    string ans = "";
    for (int i = 0; i < min(first.size(), last.size()); ++i) {
        if (first[i] != last[i]) break;
        ans += first[i];
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Master string implementations for word reversing, Roman numeral conversion, robust Atoi parsing, and longest common prefix.
// - Approach: Stringstream parsing, Lookahead subtraction for Roman, Finite State clamp for Atoi, and Lexicographical sort comparison for LCP.
// - Complexity: Time: O(N), Space: O(1) auxiliary memory.

// =========================================================
// 24. PALINDROME INSERTIONS, COUNT AND SAY, COMPARE VERSIONS
// =========================================================

int minInsertionsToMakePalindrome(string s) {
    string rev = s;
    reverse(rev.begin(), rev.end());
    int n = s.size();
    vvi dp(n + 1, vi(n + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s[i - 1] == rev[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    int lps = dp[n][n];
    return n - lps;
}

string countAndSay(int n) {
    if (n == 1) return "1";
    string s = "1";
    for (int i = 2; i <= n; ++i) {
        string nextStr = "";
        int count = 1;
        for (int j = 1; j < (int)s.size(); ++j) {
            if (s[j] == s[j - 1]) count++;
            else {
                nextStr += to_string(count) + s[j - 1];
                count = 1;
            }
        }
        nextStr += to_string(count) + s.back();
        s = nextStr;
    }
    return s;
}

int compareVersion(string version1, string version2) {
    int i = 0, j = 0, n1 = version1.size(), n2 = version2.size();
    while (i < n1 || j < n2) {
        long long num1 = 0, num2 = 0;
        while (i < n1 && version1[i] != '.') num1 = num1 * 10 + (version1[i++] - '0');
        while (j < n2 && version2[j] != '.') num2 = num2 * 10 + (version2[j++] - '0');

        if (num1 < num2) return -1;
        if (num1 > num2) return 1;
        i++; j++;
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Minimum insertions to form palindrome ($N - 	ext{LPS}(S)$), Count and Say sequence, and semantic Version number comparison.
// - Approach: Dynamic programming LCS with reversed string, run-length string building, and chunked two-pointer version parsing.
// - Complexity: Time: O(N^2) for palindrome insertions, O(N) for version comparison, Space: O(N^2) / O(1).

// =========================================================
// 25. 0/1 KNAPSACK & MAXIMUM SUM INCREASING SUBSEQUENCE (MSIS)
// =========================================================

int knapSack01(int W, const vi &wt, const vi &val, int n) {
    vi prev(W + 1, 0);
    for (int w = wt[0]; w <= W; ++w) prev[w] = val[0];

    for (int ind = 1; ind < n; ++ind) {
        for (int cap = W; cap >= 0; --cap) {
            int notTaken = prev[cap];
            int taken = INT_MIN;
            if (wt[ind] <= cap) taken = val[ind] + prev[cap - wt[ind]];
            prev[cap] = max(notTaken, taken);
        }
    }
    return prev[W];
}

int maxSubsequenceSumIncreasing(const vi &arr) {
    int n = arr.size();
    vi dp = arr; // dp[i] = max sum of increasing subsequence ending at i
    int maxSum = 0;

    for (int i = 0; i < n; ++i) {
        for (int prev = 0; prev < i; ++prev) {
            if (arr[prev] < arr[i]) {
                dp[i] = max(dp[i], arr[i] + dp[prev]);
            }
        }
        maxSum = max(maxSum, dp[i]);
    }
    return maxSum;
}
// Interview Explanation:
// - Problem Statement: 0/1 Knapsack optimal value in O(W) 1D space, and Maximum Sum Increasing Subsequence (MSIS).
// - Approach: Single array reverse capacity 0/1 Knapsack DP + LIS sum variation tracking maximum accumulated sum ending at index i.
// - Complexity: Time: O(N * W) for Knapsack, O(N^2) for MSIS, Space: O(W) & O(N).

// =========================================================
// 26. MATRIX CHAIN MULTIPLICATION (MCM) & MIN PALINDROME CUTS
// =========================================================

int matrixMultiplicationMCM(const vi &arr) {
    int n = arr.size();
    vvi dp(n, vi(n, 0));

    for (int len = 2; len < n; ++len) {
        for (int i = 1; i < n - len + 1; ++i) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k <= j - 1; ++k) {
                int cost = dp[i][k] + dp[k + 1][j] + arr[i - 1] * arr[k] * arr[j];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    return dp[1][n - 1];
}

int minCutPalindrome(string s) {
    int n = s.size();
    vector<vector<bool>> isPal(n, vector<bool>(n, false));

    for (int i = n - 1; i >= 0; --i) {
        for (int j = i; j < n; ++j) {
            if (s[i] == s[j] && (j - i <= 2 || isPal[i + 1][j - 1])) {
                isPal[i][j] = true;
            }
        }
    }

    vi dp(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        int minCuts = INT_MAX;
        for (int j = i; j < n; ++j) {
            if (isPal[i][j]) {
                minCuts = min(minCuts, 1 + dp[j + 1]);
            }
        }
        dp[i] = minCuts;
    }
    return dp[0] - 1;
}
// Interview Explanation:
// - Problem Statement: Find minimum matrix multiplication operations (MCM), and minimum cuts needed for palindrome partitioning (LeetCode 132).
// - Approach: Classic Interval DP with optimal partition cut $k \in [i, j-1]$ + 1D Front Partition DP with precomputed palindrome table.
// - Complexity: Time: O(N^3) for MCM, O(N^2) for Palindrome Cuts, Space: O(N^2).

// =========================================================
// 27. MAXIMUM PROFIT IN JOB SCHEDULING (LEETCODE 1235)
// =========================================================

struct JobInterval {
    int start, end, profit;
};

int jobSchedulingDP(vi &startTime, vi &endTime, vi &profit) {
    int n = startTime.size();
    vector<JobInterval> jobs(n);
    for (int i = 0; i < n; ++i) jobs[i] = {startTime[i], endTime[i], profit[i]};

    sort(jobs.begin(), jobs.end(), [](const JobInterval &a, const JobInterval &b) {
        return a.end < b.end;
    });

    vi dp(n, 0);
    dp[0] = jobs[0].profit;

    for (int i = 1; i < n; ++i) {
        int includeProfit = jobs[i].profit;
        // Binary search for latest non-overlapping job
        int low = 0, high = i - 1, lastNonOverlap = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (jobs[mid].end <= jobs[i].start) {
                lastNonOverlap = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        if (lastNonOverlap != -1) includeProfit += dp[lastNonOverlap];
        dp[i] = max(dp[i - 1], includeProfit);
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Find maximum profit from overlapping jobs with given start, end times and profits.
// - Approach: Dynamic Programming + Binary Search on Finish Time ($O(N \log N)$).
// - Intuition: Sort jobs by end time; for each job, binary search the latest non-overlapping predecessor and take `max(dp[i-1], job.profit + dp[latest])`.
// - Complexity: Time: O(N log N) sorting and binary search lookups, Space: O(N) dp array.

// =========================================================
// 28. ADVANCED STRING MATCHING (RABIN-KARP, Z-ALGORITHM, KMP)
// =========================================================

vi rabinKarpSearch(const string &pat, const string &txt) {
    vi indices;
    int m = pat.size(), n = txt.size();
    if (m > n) return indices;
    ll pHash = 0, tHash = 0, h = 1, d = 256;

    for (int i = 0; i < m - 1; ++i) h = (h * d) % MOD;
    for (int i = 0; i < m; ++i) {
        pHash = (d * pHash + pat[i]) % MOD;
        tHash = (d * tHash + txt[i]) % MOD;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (pHash == tHash) {
            bool match = true;
            for (int j = 0; j < m; ++j) {
                if (txt[i + j] != pat[j]) { match = false; break; }
            }
            if (match) indices.push_back(i);
        }
        if (i < n - m) {
            tHash = (d * (tHash - txt[i] * h) + txt[i + m]) % MOD;
            if (tHash < 0) tHash += MOD;
        }
    }
    return indices;
}

vi computeZArray(const string &s) {
    int n = s.size();
    vi z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

vi KMPSearch(const string &pat, const string &txt) {
    vi indices;
    int m = pat.size(), n = txt.size();
    vi lps(m, 0);
    int len = 0, i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) {
            len++; lps[i++] = len;
        } else {
            if (len != 0) len = lps[len - 1];
            else lps[i++] = 0;
        }
    }
    i = 0; int j = 0;
    while (i < n) {
        if (pat[j] == txt[i]) { i++; j++; }
        if (j == m) {
            indices.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return indices;
}
// Interview Explanation:
// - Problem Statement: Find all occurrences of pattern string inside text using linear string search algorithms.
// - Approach: Rabin-Karp Rolling Hash, Z-Algorithm Segment Windows, and KMP Longest Prefix Suffix (LPS).
// - Complexity: Time: O(N + M) linear pattern matching, Space: O(M) LPS / Z array.


// =========================================================
// 29. NINJA'S TRAINING (2D DP) & SET MATRIX ZEROES (LC 73)
// =========================================================

int ninjaTraining(int n, vvi &points) {
    vi prev(4, 0);
    prev[0] = max(points[0][1], points[0][2]);
    prev[1] = max(points[0][0], points[0][2]);
    prev[2] = max(points[0][0], points[0][1]);
    prev[3] = max({points[0][0], points[0][1], points[0][2]});

    for (int day = 1; day < n; ++day) {
        vi curr(4, 0);
        for (int last = 0; last < 4; ++last) {
            for (int task = 0; task < 3; ++task) {
                if (task != last) {
                    curr[last] = max(curr[last], points[day][task] + prev[task]);
                }
            }
        }
        prev = curr;
    }
    return prev[3];
}

void setZeroes(vvi &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    int col0 = 1;

    for (int i = 0; i < m; ++i) {
        if (matrix[i][0] == 0) col0 = 0;
        for (int j = 1; j < n; ++j) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }
    }

    for (int i = m - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 1; --j) {
            if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                matrix[i][j] = 0;
            }
        }
        if (col0 == 0) matrix[i][0] = 0;
    }
}
// Interview Explanation:
// - Problem Statement: Maximize points in Ninja's Training without repeating consecutive day tasks, and set matrix zeroes in-place in O(1) space.
// - Approach: 1D Space Optimized State Machine DP & First Row/Col Marker Tracking.
// - Complexity: Time: O(N) & O(M * N), Space: O(1) in-place auxiliary memory.


// ============================================================================
// CONSOLIDATION INDEX: DEDUPLICATED SDE SHEET PROBLEMS & THEIR LOCATIONS
// ============================================================================
/*
The following problems are covered in other dedicated files in the DSA suite:

+------------------------------------------------------------------+-------------------------------------------------------------+
| PROBLEM NAME & LEETCODE NUMBER                                   | PRIMARY LOCATION(S) IN REPOSITORY                           |
+------------------------------------------------------------------+-------------------------------------------------------------+
| All Tree Traversals, Views, LCA, Paths, Width, Morris, BST       | DSA-Code/notebook-7.cpp (Problems 1 to 43)                  |
| Next Permutation (LeetCode 31)                                   | DSA-Code/notebook-4.cpp (Problem 2), interview.cpp          |
| 3 Sum (LeetCode 15)                                              | DSA-Code/sorting_template.cpp, interview.cpp                |
| Kadane's Algorithm - Maximum Subarray (LeetCode 53)              | DSA-Code/notebook-2.cpp (Problem 8), last-moment.cpp        |
| Count Inversions                                                 | DSA-Code/sorting_template.cpp, interview.cpp                |
| Maximum Product Subarray (LeetCode 152)                          | DSA-Code/notebook-10.cpp, last-moment.cpp                   |
| Find Peak Element (LeetCode 162)                                 | DSA-Code/sorting_template.cpp, last-moment.cpp              |
| Koko Eating Bananas (LeetCode 875)                               | DSA-Code/sorting_template.cpp, interview.cpp                |
| Aggressive Cows (SPOJ / GFG)                                     | DSA-Code/sorting_template.cpp                               |
| Median of 2 Sorted Arrays (LeetCode 4)                           | DSA-Code/notebook-4.cpp (Problem 1), interview.cpp          |
| Power Set / All Subsets (LeetCode 78)                            | DSA-Code/recursion_template.cpp, notebook-2.cpp             |
| Combination Sum I (LeetCode 39)                                  | DSA-Code/recursion_template.cpp, heap_interval_template.cpp |
| N Queen (LeetCode 51)                                            | DSA-Code/recursion_template.cpp, interview.cpp              |
| Sudoku Solver (LeetCode 37)                                      | DSA-Code/recursion_template.cpp, interview.cpp              |
| Word Search (LeetCode 79)                                        | DSA-Code/recursion_template.cpp, interview.cpp              |
| Next Greater Element (LeetCode 496 / 503)                        | DSA-Code/stack_template.cpp, notebook-4.cpp                 |
| Trapping Rainwater (LeetCode 42)                                 | DSA-Code/stack_template.cpp, interview.cpp                  |
| Largest Rectangle in a Histogram (LeetCode 84)                   | DSA-Code/stack_template.cpp, last-moment.cpp                |
| Asteroid Collision (LeetCode 735)                                | DSA-Code/stack_template.cpp                                 |
| Sliding Window Maximum (LeetCode 239)                            | DSA-Code/notebook-2.cpp (Problem 7), stack_template.cpp     |
| LRU Cache (LeetCode 146)                                         | DSA-Code/notebook-5.cpp, stack_template.cpp                 |
| K-th Largest Element in an Array (LeetCode 215)                  | DSA-Code/notebook-4.cpp (QuickSelect), sorting_template.cpp |
| Dijkstra's Algorithm (Single Source Shortest Path)               | DSA-Code/notebook-8.cpp (Problem 10), notebook-3.cpp        |
| Bellman Ford Algorithm                                           | DSA-Code/notebook-8.cpp, notebook-3.cpp                     |
| Floyd Warshall Algorithm                                         | DSA-Code/notebook-8.cpp, notebook-3.cpp                     |
| Find MST Weight (Prim's & Kruskal's Algorithm)                   | DSA-Code/notebook-1.cpp (Prob 5 & 6), notebook-8.cpp (16&17)|
| Longest Increasing Subsequence (LeetCode 300)                    | DSA-Code/notebook-2.cpp (Problem 9), notebook-10.cpp        |
| Longest Common Subsequence (LeetCode 1143)                       | DSA-Code/notebook-2.cpp (Problem 10), notebook-10.cpp       |
| Edit Distance (LeetCode 72)                                      | DSA-Code/notebook-10.cpp, interview.cpp                     |
| Best Time to Buy and Sell Stock (LeetCode 121)                   | DSA-Code/notebook-10.cpp, interview.cpp                     |
| Best Time to Buy and Sell Stock IV (LeetCode 188)                | DSA-Code/notebook-10.cpp, heap_interval_template.cpp        |
| Burst Balloons (LeetCode 312)                                    | DSA-Code/notebook-10.cpp, interview.cpp                     |
| Trie Implementation & Search (LeetCode 208)                      | DSA-Code/alphabet_trie.cpp, binary_trie.cpp                 |
| Maximum XOR of 2 Numbers & with Element (LC 421 & 1707)          | DSA-Code/alphabet_trie.cpp, binary_trie.cpp                 |
| Coin Change I & II (LeetCode 322 & 518)                          | DSA-Code/notebook-10.cpp                                    |
| Super Egg Drop (LeetCode 887)                                    | DSA-Code/notebook-10.cpp                                    |
+------------------------------------------------------------------+-------------------------------------------------------------+
*/
