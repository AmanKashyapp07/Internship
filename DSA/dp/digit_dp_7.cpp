class Solution {
public:
    int monotoneIncreasingDigits(int n) {
        string s = to_string(n);

        int mark = s.size();
        int length = s.size();
        for(int i = length - 1; i > 0; i--) {
            if(s[i-1] > s[i]) {
                mark = i; // mark the position where the digits start to decrease
                s[i-1]--; // decrement the previous digit to make the number monotone increasing
            }
        }

        for (int i = mark; i < s.size(); i++)
            s[i] = '9';

        return stoi(s);
    }
};

// leetcode problem - 738. Monotone Increasing Digits