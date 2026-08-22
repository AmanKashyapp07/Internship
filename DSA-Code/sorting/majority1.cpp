/**
 * Problem: Majority Element I
 * Link: https://leetcode.com/problems/majority-element/
 * Category: Array / Boyer-Moore
 * 
 * Description:
 * Find the element that appears more than n/2 times.
 * 
 * Logic/Approach:
 * Boyer-Moore Majority Vote algorithm.
 */

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

vector<int> majorityElementK(vector<int>& nums, int k) {
    unordered_map<int, int> candidates;

    for (int x : nums) {
        if (candidates.count(x)) candidates[x]++;

        else if (candidates.size() < k - 1) candidates[x] = 1;

        else {
            vector<int> eraseList;

            for (auto& [num, cnt] : candidates) {
                cnt--;
                if (cnt == 0)
                    eraseList.push_back(num);
            }

            for (int num : eraseList) candidates.erase(num);
        }
    }

    unordered_map<int, int> freq;
    for (int x : nums) if (candidates.count(x)) freq[x]++;
    vector<int> ans;
    for (auto& [num, cnt] : freq) if (cnt > (int)nums.size() / k) ans.push_back(num);


    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arr(n);
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // Boyer-Moore Majority Vote (n/2)
    int candidate = arr[0];
    int count = 1;

    for (int i = 1; i < n; i++) {
        if (arr[i] == candidate)
            count++;
        else
            count--;

        if (count == 0) {
            candidate = arr[i];
            count = 1;
        }
    }

    int freq = 0;
    for (int x : arr) {
        if (x == candidate)
            freq++;
    }

    if (freq <= n / 2) cout << -1 << '\n';
    else cout << candidate << '\n';

    vector<int> ans = majorityElementK(arr, 2);

    if (ans.empty()) cout << -1 << '\n';
    else cout << ans[0] << '\n';

    return 0;
}