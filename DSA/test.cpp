class Solution
{
public:
    vector<string> findAndReplacePattern(vector<string> &words, string pattern)
    {
        vector<string> ans;
        for (auto &w : words)
        {
            int flag = 1;
            int length = w.size();
            map<char, char> m1;
            map<char, char> m2;
            for (int i = 0; i < length; i++)
            {
                int char1 = w[i];
                int char2 = pattern[i];
                if (m1.find(char1) == m1.end() && m2.find(char2) == m2.end())
                {
                    m1[char1] = char2;
                    m2[char2] = char1;
                }
                else if (m1[char1] != char2 || m2[char2] != char1)
                {
                    flag = 0;
                    break;
                }
            }
            if (flag)
            {
                ans.push_back(w);
            }
        }
        return ans;
    }
};