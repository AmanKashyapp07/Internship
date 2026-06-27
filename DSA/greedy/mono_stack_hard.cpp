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

using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()

// --------------------------------------------------------------------

vector<int> maxSubsequence(vector<int>& nums, int k) {
    int drop = nums.size() - k;
    vector<int> st;

    for (int x : nums) {
        while (!st.empty() && drop > 0 && st.back() < x) {
            st.pop_back();
            drop--;
        }
        st.push_back(x);
    }

    st.resize(k);
    return st;
}

bool greaterSuffix(vector<int>& a, int i,
                   vector<int>& b, int j) {

    while (i < a.size() && j < b.size() &&
           a[i] == b[j]) {
        i++;
        j++;
    }

    if (j == b.size()) return true;
    if (i == a.size()) return false;

    return a[i] > b[j];
}

vector<int> mergeArrays(vector<int>& a,
                        vector<int>& b) {

    vector<int> res;

    int i = 0;
    int j = 0;

    while (i < a.size() || j < b.size()) {

        if (greaterSuffix(a, i, b, j))
            res.push_back(a[i++]);
        else
            res.push_back(b[j++]);
    }

    return res;
}

vector<int> createMaximumNumber(vector<int>& nums1,
                                vector<int>& nums2,
                                int k) {

    int n = nums1.size();
    int m = nums2.size();

    vector<int> answer(k, 0);

    int start = max(0, k - m);
    int end   = min(k, n);

    for (int take1 = start; take1 <= end; take1++) {

        int take2 = k - take1;

        vector<int> part1 =
            maxSubsequence(nums1, take1);

        vector<int> part2 =
            maxSubsequence(nums2, take2);

        vector<int> candidate =
            mergeArrays(part1, part2);

        if (greaterSuffix(candidate, 0,
                          answer, 0)) {
            answer = candidate;
        }
    }

    return answer;
}

// --------------------------------------------------------------------

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> nums1 = {3, 4, 6, 5};
    vector<int> nums2 = {9, 1, 2, 5, 8, 3};

    int k = 5;

    vector<int> ans =
        createMaximumNumber(nums1, nums2, k);

    for (int x : ans)
        cout << x << " ";

    cout << '\n';

    return 0;
}