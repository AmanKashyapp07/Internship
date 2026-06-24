/**
 * Problem: Expression Add Operators (https://leetcode.com/problems/expression-add-operators/)
 * Add operators (+, -, *) to digits string to evaluate to target.
 * Time: O(4^N) worst-case time, O(N) space.
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