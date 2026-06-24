/**
 * Problem: Inversion Count
 * Link: N/A
 * Category: Sorting / Binary Indexed Tree
 * 
 * Description:
 * Count how many pairs (i, j) exist such that i < j and arr[i] > arr[j].
 * 
 * Logic/Approach:
 * Merge sort count pass or range sum queries using Fenwick tree.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

using vi = vector<int>;

int mergeAndCount(int l, int mid, int r, vector<int>& nums) {
    int n1 = mid - l + 1;
    int n2 = r - mid;

    vector<int> left(n1);
    vector<int> right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = nums[l + i];

    for (int j = 0; j < n2; j++)
        right[j] = nums[mid + 1 + j];

    int i = 0, j = 0, k = l;
    int count = 0;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) nums[k++] = left[i++];
        else nums[k++] = right[j++], count += (n1 - i); // inversions, if left[i] > right[j], then all elements from left[i] to left[n1-1] will be greater than right[j], contributing to inversions
    } // merging the two halves, some portion will be left in either left or right, we will copy them to nums, which will not affect the count of inversions as they are already sorted and will not contribute to inversions

    while (i < n1) nums[k++] = left[i++]; // copy remaining elements of left
    while (j < n2) nums[k++] = right[j++]; // copy remaining elements of right

    return count;
}

int helper(int l, int r, vector<int>& nums) {
    if (l >= r) return 0;

    int mid = l + (r - l) / 2; 

    int count = 0;

    count += helper(l, mid, nums); // inversions in left half
    count += helper(mid + 1, r, nums); // inversions in right half
    count += mergeAndCount(l, mid, r, nums); // inversions across halves

    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vi nums(n);

    for (int &x : nums) cin >> x;

    cout << helper(0, n - 1, nums) << '\n';

    return 0;
}