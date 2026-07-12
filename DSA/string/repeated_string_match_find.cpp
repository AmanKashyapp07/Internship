/**
 * Problem: Repeated String Match (Find)
 * Link: https://leetcode.com/problems/repeated-string-match/
 * Category: String
 * 
 * Description:
 * Find min repeats of a to contain b.
 * 
 * Logic/Approach:
 * String repetition and standard index search.
 */

#include <string>
#include <iostream>

using namespace std;

class Solution {
public:
    int repeatedStringMatch(string a, string b) {

        string s = a;
        int cnt = 1;

        while(s.size() < b.size()) {
            s += a;
            cnt++;
        }

        if(s.find(b) != string::npos) return cnt; // s.find(b) returns the index of the first occurrence of b in s, or string::npos if not found. If found, return cnt.
        s += a;
        if(s.find(b) != string::npos) return cnt + 1; // it is sure to be found in the next repetition if it is not found in the current repetition, because the length of b is less than or equal to the length of a, and we have already repeated a enough times to cover the length of b.

        return -1;
    }
};
