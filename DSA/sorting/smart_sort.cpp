/**
 * LeetCode 88 - Merge Sorted Array
 *
 * nums1 has size m+n.
 * First m elements are valid.
 * nums2 has n elements.
 *
 * Merge nums2 into nums1 in-place.
 *
 * Time: O(m+n)
 * Space: O(1)
 */

class Solution {
public:
    void merge(vector<int>& nums1, int m,
               vector<int>& nums2, int n) {

        int i = m - 1;
        int j = n - 1;
        int k = m + n - 1;

        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j])
                nums1[k--] = nums1[i--];
            else
                nums1[k--] = nums2[j--];
        }

        while (j >= 0)
            nums1[k--] = nums2[j--];
    }
};