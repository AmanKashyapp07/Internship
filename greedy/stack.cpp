/**
 * ============================================================================
 *                    MONOTONIC STACK - INTERVIEW/OA CHEAT SHEET
 * ============================================================================
 *
 * MONOTONIC STACK = Maintain elements in sorted order inside stack.
 *
 * ----------------------------------------------------------------------------
 * VARIANT 1 : PREVIOUS SMALLER ELEMENT (PSE)
 * ----------------------------------------------------------------------------
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 *     st.pop();
 *
 * pse[i] = st.empty() ? -1 : st.top();
 *
 * Stack Property:
 *     Strictly Increasing
 *
 * Questions:
 *     - Previous Smaller Element
 *     - Sum of Subarray Minimums (one side)
 *     - Histogram
 *     - Max of Minimum for Every Window Size
 *
 * ----------------------------------------------------------------------------
 * VARIANT 2 : PREVIOUS GREATER ELEMENT (PGE)
 * ----------------------------------------------------------------------------
 *
 * while (!st.empty() && arr[st.top()] <= arr[i])
 *     st.pop();
 *
 * pge[i] = st.empty() ? -1 : st.top();
 *
 * Stack Property:
 *     Strictly Decreasing
 *
 * Questions:
 *     - Previous Greater Element
 *     - Sum of Subarray Maximums
 *
 * ----------------------------------------------------------------------------
 * VARIANT 3 : NEXT SMALLER ELEMENT (NSE)
 * ----------------------------------------------------------------------------
 *
 * Traverse Right → Left
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 *     st.pop();
 *
 * nse[i] = st.empty() ? n : st.top();
 *
 * Stack Property:
 *     Strictly Increasing
 *
 * ----------------------------------------------------------------------------
 * VARIANT 4 : NEXT GREATER ELEMENT (NGE)
 * ----------------------------------------------------------------------------
 *
 * Traverse Right → Left
 *
 * while (!st.empty() && arr[st.top()] <= arr[i])
 *     st.pop();
 *
 * nge[i] = st.empty() ? n : st.top();
 *
 * Stack Property:
 *     Strictly Decreasing
 *
 * ============================================================================
 *                 DUPLICATE HANDLING (MOST IMPORTANT)
 * ============================================================================
 *
 * STRICT          : >  or <
 * NON-STRICT      : >= or <=
 *
 * Rule:
 *
 *     If duplicates exist,
 *     one side STRICT
 *     one side NON-STRICT
 *
 * Otherwise duplicate contribution gets counted twice.
 *
 * Example:
 *
 * Sum of Subarray Minimums
 *
 * PSE -> >
 * NSE -> >=
 *
 * OR
 *
 * PSE -> >=
 * NSE -> >
 *
 * Both are correct.
 *
 * Never:
 *
 * PSE -> >
 * NSE -> >
 *
 * Never:
 *
 * PSE -> >=
 * NSE -> >=
 *
 * ============================================================================
 *                 ONE PASS POP FORMULA (VERY IMPORTANT)
 * ============================================================================
 *
 * Histogram Pattern:
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 * {
 *      int mid = st.top();
 *      st.pop();
 *
 *      int left  = st.empty() ? -1 : st.top();
 *      int right = i;
 *
 *      // contribution of arr[mid]
 * }
 *
 * Interpretation:
 *
 * left  = Previous Smaller
 * right = Next Smaller
 *
 * width = right - left - 1
 *
 * Used in:
 *
 *     Largest Rectangle Histogram
 *     Max Of Minimums
 *     Sum Of Subarray Minimums
 *     Sum Of Subarray Maximums
 *
 * ============================================================================
 *                CONTRIBUTION TECHNIQUE
 * ============================================================================
 *
 * If:
 *
 * left  = distance to previous boundary
 * right = distance to next boundary
 *
 * Then:
 *
 * Contribution Count
 *
 * count = left * right
 *
 * Because:
 *
 * left choices for start
 * right choices for end
 *
 * Used in:
 *
 *     Sum Of Subarray Minimums
 *     Sum Of Subarray Maximums
 *
 * Formula:
 *
 * contribution = arr[i] * left * right
 *
 * ============================================================================
 *               HISTOGRAM FAMILY
 * ============================================================================
 *
 * Largest Rectangle in Histogram
 *
 * width  = NSE - PSE - 1
 * area   = height * width
 *
 * Pattern:
 *
 * Pop when smaller appears.
 *
 * while(arr[st.top()] >= arr[i])
 *
 * ============================================================================
 *               MAX OF MINIMUM FOR EVERY WINDOW SIZE
 * ============================================================================
 *
 * width = NSE - PSE - 1
 *
 * Element becomes minimum for window size:
 *
 * len = width
 *
 * ans[len] = max(ans[len], arr[i]);
 *
 * Final:
 *
 * suffix maximum
 *
 * ============================================================================
 *               SUM OF SUBARRAY MINIMUMS
 * ============================================================================
 *
 * Previous Smaller Strict
 * Next Smaller Equal
 *
 * left  = i - pse[i]
 * right = nse[i] - i
 *
 * contribution
 *
 * arr[i] * left * right
 *
 * ============================================================================
 *               SUM OF SUBARRAY MAXIMUMS
 * ============================================================================
 *
 * Previous Greater Strict
 * Next Greater Equal
 *
 * left  = i - pge[i]
 * right = nge[i] - i
 *
 * contribution
 *
 * arr[i] * left * right
 *
 * ============================================================================
 *               NEXT GREATER ELEMENT (CLASSIC)
 * ============================================================================
 *
 * Traverse Right -> Left
 *
 * while(arr[st.top()] <= arr[i])
 *      st.pop();
 *
 * Answer:
 *
 * st.top()
 *
 * Questions:
 *
 *     Next Greater Element I
 *     Next Greater Element II
 *     Daily Temperatures
 *
 * ============================================================================
 *               DAILY TEMPERATURES
 * ============================================================================
 *
 * Maintain decreasing stack.
 *
 * Pop smaller temperatures.
 *
 * Remaining top:
 *
 * Next greater temperature.
 *
 * answer[i] = st.top() - i
 *
 * ============================================================================
 *               STOCK SPAN
 * ============================================================================
 *
 * Maintain decreasing stack.
 *
 * Pop:
 *
 * arr[st.top()] <= arr[i]
 *
 * Span:
 *
 * i - previous_greater_index
 *
 * ============================================================================
 *               TRAPPING RAIN WATER
 * ============================================================================
 *
 * Monotonic Decreasing Stack
 *
 * Whenever larger element arrives:
 *
 * Pop valley.
 *
 * Compute:
 *
 * width  = current - leftBoundary - 1
 *
 * height =
 * min(leftWall,rightWall) - valleyHeight
 *
 * ============================================================================
 *               MONOTONIC DEQUE (SLIDING WINDOW)
 * ============================================================================
 *
 * NOT STACK.
 *
 * Used for:
 *
 * Sliding Window Maximum
 * Sliding Window Minimum
 *
 * Maximum:
 *
 * while(arr[dq.back()] <= arr[i])
 *      dq.pop_back();
 *
 * Minimum:
 *
 * while(arr[dq.back()] >= arr[i])
 *      dq.pop_back();
 *
 * ============================================================================
 *               RECOGNITION PATTERNS
 * ============================================================================
 *
 * Keywords:
 *
 * 1. Nearest Greater
 * 2. Nearest Smaller
 * 3. First Greater on Left
 * 4. First Greater on Right
 * 5. First Smaller on Left
 * 6. First Smaller on Right
 * 7. Span
 * 8. Contribution
 * 9. Histogram
 * 10. Window Minimum/Maximum
 * 11. Range Influence
 * 12. Element Controls Interval
 *
 * => Think Monotonic Stack
 *
 * ============================================================================
 *               MASTER MEMORY TRICK
 * ============================================================================
 *
 * MINIMUM PROBLEM
 *
 *     pop >=
 *
 * MAXIMUM PROBLEM
 *
 *     pop <=
 *
 * DUPLICATES
 *
 *     One side strict
 *     One side non-strict
 *
 * CONTRIBUTION
 *
 *     left * right
 *
 * HISTOGRAM
 *
 *     width = NSE - PSE - 1
 *
 * ============================================================================
 */

