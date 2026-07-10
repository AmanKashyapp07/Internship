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
        int freq[3] = {0};
        int left = 0, ans = 0;

        for (char c : s)
        {
            freq[c - 'a']++;

            while (freq[0] && freq[1] && freq[2])
            {
                freq[s[left] - 'a']--;
                left++;
            }

            ans += left; // from 0, 1, ..., left-1 to right, all substrings are valid
        }

        return ans;
    }
};