class Solution {
public:
    string str;

    long long solve(int pos, bool started, bool tight, vector<bool>& used) {
        if (pos == str.size()) {
            return 1;
        }

        int limit = tight ? str[pos] - '0' : 9;
        long long ans = 0;

        // ----------------------------------------------------
        // Option 1 : Skip this position (still leading zeros)
        // ----------------------------------------------------
        if (!started) {
            ans += solve(
                pos + 1,
                false,
                tight && (0 == limit),
                used
            );
        }

        // ----------------------------------------------------
        // Option 2 : Start / Continue the number
        // ----------------------------------------------------
        for (int d = (started ? 0 : 1); d <= limit; d++) {
            if (used[d]) continue;
            used[d] = true; 
            ans += solve(
                pos + 1,
                true,
                tight && (d == limit),
                used
            );
            used[d] = false; 
        }

        return ans;
    }

    int numDupDigitsAtMostN(int n) {
        str = to_string(n);
        vector<bool> used(10, false);
        long long uniqueDigitsCount = solve(0, false, true, used);
        long long total = n+1; // as we are including zero also
        return total - uniqueDigitsCount; // Count of numbers with at least one duplicate digit
    }
};