vector<int> previousSmallerElement(const vector<int>& arr) {
    int n = arr.size();
    vector<int> pse(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && arr[st.top()] >= arr[i]) {
            st.pop();
        }
        pse[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    return pse;
}

vector<int> nextSmallerElement(const vector<int>& arr) {
    int n = arr.size();
    vector<int> nse(n);
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && arr[st.top()] >= arr[i]) {
            st.pop();
        }
        nse[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    return nse;
}

vector<int> previousGreaterElement(const vector<int>& arr) {
    int n = arr.size();
    vector<int> pge(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && arr[st.top()] <= arr[i]) {
            st.pop();
        }
        pge[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    return pge;
}

vector<int> nextGreaterElement(const vector<int>& arr) {
    int n = arr.size();
    vector<int> nge(n);
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && arr[st.top()] <= arr[i]) {
            st.pop();
        } // at the top of the stack, we have the next greater element for the current element, if the stack is empty, then there is no next greater element for the current element, so we assign n to the current index in the nge array, otherwise we assign the index of the next greater element to the current index in the nge array
        nge[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    return nge;
}

int largestRectangleInHistogram(const vector<int>& heights) {
    int n = heights.size();
    stack<int> st;
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        int curHeight = (i == n ? 0 : heights[i]);

        while (!st.empty() && heights[st.top()] >= curHeight) {
            int height = heights[st.top()];
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int width = i - left - 1;
            // for the height popped out, left is previous smaller element and right is next smaller element, so width = right - left - 1, 
            maxArea = max(maxArea, height * width);
        }

        st.push(i);
    }

    return maxArea;
}

// Next Greater Element Value
vector<int> nextGreaterValue(vector<int>& arr) {
    int n = arr.size();
    vector<int> ans(n, -1);
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {

        while (!st.empty() && st.top() <= arr[i])
            st.pop();

        if (!st.empty())
            ans[i] = st.top();

        st.push(arr[i]);
    }

    return ans;
}

int sumOfSubarrayMinimums(const vector<int>& arr) {
    int n = arr.size();
    stack<int> st;
    long long sum = 0;
    for(int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            long long leftCount = mid - left;
            long long rightCount = right - mid;

            sum += (long long)arr[mid] * leftCount * rightCount;
            sum %= MOD;
        }
        st.push(i);
    }

    return sum;
}

