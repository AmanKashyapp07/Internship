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
    void helper(vector<string> &result, const string &num, int target, int pos, long long currentValue, long long lastValue, string expression) {
        if (pos == num.size()) {
            if (currentValue == target) result.push_back(expression);
            return;
        }
        for (size_t i = pos; i < num.size(); ++i) {
            if (i != pos && num[pos] == '0') break; // Prevent leading zeros
            string currentStr = num.substr(pos, i - pos + 1);
            long long currentNum = stoll(currentStr);
            if (pos == 0) {
                helper(result, num, target, i + 1, currentNum, currentNum, currentStr);
            } else {
                helper(result, num, target, i + 1, currentValue + currentNum, currentNum, expression + "+" + currentStr);
                helper(result, num, target, i + 1, currentValue - currentNum, -currentNum, expression + "-" + currentStr);
                // Adjust for operator precedence: subtract lastValue and add lastValue * currentNum
                helper(result, num, target, i + 1, currentValue - lastValue + lastValue * currentNum, lastValue * currentNum, expression + "*" + currentStr);
            }
        }
    }
public:
    vector<string> addOperators(string num, int target) {
        vector<string> result;
        if (!num.empty()) helper(result, num, target, 0, 0, 0, "");
        return result;
    }
};