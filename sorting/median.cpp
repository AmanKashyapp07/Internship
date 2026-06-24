/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
 */

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

int kthElement(vector<int> &nums1, vector<int> &nums2, int k)
{
    if (nums1.size() > nums2.size())
        return kthElement(nums2, nums1, k);

    int m = nums1.size();
    int n = nums2.size();

    int left = max(0, k - n); // k-n means we need at least k-n elements from nums1 to reach k elements in total
    int right = min(k, m); // we can't take more than k elements from nums1, and also can't take more than m elements

    while (left <= right)
    {
        int i = left + (right - left) / 2;
        int j = k - i;

        int L1 = (i == 0) ? INT_MIN : nums1[i - 1];
        int R1 = (i == m) ? INT_MAX : nums1[i];

        int L2 = (j == 0) ? INT_MIN : nums2[j - 1];
        int R2 = (j == n) ? INT_MAX : nums2[j];

        if (L1 <= R2 && L2 <= R1)
        {
            return max(L1, L2);
        }
        else if (L1 > R2)
        {
            right = i - 1;
        }
        else
        {
            left = i + 1;
        }
    }

    return -1;
} // here k is 1-based index

double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2)
{
    int total = nums1.size() + nums2.size();
    if (total % 2 == 1)
    {
        return kthElement(nums1, nums2, total / 2 + 1); // if length is odd, the median is the middle element, which is at index total/2 (0-based) or total/2 + 1 (1-based)
    }
    else // if length is even, the median is the average of the two middle elements, which are at indices total/2 - 1 and total/2 (0-based) or total/2 and total/2 + 1 (1-based)
    {
        int left = kthElement(nums1, nums2, total / 2);
        int right = kthElement(nums1, nums2, total / 2 + 1);
        return (left + right) / 2.0;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    vector<int> nums = {1, 2, 3, 4, 5};
    vector<int> nums2 = {5, 4, 3, 2};

    cout << findMedianSortedArrays(nums, nums2) << endl; // Output: 3.0

    return 0;
}
