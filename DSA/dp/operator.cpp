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
    vector<string> result;
    string str;
    int targetSum;

    void helper(int pos, long long currentValue, long long lastValue, string expression) {
        if (pos == str.size()) {
            if (currentValue == targetSum) result.push_back(expression);
            return;
        }
        for (size_t i = pos; i < str.size(); ++i) {
            if (i != pos && str[pos] == '0') break; // Prevent leading zeros
            string currentStr = str.substr(pos, i - pos + 1);
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
        str = num;
        targetSum = target;
        result.clear();
        if (!str.empty()) {
            helper(0, 0, 0, "");
        }
        return result;
    }
};