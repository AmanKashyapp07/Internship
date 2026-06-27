#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class Solution {
private:
    bool canAllocate(vector<int>& arr, int m, long long maxPages) {
        int students = 1;
        long long pages = 0;

        for (int book : arr) {
            if (pages + book <= maxPages) {
                pages += book;
            } else {
                students++;
                pages = book;

                if (students > m)
                    return false;
            }
        }

        return true;
    }

public:
    int findPages(vector<int>& arr, int m) {
        int n = arr.size();

        if (m > n)
            return -1;

        long long low = *max_element(arr.begin(), arr.end());
        long long high = accumulate(arr.begin(), arr.end(), 0LL);
        long long ans = -1;

        while (low <= high) {
            long long mid = low + (high - low) / 2;

            if (canAllocate(arr, m, mid)) {
                ans = mid;
                high = mid - 1;   // try for smaller answer
            } else {
                low = mid + 1;    // need larger limit
            }
        }

        return (int)ans;
    }
};