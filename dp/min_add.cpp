/**
 * Problem: Minimum Add to Make Parentheses Valid (https://leetcode.com/problems/minimum-add-to-make-parentheses-valid/)
 * Find the minimum number of parentheses to add to make the string valid.
 * Time: O(N) time, O(1) space.
 */
#include <iostream>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    string s; cin >> s;
    int balanced = 0, count = 0;
    // Greedy tracking: balanced maintains open paren count, count tracks unmatched close parens
    for (char c : s) {
        if (c == '(') balanced++;
        else if (--balanced < 0) { count++; balanced = 0; }
    }
    cout << count + balanced << "\n";
}
