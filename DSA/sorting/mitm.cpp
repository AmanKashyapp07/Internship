class Solution {
public:
    int minimumDifference(vector<int>& nums) {
        int n = nums.size() / 2;

        vector<int> left(nums.begin(), nums.begin() + n);
        vector<int> right(nums.begin() + n, nums.end());

        vector<vector<long long>> leftSums(n + 1), rightSums(n + 1); // leftSums[i] will store all possible sums of subsets of size i from the left half, and rightSums[i] will do the same for the right half.

        // Generate all subset sums for left half
        for (int mask = 0; mask < (1 << n); mask++) {
            long long sum = 0;
            int cnt = 0;

            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    sum += left[i];
                    cnt++;
                }
            }

            leftSums[cnt].push_back(sum);
        }

        // Generate all subset sums for right half
        for (int mask = 0; mask < (1 << n); mask++) {
            long long sum = 0;
            int cnt = 0;

            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    sum += right[i];
                    cnt++;
                }
            }

            rightSums[cnt].push_back(sum);
        }

        for (int i = 0; i <= n; i++) {
            sort(rightSums[i].begin(), rightSums[i].end());
        }

        long long total = accumulate(nums.begin(), nums.end(), 0LL);
        long long ans = LLONG_MAX;

        for (int leftCnt = 0; leftCnt <= n; leftCnt++) {
            int rightCnt = n - leftCnt;

            for (long long leftSum : leftSums[leftCnt]) {
                long long target = total / 2 - leftSum;

                auto &vec = rightSums[rightCnt];
                auto it = lower_bound(vec.begin(), vec.end(), target);

                if (it != vec.end()) {
                    long long chosen = leftSum + *it;
                    ans = min(ans, llabs(total - 2 * chosen));
                }

                if (it != vec.begin()) {
                    --it;
                    long long chosen = leftSum + *it;
                    ans = min(ans, llabs(total - 2 * chosen));
                }
            }
        }

        return (int)ans;
    }
};