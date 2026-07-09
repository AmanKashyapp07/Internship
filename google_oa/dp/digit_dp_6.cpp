class Solution {
public:
    string str;
    int maxNumber = INT_MIN;

    void solve(int pos, bool started, bool tight, string& current, int prevDigit = -1) {
        if (pos == str.size()) {
            if (!current.empty())
                maxNumber = max(maxNumber, stoi(current));
            return;
        }

        int limit = tight ? str[pos] - '0' : 9;

        // Skip current position (still leading zeros)
        if (!started) {
            solve(pos + 1, false, tight && (limit == 0), current, prevDigit);
        }

        // Place a digit
        for (int d = (started ? 0 : 1); d <= limit; d++) {
            if (prevDigit != -1 && d < prevDigit) continue; // we will only allow digits that are greater than or equal to the previous digit to ensure monotonicity
            current.push_back(d + '0');
            solve(pos + 1, true, tight && (d == limit), current, d);
            current.pop_back();
        }
    }

    int monotoneIncreasingDigits(int n) {
        str = to_string(n);
        string current;

        solve(0, false, true, current);

        return maxNumber;
    }
};

// leetcode problem - 738. Monotone Increasing Digits