/**
 * Problem: LeetCode 1358 - Number of Substrings Containing All Three Characters
 * Category: Sliding Window / Two Pointers
 * * Time Complexity:  O(N) - Each character is visited at most twice (once by r, once by l).
 * Space Complexity: O(1) - Constant space used for the 3-element frequency array.
 */

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution
{
public:
    int numberOfSubstrings(string s)
    {
        // Frequency array to keep track of 'a', 'b', and 'c' counts
        vector<int> count(3, 0);
        int totalSubstrings = 0;
        int l = 0;
        int n = s.length();

        for (int r = 0; r < n; ++r)
        {
            // Include the current character in our window
            count[s[r] - 'a']++;

            // While the current window contains at least one of each ('a', 'b', 'c')
            while (count[0] > 0 && count[1] > 0 && count[2] > 0)
            {
                // If the window s[l...r] is valid, then all substrings
                // starting from index 0 up to l and ending at r are also valid.
                // The total number of such valid substrings ending at 'r' is (n - r).
                // Alternatively, adding (n - r) when 'l' matches tells us how many
                // valid suffixes start at 'l'.
                totalSubstrings += (n - r); // if l is fixed, then all substrings starting from l to r, l to r+1, ..., l to n-1 are valid

                // Shrink the window from the left to find the next boundary
                count[s[l] - 'a']--;
                l++;
            }
        }

        return totalSubstrings;
    }
    int numberOfSubstrings2(string s)
    {
        vector<int> cnt(3, 0);
        int l = 0;
        int ans = 0;

        for (int r = 0; r < s.size(); r++)
        {
            cnt[s[r] - 'a']++;

            while (cnt[0] && cnt[1] && cnt[2])
            {
                cnt[s[l] - 'a']--;
                l++;
            }

            ans += l;
        }

        return ans;
    }
};