class Solution {
public:
    vector<string> ans;
    unordered_set<string> seen;
    int minRemoved = INT_MAX;

    void dfs(int idx, int balance, int removed,
             string &curr, string &s) {

        if (idx == s.size()) {

            if (balance != 0)
                return;
            if (removed < minRemoved) {
                minRemoved = removed;
                ans.clear();
                seen.clear();
            }
            if (removed == minRemoved && !seen.count(curr)) {
                ans.push_back(curr);
                seen.insert(curr);
            }
            return;
        }

        char ch = s[idx];

        // Letter
        if (isalpha(ch)) {
            curr.push_back(ch);
            dfs(idx + 1, balance, removed, curr, s);
            curr.pop_back();
      
        }

        else if (ch == '(') {

            // Keep '('
            curr.push_back('(');
            dfs(idx + 1, balance + 1, removed, curr, s);
            curr.pop_back();

            // Remove '('
            dfs(idx + 1, balance, removed + 1, curr, s);
        }
        else if (ch == ')') { // ')'

            // Keep ')' only if it matches
            if (balance > 0) {
                curr.push_back(')');
                dfs(idx + 1, balance - 1, removed, curr, s);
                curr.pop_back();
            }

            // Remove ')'
            dfs(idx + 1, balance, removed + 1, curr, s);
        }
    }

    vector<string> removeInvalidParentheses(string s) {
        string curr;
        dfs(0, 0, 0, curr, s);
        return ans;
    }
};