int sumOfSubarrayMaximums(const vector<int>& arr) {
    int n = arr.size();
    stack<int> st;
    long long sum = 0;
    for(int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            long long leftCount = mid - left;
            long long rightCount = right - mid;

            sum += (long long)arr[mid] * leftCount * rightCount;
            sum %= MOD;
        }
        st.push(i);
    }

    return sum;
}

int rainwaterTrapped(const vector<int>& height) {
    int n = height.size();
    stack<int> st;
    int water = 0;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && height[st.top()] < height[i]) {
            int valley = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            if (left == -1) break;
            int right = i;
            int width = right - left - 1;
            int boundedHeight = min(height[left], height[right]) - height[valley];
            water += width * boundedHeight;
        }
        st.push(i);
    }

    return water;
}

// for minimum : use >= in while loop, for maximum : use <= in while loop

vector<int> dailyTemperatures(const vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> answer(n, 0);
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && temperatures[st.top()] <= temperatures[i]) {
            st.pop();
        }
        if (!st.empty()) {
            answer[i] = st.top() - i;
        }
        st.push(i);
    }

    return answer;
}

string removeKdigits(string num, int k) {
    string result;
    for (char digit : num) {
        while (!result.empty() && k > 0 && result.back() > digit) { // if the last digit in result is greater than the current digit, we can remove it to make the number smaller
            result.pop_back();
            k--;
        }
        result.push_back(digit); // add the current digit to the result, this is best because we are adding the current digit to the result, and we are removing the last digit from the result if it is greater than the current digit, so we are making the number smaller
    }
    while (k > 0 && !result.empty()) { // if we still have k digits to remove, we can remove the last digit from the result, this is best because we are removing the last digit from the result, which is the largest digit in the result, so we are making the number smaller
        result.pop_back();
        k--;
    }
    // Remove leading zeros
    size_t startpos = result.find_first_not_of('0');
    if (string::npos != startpos) {
        result = result.substr(startpos);
    } else {
        result = "0";
    }
    return result;
}

