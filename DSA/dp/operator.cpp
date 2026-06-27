/**
 * LeetCode 282 - Expression Add Operators
 *
 * Description:
 * Given a string num containing only digits and an integer target, return all possibilities to insert
 * the binary operators '+', '-', and/or '*' between the digits of num so that the value evaluates to target.
 *
 * Approach:
 * - Backtracking / depth-first search.
 * - Traverse the digit string, selecting numeric prefixes.
 * - Keep track of: current evaluated value, last operand value (to handle multiplication precedence correctly), and the constructed expression.
 *
 * Time Complexity: O(4^n)
 * Space Complexity: O(n) recursion stack.
 */

#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    vector<string> result_;
    string num_;
    int target_;

    void helper(int pos, long long currentValue, long long lastValue, string expression) {
        if (pos == num_.size()) {
            if (currentValue == target_) result_.push_back(expression);
            return;
        }
        for (size_t i = pos; i < num_.size(); ++i) {
            if (i != pos && num_[pos] == '0') break; // Prevent leading zeros
            string currentStr = num_.substr(pos, i - pos + 1);
            long long currentNum = stoll(currentStr);
            if (pos == 0) {
                helper(i + 1, currentNum, currentNum, currentStr);
            } else {
                helper(i + 1, currentValue + currentNum, currentNum, expression + "+" + currentStr);
                helper(i + 1, currentValue - currentNum, -currentNum, expression + "-" + currentStr);
                // Adjust for operator precedence: subtract lastValue and add lastValue * currentNum
                helper(i + 1, currentValue - lastValue + lastValue * currentNum, lastValue * currentNum, expression + "*" + currentStr);
            }
        }
    }

    vector<string> addOperators(string num, int target) {
        num_ = num;
        target_ = target;
        result_.clear();
        if (!num_.empty()) {
            helper(0, 0, 0, "");
        }
        return result_;
    }
};