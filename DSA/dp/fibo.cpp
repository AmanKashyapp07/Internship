class Solution {
public:
    bool hasLeadingZero(const string &s) {
        return s.size() > 1 && s[0] == '0';
    }

    bool buildFib(int index, string &num, long long prev1, long long prev2,
                  vector<int> &sequence) {

        if (index == num.size())
            return sequence.size() >= 3;

        long long next = prev1 + prev2;

        if (next > INT_MAX)
            return false;

        string nextStr = to_string(next);
        int new_length = nextStr.size();
        int new_index = index + new_length;
        if(new_index > num.size()) 
            return false;

        if (num.substr(index, new_length) != nextStr)
            return false;

        sequence.push_back(next);

        if (buildFib(new_index, num, prev2, next, sequence))
            return true;

        sequence.pop_back();
        return false;
    }

    vector<int> splitIntoFibonacci(string num) {

        int n = num.size();

        for (int i = 0; i < min(10, n - 2); i++) {

            string first = num.substr(0, i + 1);

            if (hasLeadingZero(first))
                break;

            long long a = stoll(first);

            if (a > INT_MAX)
                break;

            for (int j = i + 1; j < min(i + 11, n - 1); j++) {

                string second = num.substr(i + 1, j - i);

                if (hasLeadingZero(second))
                    break;

                long long b = stoll(second);

                if (b > INT_MAX)
                    break;

                vector<int> sequence = {(int)a, (int)b};

                if (buildFib(j + 1, num, a, b, sequence))
                    return sequence;
            }
        }

        return {};
    }
};