string removeDuplicateLetters(string s) {
    vector<int> last(26);

    for (int i = 0; i < s.size(); i++)
        last[s[i] - 'a'] = i;

    vector<bool> used(26, false);
    string ans;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        if (used[c - 'a'])
            continue;

        while (!ans.empty() && // this condition to check ans is not empty, because we cannot pop from an empty string
               ans.back() > c && // this condition to check if the last character in ans is greater than the current character, because we want to maintain the lexicographical order, so we want to remove the last character if it is greater than the current character
               last[ans.back() - 'a'] > i) { // this condition to check if the last character in ans appears later in the string, because we want to maintain the lexicographical order, so we want to remove the last character if it appears later in the string, because we can add it back later, so we can remove it now to make the string smaller, if not, we cannot remove it because we will lose that character and the final string will not have all the characters, so we need to check if the last character appears later in the string, if it does, we can remove it now to make the string smaller, if not, we cannot remove it because we will lose that character and the final string will not have all the characters
            used[ans.back() - 'a'] = false;
            ans.pop_back();
        }

        ans.push_back(c);
        used[c - 'a'] = true;
    }

    return ans;
}

vector<int> NGEinCircularArray(const vector<int>& arr) {
    int n = arr.size();
    vector<int> nge(n, -1);
    stack<int> st;

    for (int i = 2 * n - 1; i >= 0; i--) {
        while (!st.empty() && st.top() <= arr[i % n]) {
            st.pop();
        }
        if (i < n) {
            if (!st.empty()) {
                nge[i] = st.top();
            }
        }
        st.push(arr[i % n]);
    }

    return nge;
}

vector<int> nge(vector<int>& nums1, vector<int>& nums2) {
    int n = nums2.size();
    map<int, int> ngeMap;
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && st.top() <= nums2[i]) {
            st.pop();
        }
        ngeMap[nums2[i]] = st.empty() ? -1 : st.top();
        st.push(nums2[i]);
    }

    vector<int> result;
    for (int num : nums1) {
        result.push_back(ngeMap[num]);
    }

    return result;
}

vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
    auto merge = [](const vector<int>& a, const vector<int>& b) {
        vector<int> result;
        auto it1 = a.begin(), it2 = b.begin();
        while (it1 != a.end() || it2 != b.end()) {
            if (lexicographical_compare(it1, a.end(), it2, b.end())) {
                result.push_back(*it2++);
            } else {
                result.push_back(*it1++);
            }
        }
        return result;
    };

    auto pickMax = [](const vector<int>& nums, int k) {
        vector<int> result;
        for (int num : nums) {
            while (!result.empty() && result.size() + (int)nums.size() - &num > k && result.back() < num) {
                result.pop_back();
            }
            if ((int)result.size() < k) {
                result.push_back(num);
            }
        }
        return result;
    };

    vector<int> best;
    for (int i = max(0, k - (int)nums2.size()); i <= min(k, (int)nums1.size()); ++i) {
        auto candidate = merge(pickMax(nums1, i), pickMax(nums2, k - i));
        if (candidate > best) {
            best = candidate;
        }
    }
    return best;
}