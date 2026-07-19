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
#include <cstring>

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
const ll P = 31;

int kthElement(vector<int>& nums1, vector<int>& nums2, int k) {
    if (nums1.size() > nums2.size())
        return kthElement(nums2, nums1, k);

    int m = nums1.size();
    int n = nums2.size();

    int left = max(0, k - n);
    int right = min(k, m);

    while (left <= right) {
        int i = left + (right - left) / 2;
        int j = k - i;

        int L1 = (i == 0) ? INT_MIN : nums1[i - 1];
        int R1 = (i == m) ? INT_MAX : nums1[i];

        int L2 = (j == 0) ? INT_MIN : nums2[j - 1];
        int R2 = (j == n) ? INT_MAX : nums2[j];

        if (L1 <= R2 && L2 <= R1) {
            return max(L1, L2);
        }
        else if (L1 > R2) {
            right = i - 1;
        }
        else {
            left = i + 1;
        }
    }

    return -1; // unreachable
} 
// For Median 
// int k = (m + n + 1) / 2;
// if ((m + n) % 2 == 1) return kthElement(nums1, nums2, k);
// else return (kthElement(nums1, nums2, k) + kthElement(nums1, nums2, k + 1)) / 2.0;

void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int i = n - 2;

    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    } // first find the first decreasing element from the right

    if (i >= 0) { // if the entire array is not in descending order
        int j = n - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        swap(nums[i], nums[j]);
    }

    reverse(nums.begin() + i + 1, nums.end()); // reverse the elements to the right of i to get the next permutation

    // eg - 2 3 1 5 4 
    // i = 1, j = 4, swap(3,4) => 2 4 1 5 3 => reverse(1,5,3) => 2 4 3 5 1
}

int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastSeen;
        int left = 0;
        int max_len = 0;

        for (int right = 0; right < s.size(); right++) {
            if (lastSeen.count(s[right]) &&
                lastSeen[s[right]] >= left) {
                left = lastSeen[s[right]] + 1;
            } // If the character has been seen and is within the current window, move the left pointer to one position right of its last seen index, if the char was seen and not in the current window, we don't need to move the left pointer, as it is already in the correct position

            lastSeen[s[right]] = right;
            max_len = max(max_len, right - left + 1);
        }

        return max_len;
} // returns length of longest substring without repeating characters in O(n) time and O(min(n, a)) space where a is the size of the alphabet.


long long minimumAbsoluteDifferenceSum(vector<int> &a) {

    int n = a.size();

    sort(a.begin(), a.end());

    int median = a[n / 2];   // Works for both odd and even n

    long long sum = 0;

    for (int x : a)

        sum += abs(1LL * x - median);

    return sum;

}


int minMoves(vector<int>& nums, int k) {
    /*
        Let the positions of all 1's be:
            pos[] = {p1, p2, ...}

        Suppose we want these k ones to occupy consecutive positions:
            x, x+1, x+2, ...

        Cost for ith selected 1:
            |pos[i] - (x + i)|
          = |(pos[i] - i) - x|

        Hence define:
            graphusted[i] = pos[i] - i

        Now every selected element wants to become the SAME value x.
        The minimum sum of absolute differences is achieved at the median.

        Using prefix sums on graphusted[], we compute the cost of every
        window of k ones in O(1), giving overall O(n).
    */

    vector<long long> pos;
    for (int i = 0; i < nums.size(); i++)
        if (nums[i]) pos.push_back(i);

    int m = pos.size();

    vector<long long> graphusted(m), prefix(m + 1);
    for (int i = 0; i < m; i++) {
        graphusted[i] = pos[i] - i;
        prefix[i + 1] = prefix[i] + graphusted[i];
    }

    long long ans = LLONG_MAX;

    for (int left = 0; left + k <= m; left++) {
        int right = left + k - 1, mid = (left + right) / 2;
        long long median = graphusted[mid];

        ans = min(ans,
            median * (mid - left) - (prefix[mid] - prefix[left]) +
            (prefix[right + 1] - prefix[mid + 1]) - median * (right - mid)
        );
    }

    return ans;
}

vector<vector<int>> 2dPrefixSum(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();

    vector<vector<int>> prefix(m, vector<int>(n));

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            prefix[i][j] = matrix[i][j];
            if(i > 0) prefix[i][j] += prefix[i - 1][j];
            if(j > 0) prefix[i][j] += prefix[i][j - 1];
            if(i > 0 && j > 0) prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }
    return prefix;
}

class Solution {
public:
    long long countSubarrays(vector<int>& nums, int k) {
        long long ans = 0;
        unordered_map<int, long long> prev, cur;

        for (int x : nums) {
            cur.clear();
            cur[x]++;

            for (auto [v, cnt] : prev)
                cur[v & x] += cnt;

            ans += cur[k];
            prev = move(cur);
        }

        return ans;
    }
}; // count subarrays with bitwise AND equal to k in O(n * log(max(nums))) time and O(n) space.

void RotateMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    for (int i = 0; i < n; i++) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
} // rotates the matrix by 90 degrees clockwise in O(n^2) time and O(1) space. Here matrix is a square matrix of size n x n.

void RotateMatrix2(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    vector<vector<int>> rotated(n, vector<int>(m));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            rotated[j][m - 1 - i] = matrix[i][j];
        }
    }

    matrix = rotated;
} // rotates the matrix by 90 degrees clockwise in O(m * n) time and O(m * n) space. Here matrix is a rectangular matrix of size m x n.

vector<vector<int>> MultiplyMatrices(vector<vector<int>>& A, vector<vector<int>>& B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<int>> C(m, vector<int>(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
} // multiplies two matrices A and B in O(m * n * p) time and O(m * p) space. Here A is of size m x n and B is of size n x p.

long long countSubsequences(vector<int>& nums, int k) {
    vector<long long> dp(k + 1, 0);

    dp[0] = 1;  // Empty subsequence

    for (int x : nums) {
        for (int sum = k; sum >= x; sum--) {
            dp[sum] += dp[sum - x];
        }
    }

    return dp[k];
}

int IncreasingArray2(vector<int>& nums) {
    long long totalCost = 0;
    priority_queue<int> medianHeap;
    for(auto x:nums){
        medianHeap.push(x);
        if(medianHeap.top() > x){
            totalCost += medianHeap.top() - x;
            medianHeap.pop();
            medianHeap.push(x);
        }
    }

    return totalCost;
} // returns the minimum cost to make the array non-decreasing by only increasing elements in O(n) time and O(1) space.