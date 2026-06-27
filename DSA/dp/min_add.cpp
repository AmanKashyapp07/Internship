/**
 * LeetCode 921 - Minimum Add to Make Parentheses Valid
 *
 * Description:
 * A parentheses string is valid if it is empty, can be written as AB, or is of
 * the form (A). Given a string s, return the minimum number of parentheses we
 * must add to make s valid.
 *
 * Approach:
 * - Greedy approach tracking balance.
 * - Maintain `balanced` (count of unmatched open parentheses) and `count`
 * (number of insertions needed).
 * - For each character:
 *   - If `(`, increment `balanced`.
 *   - If `)`, decrement `balanced`. If it drops below 0, increment `count` and
 * reset `balanced = 0`.
 * - Total additions needed = `count + balanced`.
 *
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  string s;
  cin >> s;
  int balanced = 0, count = 0;
  // Greedy tracking: balanced maintains open paren count, count tracks
  // unmatched close parens
  for (char c : s) {
    if (c == '(')
      balanced++;
    else if (--balanced < 0) {
      count++;
      balanced = 0;
    }
  }
  cout << count + balanced << "\n";
}